cmake_minimum_required(VERSION 3.0.0)
project(moteur-de-recherche)

SET(LIBRARY_OUTPUT_PATH ${moteur-de-recherche_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${moteur-de-recherche_SOURCE_DIR}/bin)
SET(CMAKE_INSTALL_PREFIX "${moteur-de-recherche_SOURCE_DIR}/install" CACHE PATH "Default install in project folder" FORCE)

if(WIN32)
 set(CMAKE_DEBUG_POSTFIX "_d")
endif(WIN32)

SUBDIRS(clucene src_kyoto src_parser src_search src_stemmer)

#Copie de la liste des flux dans les dossiers de binaires
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/Release/)

#Copie de Tika dans les dossiers de binaires
file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${moteur-de-recherche_SOURCE_DIR}/bin/Release/)
