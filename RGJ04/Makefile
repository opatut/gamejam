default: bake

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

bakerun: build bake run

go: build bakerun

run:
	cd bin; ./game

clean:
	rm -r build
