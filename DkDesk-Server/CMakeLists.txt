cmake_minimum_required(VERSION 3.8)

project(DkDesk_Server)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS  Network REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060000)

aux_source_directory(. MY_SOURCES)
aux_source_directory(./common SRC_COMMON)

add_executable(DkDesk_Server ${MY_SOURCES} ${SRC_COMMON} )

target_link_libraries(DkDesk_Server Qt6::Network)