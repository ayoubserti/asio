cmake_minimum_required(VERSION 2.8)


project(asio-samples)


#function target builder
function(register_target tname)
		file(GLOB SOURCE_FILES_${tname} "${CMAKE_CURRENT_SOURCE_DIR}/${tname}/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/${tname}/*.h")
		
		add_executable(${tname} ${SOURCE_FILES_${tname}})
		target_include_directories(${tname} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../asio/include")
 		SET_TARGET_PROPERTIES("${tname}" PROPERTIES COMPILE_FLAGS "-MTd")

endfunction()

#function register http_parser
function(register_http_parser)
	file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../http-parser/http_parser.c" "${CMAKE_CURRENT_SOURCE_DIR}/../http-parser/http_parser.h")
	add_library(http_parser STATIC ${SOURCE_FILES})
	target_include_directories (http_parser PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/../http-parser" )
	SET_TARGET_PROPERTIES(http_parser PROPERTIES COMPILE_FLAGS "-MTd")

endfunction()



register_http_parser()
register_target(hello-world)
register_target(socket-server)
register_target(http-server)
add_dependencies(http-server http_parser)
target_include_directories (http-server PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/../http-parser" )
target_link_libraries( http-server http_parser)

