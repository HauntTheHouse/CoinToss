cmake_minimum_required(VERSION 3.0.0)
project(CoinToss VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

file(COPY shaders DESTINATION .)
file(COPY models DESTINATION .)

file(GLOB SRC_LIST CONFIGURE_DEPENDS
    include/*.h
    src/*.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_LIST}
)

add_subdirectory(libs)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm
    BulletDynamics
    BulletCollision
    LinearMath
    imgui
    assimp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    libs/glfw/include
    libs/glad/include
    libs/glm
    libs/bullet3/src
    libs/stb
    libs/imgui
    libs/assimp/include
)
