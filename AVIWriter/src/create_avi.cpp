//
// Created by supragyaraj on 6/3/18.
//

#include "AviEncode/AviEncode.h"

int main(int argc, char** argv){

    AviEncode::AviContainer aviout("AviFile.avi", 100000);
    aviout.setAviMainHeader(AviEncode::createFourCC("avih"), 20, 20000, 2, 0, 3000, 0, 1, 10000, 1920, 1080);
    aviout.WriteHeaderSequence();
    aviout.flushBuffer();
    return 0;
}