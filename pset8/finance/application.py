import os

import datetime
from jinja2 import Template
from collections import defaultdict
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Display a table with all the curent user's stocks,the number of shares
    # of each, the curent price of each stock, and the total value of each holding
    # Display de user's current cash balance.

    user_id = session["user_id"]

    # Retrive data from shares
    shares = db.execute("SELECT * FROM shares WHERE id=:id", id=user_id)
    lengthS = len(shares)
    # Returne cash from users
    data = db.execute("SELECT * FROM users WHERE id=:id", id=user_id)
    cash = usd(data[0]['cash'])
    cash_int = data[0]['cash']

    symbols = {}
    total_amount = 0
    for row in shares:
        data1 = lookup(row['symbol'])
        symbol = data1['symbol']
        price = data1['price']
        symbols[symbol] = price
        row['price'] = price
        total_amount += (row['price'] * row['amount'])

    total_amount = usd(total_amount + cash_int)
    for row in shares:
        row['value'] = usd(row['price'] * row['amount'])
        row['price'] = usd(row['price'])

    return render_template("index.html", shares=shares, cash=cash, length=lengthS, symbol=symbols, total=total_amount)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # When requested via GET, should display form to buy a stock.
    if request.method == "GET":
        return render_template("buy.html")
    # When form is submitted via POST, purchase the stock so long as the user can afford it
    else:
        symbol = request.form.get('symbol')
        shares = request.form.get('shares')
        data = lookup(symbol)
        if data is None:
            return apology("The stock name is not valid", 667)
        if int(shares) < 1:
            return apology("Number of share should be > 0",)
        # check if the user can afford it amount of money - price * shares > 0
        # Add a table to the database to store how many share the user owns.
        name = data['name']
        price = data['price']
        user_id = session["user_id"]
        data_cash = db.execute(
            'SELECT cash FROM users WHERE id = :user_id', user_id=user_id)
        date = datetime.datetime.now()
        print(date)
        for cash in data_cash:
            amount = cash['cash']
            if amount < (int(shares) * price):
                return apology("Insuficient Founds")
            else:
                if not db.execute("SELECT * FROM shares WHERE id=:id AND symbol=:symbol", id=user_id, symbol=symbol):
                    db.execute("INSERT INTO shares (id, symbol, amount, name) VALUES (:id, :symbol, :amount, :name)",
                               id=user_id, symbol=symbol, amount=shares, name=name)
                else:
                    db.execute("UPDATE shares SET amount=amount+:more WHERE symbol=:symbol AND id=:id",
                               more=shares, symbol=symbol, id=user_id)
                money_left = amount - round(int(shares) * price)
                db.execute("UPDATE users SET cash =:amount WHERE id=:id",
                           amount=money_left, id=user_id)
                db.execute("INSERT INTO history (id, symbol, amount, price, DateCreated) VALUES (:id, :symbol, :amount, :price, :date )",
                           id=user_id, symbol=symbol, amount=shares, price=price, date=date)

    return redirect('/')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    data = db.execute("SELECT * FROM history WHERE id=:id", id=user_id)
    for row in data:
        row['price'] = usd(row['price'])
    print(data)

    return render_template("history.html", data=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # When requested via GET, should diplay form to request a stock quote.
    if request.method == "GET":
        return render_template("quote.html")

    # When form is submited via Post, lookup the stock symbol by calling the lookup
    else:
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("The symbol is not valid")

        data = lookup(symbol)

        if data == None:
            return apology("The symbol is not valid")

        display = f"A share of {data['name']} ({data['symbol']}) costs {usd(data['price'])}"
        return render_template('quoted.html', display=display)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # When requested via GET, sould diplay registration form.
    if request.method == "GET":
        return render_template("registration.html")
    else:
        # When form is submitted via POST, insert the new user into users table.
        # Be sure to check for invalit inputs, and to hash the user's password
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username or not password:
            return apology("You must enter username and password")

        elif password != confirmation:
            return apology("Password and confirmation are not same")

        userList = db.execute("Select username FROM users")
        for user in userList:
            if username in userList:
                return apology("User already registered", 888)
        else:
            pHash = generate_password_hash(password, salt_length=8)
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :pHash)",
                       username=username, pHash=pHash)
            return redirect("/login")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # When requested via GET, should display form to sell a stock
    user_id = session["user_id"]
    data = db.execute('SELECT * FROM shares WHERE id=:id', id=user_id)

    symbols = []
    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    dateNow = datetime.datetime.now()

    for row in data:
        symbols.append(row['symbol'])

    if request.method == "POST":
        if not shares or int(shares) < 1:
            return apology("You have to enter a valid value", 888)
        for row in data:
            if symbol in row['symbol']:
                if int(shares) <= row['amount']:
                    shareNeg = - int(shares)
                    print(shareNeg)
                    db.execute("UPDATE shares SET amount = amount - :shares WHERE id=:id AND symbol = :symbol",
                               shares=shares, id=user_id, symbol=symbol)
                    price = lookup(symbol)
                    amount = int(price['price']) * int(shares)
                    db.execute(
                        "UPDATE users SET cash = cash + :amount WHERE id=:id", amount=amount, id=user_id)
                    db.execute("INSERT INTO history (id, symbol, amount, price, DateCreated) VALUES (:id, :symbol, :amount, :price, :date )",
                               id=user_id, symbol=symbol, amount=shareNeg, price=(price['price']), date=dateNow)
                    db.execute("DELETE FROM shares WHERE amount = 0")
                else:
                    return apology("Insuficient Shares", 888)

    else:
        return render_template('sell.html', symbols=symbols)

    return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
