cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
project(Zarn LANGUAGES CXX)

include(settings.cmake)

include_directories(${INCLUDE_PATH})
include_directories(${BOOST})
include_directories(${ZAR_D}/include)

set(CMAKE_CXX_STANDARD 17)

file(GLOB HEADERS ${INCLUDE_PATH}/*.h* ${INCLUDE_PATH}/tcp/tcp.h)
file(GLOB SOURCES ${SOURCE_PATH}/*.c* ${SOURCE_PATH}/tcp/tcp.cpp)

file(GLOB LIB_FILES ${ZAR_D}/lib/x64/*.lib)
#message(STATUS "Libs found: ${LIB_FILES}")

option(USE_GRPC "Use gRPC connection" ON)
if (USE_GRPC)
	# Добавить библиотеки grpc
	
	cmake_path(CONVERT ${GRPC_PATH} TO_CMAKE_PATH_LIST GRPC_PATH)
	message("Native GRPC_PATH is \"${GRPC_PATH}\"")

	list(APPEND CMAKE_MODULE_PATH "${GRPC_PATH}"/cmake)
	list(APPEND CMAKE_PREFIX_PATH "${GRPC_PATH}"/cmake)

	include(../protos/cmake/mmskngrpc.cmake)
	include(../protos/cmake/mmsknproto.cmake)

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

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})



target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_FILES})

if (USE_GRPC)
	target_include_directories(${PROJECT_NAME} 
		PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/proto
	)

	target_link_libraries(${PROJECT_NAME} 
		PRIVATE 
		mmskn_grpc_proto
		${_REFLECTION}
		${_GRPC_GRPCPP}
		${_PROTOBUF_LIBPROTOBUF}
	)
endif()


set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${ZAR_D}/Run)