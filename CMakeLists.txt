cmake_minimum_required(VERSION 2.8)

set (PROJECT TimeTracker)

cmake_policy(SET CMP0011 NEW)
cmake_policy(SET CMP0003 OLD)

set (LIB_HEADERS
        wrapper.h)

set (LIB_SOURCES
        wrapper.c)

set (HEADERS
        header.h
        wrapper.h)

set (SOURCES
        function.c)

set (SQL_WRAPPER wsql)

project (${PROJECT})

include(FindLibSQLite.cmake)

message(STATUS ${PC_SQLITE3_INCLUDE_DIRS})

if (MSVC)
        add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (-Wall)
else ()
        message ("Unknown compiler")
endif ()


add_library (${SQL_WRAPPER} SHARED ${LIB_HEADERS} ${LIB_SOURCES})

add_executable(${PROJECT} ${HEADERS} ${SOURCES})

target_link_libraries (${PROJECT} ${SQL_WRAPPER})



#aux_source_directory(. SRC_LIST)
