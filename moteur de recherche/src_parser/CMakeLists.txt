project(parser)

cmake_policy(SET CMP0020 NEW)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt5Network)

set(SOURCES
	Constantes.cpp
	IO.cpp
	Item.cpp
	Parser.cpp
	Tika.cpp
	Indexeur.cpp
	Dictionnaire.cpp
	Stemmer.cpp
	Classifier.cpp
)

set(HEADERS
    Constantes.h
	IO.h
	Item.h
	Parser.h
	Tika.h
	Indexeur.h
	Dictionnaire.h
	Stemmer.h
	Classifier.h
)

INCLUDE_DIRECTORIES(${kyoto_SOURCE_DIR} ${clucene_SOURCE_DIR}/include ${stemmer_SOURCE_DIR}/include)

add_executable(parser
				main.cpp
)

add_library(parser-lib STATIC
			${SOURCES}
			${HEADERS}
)

target_link_libraries(parser-lib
					Qt5::Core
					Qt5::Xml
					Qt5::Network
					kyoto
					clucene-core
					clucene-shared
					stemmer
)

target_link_libraries(parser parser-lib)