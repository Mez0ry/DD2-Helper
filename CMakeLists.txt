cmake_minimum_required( VERSION 3.0...4.0)
set(CMAKE_CONFIGURATION_TYPES "Debug\;Release")

set(PROJECT_NAME DD2 LANGUAGES CXX)
project(${PROJECT_NAME} )

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-D_DEBUG)
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
    add_definitions(-D_RELEASE) 
endif()

set(PROCESSOR_ARHITECTURE_TYPE "UNKNOWN")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PROCESSOR_ARHITECTURE_TYPE "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PROCESSOR_ARHITECTURE_TYPE "x32")
else()
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" LOWER_CASE_PROC)

    if(${LOWER_CASE_PROC} STREQUAL "arm" OR ${LOWER_CASE_PROC} STREQUAL "aarch64")
        set(PROCESSOR_ARHITECTURE_TYPE "ARM")
    elseif(${LOWER_CASE_PROC} STREQUAL "powerpc" OR ${LOWER_CASE_PROC} STREQUAL "ppc64le")
        set(PROCESSOR_ARHITECTURE_TYPE "PowerPC")
    elseif(${LOWER_CASE_PROC} STREQUAL "mips" OR ${LOWER_CASE_PROC} STREQUAL "mipsel")
        set(PROCESSOR_ARHITECTURE_TYPE "MIPS")
    elseif(${LOWER_CASE_PROC} STREQUAL "riscv64")
        set(PROCESSOR_ARHITECTURE_TYPE "RISC-V 64-bit")
    else()
        message(ERROR "Unrecognized arhitecture type: ${CMAKE_SYSTEM_PROCESSOR}. Setting PROCESSOR_ARHITECTURE_TYPE to Unknown.")
    endif()
endif()

message(STATUS "Building for arhitecture: ${PROCESSOR_ARHITECTURE_TYPE}, build type: ${CMAKE_BUILD_TYPE}")

#custom targets
add_custom_target(C_CLEANUP
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}/lib"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}/bin"
    COMMENT "Cleaning output directory ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}"
)

set(COPY_MARKER_FILE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}/bin/resources/marker)

add_custom_command(
    OUTPUT ${COPY_MARKER_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}/bin/resources
    COMMAND ${CMAKE_COMMAND} -E touch ${COPY_MARKER_FILE}
    DEPENDS ${CMAKE_SOURCE_DIR}/resources
    COMMENT "Copying entire resources directory to binary folder"
)

add_custom_target(copy_resources ALL DEPENDS ${COPY_MARKER_FILE})

set(HEADER_FILES  
    ${CMAKE_SOURCE_DIR}/include/App.hpp
    ${CMAKE_SOURCE_DIR}/include/HandleGuard.hpp
    ${CMAKE_SOURCE_DIR}/include/VKeys.hpp
    ${CMAKE_SOURCE_DIR}/include/Window.hpp
    ${CMAKE_SOURCE_DIR}/include/KeyboardInput.hpp
    ${CMAKE_SOURCE_DIR}/include/MouseInput.hpp
    ${CMAKE_SOURCE_DIR}/include/UserConfig.hpp
    ${CMAKE_SOURCE_DIR}/include/JsonFileWatcher.hpp
    ${CMAKE_SOURCE_DIR}/include/KeySequence.hpp

)

set(SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/App.cpp
    ${CMAKE_SOURCE_DIR}/src/Window.cpp
    ${CMAKE_SOURCE_DIR}/src/KeyboardInput.cpp
    ${CMAKE_SOURCE_DIR}/src/MouseInput.cpp
    ${CMAKE_SOURCE_DIR}/src/UserConfig.cpp
    ${CMAKE_SOURCE_DIR}/src/JsonFileWatcher.cpp
    ${CMAKE_SOURCE_DIR}/src/KeySequence.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} copy_resources)
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -O3
            -DNDEBUG         
            -flto             
            -march=native    
            -mtune=native
            -funroll-loops   
            -fomit-frame-pointer
        )
        target_link_options(${PROJECT_NAME} PRIVATE
            -flto
        )
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#target properties
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}/bin"
    PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROCESSOR_ARHITECTURE_TYPE}/bin"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -O3
        -DNDEBUG
        -march=native
        -mtune=native
        -funroll-loops
        -fomit-frame-pointer
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /O2
        /Ob2
        /DNDEBUG
        /GL
    )
endif()

#vendor rapidjson
    #target_link_libraries(${PROJECT_NAME} RapidJSON::RapidJSON)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/rapidjson/include)
#end(rapidjson)