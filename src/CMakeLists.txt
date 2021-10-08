CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

PROJECT(TYPEWRITER_EXECUTABLES)

IF (NOT DEFINED MAIN_PROJECT_DIR)
    MESSAGE(FATAL_ERROR "MAIN_PROJECT_DIR IS NOT DEFINED")
ENDIF()

FIND_PACKAGE(resource-injector)

INCLUDE(${CMAKE_SCRIPTS}/ClangFormat.cmake)

SET(TARGET_KB_READER ${PROJECT_NAME}_KB_READER)
SET(TARGET_TW_SOUND ${PROJECT_NAME}_TW_SND)

SET(EXTRA_COMPILATION_FLAGS -O3
                            -Wall                   
                            -Werror                 
                            -Wpedantic              
                            -Wextra                 
                            -Wconversion            
                            -Wcast-align            
                            -Wunused                
                            -Wshadow                
                            -Wold-style-cast        
                            -Wpointer-arith         
                            -Wcast-qual             
                            -Wno-missing-braces)
SET(EXTRA_LINK_FLAGS -fdata-sections -ffunction-sections -Wl,--gc-sections)

FILE(GLOB_RECURSE SOURCES_KB         ${MAIN_SOURCES_DIR}/keyboard/*.cpp  )
FILE(GLOB_RECURSE SOURCES_OS         ${MAIN_SOURCES_DIR}/os_linux/*.cpp  )
FILE(GLOB_RECURSE SOURCES_SOUND      ${MAIN_SOURCES_DIR}/sound/*.cpp     )
FILE(GLOB_RECURSE SOURCES_TYPEWRITER ${MAIN_SOURCES_DIR}/typewriter/*.cpp)

FILE(GLOB_RECURSE SOURCES_ALL ${MAIN_SOURCES_DIR}/*.cpp)
FILE(GLOB_RECURSE HEADERS_ALL ${MAIN_INCLUDE_DIR}/*.hpp)

SET(MAIN_KB_READER ${MAIN_SOURCES_DIR}/kb_reader.cpp)
SET(MAIN_TW_SOUND ${MAIN_SOURCES_DIR}/typewriter_sound_player.cpp)

SET(SOURCES_KB_READER ${SOURCES_KB} ${SOURCES_OS})
SET(SOURCES_TW_SOUND ${SOURCES_SOUND} ${SOURCES_TYPEWRITER} ${SOURCES_OS})

ADD_EXECUTABLE(${TARGET_KB_READER} ${MAIN_KB_READER} ${SOURCES_KB_READER})
ADD_DEPENDENCIES(${TARGET_KB_READER} CLANG_FORMAT_TARGET)

ADD_EXECUTABLE(${TARGET_TW_SOUND} ${MAIN_TW_SOUND} ${SOURCES_TW_SOUND})
TARGET_LINK_LIBRARIES(${TARGET_TW_SOUND} -lSDL2 -lSDL2_mixer -lrt)
ADD_DEPENDENCIES(${TARGET_KB_READER} CLANG_FORMAT_TARGET)

INJECT_RESOURCES(
        TARGET ${TARGET_TW_SOUND}
        RESOURCES_DIR ${PROJECT_SOURCE_DIR}/../assets
        GENERATED_DIR ${CMAKE_BINARY_DIR}/generated
        RESOURCES
            wav/down/1.wav AS wav_down_1
            wav/down/2.wav AS wav_down_2
            wav/down/3.wav AS wav_down_3
            wav/down/4.wav AS wav_down_4
            wav/down/5.wav AS wav_down_5

            wav/up/1.wav AS wav_up_1
            wav/up/2.wav AS wav_up_2
            wav/up/3.wav AS wav_up_3
            wav/up/4.wav AS wav_up_4
            wav/up/5.wav AS wav_up_5

            wav/up/carrige_return.wav AS wav_up_carrige_return

        COMPILE_TIME_RESOURCES
            config.txt AS config
)
TARGET_COMPILE_DEFINITIONS(${TARGET_TW_SOUND} PRIVATE -DMAX_SOUNDS_PER_BUTTON=3)

SET_TARGET_PROPERTIES (
    ${TARGET_KB_READER} 
    ${TARGET_TW_SOUND}
    PROPERTIES
        CXX_STANDARD        20
        INCLUDE_DIRECTORIES ${MAIN_INCLUDE_DIR}
        COMPILE_FLAGS       ${EXTRA_COMPILATION_FLAGS}
        LINK_FLAGS          ${EXTRA_LINK_FLAGS}
)
