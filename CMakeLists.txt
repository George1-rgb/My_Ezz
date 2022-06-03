cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME My_Ezz)

add_subdirectory(My_Ezz_Editor)
add_subdirectory(My_Ezz_Core)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT My_Ezz_Editor)