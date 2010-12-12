default: bake

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

bakerun: build bake run

go: build bakerun

godb: build bake gdb

run:
	cd bin; ./ameboid

gdb:
	cd bin; gdb ./ameboid

clean:
	rm -r build
