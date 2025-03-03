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