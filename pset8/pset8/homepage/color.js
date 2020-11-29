document.querySelector("#blink").addEventListener("click", function() {
    setInterval(function() {
        document.querySelector("body").className = "red";
    }, 200);
    setInterval(function() {
        document.querySelector("body").className = "blue";
    }, 300);
    setInterval(function() {
        document.querySelector("body").className = "green";
    }, 400);
});
