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


#include "row.hh"
#include "utilities.hh"
#include <stdexcept>

namespace mm
{
namespace sqlite
{
row::row() = default;


row::~row() = default;


row::row(std::string const& name, column const& value) { append(name, value); }


row::row(std::map<std::string, column> const& names_and_values)
    : m_columns {names_and_values}
{
    for (auto const& v : m_columns)
        valid_sqlite_identifier(v.first);
}


void row::append(std::string const& name, column const& value)
{
    valid_sqlite_identifier(name);
    m_columns.emplace(name, value);
}


std::map<std::string, column> const& row::columns() const { return m_columns; }


void row::bind(std::shared_ptr<sqlite3_stmt> const& sqlite_statement) const
{
    for (auto const& v : m_columns)
        v.second.bind(sqlite_statement);
}
} // namespace sqlite
} // namespace mm
