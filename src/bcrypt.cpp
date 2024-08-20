#include <string>
#include <random>
#include <crypt.h>

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

std::string bcrypt::gensalt(const int &factor, const std::string &minor, const size_t &random_bytes) {
    if (factor < 4 || factor > 31)
    {
        throw bcrypt::exception::gensalt ("{}(): factor must be >= 4 and <= 31, but factor = {}", __FUNCTION__, factor);
    }

    if (minor != "a" && minor != "b")
    {
        throw bcrypt::exception::gensalt ("{}(): invalid minor version, minor = {}", __FUNCTION__, minor);
    }

    std::string prefix = "$2" + minor + "$";
    std::string salt;
    // max 31
    salt.resize(BCRYPT_HASHSIZE);
    const std::string rstr = random_string(random_bytes);
    if(crypt_gensalt_rn (prefix.data(), factor, rstr.data(), static_cast<int> (rstr.size()), (char *) salt.data(), static_cast<int> (salt.size())) == nullptr)
    {
        throw bcrypt::exception::gensalt ("{}(): crypt_gensalt_rn(...) == nullptr.", __FUNCTION__);
    }

    return std::move(resolve_string(salt));
}

std::string bcrypt::hash(const std::string &str, const std::string &salt) {
    std::string hash;
    hash.resize(sizeof (struct crypt_data));
    if (crypt_rn(str.data(), salt.data(), (void *) hash.data(), static_cast<int> (hash.size())) == nullptr)
    {
        throw bcrypt::exception::hash ("{}(): crypto_rn(...) == nullptr.", __FUNCTION__);
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