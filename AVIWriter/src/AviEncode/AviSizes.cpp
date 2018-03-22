/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#include "AviEncode.h"

void AviEncode::init_sizes() {
    sz.List = sizeof(AviEncode::avi_list_h);
    sz.Chunk = sizeof(AviEncode::avi_chunk_h);
    sz.FOURCC = sizeof(AviEncode::FOURCC);
    sz._uint32_t = sizeof(uint32_t);
    sz.AVIMAINHEADER = sizeof(AviEncode::AVIMAINHEADER);
    sz.AVISTREAMHEADER = sizeof(AviEncode::AVISTREAMHEADER);
    sz.AVIBITMAPINFOHEADER = sizeof(AviEncode::BITMAPINFOHEADER);

    std::cout<<"INIT SIZES called"<<std::endl;
    std::cout<<sz.List<<" "<<sz.Chunk<<" "<<sz.FOURCC<<" "<<sz._uint32_t<<" "<<std::endl;
}
