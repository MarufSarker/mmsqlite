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


#pragma once

#include <string>
#include <map>
#include <memory>
#include <sqlite3.h>
#include "enums.hh"
#include "column.hh"

namespace mm
{
namespace sqlite
{
class row
{
public:
    row();
    ~row();

    row(std::string const& name, column const& value);
    row(std::map<std::string, column> const& names_and_values);

    void append(std::string const& name, column const& value);

    std::map<std::string, column> const& columns() const;

    void bind(std::shared_ptr<sqlite3_stmt> const& sqlite_statement) const;


private:
    std::map<std::string, column> m_columns = {};
};
} // namespace sqlite
} // namespace mm
