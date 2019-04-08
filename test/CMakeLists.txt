INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/fft/include)

INCLUDE(../cmake/CMakeBuildFlags.cmake)

IF(FFT_BUILD_SHARED)
	ADD_EXECUTABLE(test_scic_fft_shared_dft src/serial/dft.c src/util.c)
	ADD_EXECUTABLE(test_scic_fft_shared_fft src/serial/fft.c src/util.c)
	ADD_EXECUTABLE(test_scic_fft_shared_openmp_fft src/openmp/fft.c src/util.c)	

	TARGET_LINK_LIBRARIES(test_scic_fft_shared_dft scic-fft ${LIBM})
	TARGET_LINK_LIBRARIES(test_scic_fft_shared_fft scic-fft ${LIBM})
	TARGET_LINK_LIBRARIES(test_scic_fft_shared_openmp_fft scic-fft ${LIBM})

	ADD_TEST(NAME test_scic_fft_shared_dft COMMAND test_scic_fft_shared_dft)
	ADD_TEST(NAME test_scic_fft_shared_fft COMMAND test_scic_fft_shared_fft)
	ADD_TEST(NAME test_scic_fft_shared_openmp_fft COMMAND test_scic_fft_shared_openmp_fft)

	IF (MSVC)
		add_custom_command(TARGET test_scic_fft_shared POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:scic_fft>/$<TARGET_FILE_NAME:scic_fft> $<TARGET_FILE_DIR:test_scic_fft_shared>)
		IF (CMAKE_BUILD_TYPE==Debug)
			add_custom_command(TARGET test_scic_fft_shared POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${scic_fft_BINARY_DIR}/scic_fft*.pdb" $<TARGET_FILE_DIR:test_scic_fft_shared>)
		ENDIF()
	ENDIF()
ENDIF()

IF(FFT_BUILD_STATIC)
	ADD_EXECUTABLE(test_scic_fft_static_dft src/serial/dft.c src/util.c)
	ADD_EXECUTABLE(test_scic_fft_static_fft src/serial/fft.c src/util.c)
	ADD_EXECUTABLE(test_scic_fft_static_openmp_fft src/openmp/fft.c src/util.c)

	TARGET_LINK_LIBRARIES(test_scic_fft_static_dft scic-fft-static ${LIBM})
	TARGET_LINK_LIBRARIES(test_scic_fft_static_fft scic-fft-static ${LIBM})
	TARGET_LINK_LIBRARIES(test_scic_fft_static_openmp_fft scic-fft-static ${LIBM})

	ADD_TEST(NAME test_scic_fft_static_dft COMMAND test_scic_fft_static_dft)
	ADD_TEST(NAME test_scic_fft_static_fft COMMAND test_scic_fft_static_fft)
	ADD_TEST(NAME test_scic_fft_static_openmp_fft COMMAND test_scic_fft_static_openmp_fft)
ENDIF()
