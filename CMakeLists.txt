cmake_minimum_required(VERSION 3.9)
project(Stellar)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/include /usr/local/include)

LINK_DIRECTORIES(/usr/local/lib)

find_package( OpenGL REQUIRED )

set (Cocoa_LIB "/System/Library/Frameworks/Cocoa.framework")
set (IOKit_LIB "/System/Library/Frameworks/IOKit.framework")
set (CoreVideo_LIB "/System/Library/Frameworks/CoreVideo.framework")

aux_source_directory(src/Model MODEL_SOURCE)
aux_source_directory(src/loader LOADER_SOURCE)
aux_source_directory(src/Render RENDER_SOURCE)


add_executable(Stellar main.cpp ${MODEL_SOURCE} ${LOADER_SOURCE} ${RENDER_SOURCE} src/glad.c src/Camera.cpp src/Shaders.cpp src/Scene.cpp)

message(${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_glu_LIBRARY} libglfw3.a ${Cocoa_LIB} ${IOKit_LIB} ${CoreVideo_LIB} libassimp.4.dylib)

add_custom_target(
        SHADERS
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/shaders
        COMMAND cp -rf ${CMAKE_SOURCE_DIR}/src/shaders ${CMAKE_BINARY_DIR}
)
add_dependencies(Stellar SHADERS)

#file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/shaders)
#file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})