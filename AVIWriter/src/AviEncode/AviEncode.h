//
// Created by supragyaraj on 7/3/18.
//
#ifndef AVIENCODE_H
#define AVIENCODE_H

#include <string.h>
#include <iostream>
#include <fstream>
#include <memory>
#define DWORD uint32_t

namespace AviEncode{

    typedef struct {
        uint8_t byte[4];
    }FourCC;

    typedef struct{
        FourCC code;
        uint32_t listsize;
        FourCC listtype;
    }avi_list_h;

    typedef struct {
        FourCC chunkID;
        uint32_t chunkSize;
    }avi_chunk_h;

    FourCC createFourCC(char* c);
    void setFourCC(FourCC* fcc, char* c);
    class AviContainer{
    private:
        char* output_filename;
        unsigned int MaxBufLen;
        unsigned int BufLen;
        std::fstream file;
        typedef struct {
            FourCC fcc;
            DWORD  cb;
            DWORD  dwMicroSecPerFrame;
            DWORD  dwMaxBytesPerSec;
            DWORD  dwPaddingGranularity;
            DWORD  dwFlags;
            DWORD  dwTotalFrames;
            DWORD  dwInitialFrames;
            DWORD  dwStreams;
            DWORD  dwSuggestedBufferSize;
            DWORD  dwWidth;
            DWORD  dwHeight;
            DWORD  dwReserved[4];
        } AVIMAINHEADER;
        AVIMAINHEADER avimainheader;
        bool is_avimainheaderset;

        void showBuffer(); // Only for debug

    public:
        AviContainer(const char*, unsigned int);
        ~AviContainer();
        int addtobuffer(char*, unsigned int);
        char* writeBuffer;
        int WriteHeaderSequence();
        void setAviMainHeader(FourCC code, DWORD usecperframe, DWORD maxbytepersec,
                              DWORD paddinggranularity, DWORD flags, DWORD totalframes,
                              DWORD initialframes, DWORD streams, DWORD suggestedbufsize,
                              DWORD width, DWORD height);
        unsigned int tellFileOffset();
        int setFileOffset(unsigned int off);
        int flushBuffer();
    };
}

#endif