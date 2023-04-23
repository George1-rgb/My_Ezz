cmake_minimum_required(VERSION 3.23)

set(MEDIA_PROJECT_NAME My_Ezz_Multimedia)

set(MEDIA_PUBLIC_INCLUDES
includes/My_Ezz_Multimedia/AudioBase.hpp
includes/My_Ezz_Multimedia/Multimedia.hpp
)

set(MEDIA_PRIVATE_INCLUDES
src/My_Ezz_Multimedia/AudioBase.cpp
src/My_Ezz_Multimedia/Multimedia.cpp
)

#set(MEDIA_PRIVATE_SOURCES

#)

set(MEDIA_ALL_SOURCES
${MEDIA_PUBLIC_INCLUDES}
${MEDIA_PRIVATE_INCLUDES}
#${MEDIA_PRIVATE_SOURCES}
)

add_library(${MEDIA_PROJECT_NAME} STATIC
${MEDIA_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
${MEDIA_ALL_SOURCES}
)

target_include_directories(${MEDIA_PROJECT_NAME} PUBLIC includes)
target_include_directories(${MEDIA_PROJECT_NAME} PRIVATE src)
target_compile_features(${MEDIA_PROJECT_NAME} PUBLIC cxx_std_20)

add_subdirectory(../external/openal ${CMAKE_CURRENT_BINARY_DIR}/OpenAL)
target_link_libraries(${MEDIA_PROJECT_NAME} PRIVATE My_Ezz_Logger OpenAL)