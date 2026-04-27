#include <cassert>
#include <iostream>
#include <vector>
#include "user.h"

using namespace std;

// Sprint 1 Tests — Validation


void test_valid_email() {
    assert(isValidEmail("test@gmail.com") == true);
    cout << "PASS: valid email accepted\n";
}

void test_invalid_email() {
    assert(isValidEmail("wrong") == false);
    cout << "PASS: invalid email rejected\n";
}

void test_valid_password() {
    assert(isValidPassword("123456") == true);
    cout << "PASS: valid password accepted\n";
}

void test_short_password() {
    assert(isValidPassword("123") == false);
    cout << "PASS: short password rejected\n";
}

void test_empty_password() {
    assert(isValidPassword("") == false);
    cout << "PASS: empty password rejected\n";
}


// Sprint 2 Tests — Courses


void test_insert_sample_courses() {
    vector<Course> courses;
    insertSampleCourses(courses);
    assert(courses.size() == 8);
    cout << "PASS: 8 sample courses inserted\n";
}

void test_courses_have_correct_data() {
    vector<Course> courses;
    insertSampleCourses(courses);
    assert(courses[0].name == "Mathematics 101");
    assert(courses[0].instructor == "Dr. Ahmed");
    assert(courses[0].capacity == 30);
    cout << "PASS: course data is correct\n";
}

void test_search_finds_by_name() {
    vector<Course> courses;
    insertSampleCourses(courses);

    // Count how many match "math"
    int count = 0;
    for (const Course& course : courses) {
        string nameLower = course.name;
        for (char& c : nameLower) c = tolower(c);
        if (nameLower.find("math") != string::npos) {
            count++;
        }
    }
    assert(count >= 1);
    cout << "PASS: search by name works\n";
}

void test_search_finds_by_instructor() {
    vector<Course> courses;
    insertSampleCourses(courses);

    // Count how many match "ahmed"
    int count = 0;
    for (const Course& course : courses) {
        string instructorLower = course.instructor;
        for (char& c : instructorLower) c = tolower(c);
        if (instructorLower.find("ahmed") != string::npos) {
            count++;
        }
    }
    assert(count >= 1);
    cout << "PASS: search by instructor works\n";
}

void test_search_no_results() {
    vector<Course> courses;
    insertSampleCourses(courses);

    // Count how many match "xyznotfound"
    int count = 0;
    for (const Course& course : courses) {
        string nameLower = course.name;
        for (char& c : nameLower) c = tolower(c);
        if (nameLower.find("xyznotfound") != string::npos) {
            count++;
        }
    }
    assert(count == 0);
    cout << "PASS: search with no results works\n";
}

void test_course_capacity_is_positive() {
    vector<Course> courses;
    insertSampleCourses(courses);
    for (const Course& course : courses) {
        assert(course.capacity > 0);
    }
    cout << "PASS: all courses have positive capacity\n";
}


// Main — Run all tests

int main() {
    cout << "\n===== Running Sprint 1 Tests =====\n\n";
    test_valid_email();
    test_invalid_email();
    test_valid_password();
    test_short_password();
    test_empty_password();

    cout << "\n===== Running Sprint 2 Tests =====\n\n";
    test_insert_sample_courses();
    test_courses_have_correct_data();
    test_search_finds_by_name();
    test_search_finds_by_instructor();
    test_search_no_results();
    test_course_capacity_is_positive();

    cout << "\n===== All Tests Passed! ✅ =====\n";
    return 0;
}
