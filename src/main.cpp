#include <vector>
#include <string>
#include "user.h"

using namespace std;

int main() {
    vector<User>   users;
    vector<Course> courses;
    insertSampleCourses(courses);
    runApp(users, courses);
    return 0;
}
