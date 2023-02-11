INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/fft/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/benchmark/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/third_party/simple-parse/src)

INCLUDE(../cmake/CMakeBuildFlags.cmake)

IF(FFT_BUILD_SHARED)
	ADD_EXECUTABLE(benchmark_scic_fft_shared main.c util.c ${CMAKE_SOURCE_DIR}/third_party/simple-parse/src/sparse.c)

	target_link_libraries(benchmark_scic_fft_shared ${LIBM} scic-fft)

	find_package(Threads REQUIRED)
	target_link_libraries(benchmark_scic_fft_shared Threads::Threads)

	find_package(OpenMP REQUIRED)
	target_link_libraries(benchmark_scic_fft_shared OpenMP::OpenMP_C)

	IF (MSVC)
		add_custom_command(TARGET benchmark_scic_fft_shared POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:scic_fft>/$<TARGET_FILE_NAME:scic_fft> $<TARGET_FILE_DIR:benchmark_scic_fft_shared>)
		IF (CMAKE_BUILD_TYPE==Debug)
			add_custom_command(TARGET benchmark_scic_fft_shared POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${scic_fft_BINARY_DIR}/scic_fft*.pdb" $<TARGET_FILE_DIR:benchmark_scic_fft_shared>)
		ENDIF()
	ENDIF()
ENDIF()

IF(FFT_BUILD_STATIC)
	ADD_EXECUTABLE(benchmark_scic_fft_static main.c util.c ${CMAKE_SOURCE_DIR}/third_party/simple-parse/src/sparse.c)

	target_link_libraries(benchmark_scic_fft_static ${LIBM} scic-fft-static)

	find_package(Threads REQUIRED)
	target_link_libraries(benchmark_scic_fft_static Threads::Threads)

	find_package(OpenMP REQUIRED)
	target_link_libraries(benchmark_scic_fft_static OpenMP::OpenMP_C)
ENDIF()
