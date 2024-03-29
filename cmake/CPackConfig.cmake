# Variable definitions are here:
# https://cmake.org/Wiki/CMake:Packaging_With_CPack
# https://cmake.org/cmake/help/v3.0/module/CPack.html

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A pure C implementations of FFT Algorithms.")
SET(CPACK_PACKAGE_VENDOR "Ulises Jeremias Cornejo Fandos")
SET(CPACK_PACKAGE_URL "https://github.com/ulises-jeremias/fft-parallel/")
SET(CPACK_PACKAGE_CONTACT "ulisescf.24@gmail.com")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
if(NOT DEFINED CPACK_RPM_PACKAGE_RELEASE)
	SET(CPACK_RPM_PACKAGE_RELEASE "2")
endif()
SET(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

#set(CPACK_SOURCE_GENERATOR "TGZ")
#set(CPACK_SOURCE_IGNORE_FILES "~$;[.]swp$;/[.]svn/;/[.]git/;.gitignore;obj*;tags;cscope.*;.ycm_extra_conf.pyc")
#set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz
                    COMMAND git archive --format=tar --prefix=${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}/ HEAD | gzip -c > ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_CUSTOM_TARGET(source_tgz
		DEPENDS ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz)

ADD_CUSTOM_TARGET(rpm
    COMMAND make source_tgz
    COMMAND rpmbuild -bb
        --define "_sourcedir ${CMAKE_CURRENT_BINARY_DIR}"
        --define "_builddir ${CMAKE_CURRENT_BINARY_DIR}/rpm"
        --define "_srcrpmdir ${CMAKE_CURRENT_BINARY_DIR}/rpm"
        --define "_rpmdir ${CMAKE_CURRENT_BINARY_DIR}/rpm"
        --define "_specdir ${CMAKE_CURRENT_BINARY_DIR}/rpm"
        ${CMAKE_PROJECT_NAME}.spec.in
    SOURCES ${CMAKE_PROJECT_NAME}.spec.in
    )
ADD_CUSTOM_TARGET(srpm
    COMMAND make source_tgz
    COMMAND rpmbuild -bs
        --define "_sourcedir ${CMAKE_CURRENT_BINARY_DIR}"
        --define "_builddir ${CMAKE_CURRENT_BINARY_DIR}/rpm"
        --define "_srcrpmdir ${CMAKE_CURRENT_BINARY_DIR}/rpm"
        --define "_rpmdir ${CMAKE_CURRENT_BINARY_DIR}/rpm"
        --define "_specdir ${CMAKE_CURRENT_BINARY_DIR}/rpm"
        ${CMAKE_PROJECT_NAME}.spec
    SOURCES ${CMAKE_PROJECT_NAME}.spec
)
