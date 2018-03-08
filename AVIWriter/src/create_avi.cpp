/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#include "AviEncode/AviEncode.h"

void setavisettings(AviEncode::avi_usersettings *settings);

int main(int argc, char **argv) {

    AviEncode::avi_usersettings avisettings;
    setavisettings(&avisettings);
    AviEncode::AviContainer aviout("AviFile.avi", avisettings);

    long framelen = 1920 * 1080 * 3;
    char *frame = new char[framelen];
    for (int i = 0; i < 10; i++) {
        for (long k = 0; k < framelen; k++)
            frame[i] = (i*2)%255;
        aviout.AddFrame(frame);
    }

    return 0;
}

// Set here the specifics of the AVI
void setavisettings(AviEncode::avi_usersettings *settings) {
    settings->height = 1920;
    settings->width = 1080;
    settings->fps = 24;
}