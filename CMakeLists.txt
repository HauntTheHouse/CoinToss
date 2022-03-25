cmake_minimum_required(VERSION 3.0.0)
project(MoneyHeist VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(MoneyHeist main.cpp)

# add GLFW
add_subdirectory(libs/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_include_directories(${PROJECT_NAME} PRIVATE libs/glfw/include)

# # add GLAD
target_include_directories(${PROJECT_NAME} PRIVATE libs/glad/include)

# add GLM
add_subdirectory(libs/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_include_directories(${PROJECT_NAME} PRIVATE libs/glm)

# add bullet3
add_subdirectory(libs/bullet3)
target_link_libraries(${PROJECT_NAME} PRIVATE BulletDynamics BulletCollision LinearMath)
target_include_directories(${PROJECT_NAME} PRIVATE libs/bullet3/src)

# add stb
target_include_directories(${PROJECT_NAME} PRIVATE libs/stb)
