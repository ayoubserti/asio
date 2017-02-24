cmake_minimum_required(VERSION 2.8)


project(asio-samples)


#function target builder
function(register_target tname)
		file(GLOB SOURCE_FILES_${tname} "${CMAKE_CURRENT_SOURCE_DIR}/${tname}/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/${tname}/*.h")
		
		add_executable(${tname} ${SOURCE_FILES_${tname}})
		target_include_directories(${tname} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../asio/include")
 		SET_TARGET_PROPERTIES("${tname}" PROPERTIES COMPILE_FLAGS "-MTd")

endfunction()



register_target(hello-world)
register_target(socket-server)