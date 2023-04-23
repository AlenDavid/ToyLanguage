BUILD_DIR=./build

build: source
	make -C ${BUILD_DIR}

source:
	cmake -S . -B ${BUILD_DIR}

hello: build
	${BUILD_DIR}/tools/toy/toy ./examples/hello.toy hello

clean:
	@rm -rf src
