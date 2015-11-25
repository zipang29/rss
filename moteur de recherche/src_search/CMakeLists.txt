project(search)

cmake_policy(SET CMP0020 NEW)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

set(SOURCES
	main.cpp
)

set(HEADERS

)

INCLUDE_DIRECTORIES(${parser_SOURCE_DIR})

add_executable(search
				${SOURCES}
)

target_link_libraries(search parser-lib)