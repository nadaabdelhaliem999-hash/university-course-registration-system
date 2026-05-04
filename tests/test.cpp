#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "user.h"

using namespace std;

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

#define CHECK(desc, expr)                         \
    do {                                          \
        if (expr) {                               \
            cout << "[PASS] " << desc << "\n";    \
            ++passed;                             \
        } else {                                  \
            cout << "[FAIL] " << desc << "\n";    \
            ++failed;                             \
        }                                         \
    } while (false)

void test_isValidEmail() {
    cout << "\n--- isValidEmail ---\n";
    CHECK("valid email",             isValidEmail("test@gmail.com"));
    CHECK("missing @ is invalid",    !isValidEmail("wrongemail.com"));
    CHECK("empty string is invalid", !isValidEmail(""));
    CHECK("only @ is valid",         isValidEmail("@"));
    CHECK("multiple @ signs",        isValidEmail("a@b@c.com"));
}

void test_isValidPassword() {
    cout << "\n--- isValidPassword ---\n";
    CHECK("exactly 6 chars",         isValidPassword("123456"));
    CHECK("more than 6 chars",       isValidPassword("longpassword"));
    CHECK("5 chars is too short",    !isValidPassword("12345"));
    CHECK("empty string is invalid", !isValidPassword(""));
    CHECK("7 chars is valid",        isValidPassword("1234567"));
}

void test_emailAlreadyExists() {
    cout << "\n--- emailAlreadyExists ---\n";
    vector<User> users = {{"alice@x.com","pass1",{}},{"bob@x.com","pass2",{}}};
    CHECK("finds alice",             emailAlreadyExists(users, "alice@x.com"));
    CHECK("finds bob",               emailAlreadyExists(users, "bob@x.com"));
    CHECK("carol not found",         !emailAlreadyExists(users, "carol@x.com"));
    CHECK("empty list = false",      !emailAlreadyExists({}, "alice@x.com"));
    CHECK("case sensitive",          !emailAlreadyExists(users, "ALICE@x.com"));
    CHECK("first in list found",     emailAlreadyExists(users, "alice@x.com"));
    CHECK("last in list found",      emailAlreadyExists(users, "bob@x.com"));
}

void test_authenticateUser() {
    cout << "\n--- authenticateUser ---\n";
    vector<User> users = {{"alice@x.com","secret",{}},{"bob@x.com","pass123",{}}};
    CHECK("correct credentials alice",   authenticateUser(users,"alice@x.com","secret"));
    CHECK("correct credentials bob",     authenticateUser(users,"bob@x.com","pass123"));
    CHECK("wrong password",              !authenticateUser(users,"alice@x.com","wrong"));
    CHECK("wrong email",                 !authenticateUser(users,"carol@x.com","secret"));
    CHECK("empty list = false",          !authenticateUser({},"alice@x.com","secret"));
    CHECK("both wrong",                  !authenticateUser(users,"wrong@x.com","wrong"));
    CHECK("email ok password wrong",     !authenticateUser(users,"alice@x.com","notright"));
    CHECK("email wrong password ok",     !authenticateUser(users,"nobody@x.com","secret"));
}

void test_handleRegister() {
    cout << "\n--- handleRegister ---\n";
    {
        vector<User> users;
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){ handleRegister(users); });
        CHECK("register success",        out.find("SUCCESS") != string::npos);
        CHECK("user added to list",      users.size() == 1);
        CHECK("email saved correctly",   users[0].email == "alice@x.com");
    }
    {
        vector<User> users;
        string out = runWithInput("notanemail\n", [&](){ handleRegister(users); });
        CHECK("invalid email rejected",  out.find("Invalid email") != string::npos);
        CHECK("no user added",           users.empty());
    }
    {
        vector<User> users;
        string out = runWithInput("user@x.com\n123\n", [&](){ handleRegister(users); });
        CHECK("short password rejected", out.find("6 characters") != string::npos);
        CHECK("no user added",           users.empty());
    }
    {
        vector<User> users = {{"alice@x.com","secret123",{}}};
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){ handleRegister(users); });
        CHECK("duplicate rejected",      out.find("already registered") != string::npos);
        CHECK("still only 1 user",       users.size() == 1);
    }
    {
        vector<User> users;
        runWithInput("alice@x.com\nsecret123\n", [&](){ handleRegister(users); });
        runWithInput("bob@x.com\npassword1\n",   [&](){ handleRegister(users); });
        CHECK("second user registered",  users.size() == 2);
    }
}

void test_handleLogin() {
    cout << "\n--- handleLogin ---\n";
    vector<User> users = {{"alice@x.com","secret123",{}}};
    string loggedInEmail = "";
    {
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){
            handleLogin(users, loggedInEmail);
        });
        CHECK("login success",           out.find("Welcome back") != string::npos);
        CHECK("loggedInEmail is set",    loggedInEmail == "alice@x.com");
    }
    {
        string out = runWithInput("alice@x.com\nwrongpass\n", [&](){
            handleLogin(users, loggedInEmail);
        });
        CHECK("wrong password rejected", out.find("Wrong email or password") != string::npos);
    }
    {
        string out = runWithInput("nobody@x.com\nsecret123\n", [&](){
            handleLogin(users, loggedInEmail);
        });
        CHECK("wrong email rejected",    out.find("Wrong email or password") != string::npos);
    }
    {
        string out = runWithInput("nobody@x.com\nwrong\n", [&](){
            handleLogin(users, loggedInEmail);
        });
        CHECK("both wrong rejected",     out.find("Wrong email or password") != string::npos);
    }
}

void test_insertSampleCourses() {
    cout << "\n--- insertSampleCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    CHECK("8 courses inserted",      courses.size() == 8);
    CHECK("first course name",       courses[0].name == "Mathematics 101");
    CHECK("first course instructor", courses[0].instructor == "Dr. Ahmed");
    CHECK("first course capacity",   courses[0].capacity == 30);
    CHECK("last course name",        courses[7].name == "Economics 101");
    CHECK("all have positive id",    courses[0].id == 1);
    CHECK("all have schedule",       !courses[0].schedule.empty());
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
        CHECK("shows instructor",    out.find("Dr. Ahmed") != string::npos);
        CHECK("shows schedule",      out.find("Sun/Tue") != string::npos);
        CHECK("shows capacity",      out.find("30") != string::npos);
        CHECK("shows header",        out.find("AVAILABLE COURSES") != string::npos);
    }
}

void test_searchCourses() {
    cout << "\n--- searchCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "math"); });
        CHECK("finds by name only",           out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "MATH"); });
        CHECK("case insensitive name",        out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "ahmed"); });
        CHECK("finds by instructor only",     out.find("Dr. Ahmed") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "xyznotfound"); });
        CHECK("no results message",           out.find("No courses found") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "101"); });
        CHECK("partial match works",          out.find("Mathematics 101") != string::npos);
    }
    {
        // "a" appears in "Mathematics" (name) AND "Dr. Ahmed" (instructor) — covers nameMatch=true AND instructorMatch=true
        string out = runWithInput("", [&](){ searchCourses(courses, "a"); });
        CHECK("both name and instructor match", out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "sara"); });
        CHECK("finds Dr. Sara by instructor", out.find("Dr. Sara") != string::npos);
    }
    {
        string out = runWithInput("", [&](){ searchCourses(courses, "computer"); });
        CHECK("finds computer science",       out.find("Computer Science") != string::npos);
    }
}

void test_handleViewCourses() {
    cout << "\n--- handleViewCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    string out = runWithInput("", [&](){ handleViewCourses(courses); });
    CHECK("shows courses",           out.find("Mathematics 101") != string::npos);
}

void test_handleSearchCourses() {
    cout << "\n--- handleSearchCourses ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    {
        string out = runWithInput("math\n", [&](){ handleSearchCourses(courses); });
        CHECK("valid search works",   out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("\n", [&](){ handleSearchCourses(courses); });
        CHECK("empty keyword error",  out.find("Please enter a keyword") != string::npos);
    }
    {
        string out = runWithInput("sara\n", [&](){ handleSearchCourses(courses); });
        CHECK("search by instructor", out.find("Dr. Sara") != string::npos);
    }
}

void test_registerForCourse() {
    cout << "\n--- registerForCourse ---\n";
    vector<User> users = {{"alice@x.com","secret",{}}};
    vector<Course> courses;
    insertSampleCourses(courses);
    int originalCapacity = courses[0].capacity;
    {
        string out = runWithInput("", [&](){
            registerForCourse(users, "alice@x.com", courses, 1);
        });
        CHECK("register success",        out.find("SUCCESS") != string::npos);
        CHECK("course added to student", users[0].enrolledCourseIds.size() == 1);
        CHECK("capacity decreased",      courses[0].capacity == originalCapacity - 1);
    }
    {
        string out = runWithInput("", [&](){
            registerForCourse(users, "alice@x.com", courses, 1);
        });
        CHECK("already enrolled error",  out.find("Already registered") != string::npos);
        CHECK("still only 1 course",     users[0].enrolledCourseIds.size() == 1);
    }
    {
        string out = runWithInput("", [&](){
            registerForCourse(users, "alice@x.com", courses, 999);
        });
        CHECK("course not found error",  out.find("Course not found") != string::npos);
    }
    {
        string out = runWithInput("", [&](){
            registerForCourse(users, "nobody@x.com", courses, 2);
        });
        CHECK("user not found error",    out.find("User not found") != string::npos);
    }
    {
        vector<User> users2 = {{"bob@x.com","pass",{}}};
        runWithInput("", [&](){ registerForCourse(users2, "bob@x.com", courses, 1); });
        runWithInput("", [&](){ registerForCourse(users2, "bob@x.com", courses, 2); });
        CHECK("register multiple courses", users2[0].enrolledCourseIds.size() == 2);
    }
    {
        vector<User> users3 = {{"carol@x.com","pass",{}}};
        vector<Course> fullCourses = {{99, "Full Course", "Dr. X", 0, "Mon"}};
        string out = runWithInput("", [&](){
            registerForCourse(users3, "carol@x.com", fullCourses, 99);
        });
        CHECK("full course error",       out.find("full") != string::npos);
    }
    {
        // alreadyEnrolled = false branch: register for a NEW course (not already enrolled)
        string out = runWithInput("", [&](){
            registerForCourse(users, "alice@x.com", courses, 3);
        });
        CHECK("register new course ok",  out.find("SUCCESS") != string::npos);
    }
}

void test_dropCourse() {
    cout << "\n--- dropCourse ---\n";
    vector<User> users = {{"alice@x.com","secret",{1}}};
    vector<Course> courses;
    insertSampleCourses(courses);
    int originalCapacity = courses[0].capacity;
    {
        string out = runWithInput("", [&](){
            dropCourse(users, "alice@x.com", courses, 1);
        });
        CHECK("drop success",        out.find("SUCCESS") != string::npos);
        CHECK("course removed",      users[0].enrolledCourseIds.empty());
        CHECK("capacity restored",   courses[0].capacity == originalCapacity + 1);
    }
    {
        string out = runWithInput("", [&](){
            dropCourse(users, "alice@x.com", courses, 1);
        });
        CHECK("not enrolled error",  out.find("not registered") != string::npos);
    }
    {
        string out = runWithInput("", [&](){
            dropCourse(users, "nobody@x.com", courses, 1);
        });
        CHECK("user not found error",out.find("User not found") != string::npos);
    }
    {
        // courseIt == courses.end() branch: user enrolled in ID 999 but it's not in courses list
        vector<User> users2 = {{"dave@x.com","pass",{999}}};
        string out = runWithInput("", [&](){
            dropCourse(users2, "dave@x.com", courses, 999);
        });
        CHECK("drop course not in list", out.find("SUCCESS") != string::npos);
    }
}

void test_viewSchedule() {
    cout << "\n--- viewSchedule ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    {
        vector<User> users = {{"alice@x.com","secret",{}}};
        string out = runWithInput("", [&](){
            viewSchedule(users, "alice@x.com", courses);
        });
        CHECK("empty schedule message",  out.find("no courses") != string::npos);
    }
    {
        vector<User> users = {{"alice@x.com","secret",{1, 2}}};
        string out = runWithInput("", [&](){
            viewSchedule(users, "alice@x.com", courses);
        });
        CHECK("shows MY SCHEDULE",       out.find("MY SCHEDULE") != string::npos);
        CHECK("shows enrolled course",   out.find("Mathematics 101") != string::npos);
        CHECK("shows second course",     out.find("Computer Science") != string::npos);
    }
    {
        vector<User> users;
        string out = runWithInput("", [&](){
            viewSchedule(users, "nobody@x.com", courses);
        });
        CHECK("user not found error",    out.find("User not found") != string::npos);
    }
    {
        // courseIt == courses.end() branch in viewSchedule: enrolled in ID 999 not in courses
        vector<User> users2 = {{"dave@x.com","pass",{999}}};
        string out = runWithInput("", [&](){
            viewSchedule(users2, "dave@x.com", courses);
        });
        CHECK("schedule with missing course", out.find("MY SCHEDULE") != string::npos);
    }
}

void test_handleRegisterForCourse() {
    cout << "\n--- handleRegisterForCourse ---\n";
    vector<User>   users = {{"alice@x.com","secret",{}}};
    vector<Course> courses;
    insertSampleCourses(courses);
    {
        runWithInput("1\n", [&](){
            handleRegisterForCourse(users, "alice@x.com", courses);
        });
        CHECK("registered via handler",   users[0].enrolledCourseIds.size() == 1);
    }
    {
        runWithInput("2\n", [&](){
            handleRegisterForCourse(users, "alice@x.com", courses);
        });
        CHECK("registered second course", users[0].enrolledCourseIds.size() == 2);
    }
}

void test_handleDropCourse() {
    cout << "\n--- handleDropCourse ---\n";
    vector<User>   users = {{"alice@x.com","secret",{1}}};
    vector<Course> courses;
    insertSampleCourses(courses);
    runWithInput("1\n", [&](){ handleDropCourse(users, "alice@x.com", courses); });
    CHECK("dropped via handler", users[0].enrolledCourseIds.empty());
}

void test_handleViewSchedule() {
    cout << "\n--- handleViewSchedule ---\n";
    vector<Course> courses;
    insertSampleCourses(courses);
    vector<User> users = {{"alice@x.com","secret",{1}}};
    string out = runWithInput("", [&](){
        handleViewSchedule(users, "alice@x.com", courses);
    });
    CHECK("shows schedule header",  out.find("MY SCHEDULE") != string::npos);
    CHECK("shows enrolled course",  out.find("Mathematics 101") != string::npos);
}

void test_handleGuestMenu() {
    cout << "\n--- handleGuestMenu ---\n";
    vector<User>   users;
    vector<Course> courses;
    string loggedInEmail = "";
    insertSampleCourses(courses);
    {
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){
            bool result = handleGuestMenu(1, users, courses, loggedInEmail);
            CHECK("case 1 returns true",  result == true);
        });
        CHECK("case 1 register works",    out.find("SUCCESS") != string::npos);
    }
    {
        string out = runWithInput("alice@x.com\nsecret123\n", [&](){
            bool result = handleGuestMenu(2, users, courses, loggedInEmail);
            CHECK("case 2 returns true",  result == true);
        });
        CHECK("case 2 login works",       out.find("Welcome back") != string::npos);
        CHECK("case 2 sets email",        loggedInEmail == "alice@x.com");
    }
    loggedInEmail = "";
    {
        string out = runWithInput("", [&](){
            bool result = handleGuestMenu(3, users, courses, loggedInEmail);
            CHECK("case 3 returns true",  result == true);
        });
        CHECK("case 3 shows courses",     out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("math\n", [&](){
            bool result = handleGuestMenu(4, users, courses, loggedInEmail);
            CHECK("case 4 returns true",  result == true);
        });
        CHECK("case 4 search works",      out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("", [&](){
            bool result = handleGuestMenu(5, users, courses, loggedInEmail);
            CHECK("case 5 returns false", result == false);
        });
        CHECK("case 5 goodbye",           out.find("Goodbye") != string::npos);
    }
    {
        string out = runWithInput("", [&](){
            bool result = handleGuestMenu(99, users, courses, loggedInEmail);
            CHECK("default returns true", result == true);
        });
        CHECK("default invalid error",    out.find("Invalid choice") != string::npos);
    }
}

void test_handleLoggedInMenu() {
    cout << "\n--- handleLoggedInMenu ---\n";
    vector<User>   users = {{"alice@x.com","secret",{}}};
    vector<Course> courses;
    string loggedInEmail = "alice@x.com";
    insertSampleCourses(courses);
    {
        string out = runWithInput("", [&](){
            bool result = handleLoggedInMenu(1, users, courses, loggedInEmail);
            CHECK("logged case 1 returns true", result == true);
        });
        CHECK("logged case 1 shows courses",    out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("math\n", [&](){
            bool result = handleLoggedInMenu(2, users, courses, loggedInEmail);
            CHECK("logged case 2 returns true", result == true);
        });
        CHECK("logged case 2 search works",     out.find("Mathematics 101") != string::npos);
    }
    {
        string out = runWithInput("1\n", [&](){
            bool result = handleLoggedInMenu(3, users, courses, loggedInEmail);
            CHECK("logged case 3 returns true", result == true);
        });
        CHECK("logged case 3 registered",       users[0].enrolledCourseIds.size() == 1);
    }
    {
        string out = runWithInput("1\n", [&](){
            bool result = handleLoggedInMenu(4, users, courses, loggedInEmail);
            CHECK("logged case 4 returns true", result == true);
        });
        CHECK("logged case 4 dropped",          users[0].enrolledCourseIds.empty());
    }
    {
        users[0].enrolledCourseIds.push_back(1);
        string out = runWithInput("", [&](){
            bool result = handleLoggedInMenu(5, users, courses, loggedInEmail);
            CHECK("logged case 5 returns true", result == true);
        });
        CHECK("logged case 5 schedule",         out.find("MY SCHEDULE") != string::npos);
    }
    {
        string out = runWithInput("", [&](){
            bool result = handleLoggedInMenu(6, users, courses, loggedInEmail);
            CHECK("logged case 6 returns true", result == true);
        });
        CHECK("logged case 6 logged out",       loggedInEmail.empty());
        CHECK("logged case 6 message",          out.find("Logged out") != string::npos);
    }
    {
        string loggedIn2 = "alice@x.com";
        string out = runWithInput("", [&](){
            bool result = handleLoggedInMenu(7, users, courses, loggedIn2);
            CHECK("logged case 7 returns false",result == false);
        });
        CHECK("logged case 7 goodbye",          out.find("Goodbye") != string::npos);
    }
    {
        string loggedIn2 = "alice@x.com";
        string out = runWithInput("", [&](){
            bool result = handleLoggedInMenu(99, users, courses, loggedIn2);
            CHECK("logged default returns true",result == true);
        });
        CHECK("logged default invalid",         out.find("Invalid choice") != string::npos);
    }
}

void test_runApp() {
    cout << "\n--- runApp ---\n";
    {
        vector<User>   users;
        vector<Course> courses;
        insertSampleCourses(courses);
        string out = runWithInput("5\n", [&](){ runApp(users, courses); });
        CHECK("runApp exits on 5",           out.find("Goodbye") != string::npos);
    }
    {
        vector<User>   users;
        vector<Course> courses;
        insertSampleCourses(courses);
        string out = runWithInput(
            "1\nalice@x.com\nsecret123\n"
            "2\nalice@x.com\nsecret123\n"
            "7\n",
            [&](){ runApp(users, courses); });
        CHECK("runApp login works",          out.find("Welcome back") != string::npos);
    }
    {
        vector<User>   users;
        vector<Course> courses;
        insertSampleCourses(courses);
        string out = runWithInput(
            "1\nalice@x.com\nsecret123\n"
            "2\nalice@x.com\nsecret123\n"
            "1\n"
            "2\nmath\n"
            "3\n1\n"
            "4\n1\n"
            "5\n"
            "6\n"
            "5\n",
            [&](){ runApp(users, courses); });
        CHECK("runApp full flow",            out.find("Logged out") != string::npos);
    }
    {
        vector<User>   users;
        vector<Course> courses;
        insertSampleCourses(courses);
        string out = runWithInput("99\n5\n", [&](){ runApp(users, courses); });
        CHECK("runApp invalid then exit",    out.find("Invalid choice") != string::npos);
    }
    {
        vector<User>   users;
        vector<Course> courses;
        insertSampleCourses(courses);
        string out = runWithInput(
            "1\nalice@x.com\nsecret123\n"
            "2\nalice@x.com\nsecret123\n"
            "99\n"
            "7\n",
            [&](){ runApp(users, courses); });
        CHECK("runApp logged in invalid",    out.find("Invalid choice") != string::npos);
    }
}

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

    cout << "\n========== SPRINT 3 TESTS ==========\n";
    test_registerForCourse();
    test_dropCourse();
    test_viewSchedule();
    test_handleRegisterForCourse();
    test_handleDropCourse();
    test_handleViewSchedule();

    cout << "\n========== MENU TESTS ==========\n";
    test_handleGuestMenu();
    test_handleLoggedInMenu();
    test_runApp();

    cout << "\n=====================================\n";
    cout << "Results: " << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
