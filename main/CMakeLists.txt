file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/components/**/*.cpp ${CMAKE_SOURCE_DIR}/main/*.cpp)

idf_component_register(SRCS "main.cpp" ${SRC_FILES}
                    INCLUDE_DIRS ""
                    INCLUDE_DIRS "../components/HAL/include"
                    INCLUDE_DIRS "../components/Driver/include"
                    INCLUDE_DIRS "../components/App/include")
