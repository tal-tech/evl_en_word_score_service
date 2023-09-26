cmake_minimum_required(VERSION 3.10)

project(GodeyeEvlENG)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/package")
######################################################################
#全局编译标志
######################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

######################################################################
#全局链接和包含
######################################################################

include_directories(${PROJECT_SOURCE_DIR})
include_directories("json.h")
find_package(jsoncpp REQUIRED)
include_directories(${jsoncpp_DIRECTORIES})

file(GLOB EVL_ENG_SO ${CMAKE_SOURCE_DIR}/det_evl_eng/kh_evl_linux/libeval_core.so)
file(GLOB BOOST_LIBRARY /usr/lib/x86_64-linux-gnu/libboost_*.so)
file(GLOB EVL_ENG ${CMAKE_SOURCE_DIR}/det_evl_eng/*.cpp)

#################################################################L#####
#全局编译输出指定
######################################################################

#IF(CMAKE_BUILD_TYPE MATCHES Debug)
set(CMAKE_VERBOSE_MAKEFILE ON)
#ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE})


######################################################################
#子工程目录
######################################################################
#add_subdirectory(base/third_party/dynamic_annotations)
#add_subdirectory(base/third_party/symbolize)
#add_subdirectory(base/third_party/xdg_mime)
#add_subdirectory(base/third_party/libevent)
#add_subdirectory(third_party)
#add_subdirectory(base)
#add_subdirectory(base/i18n)
#add_subdirectory(url)
#add_subdirectory(grpc)
#add_subdirectory(breakpad)
######################################################################
#编译对象
######################################################################

add_executable(GodeyeEvlENG
        det_evl_eng/evl.engeneral.cpp
        jsoncpp/jsoncpp.cpp
        det_evl_eng/task.cpp
        det_evl_eng/object_pool.cpp
)
        
target_include_directories(GodeyeEvlENG PRIVATE
        ${PROJECT_SOURCE_DIR}/det_evl_eng/kh_evl_linux
        ${PROJECT_SOURCE_DIR}/det_evl_eng
)

target_link_libraries(GodeyeEvlENG
        ${EVL_ENG_SO}
        ${BOOST_LIBRARY}
)

######################################################################
#程序发布设置
######################################################################

install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ DESTINATION bin)
install(DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ DESTINATION lib)
install(DIRECTORY det_evl_eng/kh_evl_linux/models DESTINATION models)
install(DIRECTORY det_evl_eng/kh_evl_linux/ DESTINATION lib
		FILES_MATCHING PATTERN "libeval_core.so")
install(DIRECTORY ai_model DESTINATION bin)
install(DIRECTORY config/ DESTINATION bin/config)
