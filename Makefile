SRC_DIR=.
BUILD_DIR=./build

build: source
	make -C ${BUILD_DIR}

source:
	cmake -S ${SRC_DIR} -B ${BUILD_DIR}

# to be removed in the future.
example: build
	${BUILD_DIR}/tools/example/example ./examples/hello.toy hello

# to be removed in the future.
run_example: example
	clang++ output.o -o main
	./main

compile: build
	${BUILD_DIR}/tools/toy/Toy ./examples/basic.toy basic --verbose

# delete build dir artifacts.
clean:
	@rm -rf ${BUILD_DIR}
	@rm -f output.o main

# requires sudo access.
install: build
	sudo cp ${BUILD_DIR}/tools/toy/Toy /usr/local/bin/toy
