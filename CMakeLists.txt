cmake_minimum_required(VERSION 3.14.7)

# set project name
project(cargs
  VERSION 1.0.3
  DESCRIPTION "A simple argument parser library"
  HOMEPAGE_URL "https://likle.github.io/cargs/"
  LANGUAGES C)

# include utilities
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(EnableWarnings)
include(CTest)
include(CreateTestList)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# configure requirements
set(CMAKE_C_STANDARD 11)

# setup target and directory names
set(LIBRARY_TARGET "cargs")
set(TEST_TARGET "cargstest")
set(INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

# enable coverage if requested
if(ENABLE_COVERAGE)
  message("-- Coverage enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# enable sanitizer
if(ENABLE_SANITIZER)
  message("-- Sanitizer enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=${ENABLE_SANITIZER}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=${ENABLE_SANITIZER}")
endif()

# add the main library
add_library(${LIBRARY_TARGET}
  "${INCLUDE_DIRECTORY}/cargs.h"
  "${SOURCE_DIRECTORY}/cargs.c")
enable_warnings(${LIBRARY_TARGET})
target_include_directories(${LIBRARY_TARGET} PUBLIC 
  $<BUILD_INTERFACE:${INCLUDE_DIRECTORY}>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(cargs PROPERTIES PUBLIC_HEADER "${INCLUDE_DIRECTORY}/cargs.h")
set_target_properties(cargs PROPERTIES DEFINE_SYMBOL CAG_EXPORTS)

# add shared library macro
if(BUILD_SHARED_LIBS)
  target_compile_definitions(cargs PUBLIC CAG_SHARED)
endif()

# add tests
if(ENABLE_TESTS)
  message("-- Tests enabled")
  enable_testing()
  create_test(DEFAULT option complex)
  create_test(DEFAULT option mixed)
  create_test(DEFAULT option ending)
  create_test(DEFAULT option long_missing_value)
  create_test(DEFAULT option short_missing_value)
  create_test(DEFAULT option long_space_value)
  create_test(DEFAULT option short_space_value)
  create_test(DEFAULT option long_equal_value)
  create_test(DEFAULT option short_equal_value)
  create_test(DEFAULT option combined)
  create_test(DEFAULT option unknown_long)
  create_test(DEFAULT option unknown_short)
  create_test(DEFAULT option alias)
  create_test(DEFAULT option simple_long)
  create_test(DEFAULT option simple)
  create_test(DEFAULT option print)
  create_test_list(DEFAULT "${TEST_DIRECTORY}/tests.h")

  add_executable(${TEST_TARGET}
    "${TEST_DIRECTORY}/main.c"
    "${TEST_DIRECTORY}/option_test.c")
  target_link_libraries(${TEST_TARGET} PUBLIC ${LIBRARY_TARGET})
  target_include_directories(${TEST_TARGET} PUBLIC "${INCLUDE_DIRECTORY}")
  enable_warnings(${TEST_TARGET})
endif()

# version file
configure_package_config_file("cmake/CargsConfig.cmake.in"
  ${CMAKE_CURRENT_BINARY_DIR}/CargsConfig.cmake
  INSTALL_DESTINATION ${LIB_INSTALL_DIR}/cargs/cmake)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/CargsConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

# pkg-config
configure_file(cargs.pc.in ${PROJECT_BINARY_DIR}/cargs.pc @ONLY)

# installing
install(TARGETS cargs
  EXPORT CargsTargets)

install(FILES
  ${PROJECT_BINARY_DIR}/cargs.pc
  DESTINATION lib/pkgconfig/)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/CargsConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/CargsConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cargs)
install(EXPORT CargsTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cargs)
