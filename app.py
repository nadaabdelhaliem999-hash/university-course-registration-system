# app. Py
from flask import Flask, render_template, request, redirect, url_for, session
from Database import get_connection, create_users_table

app = Flask(__name__)
app.secret_key = "university_secret_key_2025"

@app.route("/register", methods=["GET", "POST"])
def register():
    error = None
    connection = None
    if request.method == "POST":
        print("PASSWORD ERROR TRIGGERED")
        name = request.form.get("name", "").strip()
        email = request.form.get("email", "").strip()
        password = request.form.get("password", "").strip()

        if not name or not email or not password:
            error = "All fields are required."
            return render_template("register.html", error=error)

        elif "@" not in email or "." not in email:
            error = "Please enter a valid email address."
            return render_template("register.html", error=error)
            

        elif len(password) < 6:
            error = "Password must be at least 6 characters."
            print("PASSWORD ERROR TRIGGERED") 
            return render_template("register.html", error=error)

        elif len(name) < 2:
            error = "Name must be at least 2 characters."
            return render_template("register.html", error=error)

        else:
            connection = get_connection()
            existing = connection.execute(
                "SELECT id FROM users WHERE email = ?", (email,)
                ).fetchone()

            if existing:
                error = "Email already registered."
                return render_template("register.html", error=error)
            else:
                connection.execute(
                    "INSERT INTO users (name, email, password) VALUES (?, ?, ?)",
                    (name, email, password)
                )
                connection.commit()
                connection.close()
                return redirect("/login")
    if connection:
        connection.close()

    return render_template("register.html", error=error)


@app.route("/")
def home():
    return redirect("/login")

@app.route("/login", methods=["GET", "POST"])
def login():
    error = None
    if request.method == "POST":
        email    = request.form["email"].strip()
        password = request.form["password"].strip()

        if not email or not password:
            error = "Please enter email and password."
        else:
            connection = get_connection()
            user = connection.execute(
                "SELECT * FROM users WHERE email = ? AND password = ?",
                (email, password)
            ).fetchone()
            connection.close()

            if user:
                session["user_id"]   = user["id"]
                session["user_name"] = user["name"]
                return redirect("/courses")
            else:
                error = "Wrong email or password."

    return render_template("login.html", error=error)

@app.route("/logout")
def logout():
    session.clear()
    return redirect("/login")    

if __name__ == "__main__":
    create_users_table()
    app.run(debug=True)
