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
//  SPRINT 1 TESTS
// ══════════════════════════════════════════
void test_isValidEmail() {
    cout << "\n--- isValidEmail ---\n";
    CHECK("valid email",             isValidEmail("test@gmail.com"));
    CHECK("missing @ is invalid",    !isValidEmail("wrongemail.com"));
    CHECK("empty string is invalid", !isValidEmail(""));
}

void test_isValidPassword() {
    cout << "\n--- isValidPassword ---\n";
    CHECK("exactly 6 chars",         isValidPassword("123456"));
    CHECK("more than 6 chars",       isValidPassword("longpassword"));
    CHECK("5 chars is too short",    !isValidPassword("12345"));
    CHECK("empty string is invalid", !isValidPassword(""));
}

void test_emailAlreadyExists() {
    cout << "\n--- emailAlreadyExists ---\n";
    vector<User> users = {{"alice@x.com","pass1",{}},{"bob@x.com","pass2",{}}};
    CHECK("finds alice",             emailAlreadyExists(users, "alice@x.com"));
    CHECK("carol not found",         !emailAlreadyExists(users, "carol@x.com"));
    CHECK("empty list = false",      !emailAlreadyExists({}, "alice@x.com"));
}

void test_authenticateUser() {
    cout << "\n--- authenticateUser ---\n";
    vector<User> users = {{"alice@x.com","secret",{}}};
    CHECK("correct credentials",     authenticateUser(users,"alice@x.com","secret"));
    CHECK("wrong password",          !authenticateUser(users,"alice@x.com","wrong"));
    CHECK("wrong email",             !authenticateUser(users,"carol@x.com","secret"));
    CHECK("empty list = false",      !authenticateUser({},"alice@x.com","secret"));
}

void test_handleRegister() {
    cout << "\n--- handleRegister ---\n";

    // Success
    {
        vector<User> users;
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){ handleRegister(users); });
        CHECK("register success",        out.find("SUCCESS") != string::npos);
        CHECK("user added to list",      users.size() == 1);
    }
    // Invalid email
    {
        vector<User> users;
        string out = runWithInput("notanemail\n", [&](){ handleRegister(users); });
        CHECK("invalid email rejected",  out.find("Invalid email") != string::npos);
        CHECK("no user added",           users.empty());
    }
    // Short password
    {
        vector<User> users;
        string out = runWithInput("user@x.com\n123\n", [&](){ handleRegister(users); });
        CHECK("short password rejected", out.find("6 characters") != string::npos);
    }
    // Duplicate email
    {
        vector<User> users = {{"alice@x.com","secret123",{}}};
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){ handleRegister(users); });
        CHECK("duplicate rejected",      out.find("already registered") != string::npos);
    }
}

void test_handleLogin() {
    cout << "\n--- handleLogin ---\n";
    vector<User> users = {{"alice@x.com","secret123",{}}};
    string loggedInEmail = "";

    // Success
    {
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){
            handleLogin(users, loggedInEmail);
        });
        CHECK("login success",           out.find("Welcome back") != string::npos);
        CHECK("loggedInEmail is set",    loggedInEmail == "alice@x.com");
    }
    // Wrong password
    {
        string out = runWithInput("alice@x.com\nwrongpass\n", [&](){
            handleLogin(users, loggedInEmail);
        });
        CHECK("wrong password rejected", out.find("Wrong email or password") != string::npos);
    }
}

// ══════════════════════════════════════════
//  SPRINT 2 TESTS
// ══════════════════════════════════════════
void test_insertSampleCourses() {
    cout << "\n--- insertSampleCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    CHECK("8 courses inserted",      courses.size() == 8);
    CHECK("first course name",       courses[0].name == "Mathematics 101");
    CHECK("first course instructor", courses[0].instructor == "Dr. Ahmed");
    CHECK("first course capacity",   courses[0].capacity == 30);
}

void test_displayCourses() {
    cout << "\n--- displayCourses ---\n";
    {
        vector<Course> empty;
        string out = runWithInput("", [&](){ displayCourses(empty); });
        CHECK("empty list message",  out.find("No courses available") != string::npos);
    }
    {
        vector<Course> courses;
        insertSampleCourses(courses);
        string out = runWithInput("", [&](){ displayCourses(courses); });
        CHECK("shows course name",   out.find("Mathematics 101") != string::npos);
    }
}

void test_searchCourses() {
    cout << "\n--- searchCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "math"); });
        CHECK("finds by name",       out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "MATH"); });
        CHECK("case insensitive",    out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "xyznotfound"); });
        CHECK("no results message",  out.find("No courses found") != string::npos);
    }
}

void test_handleSearchCourses() {
    cout << "\n--- handleSearchCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    {
        string out = runWithInput("math\n", [&](){ handleSearchCourses(courses); });
        CHECK("valid search works",  out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("\n", [&](){ handleSearchCourses(courses); });
        CHECK("empty keyword error", out.find("Please enter a keyword") != string::npos);
    }
}

// ══════════════════════════════════════════
//  SPRINT 3 TESTS
// ══════════════════════════════════════════
void test_registerForCourse() {
    cout << "\n--- registerForCourse ---\n";

    vector<User> users = {{"alice@x.com","secret",{}}};
    vector<Course> courses;
    insertSampleCourses(courses);
    int originalCapacity = courses[0].capacity;

    // Success
    {
        string out = runWithInput("", [&](){
            registerForCourse(users, "alice@x.com", courses, 1);
        });

        CHECK("register success",
              out.find("SUCCESS") != string::npos);

        CHECK("course added to student",
              users[0].enrolledCourseIds.size() == 1);

        CHECK("capacity decreased",
              courses[0].capacity == originalCapacity - 1);
    }

    // Already enrolled
    {
        string out = runWithInput("", [&](){
            registerForCourse(users, "alice@x.com", courses, 1);
        });

        CHECK("already enrolled error",
              out.find("ERROR: Already registered!") != string::npos);
    }

    // Course not found
    {
        string out = runWithInput("", [&](){
            registerForCourse(users, "alice@x.com", courses, 999);
        });

        CHECK("course not found error",
              out.find("ERROR: Course not found!") != string::npos);
    }

}

void test_dropCourse() {
    cout << "\n--- dropCourse ---\n";
    vector<User> users   = {{"alice@x.com","secret",{1}}};
    vector<Course> courses;
    insertSampleCourses(courses);
    int originalCapacity = courses[0].capacity;

    // Success
    {
        string out = runWithInput("", [&](){
            dropCourse(users, "alice@x.com", courses, 1);
        });
        CHECK("drop success",            out.find("SUCCESS") != string::npos);
        CHECK("course removed",          users[0].enrolledCourseIds.empty());
        CHECK("capacity restored",       courses[0].capacity == originalCapacity + 1);
    }

    // Not enrolled
    {
        string out = runWithInput("", [&](){
            dropCourse(users, "alice@x.com", courses, 1);
        });
        CHECK("not enrolled error",      out.find("not registered") != string::npos);
    }

    // User not found
    {
        string out = runWithInput("", [&](){
            dropCourse(users, "nobody@x.com", courses, 1);
        });
        CHECK("user not found error",    out.find("User not found") != string::npos);
    }
}

void test_viewSchedule() {
    cout << "\n--- viewSchedule ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);

    // Empty schedule
    {
        vector<User> users = {{"alice@x.com","secret",{}}};
        string out = runWithInput("", [&](){
            viewSchedule(users, "alice@x.com", courses);
        });
        CHECK("empty schedule message",  out.find("no courses") != string::npos);
    }

    // Schedule with courses
    {
        vector<User> users = {{"alice@x.com","secret",{1, 2}}};
        string out = runWithInput("", [&](){
            viewSchedule(users, "alice@x.com", courses);
        });
        CHECK("shows MY SCHEDULE",       out.find("MY SCHEDULE") != string::npos);
        CHECK("shows enrolled course",   out.find("Mathematics 101") != string::npos);
    }

    // User not found
    {
        vector<User> users;
        string out = runWithInput("", [&](){
            viewSchedule(users, "nobody@x.com", courses);
        });
        CHECK("user not found error",    out.find("User not found") != string::npos);
    }
}

void test_handleRegisterForCourse() {
    cout << "\n--- handleRegisterForCourse ---\n";
    vector<User> users   = {{"alice@x.com","secret",{}}};
    vector<Course> courses;
    insertSampleCourses(courses);

    string out = runWithInput("1\n", [&](){
        handleRegisterForCourse(users, "alice@x.com", courses);
    });
    CHECK("registered via handler",  users[0].enrolledCourseIds.size() == 1);
}

void test_handleDropCourse() {
    cout << "\n--- handleDropCourse ---\n";
    vector<User> users   = {{"alice@x.com","secret",{1}}};
    vector<Course> courses;
    insertSampleCourses(courses);

    string out = runWithInput("1\n", [&](){
        handleDropCourse(users, "alice@x.com", courses);
    });
    CHECK("dropped via handler",     users[0].enrolledCourseIds.empty());
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
    test_handleSearchCourses();

    cout << "\n========== SPRINT 3 TESTS ==========\n";
    test_registerForCourse();
    test_dropCourse();
    test_viewSchedule();
    test_handleRegisterForCourse();
    test_handleDropCourse();

    cout << "\n=====================================\n";
    cout << "Results: " << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}