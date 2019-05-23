cmake_minimum_required(VERSION 3.1)

project(chatServer)

set(Boost_USE_STATIC_LIBS ON)

set(BOOST_COMPONENTS
    asio
    thread
    algorithm)

set(SOURCE_TALKTOCLIENT talkToClient.cpp)
set(HEADER_TALKTOCLIENT talkToClient.h)
set(SOURCE_SERVER chatServer.cpp)
set(HEADER_SERVER chatServer.h)

find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED_FILES)

add_library(server STATIC ${SOURCE_SERVER} ${HEADER_SERVER})
add_library(talkToClient STATIC ${SOURCE_TALKTOCLIENT} ${HEADER_TALKTOCLIENT})

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} server talkToClient Boost::thread)
