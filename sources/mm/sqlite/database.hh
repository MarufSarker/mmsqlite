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
#include <memory>
#include <vector>
#include <sqlite3.h>
#include "row.hh"

namespace mm
{
namespace sqlite
{
class database
{
public:
    database();
    ~database();

    database(std::string const& path, int const& flags);

    void open(std::string const& path, int const& flags);
    void close();
    bool opened() const;

    std::vector<row> execute(std::string const& sql_);
    std::vector<row> execute(std::string const& sql_, row const& row_);

    void logging(bool const& enable);
    bool logging() const;


private:
    std::shared_ptr<sqlite3> m_sqlite;
    bool                     m_logging = false;
};
} // namespace sqlite
} // namespace mm
