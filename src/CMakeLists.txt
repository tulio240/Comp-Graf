set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

list(REMOVE_ITEM SOURCES
    ${SRC_DIR}/clock.cpp
    ${SRC_DIR}/main_2d.cpp
    ${SRC_DIR}/main_3d.cpp
    ${SRC_DIR}/main_gpgpu.cpp
    ${SRC_DIR}/solar.cpp
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

# ---- Executável 1: clock ----
add_executable(clock ${SOURCES} ${SRC_DIR}/clock.cpp)
target_include_directories(clock PRIVATE ${PROJECT_INCLUDES} ${INCLUDE_LIBS})
target_link_libraries(clock PRIVATE ${LINK_LIBS})
target_include_directories(clock PRIVATE ../include)

# ---- Executável 2: main_2d ----
add_executable(main_2d ${SOURCES} ${SRC_DIR}/main_2d.cpp)
target_include_directories(main_2d PRIVATE ${PROJECT_INCLUDES} ${INCLUDE_LIBS})
target_link_libraries(main_2d PRIVATE ${LINK_LIBS})
target_include_directories(main_2d PRIVATE ../include)

# ---- Executável 3: main_3d ----
add_executable(main_3d ${SOURCES} ${SRC_DIR}/main_3d.cpp)
target_include_directories(main_3d PRIVATE ${PROJECT_INCLUDES} ${INCLUDE_LIBS})
target_link_libraries(main_3d PRIVATE ${LINK_LIBS})
target_include_directories(main_3d PRIVATE ../include)

add_executable(main_gpgpu ${SOURCES} ${SRC_DIR}/main_gpgpu.cpp)
target_include_directories(main_gpgpu PRIVATE ${PROJECT_INCLUDES} ${INCLUDE_LIBS})
target_link_libraries(main_gpgpu PRIVATE ${LINK_LIBS})
target_include_directories(main_gpgpu PRIVATE ../include)

add_executable(solar ${SOURCES} ${SRC_DIR}/solar.cpp)
target_include_directories(solar PRIVATE ${PROJECT_INCLUDES} ${INCLUDE_LIBS})
target_link_libraries(solar PRIVATE ${LINK_LIBS})
target_include_directories(solar PRIVATE ../include)

foreach(exec clock main_2d main_3d main_gpgpu solar)
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
