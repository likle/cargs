cmake_minimum_required(VERSION 3.16)
project(cargsdemo)

include(FetchContent)
FetchContent_Declare(cargs
  GIT_REPOSITORY git@github.com:likle/cargs.git
  GIT_TAG stable
)
FetchContent_MakeAvailable(cargs)

add_executable(cargsdemo main.c)

target_link_libraries(cargsdemo cargs)
