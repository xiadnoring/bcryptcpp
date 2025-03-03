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

/** High-level libcrypt interfaces.
 *
 * Copyright 2007-2017 Thorsten Kukuk and Zack Weinberg
 * Copyright 2018-2021 Björn Esser
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 *
 * github: https://github.com/besser82/libxcrypt
 * license: https://github.com/besser82/libxcrypt/blob/develop/LICENSING
 **/


#pragma once

#ifdef _WIN32
#   include <errno.h>
#endif

#if defined(BCRYPT_CPP_FORMAT_SUPPORT) || __cplusplus >= 202002L
#   include <format>
#else
#   include <fmt/format.h>
    using namespace fmt;
#endif

#include <exception>
#include <string>

#if defined(_WIN32)
#  if defined(EXPORTING_BCRYPT)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC
#  endif
#else // non windows
#  define DECLSPEC
#endif

#include <string>

#define BCRYPT_EXCEPT noexcept(false)

namespace bcrypt::exception {
    class base : public std::exception {
    public:
         template <class... Args>
         explicit base (const std::string &msg, Args&& ...args) {
              using namespace std;
              this->message = vformat(msg, make_format_args(args...));
              if (errno != 0) { this->message += format("\nerrno = {} (look up in errno-base.h).", errno); }
         }
         [[nodiscard]] const char * what() const noexcept override { return this->message.data(); }
    protected: std::string message;
    };

    class gensalt : public bcrypt::exception::base {
    public:
        template <class... Args> explicit gensalt (const std::string &msg, Args&& ...args) : base (msg, args...) {};
    };

    class hash : public bcrypt::exception::base {
    public:
        template <class... Args> explicit hash (const std::string &msg, Args&& ...args) : base (msg, args...) {};
    };

    class compare : public bcrypt::exception::base {
    public:
        template <class... Args> explicit compare (const std::string &msg, Args&& ...args) : base (msg, args...) {};
    };
}


namespace bcrypt {
    /**
     * generate a salt
     * @param factor number of rounds
     * @param minor a or b
     * @param random_bytes the length of the random string
     * @return
     */
    DECLSPEC std::string gensalt (const int &factor, char minor = 'b', const size_t &random_bytes = 16) BCRYPT_EXCEPT;

    /**
     * generate a salt
     * @param factor number of rounds
     * @param minor a or b
     * @param random_bytes the random string
     * @return
     */
    DECLSPEC std::string gensalt (const int &factor, char minor, std::string_view random_bytes) BCRYPT_EXCEPT;

    /**
     * generate a salt
     * @param factor number of rounds
     * @param minor a or b
     * @param random_bytes the random string
     * @return
     */
    DECLSPEC std::string gensalt (const int &factor, char minor, const std::string &random_bytes) BCRYPT_EXCEPT;


    /**
     * generate a salt
     * @param factor number of rounds
     * @param minor a or b
     * @param random_bytes the random string
     * @return
     */
    DECLSPEC std::string gensalt (const int &factor, char minor, const char *random_bytes) BCRYPT_EXCEPT;

    /**
     * hash data using a salt
     * @param data the data to encrypt
     * @param salt the salt to use when hashing
     * @return hash
     */
    DECLSPEC std::string hash (const std::string &data, const std::string &salt) BCRYPT_EXCEPT;

    /**
     * compare raw data to hash
     * @param hash expected hash
     * @param origin data to hash and compare
     * @return true if hashed data matches hash, false otherwise
     */
    bool DECLSPEC compare (const std::string &hash, const std::string &origin) BCRYPT_EXCEPT;

    /**
        * hash data using a salt
        * @param data the data to encrypt
        * @param salt the salt to use when hashing
        * @return hash
        */
    DECLSPEC std::string hash (std::string_view data, std::string_view salt) BCRYPT_EXCEPT;

    /**
     * compare raw data to hash
     * @param hash expected hash
     * @param origin data to hash and compare
     * @return true if hashed data matches hash, false otherwise
     */
    bool DECLSPEC compare (std::string_view hash, std::string_view origin) BCRYPT_EXCEPT;


    /**
        * hash data using a salt
        * @param data the data to encrypt
        * @param salt the salt to use when hashing
        * @return hash
        */
    DECLSPEC std::string hash (const char *data, const char *salt) BCRYPT_EXCEPT;

    /**
     * compare raw data to hash
     * @param hash expected hash
     * @param origin data to hash and compare
     * @return true if hashed data matches hash, false otherwise
     */
    bool DECLSPEC compare (const char *hash, const char *origin) BCRYPT_EXCEPT;
}

#undef BCRYPT_EXCEPT