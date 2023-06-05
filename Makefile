BUILD_DIR=./build

build: source
	make -C ${BUILD_DIR}

source:
	cmake -S . -B ${BUILD_DIR}

example: build
	${BUILD_DIR}/tools/example/example ./examples/hello.toy hello

run_example: example
	clang++ output.o -o main
	./main

compile: build
	${BUILD_DIR}/tools/toy/toy ./examples/basic.toy basic

clean:
	@rm -rf src
