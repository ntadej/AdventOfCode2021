# Use C++20
set(CMAKE_CXX_STANDARD 20)

# Build configuration
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
message(STATUS "Using build configuration: ${CMAKE_BUILD_TYPE}")

# Various compiler configs
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unknown-pragmas")

# Make final release build smaller
if(CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "Using GCC")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
endif()

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# clang-tidy
find_program(
  CLANG_TIDY_EXE
  NAMES clang-tidy
  PATHS /usr/local/opt/llvm/bin /usr/local/bin /usr/bin
  DOC "Path to clang-tidy executable"
)
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "--header-filter=src/.*")
endif()

# Task helper
function(setup_day day)

  set(_target day${day})

  add_executable(${_target} src/${_target}.cxx)

  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND CLANG_TIDY_EXE)
    set_target_properties(
      ${_target} PROPERTIES
      C_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
      CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )
  endif()

  # In case we are building optimised executables with debug info, and we have
  # objcopy available, detach the debug information into a separate file.
  if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" AND CMAKE_OBJCOPY)
    add_custom_command(TARGET ${_target} POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} --only-keep-debug ${_target} ${_target}.dbg
      COMMAND ${CMAKE_OBJCOPY} --strip-debug ${_target}
      COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink=${_target}.dbg ${_target}
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMENT "Detaching debug info of ${_target} into ${_target}.dbg")
  endif()

  # Add test
  add_test(NAME ${_target} COMMAND ${_target} inputs/${_target}.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

endfunction()
