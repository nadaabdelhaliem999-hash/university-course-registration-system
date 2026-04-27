#ifndef USER_H
#define USER_H

#include <string>
#include <string_view>
#include <vector>

// ─── Sprint 1: User ───────────────────────
struct User {
    std::string email;
    std::string password;
};

bool isValidEmail(std::string_view email);
bool isValidPassword(std::string_view password);

// ─── Sprint 2: Course ─────────────────────
struct Course {
    int id;
    std::string name;
    std::string instructor;
    int capacity;
    std::string schedule;
};

void insertSampleCourses(std::vector<Course>& courses);
void displayCourses(const std::vector<Course>& courses);
void searchCourses(const std::vector<Course>& courses, std::string_view keyword);

#endif
