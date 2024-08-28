/**
 * Copyright (C) 2023-2024, Xiadnoring (Timur Zajnullin).
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


#ifndef BCRYPT_H
#define BCRYPT_H

#if defined(_WIN32)
#  if defined(EXPORTING_BCRYPT)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

#define BCRYPT_EXCEPT noexcept(false)

namespace bcrypt {
    /**
     * generate a salt
     * @param factor number of rounds
     * @param minor a or b
     * @param random_bytes the length of the random string
     * @return 
     */
    std::string DECLSPEC gensalt (const int &factor, char minor = 'b', const size_t &random_bytes = 16) BCRYPT_EXCEPT;

    /**
     * hash data using a salt
     * @param data the data to encrypt
     * @param salt the salt to use when hashing
     * @return hash
     */
    std::string DECLSPEC hash (const std::string &data, const std::string &salt) BCRYPT_EXCEPT;

    /**
     * compare raw data to hash
     * @param hash expected hash
     * @param origin data to hash and compare
     * @return true if hashed data matches hash, false otherwise
     */
    bool DECLSPEC compare (const std::string &hash, const std::string &origin) BCRYPT_EXCEPT;
}

#undef BCRYPT_EXCEPT

#endif //BCRYPT_H
