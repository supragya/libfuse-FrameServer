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

    long framelen = 480 * 270 * 3;
    char *frame = (char *) malloc(sizeof(char) * framelen);
    for (int i = 0; i < 270; i++) {
        for (int j = 0; j < 480; j++) {
            for (int o = 0; o < 3; o++)
                frame[3 * (i * 480) + 3 * j + o] = (int) 255 * ((480.0 * i + j) / (480 * 270));
        }
    }
    for (int i = 0; i < 100; i++) {
        aviout.AddFrame(frame);
    }
    return 0;
}

// Set here the specifics of the AVI
void setavisettings(AviEncode::avi_usersettings *settings) {
    settings->height = 480;
    settings->width = 270;
    settings->fps = 24;
    settings->framecnt = 100;
}