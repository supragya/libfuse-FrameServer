/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd318189(v=vs.85).aspx
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd318229(v=vs.85).aspx
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd318183(v=vs.85).aspx
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd318180(v=vs.85).aspx
 */

#ifndef AVIENCODE_H
#define AVIENCODE_H

#include <string.h>
#include <iostream>
#include <fstream>
#include <memory>

#define DWORD uint32_t
#define WORD uint16_t
#define LONG long

namespace AviEncode {

    typedef struct {
        uint8_t byte[4];
    } FOURCC;

    typedef struct {
        FOURCC code;
        uint32_t listsize;
        FOURCC listtype;
    } avi_list_h;

    typedef struct {
        FOURCC chunkID;
        uint32_t chunkSize;
    } avi_chunk_h;

    typedef struct {
        unsigned int height;
        unsigned int width;
        unsigned int fps;
        unsigned int framecnt;
    } avi_usersettings;

    typedef struct _avimainheader {
        FOURCC fcc;
        DWORD cb;
        DWORD dwMicroSecPerFrame;
        DWORD dwMaxBytesPerSec;
        DWORD dwPaddingGranularity;
        DWORD dwFlags;
        DWORD dwTotalFrames;
        DWORD dwInitialFrames;
        DWORD dwStreams;
        DWORD dwSuggestedBufferSize;
        DWORD dwWidth;
        DWORD dwHeight;
        DWORD dwReserved[4];
    } AVIMAINHEADER;

    typedef struct _avistreamheader {
        FOURCC fcc;
        DWORD cb;
        FOURCC fccType;
        FOURCC fccHandler;
        DWORD dwFlags;
        WORD wPriority;
        WORD wLanguage;
        DWORD dwInitialFrames;
        DWORD dwScale;
        DWORD dwRate;
        DWORD dwStart;
        DWORD dwLength;
        DWORD dwSuggestedBufferSize;
        DWORD dwQuality;
        DWORD dwSampleSize;
        struct {
            uint8_t left;
            uint8_t top;
            uint8_t right;
            uint8_t bottom;
        } rcFrame;
    } AVISTREAMHEADER;

    typedef struct tagBITMAPINFOHEADER {
        DWORD biSize;
        DWORD biHeight;
        DWORD biWidth;
        WORD biPlanes;
        WORD biBitCount;
        FOURCC biCompression;
        DWORD biSizeImage;
        DWORD biXPelsPerMeter;
        DWORD biYPelsPerMeter;
        DWORD biClrUsed;
        DWORD biClrImportant;
    } BITMAPINFOHEADER;

    void fcccpy(AviEncode::FOURCC *fcc, std::string str);

    class AviContainer {
    private:
        char *output_filename;
        unsigned int MaxBufLen;
        std::fstream file;
        avi_usersettings usersettings;

        int WriteHeaderSequence();

    public:
        AviContainer(const char *, avi_usersettings settings);

        void AddFrame(char *framedata);

        ~AviContainer();
    };
}

#endif