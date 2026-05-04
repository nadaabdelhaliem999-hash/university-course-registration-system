#ifndef USER_H
#define USER_H

#include <string>
#include <string_view>
#include <vector>

struct User {
    std::string email;
    std::string password;
    std::vector<int> enrolledCourseIds;
};

bool isValidEmail(std::string_view email);
bool isValidPassword(std::string_view password);
bool emailAlreadyExists(const std::vector<User>& users, std::string_view email);
bool authenticateUser(const std::vector<User>& users,
                      std::string_view email,
                      std::string_view password);
void handleRegister(std::vector<User>& users);
void handleLogin(const std::vector<User>& users, std::string& loggedInEmail);

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
void handleViewCourses(const std::vector<Course>& courses);
void handleSearchCourses(const std::vector<Course>& courses);

bool registerForCourse(std::vector<User>& users,
                       std::string_view email,
                       std::vector<Course>& courses,
                       int courseId);
bool dropCourse(std::vector<User>& users,
                std::string_view email,
                std::vector<Course>& courses,
                int courseId);
void viewSchedule(const std::vector<User>& users,
                  std::string_view email,
                  const std::vector<Course>& courses);
void handleRegisterForCourse(std::vector<User>& users,
                              std::string_view loggedInEmail,
                              std::vector<Course>& courses);
void handleDropCourse(std::vector<User>& users,
                      std::string_view loggedInEmail,
                      std::vector<Course>& courses);
void handleViewSchedule(const std::vector<User>& users,
                        std::string_view loggedInEmail,
                        const std::vector<Course>& courses);

bool handleGuestMenu(int choice,
                     std::vector<User>& users,
                     const std::vector<Course>& courses,
                     std::string& loggedInEmail);
bool handleLoggedInMenu(int choice,
                        std::vector<User>& users,
                        std::vector<Course>& courses,
                        std::string& loggedInEmail);

// runApp: the full application loop — testable without main()
void runApp(std::vector<User>& users, std::vector<Course>& courses);

#endif
