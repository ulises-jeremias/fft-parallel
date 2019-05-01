INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(include)

SET(FFT_MAIN_HEADERS
		include/scic/fft.h
)

add_subdirectory(src)

INSTALL(FILES ${FFT_MAIN_HEADERS} DESTINATION include/scic)
# INSTALL(DIRECTORY include/scic/fft DESTINATION include/scic)
