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
class statement
{
public:
    statement() = delete;
    ~statement();

    statement(std::shared_ptr<sqlite3> const& sqlite_database);

    std::string sql() const;
    std::string expanded_sql() const;
    std::string normalized_sql() const;

    void log_error() const;

    void prepare(std::string const& sql_);
    void bind(row const& row_) const;
    void clear_bindings() const;
    void step();
    void reset() const;
    void finalize();

    bool has_row() const;
    row  get_row() const;

    std::vector<row> execute(std::string const& sql_);
    std::vector<row> execute(std::string const& sql_, row const& row_);

    void logging(bool const& enable);
    bool logging() const;


private:
    std::shared_ptr<sqlite3_stmt>   m_statement;
    std::shared_ptr<sqlite3> const& m_database;
    bool                            m_has_row     = false;
    bool                            m_logging     = false;
    bool                            m_step_logged = false;
};
} // namespace sqlite
} // namespace mm
