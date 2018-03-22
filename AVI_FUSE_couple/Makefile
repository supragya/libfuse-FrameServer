OUT_DIR := bin
$(shell mkdir -p $(OUT_DIR))
bin/FrameServer: src/couple.cpp src/AviEncode/AviEncode.cpp
	g++ -o bin/FrameServer src/couple.cpp src/AviEncode/* -I src/AviEncode/ -std=c++11 -Wall -pedantic -D_FILE_OFFSET_BITS=64


