#Sample definitions
cmake_minimum_required (VERSION 2.8)
project (WHAIDemo)

execute_process(COMMAND cp -r ${CMAKE_SOURCE_DIR}/WHAIDemo/config ${PROJECT_BINARY_DIR}/config)

#Define source files
set(SOURCE
    ./src/WHAIDemo.cpp
  )

add_executable(WHAIDemo ${SOURCE})

target_link_libraries(WHAIDemo ${FEZOOLIB} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${OPENNI2_LIB} ${CURL} ${LIBGLSL})


set_target_properties(WHAIDemo PROPERTIES INSTALL_RPATH "${OPENNI2_ROOT}/Redist;${Boost_LIBRARY_DIRS}")

