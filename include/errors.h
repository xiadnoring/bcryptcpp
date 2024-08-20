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

#ifndef ERRORS_H
#define ERRORS_H

#include <fmt/format.h>
#include <exception>
#include <string>

namespace bcrypt::exception {
    class base : public std::exception {
    public:
        template <class... Args>
        explicit base (const std::string &msg, Args&& ...args) {
            message = fmt::vformat(msg, fmt::make_format_args(args...));
            if (errno != 0) { message += fmt::format("\nerrno = {} (look up in errno-base.h).", errno); }
        }
        [[nodiscard]] const char * what() const noexcept override { return message.data(); }
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

#endif //ERRORS_H
