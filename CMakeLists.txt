cmake_minimum_required(VERSION 3.22)

project(Limit VERSION 0.1.0 LANGUAGES C CXX)

option(LIMIT_BUILD_TESTS "Build Limit tests" ON)
option(LIMIT_REPRODUCIBLE "Enable reproducible build flags" ON)
option(LIMIT_SANITIZE "Enable Address/Undefined sanitizers (Debug only)" OFF)
option(LIMIT_COVERAGE "Enable coverage flags (Debug only)" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)

if(LIMIT_REPRODUCIBLE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
      -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.
      -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.
    )
  endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Werror)
endif()

if(LIMIT_SANITIZE)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(FATAL_ERROR "LIMIT_SANITIZE requires Debug build type.")
  endif()
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
  else()
    message(FATAL_ERROR "LIMIT_SANITIZE is only supported on GCC/Clang.")
  endif()
endif()

if(LIMIT_COVERAGE)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(FATAL_ERROR "LIMIT_COVERAGE requires Debug build type.")
  endif()
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-O0 -g --coverage)
    add_link_options(--coverage)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-O0 -g -fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping)
  else()
    message(FATAL_ERROR "LIMIT_COVERAGE is only supported on GCC/Clang.")
  endif()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/JUCE/CMakeLists.txt")
  message(FATAL_ERROR "JUCE not found. Place JUCE in third_party/JUCE (submodule recommended).")
endif()

add_subdirectory(third_party/JUCE)

juce_add_gui_app(Limit
  PRODUCT_NAME "Limit"
  VERSION ${PROJECT_VERSION}
  COMPANY_NAME "Limit"
  BUNDLE_ID "com.limit.instrument"
  NEEDS_CURL FALSE
)

juce_generate_juce_header(Limit)

target_sources(Limit
  PRIVATE
    src/limit-app.cpp
    src/main-component.cpp
    src/dev-controller.cpp
    src/keymap.cpp
    src/ui-layout.cpp
)

target_compile_definitions(Limit
  PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

target_link_libraries(Limit
  PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
  PRIVATE
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)

if(NOT LIMIT_COVERAGE)
  target_link_libraries(Limit PRIVATE juce::juce_recommended_lto_flags)
endif()

target_include_directories(Limit PRIVATE src)

if(LIMIT_BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
  )
  FetchContent_MakeAvailable(Catch2)

  enable_testing()
  include(Catch)

  add_executable(limit-tests
    tests/smoke-test.cpp
    tests/main-component-test.cpp
    tests/ui-layout-test.cpp
    src/dev-controller.cpp
    src/keymap.cpp
    src/main-component.cpp
    src/ui-layout.cpp
  )
  target_include_directories(limit-tests PRIVATE src)
  target_compile_definitions(limit-tests PRIVATE JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)
  target_link_libraries(limit-tests
    PRIVATE
      Catch2::Catch2WithMain
      juce::juce_audio_utils
      juce::juce_recommended_config_flags
      juce::juce_recommended_warning_flags
  )

  catch_discover_tests(limit-tests)
endif()
