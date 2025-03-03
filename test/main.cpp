/**
 * Copyright (C) 2023-2025, Xiadnoring (Timur Zajnullin).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#include <iostream>
#include <string>
#include <cassert>

#include <bcryptcpp/bcryptcpp.hpp>

static int errcnt = 0;
static int testcnt = 0;

#define bassert(expr) bassert_ (expr, #expr, __FILE__, __LINE__)
#define exceptassert(expr) try { expr; bassert_(false, #expr, __FILE__, __LINE__); } catch (...) { bassert_(true, #expr, __FILE__, __LINE__); }

void bassert_ (bool res, std::string assert_expr, std::string assert_file, std::size_t assert_line) {
    if (res) {
        std::cout << '#' << testcnt++ << " OK" << std::endl;
    }
    else {
        std::cout << '#' << testcnt++ << " FAILED: " << assert_expr << " [" << assert_file << ": " << assert_line << "]" << std::endl;
        errcnt++;
    }
}

int main () {
    std::string origin = "hello world :<";
    std::string valid_hash = bcrypt::hash (origin, bcrypt::gensalt (12));
    std::string invalid_hash = "$2a$10$66f5mZTOUzLxZOPzW1bSEeEkKF1vPztrzH/PaDcNfDKyRlj0.N3H.";

    /* valid hash */
    bassert(bcrypt::compare(valid_hash, origin));

    /* invalid origin */
    bassert(!bcrypt::compare(valid_hash, "hello world :>"));

    /* invalid origin */
    bassert(!bcrypt::compare(valid_hash, "hellо world :<"));

    /* invalid hash */
    bassert(!bcrypt::compare(invalid_hash, origin));

    /* check the equality */
    bassert(bcrypt::gensalt (12, 'b', "1234567890ABCDEF") == "$2b$12$KRGxLBS0Lxe3KCDAOyPDPe");

    /* check the equality */
    bassert(bcrypt::gensalt (12, 'a', "1234567890ABCDEF") == "$2a$12$KRGxLBS0Lxe3KCDAOyPDPe");

    /* validate input */
    exceptassert(bcrypt::compare("hm", origin));

    /* validate input */
    exceptassert(bcrypt::gensalt(100));

    /* validate input */
    exceptassert(bcrypt::gensalt(-5, 'b'));

    /* validate input */
    exceptassert(bcrypt::gensalt(78, 'a'));

    /* validate input */
    exceptassert(bcrypt::gensalt(4, '\a'));

    /* check the equality */
    bassert("$2a$04$WUHhXETkX0DgW0PjXkbfWeWhWIPfFHFc84aZn6.RKOKXW8yHu0bPu" == bcrypt::hash ("", bcrypt::gensalt (4, 'a', "abcdefgabcdefgabcdefgabcdefg")));

    /* validate input */
    exceptassert(bcrypt::hash ("", bcrypt::gensalt (4, 'a', "abcd")));

    assert(errcnt==0 && "some tests failed");

    return 0;
}