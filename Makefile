BUILD_DIR=./build

build: source
	make -C ${BUILD_DIR}

source:
	cmake -S . -B ${BUILD_DIR}

hello: build
	${BUILD_DIR}/tools/toy/toy ./examples/hello.toy hello

call: build
	${BUILD_DIR}/tools/toy/toy

compile: call
	clang++ output.o -o main

clean:
	@rm -rf src
