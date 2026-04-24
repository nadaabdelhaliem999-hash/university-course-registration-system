import pytest
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from app import app


@pytest.fixture
def client():
    # Create database tables before each test
  
    
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
    response = client.post("/login", data={
        "email": "wrong@email.com",
        "password": "wrongpass"
    })
    assert response.status_code == 200
