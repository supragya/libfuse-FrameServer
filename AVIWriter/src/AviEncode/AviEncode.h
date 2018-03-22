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

#define MAXHEADERBUFLEN 400

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
        uint32_t cb;
        uint32_t MicroSecPerFrame;
        uint32_t MaxBytesPerSec;
        uint32_t PaddingGranularity;
        uint32_t Flags;
        uint32_t TotalFrames;
        uint32_t InitialFrames;
        uint32_t Streams;
        uint32_t SuggestedBufferSize;
        uint32_t Width;
        uint32_t Height;
        uint32_t Reserved[4];
    } AVIMAINHEADER;

    typedef struct _avistreamheader {
        FOURCC fcc;
        uint32_t cb;
        FOURCC fccType;
        FOURCC fccHandler;
        uint32_t Flags;
        uint16_t Priority;
        uint16_t Language;
        uint32_t InitialFrames;
        uint32_t Scale;
        uint32_t Rate;
        uint32_t Start;
        uint32_t Length;
        uint32_t SuggestedBufferSize;
        uint32_t Quality;
        uint32_t SampleSize;
        struct {
            uint8_t left;
            uint8_t top;
            uint8_t right;
            uint8_t bottom;
        } rcFrame;
    } AVISTREAMHEADER;

    typedef struct tagBITMAPINFOHEADER {
        uint32_t Size;
        uint32_t Height;
        uint32_t Width;
        uint16_t Planes;
        uint16_t BitCount;
        FOURCC Compression;
        uint32_t SizeImage;
        uint32_t XPelsPerMeter;
        uint32_t YPelsPerMeter;
        uint32_t ClrUsed;
        uint32_t ClrImportant;
    } BITMAPINFOHEADER;


    enum sizeType {
        RIFF,
        HDRL,
        STRL,
        MOVI,
        VIDFRAME
    };
    
    struct sizes{
        uint32_t List;
        uint32_t Chunk;
        uint32_t FOURCC;
        uint32_t _uint32_t;
        uint32_t AVIMAINHEADER;
        uint32_t AVISTREAMHEADER;
        uint32_t AVIBITMAPINFOHEADER;
    };

    static sizes sz;

    void fcccpy(AviEncode::FOURCC *fcc, std::string str);

    void fcccpy(AviEncode::FOURCC *fcc, int hexval);

    void init_sizes();

    class AviContainer {
    private:
        char *output_filename, *hdrBuffer;
        unsigned int hdrBufferLen;
        std::fstream file;
        avi_usersettings usersettings;

        uint32_t calculateSize(sizeType type);

        avi_list_h riffHeader();

        avi_list_h hdrlHeader();

        AVIMAINHEADER avihHeader();

        avi_list_h strlHeader();

        AVISTREAMHEADER strhHeader();

        avi_chunk_h strfHeader_c();

        BITMAPINFOHEADER strfHeader_v();

        avi_list_h moviHeader();

        int WriteHeaderSequence();

    public:
        AviContainer(const char *, avi_usersettings settings);

        void AddFrame(char *framedata);

        ~AviContainer();
    };
}

#endif