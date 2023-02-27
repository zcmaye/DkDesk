cmake_minimum_required(VERSION 3.8)

project(QtProject)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060000)

#根据ui文件生成头文件
qt_wrap_ui(QtProject "MainWindow.ui")	
#包含ui生成的头文件的目录
include_directories(CMAKE_INCLUDE_PATH ${CMAKE_BINARY_DIR})

aux_source_directory(. MY_SOURCES)
aux_source_directory(./common SRC_COMMON)
aux_source_directory(./network SRC_NETWORK)


add_executable(QtProject 
${MY_SOURCES} 
${SRC_COMMON} 
${SRC_NETWORK} "assets.qrc" "MainWindow.ui")

target_link_libraries(QtProject Qt6::Widgets Qt6::Network)