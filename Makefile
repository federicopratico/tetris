OPTIONS=-DNDEBUG -Wall -Wextra

sanitizers: OPTIONS=-fsanitize=address -fno-omit-frame-pointer -g -DDEBUG -Wall -Wextra
sanitizers: build/tetris


build/tetris: build/main.o build/tetris.o
	g++ ${OPTIONS} build/main.o build/tetris.o -o tetris

build/main.o: src/main.cpp include/tetris.hpp
	g++ ${OPTIONS} -c src/main.cpp -o build/main.o -I include/

build/tetris.o: src/tetris.cpp include/tetris.hpp
	g++ ${OPTIONS} -c src/tetris.cpp -o build/tetris.o -I include/

clean:
	rm -rf build/*.o tetris





