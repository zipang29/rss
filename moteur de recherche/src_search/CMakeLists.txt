project(search)

cmake_policy(SET CMP0020 NEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5WebKitWidgets)

set(SOURCES
	main.cpp
	SearchEngine.cpp
	MainWindow.cpp
)

set(HEADERS
	SearchEngine.h
	MainWindow.h
)

INCLUDE_DIRECTORIES(${parser_SOURCE_DIR} ${clucene_SOURCE_DIR}/include)

add_executable(search
				${SOURCES}
				${HEADERS}
)

target_link_libraries(search parser-lib Qt5::Widgets Qt5::WebKitWidgets)