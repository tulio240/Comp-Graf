set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

list(REMOVE_ITEM SOURCES
    ${SRC_DIR}/clock.cpp
    ${SRC_DIR}/main_2d.cpp
    ${SRC_DIR}/main_3d.cpp
    ${SRC_DIR}/main_gpgpu.cpp
    ${SRC_DIR}/solar.cpp
    ${SRC_DIR}/scene_3d.cpp
)

set(INCLUDE_LIBS ${GLAD_ROOT_DIR}/include
    ${GLFW_ROOT_DIR}/include
    ${GLM_ROOT_DIR}/include
    ${STB_ROOT_DIR}/include
    )

set(LINK_LIBS glad 
              glfw 
              glm 
              stb
    )

foreach(exec clock main_2d main_3d main_gpgpu solar scene_3d)
    string(CONCAT CPP_STRING "${exec}" ".cpp")

    add_executable(${exec} ${SOURCES} ${SRC_DIR}/${CPP_STRING})
    target_include_directories(${exec} PRIVATE ${PROJECT_INCLUDES} ${INCLUDE_LIBS})
    target_link_libraries(${exec} PRIVATE ${LINK_LIBS})
    target_include_directories(${exec} PRIVATE ../include)

    add_custom_command(
        TARGET ${exec} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/shaders
                $<TARGET_FILE_DIR:${exec}>/shaders
    )

    add_custom_command(
        TARGET ${exec} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/include
                $<TARGET_FILE_DIR:${exec}>/include
    )

    add_custom_command(
        TARGET ${exec} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/images
                $<TARGET_FILE_DIR:${exec}>/images
    )
endforeach()
