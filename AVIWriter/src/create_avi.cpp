/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#include "AviEncode/AviEncode.h"
#include "AviEncode/SyntheticFrames.h"

void setavisettings(AviEncode::avi_usersettings *settings);

int main(int argc, char **argv) {

    AviEncode::avi_usersettings avisettings;
    setavisettings(&avisettings);
    AviEncode::AviContainer aviout("AviFile.avi", avisettings);

    long framelen = 480 * 270 * 3;
    char* frame = new char[framelen];
    SFrame::GrayScaleGradient(frame, 480, 270);
    for (int i = 0; i < 10; i++) {
        aviout.AddFrame(frame);
    }
    return 0;
}

void setavisettings(AviEncode::avi_usersettings *settings) {
    settings->height = 480;
    settings->width = 270;
    settings->fps = 24;
    settings->framecnt = 100;
}