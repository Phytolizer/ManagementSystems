cmake_minimum_required(VERSION 3.12...3.22)

project("LeetCode in C" LANGUAGES "C")

add_library("management" "management/src/util.c")
target_include_directories("management" PUBLIC "management/include")
