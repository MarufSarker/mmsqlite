# mmsqlite
# Copyright (C) 2022  Maruf Sarker
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# [ Project

project(mmsqlite)

if(NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 11)
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ] Project

# [ Options

set(MM_SQLITEORG_DIR "" CACHE PATH "SQLite sources directory")

# ] Options

# [ Files

file(GLOB_RECURSE MM_HEADERS
    "sources/mm/sqlite/*.hh"
)

file(GLOB_RECURSE MM_SOURCES
    "sources/mm/sqlite/*.cc"
)

# ] Files

# [ SQLite

if(NOT MM_SQLITEORG_DIR)
    find_package(SQLite3 REQUIRED)
else()
    file(GLOB_RECURSE MM_SQLITEORG_FILES
        "${MM_SQLITEORG_DIR}/sqlite3.h"
        "${MM_SQLITEORG_DIR}/sqlite3ext.h"
        "${MM_SQLITEORG_DIR}/sqlite3.c"
    )

    list(APPEND MM_SOURCES "${MM_SQLITEORG_FILES}")
endif()

# ] SQLite

# [ Library

add_library(${PROJECT_NAME}
    ${MM_SOURCES}
    ${MM_HEADERS}
)

# ] Library

# [ Compiler Options

set(MM_GNU_CXX_COMMON_FLAGS
    -s
    -fPIC
    -ftime-report
    -fdiagnostics-color=auto
    -Wall
    -Wextra
    -Wunused
    -Wshadow
    -Wpedantic
    -Wformat=2
    -Wconversion
    -Wcast-align
    -Wlogical-op
    -Wuseless-cast
    -Wold-style-cast
    -Wduplicated-cond
    -Wsign-conversion
    -Wdouble-promotion
    -Wnon-virtual-dtor
    -Wnull-dereference
    -Woverloaded-virtual
    -Wduplicated-branches
)

set(MM_GNU_CXX_COMPILE_FLAGS_RELEASE
    -O3
    -DNDEBUG
    ${MM_GNU_CXX_COMMON_FLAGS}
)

set(MM_GNU_CXX_COMPILE_FLAGS_DEBUG
    -O0
    -g3
    -ggdb3
    ${MM_GNU_CXX_COMMON_FLAGS}
)

# ] Compiler Options

# [ Target Options

find_package(Threads REQUIRED)

if(MM_SQLITEORG_DIR)
    target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        SQLITE_USE_URI=1
        SQLITE_THREADSAFE=1
        SQLITE_SECURE_DELETE
        SQLITE_ENABLE_NORMALIZE
    )
endif()

target_link_libraries(${PROJECT_NAME}
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

if(NOT MM_SQLITEORG_DIR)
    target_link_libraries(${PROJECT_NAME}
        ${SQLite3_LIBRARIES}
    )

    target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${SQLite3_INCLUDE_DIRS}"
    )
else()
    target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${MM_SQLITEORG_DIR}"
    )
endif()

target_compile_options(${PROJECT_NAME}
PUBLIC
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:DEBUG>>:
        ${MM_GNU_CXX_COMPILE_FLAGS_DEBUG}>
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CONFIG:DEBUG>>:
        ${MM_GNU_CXX_COMPILE_FLAGS_DEBUG}>

    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:RELEASE>>:
        ${MM_GNU_CXX_COMPILE_FLAGS_RELEASE}>
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CONFIG:RELEASE>>:
        ${MM_GNU_CXX_COMPILE_FLAGS_RELEASE}>
)

# ] Target Options
