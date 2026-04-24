import sqlite3

# Connect to database
def get_connection():
    connection = sqlite3.connect("university.db")
    connection.row_factory = sqlite3.Row
    return connection

# Create users table
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
    print("Users table created!")

if __name__ == "__main__":
    create_users_table()
