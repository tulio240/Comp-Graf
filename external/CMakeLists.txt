set(GLAD_ROOT_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/glad      CACHE STRING "glad root directory")
set(GLFW_ROOT_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/glfw      CACHE STRING "GLFW root directory")

# All
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Building dependencies as static libraries" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "Disabling dependencies documentation generation" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "Disabling dependencies tests generation" FORCE)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLAD_ROOT_DIR})
add_subdirectory(${GLFW_ROOT_DIR})
