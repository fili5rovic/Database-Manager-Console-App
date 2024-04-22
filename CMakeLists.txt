cmake_minimum_required(VERSION 3.27)
project(PrviProjekat)

set(CMAKE_CXX_STANDARD 17)

add_executable(PrviProjekat main.cpp
        Statement.cpp
        Statement.h
        Table.cpp
        Table.h
        Record.cpp
        Record.h
        Database.cpp
        Database.h
        Select.cpp
        Select.h
        Insert.cpp
        Insert.h
        Update.cpp
        Update.h
        Delete.cpp
        Delete.h
        Filter.cpp
        Filter.h
        Condition.cpp
        Condition.h
        Equal.cpp
        Equal.h
        NotEqual.cpp
        NotEqual.h
        Format.cpp
        Format.h
        CustomFormat.cpp
        CustomFormat.h
        SQLFormat.cpp
        SQLFormat.h
        DBController.h)
