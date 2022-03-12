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


#include "database.hh"
#include "statement.hh"
#include <stdexcept>

namespace mm
{
namespace sqlite
{
database::database() = default;


database::~database() { close(); }


database::database(std::string const& path, int const& flags)
{
    open(path, flags);
}


void database::open(std::string const& path, int const& flags)
{
    if (m_sqlite)
        throw std::runtime_error {"Database already initialized."};

    sqlite3* sqlite_ptr = nullptr;

    int const result =
        sqlite3_open_v2(path.c_str(), &sqlite_ptr, flags, nullptr);

    static auto _close = [](sqlite3* ptr)
    {
        sqlite3_close_v2(ptr);
        ptr = nullptr;
    };

    if (result != SQLITE_OK)
    {
        _close(sqlite_ptr);
        throw std::runtime_error {"Failed to open sqlite database."};
    }

    m_sqlite.reset(sqlite_ptr, _close);
}


void database::close() { m_sqlite.reset(); }


bool database::opened() const { return m_sqlite != nullptr; }


std::vector<row> database::execute(std::string const& sql_)
{
    return execute(sql_, {});
}


std::vector<row> database::execute(std::string const& sql_, row const& row_)
{
    if (!opened())
        throw std::runtime_error {"Database is not opened."};
    statement stmt {m_sqlite};
    stmt.logging(m_logging);
    return stmt.execute(sql_, row_);
}


void database::logging(bool const& enable) { m_logging = enable; }


bool database::logging() const { return m_logging; }
} // namespace sqlite
} // namespace mm
