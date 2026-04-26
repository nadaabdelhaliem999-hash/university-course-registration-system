import os
import sys
from flask import Flask, render_template, request, redirect, session
from database import get_connection, init_db
from werkzeug.security import generate_password_hash, check_password_hash

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

app = Flask(__name__)


# CONSTANTS - Fixes the duplicate literal code smell

LOGIN_URL = "/login"
REGISTER_URL = "/register"
COURSES_URL = "/courses"
HOME_URL = "/"


# SECURITY - Fixes hardcoded secret key code smell

app.secret_key = os.environ.get("SECRET_KEY", os.urandom(24))

# Initialize DB
init_db()



# HOME ROUTE

@app.route(HOME_URL, methods=["GET"])
def home():
    return redirect(LOGIN_URL)  


# REGISTER ROUTE

@app.route(REGISTER_URL, methods=["GET", "POST"])
def register():
    error = None

    if request.method == "POST":
        name = request.form["name"].strip()
        email = request.form["email"].strip()
        password = request.form["password"].strip()

        if not name or not email or not password:
            error = "All fields are required."
        elif "@" not in email or "." not in email:
            error = "Please enter a valid email address."
        elif len(password) < 6:
            error = "Password must be at least 6 characters."
        elif len(name) < 2:
            error = "Name must be at least 2 characters."
        else:
            conn = get_connection()

            existing = conn.execute(
                "SELECT id FROM users WHERE email = ?", (email,)
            ).fetchone()

            if existing:
                error = "This email is already registered."
            else:
               
                hashed_password = generate_password_hash(password)
                conn.execute(
                    "INSERT INTO users (name, email, password) VALUES (?, ?, ?)",
                    (name, email, hashed_password)  
                )
                conn.commit()
                conn.close()
                return redirect(LOGIN_URL) 
            conn.close()

    return render_template("register.html", error=error)



# LOGIN ROUTE

@app.route(LOGIN_URL, methods=["GET", "POST"])
def login():
    error = None

    if request.method == "POST":
        email = request.form["email"].strip()
        password = request.form["password"].strip()

        if not email or not password:
            error = "Please enter your email and password."
        else:
            conn = get_connection()
h
            user = conn.execute(
                "SELECT * FROM users WHERE email = ?", (email,)
            ).fetchone()

            conn.close()

            if user and check_password_hash(user["password"], password):
                session["user_id"] = user["id"]
                session["user_name"] = user["name"]
                return redirect(COURSES_URL)  
            else:
                error = "Wrong email or password."

    return render_template("login.html", error=error)


# LOGOUT ROUTE

@app.route("/logout", methods=["GET"])
def logout():
    session.clear()
    return redirect(LOGIN_URL)  
    
if __name__ == "__main__":
    app.run(debug=True) 





if __name__ == "__main__":
    app.run(debug=True)
