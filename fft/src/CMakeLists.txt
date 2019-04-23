INCLUDE_DIRECTORIES(../.)
INCLUDE_DIRECTORIES(../include)

SET(FFT_SUBDIRS
	util
	serial
	pthread-fft
	openmp
)

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
	MESSAGE(STATUS "No CMAKE_BUILD_TYPE specified, using default RELEASE")
ENDIF()
MESSAGE(STATUS "Built type is set to ${CMAKE_BUILD_TYPE}")

IF(CMAKE_COMPILER_IS_WINDOWS)
	SET(CMAKE_DEBUG_POSTFIX d)
ENDIF()

# unit tests do not use strict-prototypes, so this is here
IF(CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_CLANG)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes")
ENDIF()

SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

foreach(subdir ${FFT_SUBDIRS})
    add_subdirectory(${subdir})
    set(FFT_LIB_SRCS ${FFT_LIB_SRCS} $<TARGET_OBJECTS:${subdir}>)
endforeach()

OPTION(FFT_BUILD_STATIC "Build static library" ON)
IF(FFT_BUILD_STATIC)
	ADD_LIBRARY(scic-fft-static STATIC ${FFT_LIB_SRCS})
	SET_TARGET_PROPERTIES(scic-fft-static PROPERTIES LINKER_LANGUAGE C)

	find_library(SCIC_ERRNO_LIB scic-errno REQUIRED)
	target_link_libraries(scic-fft-static ${SCIC_ERRNO_LIB})

	find_package(Threads REQUIRED)
	target_link_libraries(scic-fft-static Threads::Threads)

	find_package(OpenMP REQUIRED)
	if(OPENMP_FOUND)
		set(LIBOPENMP "${OpenMP_C_FLAGS}")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	endif()
	target_link_libraries(scic-fft-static ${LIBOPENMP})

	if(CMAKE_COMPILER_IS_GNUC)
		SET_TARGET_PROPERTIES(scic-fft-static PROPERTIES OUTPUT_NAME "scic-fft")
	endif()
	list(APPEND FFT_LIB_TARGET scic-fft-static)
	SET_TARGET_PROPERTIES(scic-fft-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	IF(APPLE)
		SET_TARGET_PROPERTIES(scic-fft-static PROPERTIES MACOSX_RPATH ON)
	ENDIF()
ENDIF()


OPTION(FFT_BUILD_SHARED "Build shared library" ON)
IF(FFT_BUILD_SHARED)
	ADD_LIBRARY(scic-fft SHARED ${FFT_LIB_SRCS})
	SET_TARGET_PROPERTIES(scic-fft PROPERTIES LINKER_LANGUAGE C)
	SET_TARGET_PROPERTIES(scic-fft PROPERTIES VERSION ${PROJECT_VERSION})
	#SET_TARGET_PROPERTIES(scic-fft PROPERTIES SOVERSION 1)
	list(APPEND FFT_LIB_TARGET scic-fft)
	SET_TARGET_PROPERTIES(scic-fft PROPERTIES CLEAN_DIRECT_OUTPUT 1)

	find_library(SCIC_ERRNO_LIB scic-errno REQUIRED)
	target_link_libraries(scic-fft ${SCIC_ERRNO_LIB})

	find_package(Threads REQUIRED)
	target_link_libraries(scic-fft Threads::Threads)

	find_package(OpenMP REQUIRED)
	if(OPENMP_FOUND)
		set(LIBOPENMP "${OpenMP_C_FLAGS}")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	endif()
	target_link_libraries(scic-fft ${LIBOPENMP})

	IF(LINUX)
		# shared library should link to libm
		FIND_LIBRARY(M_LIB m)
		TARGET_LINK_LIBRARIES(scic-fft ${M_LIB})
	ENDIF()
	IF(APPLE)
		SET_TARGET_PROPERTIES(scic-fft PROPERTIES MACOSX_RPATH ON)
	ENDIF()
	#IF(CMAKE_COMPILER_IS_WINDOWS)
	#	SET_TARGET_PROPERTIES(scic-fft PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
	#ENDIF()
ENDIF()


IF(NOT FFT_BUILD_STATIC AND NOT FFT_BUILD_SHARED)
	MESSAGE(FATAL_ERROR "Both shared and static libraries are disabled. Please specify one or the other or both.")
ENDIF(NOT FFT_BUILD_STATIC AND NOT FFT_BUILD_SHARED)

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET(LIB_SUFFIX "64")
ENDIF()

INSTALL(TARGETS ${FFT_LIB_TARGET}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
