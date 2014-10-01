include_directories(${BMFEC_SOURCE_DIR}/src/
            ${CMAKE_CURRENT_BINARY_DIR}
			${BMFEC_SOURCE_DIR}/deps/libbmwrapper/src/
			${BMFEC_SOURCE_DIR}/deps/fecpp/src/
			)

set(
CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/
)




set(BMFEC_CLIENT_SOURCE
   ${BMFEC_CLIENT_SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)


add_subdirectory(engine)
add_subdirectory(graphics)
add_subdirectory(interfaces)
add_subdirectory(managers)
add_subdirectory(network)
add_subdirectory(parsers)
add_subdirectory(tr1)
add_subdirectory(util)
add_subdirectory(widgets)
add_subdirectory(wintypes)
add_subdirectory(wintypes/ncurses)
add_subdirectory(wintypes/sdl)



add_executable(bmfec-client ${BMFEC_CLIENT_SOURCE} ${BMFEC_CLIENT_HEADERS})

target_link_libraries(bmfec-client ${Boost_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

set_target_properties(bmfec-client PROPERTIES LINK_FLAGS "-lncurses -lbmwrapper")