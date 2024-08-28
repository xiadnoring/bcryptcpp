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

#include <string>
#include <random>

#include "legacy.h"
#include "bcrypt.h"
#include "errors.h"

#define BCRYPT_HASHSIZE 31

/**
 * unsafe random string
 *
 * @param len the size of the generated random string
 * @return random string
 */
std::string random_string (const size_t &len) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist256(0,255);

    std::string rnd;
    rnd.resize(len);

    for (size_t i = 0; i < len; i++)
    {
        rnd[i] = static_cast <char> (dist256(rng));
    }

    return std::move(rnd);
}

/**
 * get rid of '\0'
 *
 * @param str string to resolve
 * @return resolved string
 */
std::string resolve_string (const std::string &str) {
    const auto pos = str.find('\0');
    if (pos != std::string::npos)
    {
        return std::move(str.substr(0, pos));
    }

    return str;
}

std::string bcrypt::gensalt(const int &factor = 10, char minor, const size_t &random_bytes) {
    if (factor < 4 || factor > 31)
    {
        throw bcrypt::exception::gensalt ("{}(): factor must be >= 4 and <= 31, but factor = {}", __FUNCTION__, factor);
    }

    if (minor != 'a' && minor != 'b')
    {
        throw bcrypt::exception::gensalt ("{}(): invalid minor version, minor = {}", __FUNCTION__, minor);
    }

    std::string salt;
    // max 31
    salt.resize(BCRYPT_HASHSIZE);
    const std::string rstr = random_string(random_bytes);
    BF_gensalt (minor, factor, reinterpret_cast <const uint8_t *> (rstr.data()), static_cast<int> (rstr.size()), (uint8_t *) salt.data(), static_cast<int> (salt.size()));

    if (salt[0] == '\0') {
        throw bcrypt::exception::gensalt ("{}(): BF_gensalt(...) -> salt = \\0 x{}.", __FUNCTION__, salt.size());
    }

    return std::move(resolve_string(salt));
}

std::string bcrypt::hash(const std::string &str, const std::string &salt) {
    std::string hash;
    hash.resize(sizeof (struct crypt_data));
    struct crypt_internal *cint = get_internal ((crypt_data *) hash.data());
    BF_full_crypt (str.data(), salt.data(), (uint8_t *) hash.data(), static_cast<int> (hash.size()), cint, sizeof (struct crypt_internal));

    if (hash[0] == '\0') {
        throw bcrypt::exception::hash ("{}(): BF_full_crypt(...) -> hash = \\0 x{}.", __FUNCTION__, hash.size());
    }

    return std::move(resolve_string (hash));
}

bool bcrypt::compare(const std::string &hash, const std::string &origin) {
    const std::string verify = bcrypt::hash(origin, hash);

    if (hash.size() != verify.size())
    {
        throw bcrypt::exception::compare ("{}(): invalid hash string: size different ({} != {})", __FUNCTION__, hash.size(), verify.size());
    }

    int ret = 0;
    for (size_t i = 0; i < hash.size(); i++)
    {
        ret |= (static_cast <const uint8_t>(hash[i]) ^ static_cast <const uint8_t>(verify[i]));
    }

    return ret == 0;
}