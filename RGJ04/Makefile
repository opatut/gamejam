default: bake

bake:
	[ -f sfml/include/SFML/Config.hpp ] || (echo "Please initialize SFML submodule." && false)
	(cd sfml && git apply ../sfml.patch)
	(cd sfml && mkdir -p build && cd build && cmake .. && make -j4)
	mkdir -p build 
	cd build; cmake ..; make -j4

bakerun: build bake run

go: build bakerun

godb: build bake gdb

run:
	cd bin; ./ameboid

gdb:
	cd bin; gdb ./ameboid

clean:
	rm -r build
