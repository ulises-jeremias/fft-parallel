#!/bin/bash

pushd `pwd`

rm -rf build
mkdir build
cd build

cmake -G "Unix Makefiles" .. -DFFT_BUILD_STATIC=ON -DFFT_BUILD_SHARED=ON && \
	make && \
	ctest && \
	sudo make install

popd
