cmake_minimum_required(VERSION 4.3.3)

project(BrainOrganoidTwin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(BrainOrganoidTwin
  src/Main.cpp
  src/SEM.cpp
)

target_include_directories(BrainOrganoidTwin
  PRIVATE include
)

# --- Fetch Catch2 automatically (no system install needed) ---
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# --- The test executable ---
enable_testing()
add_executable(tests test/test_main.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Register with ctest, and let Catch2 auto-discover individual TEST_CASEs
include(Catch)
catch_discover_tests(tests)
