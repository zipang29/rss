cmake_minimum_required(VERSION 3.0.0)
project(moteur-de-recherche)

SET(LIBRARY_OUTPUT_PATH ${moteur-de-recherche_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${moteur-de-recherche_SOURCE_DIR}/bin)
SET(CMAKE_INSTALL_PREFIX "${moteur-de-recherche_SOURCE_DIR}/install" CACHE PATH "Default install in project folder" FORCE)

SUBDIRS(src_kyoto src_parser)

file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/Release/)

file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/Release/)
