cmake_minimum_required(VERSION 3.0.0)
project(moteur-de-recherche)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
SET(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Default install in project folder" FORCE)

if(WIN32)
 set(CMAKE_DEBUG_POSTFIX "_d")
endif(WIN32)

SUBDIRS(clucene src_kyoto src_parser src_search src_stemmer)

#Copie des listes de flux dans les dossiers de binaires
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/ListFluxRSS-v2.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)

file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSEntrainement_fr.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSEntrainement_fr.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSEntrainement_fr.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)

file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSEntrainement_en.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSEntrainement_en.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSEntrainement_en.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)

file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSTest_fr.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSTest_fr.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSTest_fr.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)

file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSTest_en.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSTest_en.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/doc/FluxRSSTest_en.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)

#Copie de Tika dans les dossiers de binaires
file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/tika/tika-server-1.10.jar DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)

#Copie de Weka dans les dossiers de binaires
file(COPY ${moteur-de-recherche_SOURCE_DIR}/weka/weka.jar DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${moteur-de-recherche_SOURCE_DIR}/weka/weka.jar DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/)
file(COPY ${moteur-de-recherche_SOURCE_DIR}/weka/weka.jar DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)