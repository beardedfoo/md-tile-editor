default: build/rom.bin

build/rom.bin: main.c
	mkdir -p build
	docker run -v `pwd`:/src -v `pwd`/build:/src/out beardedfoo/gendev:0.3.0

# Uses github.com/beardedfoo/devkit-mega-everdrive-x7
run-megaedx7:
	megaedx7-run build/rom.bin
