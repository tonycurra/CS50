# problem set 9: https://cs50.harvard.edu/x/2022/psets/9/finance/

import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# $ export API_KEY=pk_e38d2f58fa384a5699ec53f4fa30161a

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get user id
    user_id = session.get("user_id")

    # Get user cash balance
    cash_list_dict = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash_list_dict[0]["cash"]

    # Get user tot value, cash + stocks. Will be updated in the for loop below
    grand_total = cash

    # Get user stocks from transaction table (it's a list of dict)
    stocks = db.execute("SELECT symbol, SUM(shares) FROM transactions GROUP BY symbol HAVING user_id = ?", user_id)

    # Add to symbols key value pairs for every dict containing different stocks
    for item in stocks:
        symbol = item["symbol"]
        quote = lookup(symbol)
        item["price"] = usd(quote["price"])
        shares = item["SUM(shares)"]
        item["tot_value"] = usd(round((quote["price"] * shares), 2))
        grand_total += round((quote["price"] * shares), 2)

    return render_template("index.html", stocks=stocks, grand_total=usd(grand_total), cash=usd(cash))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Ensure symbol is provided
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure symbol is valid
        elif lookup(request.form.get("symbol")) == None:
            return apology("Symbol not valid", 400)

        # Ensure number of shares is integer
        shares = int(request.form.get("shares"))

        if isinstance(shares, int) == False:
            return apology("Shares must be a integer number", 400)

        # Save users data in variables
        user_id = session.get("user_id")

        # Stock data
        quote = lookup(request.form.get("symbol"))
        stock_name = quote["name"]
        stock_price = quote["price"]

        # Ensure user has enough funds
        # db.execute returns a list of dict, we want to get the int value inside the dict
        cash_list_dict = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = cash_list_dict[0]["cash"]
        price = stock_price * shares
        if user_cash < price:
            return apology("You don't have enough funds", 403)

        # Update user's cash
        user_cash = user_cash - price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash, user_id)

        # INSERT data to transactions table
        timestamp = datetime.now()
        action = "buy"
        symbol = request.form.get("symbol").upper()
        db.execute("INSERT INTO transactions (user_id, timestamp, action, symbol, stock_price, shares, price) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   user_id, timestamp, action, symbol, stock_price, shares, price)
        return render_template("bought.html", stock_name=stock_name, stock_price=usd(stock_price), symbol=symbol, shares=shares, price=usd(price))

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session.get("user_id")
    history = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY id DESC", user_id)
    return render_template("history.html", history=history)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
    if request.method == "POST":
        # Ensure symbol is provided
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure symbol is valid
        elif lookup(request.form.get("symbol")) == None:
            return apology("Symbol not valid", 400)
        #
        else:
            quote = lookup(request.form.get("symbol"))
            name = quote["name"]
            price = usd(quote["price"])
            symbol = quote["symbol"]
            return render_template("quoted.html", name=name, price=price, symbol=symbol)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username doesn't exist already
        if len(rows) != 0:
            return apology("Username already exists", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("Must provide password", 400)

        # Ensure password confirmation matches
        elif request.form.get("confirmation") != request.form.get("password") or not request.form.get("confirmation"):
            return apology("Passwords don't match, try again", 400)

        # Hash password
        hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)

        # Insert user and hash in table
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get("username"), hash)
        return render_template("success.html")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # put a (-) on shares, so you'll have an updated sum
    if request.method == "POST":
        # Ensure symbol is provided
        if not request.form.get("symbol"):
            return apology("Must provide symbol", 403)

        # Ensure symbol is valid
        elif lookup(request.form.get("symbol")) == None:
            return apology("Symbol not valid", 403)

        # Save Symbol into variable for simpler use
        symbol = request.form.get("symbol").upper()

        # Ensure number of shares is integer
        shares = int(request.form.get("shares"))
        if isinstance(shares, int) == False:
            return apology("Shares must be a integer number", 403)

        # Save users data in variables
        user_id = session.get("user_id")

        # Ensure user has number of shares
        user_shares = db.execute("SELECT SUM(shares) FROM transactions WHERE user_id = ? AND symbol = ?", user_id, symbol)
        usr_shares = user_shares[0]["SUM(shares)"]
        if usr_shares < shares:
            return apology("You don't have enough shares", 403)
        elif usr_shares == 0:
            return apology("You don't have any shares of this stock", 403)

        # Stock data
        quote = lookup(request.form.get("symbol"))
        stock_name = quote["name"]
        stock_price = quote["price"]

        # GET user cash value
        # db.execute returns a list of dict, we want to get the int value inside the dict
        cash_list_dict = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = cash_list_dict[0]["cash"]
        price = stock_price * shares

        # Update user's cash
        user_cash = user_cash + price
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   user_cash, user_id)

        # INSERT data to transactions table
        timestamp = datetime.now()
        action = "sell"
        sold_shares = - shares

        db.execute("INSERT INTO transactions (user_id, timestamp, action, symbol, stock_price, shares, price) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   user_id, timestamp, action, symbol, stock_price, sold_shares, price)
        return render_template("sold.html", stock_name=stock_name, stock_price=usd(stock_price), symbol=symbol, shares=shares, price=usd(price))

    else:
        return render_template("sell.html")

