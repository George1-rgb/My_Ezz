cmake_minimum_required(VERSION 3.23)

set(LOG_PROJECT_NAME My_Ezz_Logger)

set(LOG_PUBLIC_INCLUDES
includes/My_Ezz_Logger/Log.hpp
)

#set(MEDIA_PRIVATE_INCLUDES

#)

set(LOG_PRIVATE_SOURCES
src/My_Ezz_Logger/Log.cpp
)

set(LOG_ALL_SOURCES
${LOG_PUBLIC_INCLUDES}
#${MEDIA_PRIVATE_INCLUDES}
${LOG_PRIVATE_SOURCES}
)

add_library(${LOG_PROJECT_NAME} STATIC
${LOG_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
${LOG_ALL_SOURCES}
)

target_include_directories(${LOG_PROJECT_NAME} PUBLIC includes)
target_include_directories(${LOG_PROJECT_NAME} PRIVATE src)
target_compile_features(${LOG_PROJECT_NAME} PUBLIC cxx_std_20)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${LOG_PROJECT_NAME} PUBLIC spdlog)

