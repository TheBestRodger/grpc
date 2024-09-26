cmake_minimum_required(VERSION 3.21)
include(settings.cmake)
# Setup project
project(KMM VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)

# Set directories for headers search
find_package(Qt5 COMPONENTS Widgets        REQUIRED)
find_package(Qt5 COMPONENTS Core           REQUIRED)
find_package(Qt5 COMPONENTS Gui            REQUIRED)
if (Qt5_FOUND)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
endif()

# Set directories for headers search
#include_directories(${QT_PATH}/include)
include_directories(${INCLUDE_PATH})
include_directories(${LIB_PATH}/as/nmpp/include)
include_directories(${LIB_PATH})
include_directories(${LIB_PATH}/win_lib/3rdparty/opencv/include)
include_directories(${LIB_PATH}/api)	
include_directories(${BOOST_PATH})	

file(GLOB   HEADERS     ${INCLUDE_PATH}/*.h* ${INCLUDE_PATH}/tcp/*.h*)
file(GLOB   SOURCES     ${SOURCE_PATH}/*.c* ${SOURCE_PATH}/tcp/*.c*)

option(USE_GRPC "Use gRPC connection" ON)
if (USE_GRPC)
	cmake_path(CONVERT ${GRPC_PATH} TO_CMAKE_PATH_LIST GRPC_PATH)
	list(APPEND CMAKE_MODULE_PATH "${GRPC_PATH}"/cmake)
	list(APPEND CMAKE_PREFIX_PATH "${GRPC_PATH}"/cmake)
	message("GRPC_PATH: ${GRPC_PATH}")

	include(../protos/cmake/mmskngrpc.cmake)
	include(../protos/cmake/mmsknproto.cmake)

	include_directories(
		"${GRPC_PATH}/include"
	)

	# mmskn_grpc_proto
	add_library(mmskn_grpc_proto
		${mmskn_grpc_srcs}
		${mmskn_grpc_hdrs}
		${mmskn_proto_srcs}
		${mmskn_proto_hdrs}
	)
	
	target_link_libraries(mmskn_grpc_proto
		${_REFLECTION}
		${_GRPC_GRPCPP}
		${_PROTOBUF_LIBPROTOBUF}
	)

	list(APPEND HEADERS ${INCLUDE_PATH}/grpc/grpc_helper.h)
	list(APPEND SOURCES ${SOURCE_PATH}/grpc/grpc_helper.cpp)

	add_compile_definitions(USE_GRPC)

endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES} ${HEADERS})

message("HEADERS: ${HEADERS}")
message("SOURCES: ${SOURCES}")

# Make groups in project file
add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES})

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:Qt5::Widgets>
                   $<TARGET_FILE:Qt5::Core>
                   $<TARGET_FILE:Qt5::Gui>
                   $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)               
file(COPY ${QT_PATH}/plugins/platforms DESTINATION ${EXECUTABLE_OUTPUT_PATH})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Widgets)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Gui)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_PATH}/as/win_trackerDll_vs/trackerDll_vs-x64.lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_PATH}/as/win_trackerDll_vs/trackerDll_vs-x64d.lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_PATH}/as/nmpp/lib/nmpp-x64.lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_PATH}/as/nmpp/lib/nmpp-x64d.lib)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_PATH}/win_lib/eac.lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_PATH}/win_lib/3rdparty/opencv/lib/opencv_world430.lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_PATH}/win_lib/3rdparty/opencv/lib/opencv_world430d.lib)

if (USE_GRPC)
	target_include_directories(${CMAKE_PROJECT_NAME} 
		PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/proto
	)

	target_link_libraries(${CMAKE_PROJECT_NAME} 
		PRIVATE 
		mmskn_grpc_proto
		${_REFLECTION}
		${_GRPC_GRPCPP}
		${_PROTOBUF_LIBPROTOBUF}
	)
endif()


# Связывает QT5 dll файлы для проекта.
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
#set_target_properties(SKN_SPO PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE" LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
