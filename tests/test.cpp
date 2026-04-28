#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "user.h"

using namespace std;

// ─── Helper: redirect cin and cout ────────
string runWithInput(const string& fakeInput, function<void()> func) {
    istringstream input(fakeInput);
    ostringstream output;
    streambuf* oldCin  = cin.rdbuf(input.rdbuf());
    streambuf* oldCout = cout.rdbuf(output.rdbuf());
    func();
    cin.rdbuf(oldCin);
    cout.rdbuf(oldCout);
    return output.str();
}

int passed = 0;
int failed = 0;

#define CHECK(desc, expr)                              \
    do {                                               \
        if (expr) {                                    \
            cout << "[PASS] " << desc << "\n";         \
            ++passed;                                  \
        } else {                                       \
            cout << "[FAIL] " << desc << "\n";         \
            ++failed;                                  \
        }                                              \
    } while (false)

// ══════════════════════════════════════════
//  SPRINT 1 — isValidEmail
// ══════════════════════════════════════════
void test_isValidEmail() {
    cout << "\n--- isValidEmail ---\n";
    CHECK("valid email",              isValidEmail("test@gmail.com"));
    CHECK("another valid email",      isValidEmail("a@b.com"));
    CHECK("missing @ is invalid",     !isValidEmail("wrongemail.com"));
    CHECK("empty string is invalid",  !isValidEmail(""));
    CHECK("@ only is valid",          isValidEmail("@"));
}

// ══════════════════════════════════════════
//  SPRINT 1 — isValidPassword
// ══════════════════════════════════════════
void test_isValidPassword() {
    cout << "\n--- isValidPassword ---\n";
    CHECK("exactly 6 chars",          isValidPassword("123456"));
    CHECK("more than 6 chars",        isValidPassword("longpassword"));
    CHECK("5 chars is too short",     !isValidPassword("12345"));
    CHECK("empty string is invalid",  !isValidPassword(""));
    CHECK("1 char is invalid",        !isValidPassword("x"));
}

// ══════════════════════════════════════════
//  SPRINT 1 — emailAlreadyExists
// ══════════════════════════════════════════
void test_emailAlreadyExists() {
    cout << "\n--- emailAlreadyExists ---\n";
    vector<User> users = {{"alice@x.com","pass1"},{"bob@x.com","pass2"}};
    CHECK("finds alice",              emailAlreadyExists(users, "alice@x.com"));
    CHECK("finds bob",                emailAlreadyExists(users, "bob@x.com"));
    CHECK("carol not found",          !emailAlreadyExists(users, "carol@x.com"));
    CHECK("empty list = false",       !emailAlreadyExists({}, "alice@x.com"));
    CHECK("case-sensitive",           !emailAlreadyExists(users, "ALICE@x.com"));
}

// ══════════════════════════════════════════
//  SPRINT 1 — authenticateUser
// ══════════════════════════════════════════
void test_authenticateUser() {
    cout << "\n--- authenticateUser ---\n";
    vector<User> users = {{"alice@x.com","secret"},{"bob@x.com","1234567"}};
    CHECK("correct credentials",       authenticateUser(users,"alice@x.com","secret"));
    CHECK("second user correct",       authenticateUser(users,"bob@x.com","1234567"));
    CHECK("wrong password",           !authenticateUser(users,"alice@x.com","wrong"));
    CHECK("wrong email",              !authenticateUser(users,"carol@x.com","secret"));
    CHECK("empty list = false",       !authenticateUser({},"alice@x.com","secret"));
    CHECK("empty password = false",   !authenticateUser(users,"alice@x.com",""));
}

// ══════════════════════════════════════════
//  SPRINT 1 — handleRegister
// ══════════════════════════════════════════
void test_handleRegister() {
    cout << "\n--- handleRegister ---\n";

    // 1. Successful registration
    {
        vector<User> users;
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){ handleRegister(users); });
        CHECK("register success message",  out.find("SUCCESS") != string::npos);
        CHECK("user added to list",        users.size() == 1);
        CHECK("email stored correctly",    users[0].email == "alice@x.com");
    }

    // 2. Invalid email
    {
        vector<User> users;
        string out = runWithInput("notanemail\n", [&](){ handleRegister(users); });
        CHECK("invalid email rejected",    out.find("Invalid email") != string::npos);
        CHECK("no user added",             users.empty());
    }

    // 3. Password too short
    {
        vector<User> users;
        string out = runWithInput("user@x.com\n123\n", [&](){ handleRegister(users); });
        CHECK("short password rejected",   out.find("6 characters") != string::npos);
        CHECK("no user added",             users.empty());
    }

    // 4. Duplicate email
    {
        vector<User> users = {{"alice@x.com","secret123"}};
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){ handleRegister(users); });
        CHECK("duplicate email rejected",  out.find("already registered") != string::npos);
        CHECK("list size unchanged",       users.size() == 1);
    }
}

// ══════════════════════════════════════════
//  SPRINT 1 — handleLogin
// ══════════════════════════════════════════
void test_handleLogin() {
    cout << "\n--- handleLogin ---\n";
    vector<User> users = {{"alice@x.com","secret123"}};

    // 1. Correct credentials
    {
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){ handleLogin(users); });
        CHECK("login success message",     out.find("Welcome back") != string::npos);
    }

    // 2. Wrong password
    {
        string out = runWithInput("alice@x.com\nwrongpass\n", [&](){ handleLogin(users); });
        CHECK("wrong password rejected",   out.find("Wrong email or password") != string::npos);
    }

    // 3. Wrong email
    {
        string out = runWithInput("nobody@x.com\nsecret123\n", [&](){ handleLogin(users); });
        CHECK("unknown email rejected",    out.find("Wrong email or password") != string::npos);
    }

    // 4. Empty user list
    {
        vector<User> empty;
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){ handleLogin(empty); });
        CHECK("empty list = wrong creds",  out.find("Wrong email or password") != string::npos);
    }
}

// ══════════════════════════════════════════
//  SPRINT 2 — insertSampleCourses
// ══════════════════════════════════════════
void test_insertSampleCourses() {
    cout << "\n--- insertSampleCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    CHECK("8 courses inserted",         courses.size() == 8);
    CHECK("first course name",          courses[0].name == "Mathematics 101");
    CHECK("first course instructor",    courses[0].instructor == "Dr. Ahmed");
    CHECK("first course capacity",      courses[0].capacity == 30);
    CHECK("last course id is 8",        courses[7].id == 8);
    bool allPositive = true;
    for (const Course& c : courses) {
        if (c.capacity <= 0) allPositive = false;
    }
    CHECK("all capacities positive",    allPositive);
}

// ══════════════════════════════════════════
//  SPRINT 2 — displayCourses
// ══════════════════════════════════════════
void test_displayCourses() {
    cout << "\n--- displayCourses ---\n";

    // Empty list branch
    {
        vector<Course> empty;
        string out = runWithInput("", [&](){ displayCourses(empty); });
        CHECK("empty list message",       out.find("No courses available") != string::npos);
    }

    // Normal list branch
    {
        vector<Course> courses;
        insertSampleCourses(courses);
        string out = runWithInput("", [&](){ displayCourses(courses); });
        CHECK("shows AVAILABLE COURSES",  out.find("AVAILABLE COURSES") != string::npos);
        CHECK("shows course name",        out.find("Mathematics 101") != string::npos);
        CHECK("shows instructor",         out.find("Dr. Ahmed") != string::npos);
        CHECK("shows capacity number",    out.find("30") != string::npos);
    }
}

// ══════════════════════════════════════════
//  SPRINT 2 — searchCourses
// ══════════════════════════════════════════
void test_searchCourses() {
    cout << "\n--- searchCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);

    {
        string out = runWithInput("", [&](){ searchCourses(courses, "math"); });
        CHECK("finds math course",        out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "ahmed"); });
        CHECK("finds by instructor",      out.find("Dr. Ahmed") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "MATH"); });
        CHECK("case-insensitive search",  out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "xyznotfound"); });
        CHECK("no results message",       out.find("No courses found") != string::npos);
    }
}

// ══════════════════════════════════════════
//  SPRINT 2 — handleViewCourses
// ══════════════════════════════════════════
void test_handleViewCourses() {
    cout << "\n--- handleViewCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    string out = runWithInput("", [&](){ handleViewCourses(courses); });
    CHECK("view shows courses",           out.find("AVAILABLE COURSES") != string::npos);
}

// ══════════════════════════════════════════
//  SPRINT 2 — handleSearchCourses
// ══════════════════════════════════════════
void test_handleSearchCourses() {
    cout << "\n--- handleSearchCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);

    {
        string out = runWithInput("math\n", [&](){ handleSearchCourses(courses); });
        CHECK("valid search finds result",    out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("\n", [&](){ handleSearchCourses(courses); });
        CHECK("empty keyword shows error",    out.find("Please enter a keyword") != string::npos);
    }
    {
        string out = runWithInput("xyznotfound\n", [&](){ handleSearchCourses(courses); });
        CHECK("no match shows message",       out.find("No courses found") != string::npos);
    }
}

// ══════════════════════════════════════════
//  Main
// ══════════════════════════════════════════
int main() {
    cout << "========== SPRINT 1 TESTS ==========\n";
    test_isValidEmail();
    test_isValidPassword();
    test_emailAlreadyExists();
    test_authenticateUser();
    test_handleRegister();
    test_handleLogin();

    cout << "\n========== SPRINT 2 TESTS ==========\n";
    test_insertSampleCourses();
    test_displayCourses();
    test_searchCourses();
    test_handleViewCourses();
    test_handleSearchCourses();

    cout << "\n=====================================\n";
    cout << "Results: " << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
