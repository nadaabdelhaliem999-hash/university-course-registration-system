import sqlite3
import os

# Use memory database for testing, real file for production
DATABASE = os.environ.get("DATABASE", "university.db")

def get_connection():
    connection = sqlite3.connect(DATABASE)
    connection.row_factory = sqlite3.Row
    return connection

def create_users_table():
    connection = get_connection()
    connection.execute("""
        CREATE TABLE IF NOT EXISTS users (
            id       INTEGER PRIMARY KEY AUTOINCREMENT,
            name     TEXT    NOT NULL,
            email    TEXT    NOT NULL UNIQUE,
            password TEXT    NOT NULL
        )
    """)
    connection.commit()
    connection.close()

def init_db():
    """Initialize the database - call this when app starts"""
    create_users_table()

if __name__ == "__main__":
    init_db()
    print("Users table created!")
