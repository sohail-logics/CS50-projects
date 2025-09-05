import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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

    # query user’s owned stocks (sum of shares per symbol)
    rows = db.execute(
        "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0",
        session["user_id"]
    )

    holdings = []
    total = 0

    for row in rows:
        symbol = row["symbol"]
        shares = row["total_shares"]
        quote = lookup(symbol)

        if not quote:
            continue

        price = float(quote["price"])
        value = shares * price
        total += value

        holdings.append({
            "symbol": symbol,
            "shares": shares,
            "price": usd(price),
            "value": usd(value)
        })

    # get user’s cash
    user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = float(user[0]["cash"])
    total += cash

    return render_template("index.html", holdings=holdings, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # get form data
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # validate symbol
        if not symbol or symbol.strip() == "":
            return apology("must provide stock symbol", 400)

        # look up stock
        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)

        # validate shares (positive integer)
        try:
            shares = int(shares)
            if shares <= 0:
                raise ValueError
        except:
            return apology("invalid share count", 400)

        # get user cash
        user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = float(user[0]["cash"])

        price = float(quote["price"])
        total_cost = price * shares

        # ensure can afford
        if cash < total_cost:
            return apology("can't afford", 400)

        # deduct cash
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_cost, session["user_id"])

        # record transaction (positive shares for buy)
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
            session["user_id"], quote["symbol"], shares, price
        )

        flash("Bought!")
        return redirect("/")

    else:
        return render_template("buy.html")




@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]

    transactions = db.execute(
        "SELECT symbol, shares, price, transacted FROM transactions WHERE user_id = ? ORDER BY transacted DESC",
        user_id,
    )

    return render_template("history.html", transactions=transactions)



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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        symbol = request.form.get("symbol")

        # Ensure symbol was provided
        if not symbol:
            return apology("must provide stock symbol", 400)

        # Lookup stock symbol
        quote = lookup(symbol)

        # Ensure stock symbol exists
        if not quote:
            return apology("invalid symbol", 400)

        # Show result
        return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=quote["price"])

    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        if not password:
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        if not confirmation:
            return apology("must provide password confirmation", 400)

        # Ensure passwords match
        if password != confirmation:
            return apology("passwords do not match", 400)

        # Insert new user into database
        try:
            new_user_id = db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)",
                username,
                generate_password_hash(password),
            )
        except ValueError:
            return apology("username already exists", 400)

        # Remember which user has logged in
        session["user_id"] = new_user_id

        # Redirect to home page
        return redirect("/")

    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # validate symbol
        if not symbol or symbol.strip() == "":
            return apology("must select a symbol", 400)

        # validate shares
        try:
            shares = int(shares)
            if shares <= 0:
                raise ValueError
        except:
            return apology("invalid share count", 400)

        # check how many shares user owns of that symbol
        row = db.execute(
            "SELECT COALESCE(SUM(shares), 0) AS total_shares FROM transactions WHERE user_id = ? AND symbol = ?",
            session["user_id"], symbol
        )
        owned = int(row[0]["total_shares"])

        if shares > owned:
            return apology("too many shares", 400)

        # lookup current price
        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)

        price = float(quote["price"])
        proceeds = price * shares

        # add cash
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", proceeds, session["user_id"])

        # record transaction (negative shares for sell)
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
            session["user_id"], quote["symbol"], -shares, price
        )

        flash("Sold!")
        return redirect("/")

    else:
        # get symbols the user currently owns (sum > 0)
        symbols = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0 ORDER BY symbol",
            session["user_id"]
        )
        return render_template("sell.html", symbols=symbols)


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    """Allow user to add cash to account"""
    user_id = session["user_id"]

    if request.method == "POST":
        try:
            amount = float(request.form.get("amount"))
        except (TypeError, ValueError):
            return apology("invalid amount", 400)

        if amount <= 0:
            return apology("amount must be positive", 400)

        # Update cash in users table
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", amount, user_id)

        flash(f"Successfully added ${amount:,.2f} to your account!")
        return redirect("/")

    else:
        return render_template("addcash.html")


