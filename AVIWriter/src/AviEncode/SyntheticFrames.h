/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#ifndef SYNTHETICFRAMES_H
#define SYNTHETICFRAMES_H

namespace SFrame{
    int GrayScaleGradient(char* frame, unsigned int width, unsigned int height);
    int AbstractFrame1(char* frame, unsigned int width, unsigned int height);
    int RGBStripes(char* frame, unsigned int width, unsigned int height);
}

#endif
