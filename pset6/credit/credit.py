from cs50 import get_string

card = get_string("Number: ") #Get CC Nr.

reverseCard = [int(x) for x in card[::-1]]
listDouble = list()
digits = list(enumerate(reverseCard, start=1))

for index, digit in digits:
    if index % 2 == 0:
        dDigit = digit * 2
        if dDigit > 9:
            listDouble.append(dDigit - 9)
        else:
            listDouble.append(dDigit)
    else:
        listDouble.append(digit)

    cardLen = [13, 15, 16]

if not sum(listDouble) % 10 == 0 or len(card) not in cardLen:
    print("INVALID")
    exit(0)

amex = ['34', '37']
mastercard = ['51', '52', '53', '54', '55']

if card[:2] in amex and len(card) == 15:
    print("AMEX")
elif card[:2] in mastercard and len(card) == 16:
    print("MASTERCARD")
elif len(card) == 16 and card[:1] == '4' or len(card) == 13 and card[:1] == '4':
    print("VISA")
else:
    print("INVALID")




