import sqlite3
import os


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


# NEW: Create courses table for Sprint 2

def create_courses_table():
    connection = get_connection()
    connection.execute("""
        CREATE TABLE IF NOT EXISTS courses (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            code        TEXT    NOT NULL UNIQUE,
            name        TEXT    NOT NULL,
            instructor  TEXT    NOT NULL,
            credits     INTEGER NOT NULL,
            description TEXT
        )
    """)
    connection.commit()
    connection.close()


# NEW: Insert sample course data

def insert_sample_courses():
    connection = get_connection()
    
    # Sample courses data
    sample_courses = [
        ("CS101", "Introduction to Programming", "Dr. Ahmed Mansour", 3, "Learn basics of programming with Python"),
        ("CS201", "Data Structures", "Dr. Sara Khalil", 4, "Arrays, linked lists, trees, and graphs"),
        ("CS301", "Database Systems", "Dr. Mohamed Ali", 3, "SQL, database design, and normalization"),
        ("CS401", "Web Development", "Dr. Nada Hassan", 3, "HTML, CSS, JavaScript, and Flask"),
        ("CS501", "Software Engineering", "Dr. Khaled Ibrahim", 4, "Software development lifecycle and Agile"),
        ("MATH101", "Calculus I", "Dr. Yasmine Fathy", 3, "Limits, derivatives, and integrals"),
        ("MATH201", "Linear Algebra", "Dr. Omar Samir", 3, "Vectors, matrices, and linear transformations"),
        ("ENG101", "Technical Writing", "Prof. Laila Ahmed", 2, "Writing technical documentation and reports"),
        ("PHY101", "Physics I", "Dr. Hany Mostafa", 4, "Mechanics, thermodynamics, and waves"),
        ("AI401", "Artificial Intelligence", "Dr. Amr Sherif", 3, "Machine learning, neural networks, and AI algorithms")
    ]
    
    for course in sample_courses:
        try:
            connection.execute("""
                INSERT OR IGNORE INTO courses (code, name, instructor, credits, description)
                VALUES (?, ?, ?, ?, ?)
            """, course)
        except Exception as e:
            print(f"Error inserting course {course[0]}: {e}")
    
    connection.commit()
    connection.close()

def init_db():
    """Initialize the database - call this when app starts"""
    create_users_table()
    create_courses_table()      # NEW: Create courses table
    insert_sample_courses()     # NEW: Insert sample data

if __name__ == "__main__":
    init_db()
    print("Database initialized with users and courses tables!")
