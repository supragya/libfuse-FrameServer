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
    char *frame = (char *)malloc(sizeof(char)*framelen);
    for (long k = 0; k < framelen; k++)
        *(frame+k) = (int)(k*255.0/framelen);
    for (int i = 0; i < 100; i++) {
        aviout.AddFrame(frame);
    }
    return 0;
}

// Set here the specifics of the AVI
void setavisettings(AviEncode::avi_usersettings *settings) {
    settings->height = 1920;
    settings->width = 1080;
    settings->fps = 24;
    settings->framecnt = 100;
}