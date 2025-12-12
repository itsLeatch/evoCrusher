#create project
cmake_minimum_required(VERSION 3.16)
project(evoCrushers VERSION 0.0.1)

#set(DCMAKE_PREFIX_PATH "D:/Programmieren/libs/libtorch-shared-with-deps-latest/libtorch/share/cmake/Torch")
#get input from cmake for c++
#configure_file(${CMAKE_SOURCE_DIR}/cmake/cmake_inputfile.h.in ${CMAKE_SOURCE_DIR}/src/projectConfig.h )

#set output dir
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})

option(SET_TORCH_MANUELLY "Manuelly set the path for Torch" OFF)
set(Torch_ROOT "" CACHE PATH "Root path to PyTorch/Torch when SET_TORCH_MANUELLY is enabled")
if(SET_TORCH_MANUELLY)
    if(Torch_ROOT)
        message(STATUS "Using Torch from ${Torch_ROOT}")
        list(APPEND CMAKE_PREFIX_PATH "${Torch_ROOT}")
    endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Setzt den Ausgabeort auch f�r spezifische Konfigurationen (z.B. Visual Studio)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})


#helper functions
macro(getSrcfiles)
set(temp "")
file(GLOB temp CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/${ARGV1}/*c" "${CMAKE_SOURCE_DIR}/${ARGV1}/*cpp" "${CMAKE_SOURCE_DIR}/${ARGV1}/*h" "${CMAKE_SOURCE_DIR}/${ARGV1}/*hpp")
list(APPEND ${ARGV0} ${temp}) 
endmacro()

#set(BUILD_SHARED_LIBS on)

configure_file(${PROJECT_SOURCE_DIR}/cmake/cmakeInput.h.in ${PROJECT_SOURCE_DIR}/src/cmakeInput.h)


#get all src files 
GETSRCFILES(project_source_files "src")

#add libaries
#add_subdirectory(libs/box2d)

find_package(ImGui-SFML CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)

find_package(Torch REQUIRED)
message(STATUS "Torch found: ${Torch_VERSION}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

#add files to exe
add_executable(${CMAKE_PROJECT_NAME} ${project_source_files} )
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)

#set compiler stuff
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

if(MSVC)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "/std:c++20")
endif()

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

#link libaries
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC box2d::box2d)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC "${TORCH_LIBRARIES}")
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ImGui-SFML::ImGui-SFML)

if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
endif (MSVC)

#copy assets
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)