OUT_DIR := bin
$(shell mkdir -p $(OUT_DIR))
bin/avi_generator: src/create_avi.cpp src/AviEncode/AviEncode.cpp
	g++ -o bin/avi_generator src/create_avi.cpp src/AviEncode/* -I src/AviEncode/ -std=c++11 -Wall -pedantic


