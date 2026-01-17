cmake_minimum_required(VERSION 3.22)

project(Limit VERSION 0.1.0 LANGUAGES C CXX)

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
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
