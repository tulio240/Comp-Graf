set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ../include)

# External dependencies includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLAD_ROOT_DIR}/include
    ${GLFW_ROOT_DIR}/include
)

# External dependencies libs
target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/include
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/include
)