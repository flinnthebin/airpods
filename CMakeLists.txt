# set the lowerbound on CMake version.
cmake_minimum_required(VERSION 3.0)

# the name of this CMake project and what language it uses
# we could list more languages if we were using more.
project(BLE LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Find Boost
find_package(Boost 1.65 REQUIRED COMPONENTS system filesystem unit_test_framework process iostreams)

# this is helpful for editors like VS Code
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# helpful compiler flags for gcc/clang
# the descriptions for these flags can be found on the GNU Compiler Collection's webpage.
add_compile_options(
  -Wall
  -Wextra
  -Werror
  -pedantic-errors
  -Wconversion
  -Wsign-conversion
  -Wdouble-promotion
  -Wcast-align
  -Wformat=2
  -Wuninitialized
  -Wnull-dereference
  -Wnon-virtual-dtor
  -Woverloaded-virtual
  -Wdeprecated-copy-dtor
  -Wold-style-cast
  -Wzero-as-null-pointer-constant
  -Wsuggest-override
  -fstack-protector-strong
)

# debug builds should be compiled with sanitizers
# sanitizers are small libraries that check things like buffer overrun with minimal runtime overhead.
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-fsanitize=address,undefined")
set(CMAKE_CXX_EXE_LINKER_FLAGS_DEBUG_INIT "-fsanitize=address,undefined")
set(CMAKE_CXX_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT "-fsanitize=address,undefined")


# add the testing library Catch2
enable_testing()
add_library(catch2_main lib/catch2_main.cpp)
target_include_directories(catch2_main PUBLIC lib)
# link the library so that other programs can get it
link_libraries(catch2_main)

# ------------------------------------------------------------ #
# -------------- DO NOT MODIFY ABOVE THIS LINE --------------- #
# ------------------------------------------------------------ #

# Add the BLE library
add_library(blelib src/ble.cpp)
target_include_directories(blelib PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(blelib Boost::iostreams)

# Add the main executable
add_executable(ble src/ble.cpp)
target_include_directories(ble PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(ble Boost::system Boost::filesystem Boost::process Boost::iostreams)

# Add the test executable
add_executable(ble_test test/ble.test.cpp)
target_include_directories(ble_test PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(ble_test blelib Boost::system Boost::filesystem Boost::unit_test_framework Boost::process Boost::iostreams)
add_test(NAME ble_test COMMAND ble_test)
