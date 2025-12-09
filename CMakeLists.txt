#create project
cmake_minimum_required(VERSION 3.16)
project(evoCrushers VERSION 0.0.1)

#get input from cmake for c++
#configure_file(${CMAKE_SOURCE_DIR}/cmake/cmake_inputfile.h.in ${CMAKE_SOURCE_DIR}/src/projectConfig.h )

#set output dir
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})

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

set(BUILD_SHARED_LIBS on)

configure_file(${PROJECT_SOURCE_DIR}/cmake/cmakeInput.h.in ${PROJECT_SOURCE_DIR}/src/cmakeInput.h)


#create var for lib directory
set(lib_dir "${CMAKE_SOURCE_DIR}/libs")

#get all src files 

GETSRCFILES(project_source_files "src")


#add libaries
add_subdirectory(libs/SFML)
add_subdirectory(libs/box2d)

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
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC sfml-window sfml-graphics box2d)