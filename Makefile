SRC_DIR=.
BUILD_DIR=./build
TARGET=output.o
OPTIONS=--emit-llvm

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

# to be removed in the future.
run_compiled: compile
	clang++ ${TARGET} -o main
	./main

run_playground: playground
	clang++ playground.o -o playground
	./playground

compile: build
	${BUILD_DIR}/tools/toy/Toy ./examples/basic.toy ${TARGET} ${OPTIONS}

playground: build
	${BUILD_DIR}/tools/playground/Playground

	@echo "\n[100%] Executed target Playground"

# delete build dir artifacts.
clean:
	@git clean -x -d -f

# requires sudo access.
install: build
	sudo cp ${BUILD_DIR}/tools/toy/Toy /usr/local/bin/toy
