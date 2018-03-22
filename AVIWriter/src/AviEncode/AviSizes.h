/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#ifndef AVISIZES_H
#define AVISIZES_H
#include "AviEncode.h"

#define szList sizeof(AviEncode::avi_list_h)
#define szChunk sizeof(AviEncode::avi_chunk_h)
#define szFOURCC sizeof(AviEncode::FOURCC)
#define sz_uint32_t sizeof(uint32_t)
#define szAVIMAINHEADER sizeof(AviEncode::AVIMAINHEADER)
#define szAVISTREAMHEADER sizeof(AviEncode::AVISTREAMHEADER)
#define szAVIBITMAPINFOHEADER sizeof(AviEncode::BITMAPINFOHEADER)

#endif