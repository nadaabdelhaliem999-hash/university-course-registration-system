import pytest
import sys
import os

# Tell Python where to find app.py and database.py
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

# Use memory database for testing
os.environ["DATABASE"] = ":memory:"

from app import app as flask_app
from database import create_users_table

@pytest.fixture
def client():
    flask_app.config["TESTING"] = True
    flask_app.config["SECRET_KEY"] = "test"

    # Create tables in memory database before each test
    create_users_table()

    with flask_app.test_client() as client:
        yield client

# ── Page Load Tests ──────────────────────
def test_register_page_loads(client):
    """Register page opens correctly"""
    response = client.get("/register")
    assert response.status_code == 200

def test_login_page_loads(client):
    """Login page opens correctly"""
    response = client.get("/login")
    assert response.status_code == 200

def test_home_redirects(client):
    """Home redirects to login"""
    response = client.get("/")
    assert response.status_code == 302

def test_logout_redirects(client):
    """Logout redirects to login"""
    response = client.get("/logout")
    assert response.status_code == 302

# ── Register Tests ───────────────────────
def test_register_empty_form(client):
    """Empty form shows error"""
    response = client.post("/register", data={
        "name": "", "email": "", "password": ""
    })
    assert response.status_code == 200

def test_register_invalid_email(client):
    """Invalid email shows error"""
    response = client.post("/register", data={
        "name": "Test", "email": "notanemail", "password": "123456"
    })
    assert response.status_code == 200

def test_register_short_password(client):
    """Short password shows error"""
    response = client.post("/register", data={
        "name": "Test", "email": "test@test.com", "password": "123"
    })
    assert response.status_code == 200

def test_register_success(client):
    """Valid registration works"""
    response = client.post("/register", data={
        "name": "Test User",
        "email": "test@test.com",
        "password": "123456"
    })
    assert response.status_code == 302

# ── Login Tests ──────────────────────────
def test_login_empty_form(client):
    """Empty login form shows error"""
    response = client.post("/login", data={
        "email": "", "password": ""
    })
    assert response.status_code == 200

def test_login_wrong_credentials(client):
    """Wrong credentials shows error"""
    response = client.post("/login", data={
        "email": "wrong@email.com",
        "password": "wrongpassword"
    })
    assert response.status_code == 200
