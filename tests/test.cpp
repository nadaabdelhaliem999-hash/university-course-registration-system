#include <cassert>
#include <iostream>
#include "user.h"

using namespace std;

// ── Email Tests ──────────────────────────
void test_valid_email() {
    assert(isValidEmail("test@gmail.com") == true);
    cout << "PASS: valid email accepted\n";
}

void test_invalid_email() {
    assert(isValidEmail("wrong") == false);
    cout << "PASS: invalid email rejected\n";
}

// ── Password Tests ───────────────────────
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

// ── Main ─────────────────────────────────
int main() {
    cout << "\n===== Running Tests =====\n\n";

    test_valid_email();
    test_invalid_email();
    test_valid_password();
    test_short_password();
    test_empty_password();

    cout << "\n===== All Tests Passed! =====\n";
    return 0;
}
