# SPDX-License-Identifier: AGPL-3.0-only
cmake_minimum_required(VERSION 3.16)
project(TxtBomTests LANGUAGES CXX)

get_filename_component(CORE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
# Optional override allows the SAME tests to run against a pre-fix source copy.
# No production file is rewritten by the test or by this build configuration.
set(TXT_READER_SOURCE "${CORE_ROOT}/TxtFile/Source/TxtFormat/TxtFile.cpp"
    CACHE FILEPATH "TxtFile.cpp implementation under test")
find_package(ICU REQUIRED COMPONENTS uc i18n data)

add_executable(txt_bom_tests
    main.cpp
    "${TXT_READER_SOURCE}"
    "${CORE_ROOT}/UnicodeConverter/UnicodeConverter.cpp"
    "${CORE_ROOT}/DesktopEditor/common/File.cpp"
    "${CORE_ROOT}/DesktopEditor/common/Base64.cpp")
# Keep u8 literals as std::string-compatible char arrays (C++17, not char8_t).
set_target_properties(txt_bom_tests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
target_include_directories(txt_bom_tests PRIVATE
    "${CORE_ROOT}" "${CORE_ROOT}/TxtFile/Source/TxtFormat")
target_link_libraries(txt_bom_tests PRIVATE ICU::uc ICU::i18n ICU::data)
target_compile_definitions(txt_bom_tests PRIVATE
    TXT_BOM_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
if(UNIX)
    target_compile_definitions(txt_bom_tests PRIVATE _LINUX)
endif()
if(APPLE)
    target_compile_definitions(txt_bom_tests PRIVATE _MAC)
endif()
# The standalone build supplies <memory>, normally provided transitively by
# the full core build, because TxtFile.cpp uses std::unique_ptr.
target_precompile_headers(txt_bom_tests PRIVATE <memory>)

enable_testing()
add_test(NAME txt_bom COMMAND txt_bom_tests)
