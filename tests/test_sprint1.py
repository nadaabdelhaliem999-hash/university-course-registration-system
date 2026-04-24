import pytest
import sqlite3

@pytest.fixture
def setup_database():
    """Create test database with users table before each test"""
    conn = sqlite3.connect(':memory:')  # or your test db path
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY,
            name TEXT,
            email TEXT UNIQUE,
            password TEXT
        )
    ''')
    conn.commit()
    yield conn
    conn.close()

def test_wrong_login_shows_error(setup_database):
    # Now the users table exists
    connection = setup_database
    # rest of your test...
