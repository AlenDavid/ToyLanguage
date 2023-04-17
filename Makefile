build: source
	make -C ./src

source:
	cmake -S . -B ./src

hello: build
	./src/tools/toy/toy ./examples/hello.toy hello

clean:
	@rm -rf src
