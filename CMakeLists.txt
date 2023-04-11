cmake_minimum_required(VERSION 3.16.0)

project(2d_nbody_gravity_sim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
  ${PROJECT_NAME}
  "src/main.cpp"
)