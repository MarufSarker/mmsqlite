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


#include "statement.hh"
#include <stdexcept>
#include <iostream>

namespace mm
{
namespace sqlite
{
statement::~statement() { finalize(); }


statement::statement(std::shared_ptr<sqlite3> const& sqlite_database)
    : m_database {sqlite_database}
{
}


std::string statement::sql() const
{
    return m_statement ? sqlite3_sql(m_statement.get()) : "";
}


std::string statement::expanded_sql() const
{
    return m_statement ? sqlite3_expanded_sql(m_statement.get()) : "";
}


std::string statement::normalized_sql() const
{
#ifdef SQLITE_ENABLE_NORMALIZE
    return m_statement ? sqlite3_normalized_sql(m_statement.get()) : "";
#else
    return "";
#endif
}


void statement::log_error() const
{
    if (!m_database)
        return;

    int const errcode          = sqlite3_errcode(m_database.get());
    int const extended_errcode = sqlite3_extended_errcode(m_database.get());
    std::string const errmsg   = sqlite3_errmsg(m_database.get());

    std::cerr << "| SQLite Error ::"
              << " Code [" << errcode << "]"
              << " Extended Code [" << extended_errcode << "]"
              << " Message [" << errmsg << "]" << std::endl;
}


void statement::prepare(std::string const& sql_)
{
    if (!m_database)
        throw std::runtime_error {"Invalid sqlite database."};

    sqlite3_stmt* stmt = nullptr;

    int const result =
        sqlite3_prepare_v2(m_database.get(), sql_.c_str(), -1, &stmt, nullptr);

    static auto _finalize = [](sqlite3_stmt* ptr)
    {
        sqlite3_finalize(ptr);
        ptr = nullptr;
    };

    if (result != SQLITE_OK)
    {
        if (m_logging)
            std::cerr << "| Error SQL :: " << sql_ << std::endl;
        log_error();
        _finalize(stmt);
        throw std::runtime_error {"Failed to prepare sqlite statement."};
    }

    m_statement.reset(stmt, _finalize);
}


void statement::bind(row const& row_) const
{
    if (!m_statement)
        throw std::runtime_error {"Statement is not initialized"};
    row_.bind(m_statement);
}


void statement::clear_bindings() const
{
    if (!m_statement)
        return;
    sqlite3_clear_bindings(m_statement.get());
}


void statement::step()
{
    if (!m_statement)
        throw std::runtime_error {"Statement is not initialized."};

    if (m_logging && !m_step_logged)
    {
        std::cout << "| Expanded SQL :: " << expanded_sql() << std::endl;
        m_step_logged = true;
    }

    int const result = sqlite3_step(m_statement.get());

    switch (result)
    {
    case SQLITE_ROW:
    {
        m_has_row = true;
        break;
    }
    case SQLITE_DONE:
    case SQLITE_OK:
    {
        m_has_row     = false;
        m_step_logged = false;
        break;
    }
    default:
    {
        log_error();
        throw std::runtime_error {"Failed to step into sqlite statement."};
    }
    }
}


void statement::reset() const
{
    if (!m_statement)
        return;
    if (sqlite3_reset(m_statement.get()) != SQLITE_OK)
        throw std::runtime_error {"Failed to reset sqlite statement."};
}


void statement::finalize() { m_statement.reset(); }


bool statement::has_row() const { return m_has_row; }


row statement::get_row() const
{
    if (!m_statement)
        throw std::runtime_error {"Statement is not initialized."};

    row result {};

    int const column_count = sqlite3_column_count(m_statement.get());

    for (int i = 0; i < column_count; ++i)
    {
        char const* name_ptr = sqlite3_column_name(m_statement.get(), i);

        if (!name_ptr)
            throw std::runtime_error {"Failed to get column name."};

        int const col_type = sqlite3_column_type(m_statement.get(), i);

        switch (col_type)
        {
        case SQLITE_INTEGER:
        {
            result.append(name_ptr,
                          column {sqlite3_column_int(m_statement.get(), i)});
            break;
        }
        case SQLITE_FLOAT:
        {
            result.append(name_ptr,
                          column {sqlite3_column_double(m_statement.get(), i)});
            break;
        }
        case SQLITE3_TEXT:
        case SQLITE_BLOB:
        {
            unsigned char const* buf =
                sqlite3_column_text(m_statement.get(), i);
            if (!buf)
                throw std::runtime_error {"Failed to get text/blob."};
            std::string s =
                buf ? reinterpret_cast<char const*>(buf) : std::string {};
            result.append(name_ptr, column {s});
            break;
        }
        case SQLITE_NULL:
        {
            result.append(name_ptr, std::string {});
            break;
        }
        default:
        {
            throw std::runtime_error {"Failed to get valid column type."};
        }
        }
    }

    return result;
}


std::vector<row> statement::execute(std::string const& sql_)
{
    return execute(sql_, {});
}


std::vector<row> statement::execute(std::string const& sql_, row const& row_)
{
    std::vector<row> results = {};
    prepare(sql_);
    bind(row_);
    while (true)
    {
        step();
        if (!has_row())
            break;
        results.push_back(get_row());
    }
    finalize();
    return results;
}


void statement::logging(bool const& enable) { m_logging = enable; }


bool statement::logging() const { return m_logging; }
} // namespace sqlite
} // namespace mm
