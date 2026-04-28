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
 
// ─── Sprint 1: Helper functions (extracted for testability + nesting fix) ───
bool emailAlreadyExists(const std::vector<User>& users, std::string_view email);
bool authenticateUser(const std::vector<User>& users,
                      std::string_view email,
                      std::string_view password);
 
// ─── Sprint 1: Action handlers (extracted to remove nesting + enable coverage) ─
void handleRegister(std::vector<User>& users);
void handleLogin(const std::vector<User>& users);
 
// ─── Sprint 2: Course ─────────────────────
struct Course {
    int         id;
    std::string name;
    std::string instructor;
    int         capacity;
    std::string schedule;
};
 
void insertSampleCourses(std::vector<Course>& courses);
void displayCourses(const std::vector<Course>& courses);
void searchCourses(const std::vector<Course>& courses, std::string_view keyword);
 
// ─── Sprint 2: Action handlers ────────────
void handleViewCourses(const std::vector<Course>& courses);
void handleSearchCourses(const std::vector<Course>& courses);
 
#endif
