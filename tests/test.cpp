#include <cassert>
#include "user.h"

int main() {
    assert(isValidEmail("test@gmail.com") == true);
    assert(isValidEmail("wrong") == false);

    assert(isValidPassword("123456") == true);
    assert(isValidPassword("123") == false);

    return 0;
}
