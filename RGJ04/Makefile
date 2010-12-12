default: bake

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

bakerun: build bake run

go: build bakerun

godb: build bake gdb

run:
	cd bin; ./game

gdb:
	cd bin; gdb ./game

clean:
	rm -r build
