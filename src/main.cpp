#include <iostream>
#include <string>

#include "bcryptcpp.hpp"

using namespace std;

int main () {
    const std::string another = "$2b$10$2B9RTOzGcJJJ6jJnL03cnetXytiaL5ihhZ4fhQpjYqcHZVetSZ8je";
    const string str = "hello world :)";
    cout << "String: " << str << '\n';
    const string hash = bcrypt::hash(str, bcrypt::gensalt(4));
    cout << "Hash: " << hash << '\n';
    cout << "Compare #1: " << bcrypt::compare (hash, str) << '\n';
    cout << "Compare #2: " << bcrypt::compare (another, str) << '\n';

    return 0;
}