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


#include "column.hh"
#include "utilities.hh"
#include <stdexcept>

namespace mm
{
namespace sqlite
{
column::column() = default;


column::~column() = default;


column::column(int const& value_, std::string const& parameter_)
    : column {to_string(value_), data_type::INTEGER, parameter_}
{
}


column::column(double const& value_, std::string const& parameter_)
    : column {to_string(value_), data_type::REAL, parameter_}
{
}


column::column(std::string const& value_, std::string const& parameter_)
    : column {value_, data_type::TEXT, parameter_}
{
}


column::column(std::string const& value_,
               data_type const&   type_,
               std::string const& parameter_)
{
    value(value_, type_);
    if (!parameter_.empty())
        parameter(parameter_);
}


std::string const& column::value() const { return m_value; }


data_type const& column::type() const { return m_type; }


std::string const& column::parameter() const { return m_parameter; }


void column::value(int const& value_)
{
    value(to_string(value_), data_type::INTEGER);
}


void column::value(double const& value_)
{
    value(to_string(value_), data_type::REAL);
}


void column::value(std::string const& value_)
{
    value(value_, data_type::TEXT);
}


void column::value(std::string const& value_, data_type const& type_)
{
    switch (type_)
    {
    case data_type::INTEGER:
    case data_type::REAL:
    case data_type::TEXT:
    {
        m_value = value_;
        m_type  = type_;
        break;
    }
    default:
    {
        throw std::runtime_error {"Invalid data type."};
    }
    }
}


void column::parameter(std::string const& parameter_)
{
    valid_sqlite_identifier(parameter_);
    m_parameter = parameter_;
}


void column::bind(std::shared_ptr<sqlite3_stmt> const& sqlite_statement) const
{
    if (m_parameter.empty())
        return;

    if (!sqlite_statement)
        throw std::runtime_error {
            "Invalid sqlite statement to bind parameter to."};

    std::string name = ":" + m_parameter;

    int const index =
        sqlite3_bind_parameter_index(sqlite_statement.get(), name.c_str());

    if (index <= 0)
        return;

    int result = SQLITE_ERROR;

    switch (m_type)
    {
    case data_type::INTEGER:
    {
        result =
            sqlite3_bind_int(sqlite_statement.get(), index, to_int(m_value));
        break;
    }
    case data_type::REAL:
    {
        result = sqlite3_bind_double(
            sqlite_statement.get(), index, to_double(m_value));
        break;
    }
    case data_type::TEXT:
    {
        result = sqlite3_bind_text(sqlite_statement.get(),
                                   index,
                                   m_value.c_str(),
                                   -1,
                                   SQLITE_TRANSIENT);
        break;
    }
    default:
    {
        throw std::runtime_error {"Invalid data type to bind value."};
    }
    }

    if (result != SQLITE_OK)
    {
        throw std::runtime_error {"Failed to bind value to sqlite statement."};
    }
}
} // namespace sqlite
} // namespace mm
