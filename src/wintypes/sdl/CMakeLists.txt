set(BMFEC_CLIENT_SOURCE
   ${BMFEC_CLIENT_SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/sdlwindow.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/SDLManager.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/BitmapFont.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/SDLTextureWrapper.cpp
   PARENT_SCOPE
)


set(BMFEC_CLIENT_HEADERS
   ${BMFEC_CLIENT_HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/sdlwindow.h
   ${CMAKE_CURRENT_SOURCE_DIR}/SDLManager.h
   ${CMAKE_CURRENT_SOURCE_DIR}/BitmapFont.h
   ${CMAKE_CURRENT_SOURCE_DIR}/SDLTextureWrapper.h
   PARENT_SCOPE
)


