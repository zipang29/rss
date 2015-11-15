project(parser)

cmake_policy(SET CMP0020 NEW)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt5Network)

find_library(CLUCENE_CORE_LIB_RELEASE
			NAME clucene-core-static
			PATHS ${moteur-de-recherche_SOURCE_DIR}/../clucene/out/lib
			)
find_library(CLUCENE_CORE_LIB_DEBUG 
			NAME clucene-core-staticd
			PATHS ${moteur-de-recherche_SOURCE_DIR}/../clucene/out/lib
			)

find_library(CLUCENE_SHARED_LIB_RELEASE
			NAME clucene-shared-static
			PATHS ${moteur-de-recherche_SOURCE_DIR}/../clucene/out/lib
			)
find_library(CLUCENE_SHARED_LIB_DEBUG
			NAME clucene-shared-staticd
			PATHS ${moteur-de-recherche_SOURCE_DIR}/../clucene/out/lib
			)

SET(CLUCENE_INCLUDE_DIR ${moteur-de-recherche_SOURCE_DIR}/../clucene/out/include CACHE PATH "Dossier d'include de CLucene")

set(SOURCES
	Constantes.cpp
	IO.cpp
	Item.cpp
	main.cpp
	Parser.cpp
	Tika.cpp
)

set(HEADERS
    Constantes.h
	IO.h
	Item.h
	Parser.h
	Tika.h
)

INCLUDE_DIRECTORIES(${kyoto_SOURCE_DIR} ${CLUCENE_INCLUDE_DIR})

add_executable(parser ${SOURCES} ${HEADERS})

target_link_libraries(parser 
					Qt5::Core
					Qt5::Xml
					Qt5::Network
					kyoto
					debug ${CLUCENE_CORE_LIB_DEBUG}
					debug ${CLUCENE_SHARED_LIB_DEBUG}
					optimized ${CLUCENE_CORE_LIB_RELEASE}
					optimized ${CLUCENE_SHARED_LIB_RELEASE}
					)
