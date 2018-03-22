/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#include "SyntheticFrames.h"

int SFrame::GrayScaleGradient(char *frame, unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < width; i++)
        for (unsigned int j = 0; j < height; j++)
            for (int o = 0; o < 3; o++)
                frame[3 * (i * height) + 3 * j + o] = (int) 255 * (((double)height * i + j) / (height * width));
    return 0;
}