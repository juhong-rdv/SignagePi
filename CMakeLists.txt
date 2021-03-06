project(SignagePi)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#opencv
find_package(OpenCV REQUIRED)
include_directories( ${OPENCV_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

#boost
find_package(Boost REQUIRED COMPONENTS system thread filesystem regex date_time program_options)
#find_package(Boost REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${Boost_LIBRARY_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ) 

#X11
include_directories(/usr/include/X11) # I assume this is the headers include - the .h files are confirmed there
link_directories(/usr/lib/x86_64-linux-gnu/) # I assume this is linked libraries

set(APPENDED_SOURCE)

# 하위 디렉토리 추가
add_subdirectory(src)
include_directories( ./src )


# 추가 할 파일의 확장자
file(GLOB SOURCES_IN_CURRENT_DIR   "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB HEADER_IN_CURRENT_DIR   "${CMAKE_CURRENT_LIST_DIR}/*.h" 
                                  "${CMAKE_CURRENT_LIST_DIR}/*.hpp")
                                    
set(APPENDED_SOURCE ${SOURCES_IN_CURRENT_DIR} ${APPENDED_SOURCE})
set(APPENDED_HEADER ${HEADER_IN_CURRENT_DIR} ${APPENDED_HEADER})

foreach(SOURCE ${APPENDED_SOURCE})
    message("${SOURCE}")
endforeach(SOURCE)

foreach(SOURCE ${APPENDED_SOURCE})
    file(RELATIVE_PATH RELATIVE ${PROJECT_SOURCE_DIR} ${SOURCE})
    
    get_filename_component(FILENAME ${RELATIVE} NAME)
    get_filename_component(PARENT_DIRECTORY ${RELATIVE} DIRECTORY)

    message(STATUS "${FILENAME} in ${PARENT_DIRECTORY}")    
    source_group(${PARENT_DIRECTORY} FILES ${SOURCE})
endforeach(SOURCE)

add_executable( SignagePi ${APPENDED_SOURCE} )

target_link_libraries(SignagePi ${OpenCV_LIBS})
target_link_libraries(SignagePi ${Boost_LIBRARIES})
target_link_libraries(SignagePi X11 )