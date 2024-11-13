cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(builder_sql)

set(PostgreSQL_AUDDITIONAL_VERSIONS "16")

add_subdirectory("C:\\work\\libpqxx-7.9.1" libpqxx-build)

add_executable(HomeworkExe main.cpp) 

target_compile_features(HomeworkExe PRIVATE cxx_std_17)

target_link_libraries(HomeworkExe PRIVATE pqxx ) 

