/* Orchid - WebRTC P2P VPN Market (on Ethereum)
 * Copyright (C) 2017-2019  The Orchid Authors
*/

/* GNU Affero General Public License, Version 3 {{{ */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
/* }}} */


#ifndef ORCHID_ERROR_HPP
#define ORCHID_ERROR_HPP

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include "log.hpp"

namespace orc {
class Error final :
    public std::exception
{
  public:
    const std::string file;
    const int line;
    std::string text;

    Error(const std::string &file, int line) :
        file(file), line(line)
    {
    }

    Error(Error &&error) = default;

    const char *what() const noexcept override {
        return text.c_str();
    }

    template <typename Type_>
    Error operator <<(const Type_ &value) && {
        std::ostringstream data;
        data << value;
        text += data.str();
        return std::move(*this);
    }
}; }

#define orc_insist_(code, text) do { \
    if ((code)) break; \
    orc::Log() << "[" << __FILE__ << ":" << std::dec << __LINE__ << "] " << text << std::endl; \
    std::terminate(); \
} while (false)

#define orc_insist(code) \
    orc_insist_(code, "orc_insist(" #code ")")

#define orc_throw(text) do { \
    orc::Log() << "[" << __FILE__ << ":" << std::dec << __LINE__ << "] " << text << std::endl; \
    throw orc::Error{__FILE__, __LINE__} << text; \
} while (false)

#define orc_adapt(error) do { \
    auto what(error.what()); \
    orc_insist(what != nullptr); \
    orc_insist(*what != '\0'); \
    orc_throw(what); \
} while (false)

#define orc_assert_(code, text) do { \
    if ((code)) break; \
    orc_throw(text); \
} while (false)

#define orc_assert(code) \
    orc_assert_(code, "orc_assert(" #code ")")

#endif//ORCHID_ERROR_HPP
