#include "user.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

// ─── Sprint 1: Validation ─────────────────
bool isValidEmail(string_view email) {
    return email.find('@') != string_view::npos;
}

bool isValidPassword(string_view password) {
    return password.length() >= 6;
}

// ─── Sprint 1: Lookup helpers ─────────────

// FIX 1: replaced manual for+if loop with std::any_of (SonarQube: "Replace for-loop by std::any_of")
bool emailAlreadyExists(const vector<User>& users, string_view email) {
    return any_of(users.begin(), users.end(),
                  [&email](const User& user) {
                      return user.email == email;
                  });
}

// FIX 1 (same rule): replaced manual for+if loop with std::any_of
bool authenticateUser(const vector<User>& users,
                      string_view email,
                      string_view password) {
    return any_of(users.begin(), users.end(),
                  [&email, &password](const User& user) {
                      return user.email == email && user.password == password;
                  });
}

// ─── Sprint 1: Action handlers ────────────
void handleRegister(vector<User>& users) {
    string email;
    string password;

    cout << "\n========== REGISTER ==========\n";
    cout << "Enter email: ";
    getline(cin, email);

    if (!isValidEmail(email)) {
        cout << "ERROR: Invalid email!\n";
        return;
    }

    cout << "Enter password (min 6 characters): ";
    getline(cin, password);

    if (!isValidPassword(password)) {
        cout << "ERROR: Password must be at least 6 characters!\n";
        return;
    }

    if (emailAlreadyExists(users, email)) {
        cout << "ERROR: Email already registered!\n";
    } else {
        users.push_back({email, password, {}});
        cout << "SUCCESS: Registered successfully!\n";
    }
}

void handleLogin(const vector<User>& users, string& loggedInEmail) {
    string email;
    string password;

    cout << "\n========== LOGIN ==========\n";
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter password: ";
    getline(cin, password);

    if (authenticateUser(users, email, password)) {
        loggedInEmail = email;
        cout << "SUCCESS: Welcome back " << email << "!\n";
    } else {
        cout << "ERROR: Wrong email or password!\n";
    }
}

// ─── Sprint 2: Course Functions ───────────
void insertSampleCourses(vector<Course>& courses) {
    courses.push_back({1, "Mathematics 101",      "Dr. Ahmed",   30, "Sun/Tue 9:00-10:30"});
    courses.push_back({2, "Computer Science 201", "Dr. Sara",    25, "Mon/Wed 11:00-12:30"});
    courses.push_back({3, "Physics 101",          "Dr. Mohamed", 30, "Sun/Tue 1:00-2:30"});
    courses.push_back({4, "English 101",          "Dr. Nour",    35, "Mon/Wed 9:00-10:30"});
    courses.push_back({5, "Chemistry 201",        "Dr. Layla",   25, "Sun/Tue 3:00-4:30"});
    courses.push_back({6, "History 101",          "Dr. Omar",    30, "Mon/Wed 1:00-2:30"});
    courses.push_back({7, "Biology 201",          "Dr. Hana",    25, "Sun/Tue 11:00-12:30"});
    courses.push_back({8, "Economics 101",        "Dr. Karim",   30, "Mon/Wed 3:00-4:30"});
}

void displayCourses(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }
    cout << "\n========== AVAILABLE COURSES ==========\n";
    for (const Course& course : courses) {
        cout << "ID:         " << course.id         << "\n";
        cout << "Name:       " << course.name        << "\n";
        cout << "Instructor: " << course.instructor  << "\n";
        cout << "Schedule:   " << course.schedule    << "\n";
        cout << "Capacity:   " << course.capacity    << " students\n";
        cout << "---------------------------------------\n";
    }
}

void searchCourses(const vector<Course>& courses, string_view keyword) {
    bool found = false;
    cout << "\n========== SEARCH RESULTS ==========\n";

    // FIX 2: changed  "string keywordLower = string(keyword)"
    //         to      "auto keywordLower = string(keyword)"
    // (SonarQube: "Replace the redundant type with auto")
    auto keywordLower = string(keyword);
    transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);

    for (const Course& course : courses) {
        string nameLower       = course.name;
        string instructorLower = course.instructor;
        transform(nameLower.begin(),       nameLower.end(),       nameLower.begin(),       ::tolower);
        transform(instructorLower.begin(), instructorLower.end(), instructorLower.begin(), ::tolower);

        bool nameMatch       = nameLower.find(keywordLower)       != string::npos;
        bool instructorMatch = instructorLower.find(keywordLower) != string::npos;

        if (nameMatch || instructorMatch) {
            cout << "ID:         " << course.id         << "\n";
            cout << "Name:       " << course.name        << "\n";
            cout << "Instructor: " << course.instructor  << "\n";
            cout << "Schedule:   " << course.schedule    << "\n";
            cout << "Capacity:   " << course.capacity    << " students\n";
            cout << "---------------------------------------\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No courses found for: " << keyword << "\n";
    }
}

// ─── Sprint 2: Action handlers ────────────
void handleViewCourses(const vector<Course>& courses) {
    displayCourses(courses);
}

void handleSearchCourses(const vector<Course>& courses) {
    string keyword;
    cout << "\nEnter search keyword: ";
    getline(cin, keyword);

    if (keyword.empty()) {
        cout << "ERROR: Please enter a keyword!\n";
    } else {
        searchCourses(courses, keyword);
    }
}

// ─── Sprint 3: Registration & Schedule ────

bool registerForCourse(std::vector<User>& users,
                       std::string_view email,
                       std::vector<Course>& courses,
                       int courseId){
    // Find user
    auto userIt = find_if(users.begin(), users.end(),
        [&email](const User& u) { return u.email == email; });

    if (userIt == users.end()) {
        cout << "ERROR: User not found!\n";
        return false;
    }

    // Fix 3: init-statement for alreadyEnrolled (SonarQube)
    if (bool alreadyEnrolled = any_of(userIt->enrolledCourseIds.begin(),
            userIt->enrolledCourseIds.end(),
            [courseId](int id) { return id == courseId; });
        alreadyEnrolled) {

        cout << "ERROR: Already registered!\n";
        return false;
    }

    // Fix 4: init-statement for courseIt (SonarQube)
    if (auto courseIt = find_if(courses.begin(), courses.end(),
            [courseId](const Course& c) { return c.id == courseId; });
        courseIt != courses.end()) {

        // Check capacity (REQUIRED by tests)
        if (courseIt->capacity <= 0) {
            cout << "ERROR: Course is full!\n";
            return false;
        }

        //  Perform registration
        userIt->enrolledCourseIds.push_back(courseId);
        courseIt->capacity--;

        cout << "SUCCESS: Registered for course!\n";
        return true;
    }

    // Course not found
    cout << "ERROR: Course not found!\n";
    return false;
}

bool dropCourse(vector<User>& users,
                string_view email,
                vector<Course>& courses,
                int courseId) {
    // Find the user
    auto userIt = find_if(users.begin(), users.end(),
                          [&email](const User& u) { return u.email == email; });

    if (userIt == users.end()) {
        cout << "ERROR: User not found!\n";
        return false;
    }

    // Find the course ID in the student's enrolled list
    auto idIt = find(userIt->enrolledCourseIds.begin(),
                     userIt->enrolledCourseIds.end(),
                     courseId);

    if (idIt == userIt->enrolledCourseIds.end()) {
        cout << "ERROR: You are not registered in this course!\n";
        return false;
    }

    // Remove course from student schedule and restore capacity
    userIt->enrolledCourseIds.erase(idIt);

    if (auto courseIt = find_if(courses.begin(), courses.end(),
        [courseId](const Course& c) { return c.id == courseId; });
    courseIt != courses.end()) {

    courseIt->capacity++;
}

    cout << "SUCCESS: Course dropped from your schedule!\n";
    return true;
}

void viewSchedule(const vector<User>& users,
                  string_view email,
                  const vector<Course>& courses) {
    auto userIt = find_if(users.begin(), users.end(),
                          [&email](const User& u) { return u.email == email; });

    if (userIt == users.end()) {
        cout << "ERROR: User not found!\n";
        return;
    }

    if (userIt->enrolledCourseIds.empty()) {
        cout << "You have no courses in your schedule.\n";
        return;
    }

    cout << "\n========== MY SCHEDULE ==========\n";
    for (int id : userIt->enrolledCourseIds) {
        auto courseIt = find_if(courses.begin(), courses.end(),
                                [id](const Course& c) { return c.id == id; });

        if (courseIt != courses.end()) {
            cout << "ID:         " << courseIt->id         << "\n";
            cout << "Name:       " << courseIt->name        << "\n";
            cout << "Instructor: " << courseIt->instructor  << "\n";
            cout << "Schedule:   " << courseIt->schedule    << "\n";
            cout << "---------------------------------------\n";
        }
    }
}

// ─── Sprint 3: Action handlers ────────────
void handleRegisterForCourse(vector<User>& users,
                              string_view loggedInEmail,
                              vector<Course>& courses) {
    displayCourses(courses);
    cout << "Enter course ID to register: ";
    int courseId;
    cin >> courseId;
    cin.ignore();
    registerForCourse(users, loggedInEmail, courses, courseId);
}

void handleDropCourse(vector<User>& users,
                      string_view loggedInEmail,
                      vector<Course>& courses) {
    viewSchedule(users, loggedInEmail, courses);
    cout << "Enter course ID to drop: ";
    int courseId;
    cin >> courseId;
    cin.ignore();
    dropCourse(users, loggedInEmail, courses, courseId);
}

void handleViewSchedule(const vector<User>& users,
                        string_view loggedInEmail,
                        const vector<Course>& courses) {
    viewSchedule(users, loggedInEmail, courses);
}
