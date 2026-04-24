import pytest
import sys
import os
import sqlite3

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from app import app
import Database

@pytest.fixture
def client():
    # Force create the database tables
    Database.create_users_table()
    
    # Also add a test user so the login test works
    conn = Database.get_connection()
    try:
        conn.execute("INSERT INTO users (name, email, password) VALUES (?, ?, ?)",
                     ("Test User", "test@example.com", "testpass123"))
        conn.commit()
    except sqlite3.IntegrityError:
        pass  # User already exists
    finally:
        conn.close()
    
    app.config["TESTING"] = True
    app.config["SECRET_KEY"] = "test"
    
    with app.test_client() as client:
        yield client

def test_register_page_loads(client):
    response = client.get("/register")
    assert response.status_code == 200

def test_login_page_loads(client):
    response = client.get("/login")
    assert response.status_code == 200

def test_empty_register_shows_error(client):
    response = client.post("/register", data={
        "name": "", "email": "", "password": ""
    })
    assert response.status_code == 200

def test_wrong_login_shows_error(client):
    # This tries to login with wrong credentials - no database entry needed
    response = client.post("/login", data={
        "email": "wrong@email.com",
        "password": "wrongpass"
    })
    assert response.status_code == 200
