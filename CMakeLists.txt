cmake_minimum_required(VERSION 3.16)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(CryptoCalculator)

add_executable(CryptoCalculator src/main.cpp)

target_include_directories(CryptoCalculator PRIVATE src)

target_link_libraries(CryptoCalculator
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component # Not needed for this example.
        )

# C++17 is used. We requires fold expressions at least.
set_target_properties(CryptoCalculator PROPERTIES CXX_STANDARD 17)

install(TARGETS CryptoCalculator RUNTIME DESTINATION "bin")

