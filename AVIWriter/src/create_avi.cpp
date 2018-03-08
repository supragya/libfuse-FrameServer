/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#include "AviEncode/AviEncode.h"

int main(int argc, char **argv) {

    AviEncode::AviContainer aviout("AviFile.avi", 100000);
    aviout.setAviMainHeader(AviEncode::createFourCC("avih"), 20, 20000, 2, 0, 3000, 0, 1, 10000, 1920, 1080);
    aviout.WriteHeaderSequence();
    aviout.flushBuffer();
    return 0;
}