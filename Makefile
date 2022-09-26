INCLUDE_PATH = src/
CPP_FLAGS = -Wall -Wextra -std=c++17

_OBJS = bin/utils/log.o\
 		bin/utils/imageProcess.o\
 		bin/engine/scene.o\
		bin/engine/menu.o\
		bin/engine/parking.o\
		bin/object/car.o\

ALLEGRO_OPTIONS = -I/usr/include/x86_64-linux-gnu -lallegro_font -lallegro -lallegro_main -lallegro_ttf -lallegro_primitives -lallegro_image

all: bin/main

clean:
	rm -rf bin/

bin/main: src/main.cpp $(_OBJS)
	g++ $^ -o $@ $(CPP_FLAGS) -I$(INCLUDE_PATH) $(ALLEGRO_OPTIONS)

bin/utils/log.o: src/utils/log.cpp
	mkdir -p bin/utils
	g++ $^ -c -o $@ $(CPP_FLAGS) -I$(INCLUDE_PATH) $(ALLEGRO_OPTIONS)

bin/utils/imageProcess.o: src/utils/imageProcess.cpp
	mkdir -p bin/utils
	g++ $^ -c -o $@ $(CPP_FLAGS) -I$(INCLUDE_PATH) $(ALLEGRO_OPTIONS)

bin/engine/scene.o: src/engine/scene.cpp
	mkdir -p bin/engine
	g++ $^ -c -o $@ $(CPP_FLAGS) -I$(INCLUDE_PATH) $(ALLEGRO_OPTIONS)

bin/engine/menu.o: src/engine/menu.cpp
	mkdir -p bin/engine
	g++ $^ -c -o $@ $(CPP_FLAGS) -I$(INCLUDE_PATH) $(ALLEGRO_OPTIONS)

bin/engine/parking.o: src/engine/parking.cpp
	mkdir -p bin/engine
	g++ $^ -c -o $@ $(CPP_FLAGS) -I$(INCLUDE_PATH) $(ALLEGRO_OPTIONS)

bin/object/car.o: src/object/car.cpp
	mkdir -p bin/object
	g++ $^ -c -o $@ $(CPP_FLAGS) -I$(INCLUDE_PATH) $(ALLEGRO_OPTIONS)
