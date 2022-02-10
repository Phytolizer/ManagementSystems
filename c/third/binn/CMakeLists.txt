cmake_minimum_required(VERSION 3.8)

project(
    binn
    LANGUAGES C
    VERSION 3.0.0
    HOMEPAGE_URL "https://github.com/liteserver/binn"
)

add_library(binn binn.c)
target_include_directories(binn PUBLIC .)
