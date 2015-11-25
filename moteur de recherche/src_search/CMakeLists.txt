project(search)

cmake_policy(SET CMP0020 NEW)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

set(SOURCES
	main.cpp
	SearchEngine.cpp
)

set(HEADERS
	SearchEngine.h
)

INCLUDE_DIRECTORIES(${parser_SOURCE_DIR} ${clucene_SOURCE_DIR}/include)

add_executable(search
				${SOURCES}
				${HEADERS}
)

target_link_libraries(search parser-lib)