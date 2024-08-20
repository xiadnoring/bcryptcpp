#include <iostream>
#include <string>
#include "bcryptcpp/bcrypt.h"

int main () {
    const std::string str = "hello world :<";
    const std::string salt = bcrypt::gensalt (12);
    const std::string hash = bcrypt::hash (str, salt);
    const std::string rand = "$2a$10$66f5mZTOUzLxZOPzW1bSEeEkKF1vPztrzH/PaDcNfDKyRlj0.N3H.";
    std::cout << "Test#0 " << (bcrypt::compare(hash, str) ? "OK" : "FAIL") << "\n";
    std::cout << "Test#1 " << (bcrypt::compare(rand, str) ? "FAIL" : "OK") << "\n";
    return 0;
}