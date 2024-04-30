cmake_minimum_required(VERSION 3.27)
project(PrviProjekat)

set(CMAKE_CXX_STANDARD 17)

add_executable(PrviProjekat main.cpp
        Statement.cpp
        Table.cpp
        Record.cpp
        Database.cpp
        Select.cpp
        Insert.cpp
        Update.cpp
        Delete.cpp
        Filter.cpp
        Condition.cpp
        Equal.cpp
        NotEqual.cpp
        Format.cpp
        CustomFormat.cpp
        SQLFormat.cpp
        QueryEditor.cpp
        StringManipulator.h
        Create.cpp
)
