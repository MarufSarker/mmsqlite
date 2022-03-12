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
#include <sqlite3.h>
#include "enums.hh"

namespace mm
{
namespace sqlite
{
class column
{
public:
    column();
    ~column();

    column(int const& value_, std::string const& parameter_ = "");
    column(double const& value_, std::string const& parameter_ = "");
    column(std::string const& value_, std::string const& parameter_ = "");
    column(std::string const& value_,
           data_type const&   type_,
           std::string const& parameter_ = "");

    std::string const& value() const;
    data_type const&   type() const;
    std::string const& parameter() const;

    void value(int const& value_);
    void value(double const& value_);
    void value(std::string const& value_);
    void value(std::string const& value_, data_type const& type_);

    void parameter(std::string const& parameter_);
    void bind(std::shared_ptr<sqlite3_stmt> const& sqlite_statement) const;


private:
    std::string m_value     = {};
    data_type   m_type      = data_type::NONE;
    std::string m_parameter = {};
};
} // namespace sqlite
} // namespace mm
