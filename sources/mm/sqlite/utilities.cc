/*
 * mmsqlite
 * Copyright (C) 2022  Maruf Sarker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "utilities.hh"
#include <stdexcept>

namespace mm
{
namespace sqlite
{
void valid_sqlite_identifier(std::string const& identifier)
{
    // reserved keywords are not checked against,
    // use [] to enclose identifiers

    std::string const msg =
        "Invalid sqlite identifier. Non-empty string, starting with alphabets "
        "and, only with following character ranges [A-Z], [a-z], [0-9] are "
        "accepted. Additionally, 'COUNT(*)' is also allowed. [" +
        identifier + "]";

    if (identifier.empty())
        throw std::runtime_error {msg};

    if (identifier == "COUNT(*)")
        return;

    for (size_t i = 0; i < identifier.size(); ++i)
    {
        char const&        chr = identifier.at(i);
        unsigned int const ord = static_cast<unsigned int>(chr);

        if (ord >= 65 && ord <= 90) // A-Z
            continue;
        if (ord >= 97 && ord <= 122) // a-z
            continue;
        if (i <= 0) // starting with alphabets
            throw std::runtime_error {msg};
        if (ord >= 48 && ord <= 57) // 0-9
            continue;
        if (ord == 95) // _
            continue;

        throw std::runtime_error {msg};
    }
}


std::string to_string(int const& value) { return std::to_string(value); }


int to_int(std::string const& value)
{
    int v = std::stoi(value);
    if (std::to_string(v) != value)
        throw std::runtime_error {"Invalid integer value."};
    return v;
}


std::string to_string(double const& value) { return std::to_string(value); }


double to_double(std::string const& value)
{
    double v = std::stod(value);
    if (std::to_string(v) != value)
        throw std::runtime_error {"Invalid double value."};
    return v;
}
} // namespace sqlite
} // namespace mm
