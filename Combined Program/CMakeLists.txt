cmake_minimum_required(VERSION 3.30)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

# Add your source files
add_executable(untitled
        main.cpp
        db.cpp
)

# Include current directory for headers
include_directories(${CMAKE_SOURCE_DIR})

# Link sqlite3 static library
target_link_libraries(untitled PRIVATE ${CMAKE_SOURCE_DIR}/libsqlite3.a)
