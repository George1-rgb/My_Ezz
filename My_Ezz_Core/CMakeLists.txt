cmake_minimum_required(VERSION 3.23)

set(CORE_PROJECT_NAME My_Ezz_Core)

set(CORE_PUBLIC_INCLUDES
includes/My_Ezz_Core/Application.hpp
includes/My_Ezz_Core/Log.hpp
includes/My_Ezz_Core/Event.hpp
includes/My_Ezz_Core/Camera.hpp
includes/My_Ezz_Core/Keys.hpp
includes/My_Ezz_Core/Input.hpp
)

set(CORE_PRIVATE_INCLUDES
src/My_Ezz_Core/Window.hpp
src/My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp
src/My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp
src/My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp
src/My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp
src/My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp
src/My_Ezz_Core/Modules/UIModule.hpp
)

set(CORE_PRIVATE_SOURCES
src/My_Ezz_Core/Application.cpp
src/My_Ezz_Core/Window.cpp
src/My_Ezz_Core/Input.cpp
src/My_Ezz_Core/Camera.cpp
src/My_Ezz_Core/Rendering/OpenGL/ShaderProgram.cpp
src/My_Ezz_Core/Rendering/OpenGL/VertexBuffer.cpp
src/My_Ezz_Core/Rendering/OpenGL/IndexBuffer.cpp
src/My_Ezz_Core/Rendering/OpenGL/VertexArray.cpp
src/My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.cpp
src/My_Ezz_Core/Modules/UIModule.cpp
)

set(CORE_ALL_SOURCES
${CORE_PUBLIC_INCLUDES}
${CORE_PRIVATE_INCLUDES}
${CORE_PRIVATE_SOURCES}
)

add_library(${CORE_PROJECT_NAME} STATIC
${CORE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
${CORE_ALL_SOURCES}
)

target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${CORE_PROJECT_NAME} PRIVATE src)
target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_20)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glm)

set(IMGUI_INCLUDES
../external/imgui/imgui.h
../external/imgui/backends/imgui_impl_glfw.h
../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
../external/imgui/imgui.cpp
../external/imgui/backends/imgui_impl_glfw.cpp
../external/imgui/backends/imgui_impl_opengl3.cpp
../external/imgui/imgui_demo.cpp
../external/imgui/imgui_draw.cpp
../external/imgui/imgui_tables.cpp
../external/imgui/imgui_widgets.cpp
)

add_library(ImGui STATIC
${IMGUI_INCLUDES}
${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)

target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${CORE_PROJECT_NAME} PRIVATE ImGui)