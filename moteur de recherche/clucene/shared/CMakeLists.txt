PROJECT(clucene-shared)

find_package(Threads REQUIRED)

SET(ZLIB_INCLUDE_DIR ${clucene-ext_SOURCE_DIR}/zlib )
SET(ZLIB_LIBRARY ${clucene-ext_BINARY_DIR})

#now write out our configuration....
CONFIGURE_FILE(${clucene-shared_SOURCE_DIR}/CLucene/clucene-config.h.cmake ${clucene-shared_BINARY_DIR}/CLucene/clucene-config.h)
CONFIGURE_FILE(${clucene-shared_SOURCE_DIR}/CLucene/_clucene-config.h.cmake ${clucene-shared_BINARY_DIR}/CLucene/_clucene-config.h)

MACRO (GET_SHARED_FILES result)
    SET ( rel ${clucene-shared_SOURCE_DIR} )

    SET(${result}
    	${rel}/CLucene/SharedHeader.cpp
   	  ${rel}/CLucene/config/gunichartables.cpp
    	${rel}/CLucene/config/repl_tcslwr.cpp
    	${rel}/CLucene/config/repl_tcstoll.cpp
    	${rel}/CLucene/config/repl_tcscasecmp.cpp
    	${rel}/CLucene/config/repl_tprintf.cpp
    	${rel}/CLucene/config/repl_lltot.cpp
    	${rel}/CLucene/config/repl_tcstod.cpp
    	${rel}/CLucene/config/utf8.cpp
    	${rel}/CLucene/config/threads.cpp
    	${rel}/CLucene/debug/condition.cpp
    	${rel}/CLucene/util/StringBuffer.cpp
    	${rel}/CLucene/util/Misc.cpp
      ${rel}/CLucene/util/dirent.cpp
        ${clucene-ext_SOURCE_DIR}/zlib/adler32.c
        ${clucene-ext_SOURCE_DIR}/zlib/compress.c
        ${clucene-ext_SOURCE_DIR}/zlib/crc32.c
        ${clucene-ext_SOURCE_DIR}/zlib/deflate.c
        ${clucene-ext_SOURCE_DIR}/zlib/gzio.c
        ${clucene-ext_SOURCE_DIR}/zlib/inffast.c
        ${clucene-ext_SOURCE_DIR}/zlib/inflate.c
        ${clucene-ext_SOURCE_DIR}/zlib/inftrees.c
        ${clucene-ext_SOURCE_DIR}/zlib/trees.c
        ${clucene-ext_SOURCE_DIR}/zlib/zutil.c
      )
ENDMACRO (GET_SHARED_FILES)

GET_SHARED_FILES(clucene_shared_Files ".")

#find our headers
file(GLOB_RECURSE HEADERS ${clucene-shared_SOURCE_DIR}/*.h)

INCLUDE_DIRECTORIES(${clucene_SOURCE_DIR}/core
                    ${clucene_SOURCE_DIR}/shared
                    ${clucene_SOURCE_DIR}/ext/zlib
)

add_library(clucene-shared SHARED
	${clucene_shared_Files} ${HEADERS}
)


#TARGET_LINK_LIBRARIES(clucene-shared ${CMAKE_THREAD_LIBS_INIT})
