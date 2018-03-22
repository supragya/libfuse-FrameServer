/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#include "AviEncode.h"
#include "AviSizes.h"
using namespace AviEncode;

uint32_t AviContainer::calculateSize(sizeType type) {
    uint32_t size = 0;
    switch (type) {
        case RIFF:
            size = szList - szFOURCC - sz_uint32_t  //Riff list
                   + szList                     // hdr1 list header
                   + szList                     // avih
                   + szAVIMAINHEADER            // AVIMAINHEADER
                   + szList                     // strl
                   + szChunk                    // strh
                   + szAVISTREAMHEADER          // AVISTREAMHEADER
                   + szChunk                    // strf
                   + szAVIBITMAPINFOHEADER      // BITMAPINFOHEADER
                   + szList                     // movi
                   + szChunk * usersettings.framecnt    // for each frame, chunk
                   + usersettings.height * usersettings.width * 3 * usersettings.framecnt; // frame data
            break;
        case HDRL:
            size = szList - szFOURCC - sz_uint32_t  // hdrl list
                   + szList                     // strh
                   + szAVISTREAMHEADER          // AVISTREAMHEADER
                   + szList                     // strf
                   + szAVIBITMAPINFOHEADER;     // AVIBITMAPINFOHEADER
            break;
        case STRL:
            size = szList - szFOURCC - sz_uint32_t
                   + szChunk
                   + szAVISTREAMHEADER
                   + szChunk
                   + szAVIBITMAPINFOHEADER;
            break;
        case MOVI:
            size = szList - szFOURCC - sz_uint32_t
                   + szChunk * usersettings.framecnt
                   + usersettings.height * usersettings.width * 3 * usersettings.framecnt;
            break;
        case VIDFRAME:
            size = usersettings.height * usersettings.width * 3;
            break;
    }
    return size;
}

avi_list_h AviContainer::riffHeader() {
    avi_list_h hdr;
    fcccpy(&hdr.code, "RIFF");
    hdr.listsize = calculateSize(RIFF);
    fcccpy(&hdr.listtype, "AVI ");
    return hdr;
}

avi_list_h AviContainer::hdrlHeader() {
    avi_list_h hdr;
    fcccpy(&hdr.code, "LIST");
    hdr.listsize = calculateSize(HDRL);
    fcccpy(&hdr.listtype, "hdrl");
    return hdr;
}

AVIMAINHEADER AviContainer::avihHeader() {
    AVIMAINHEADER hdr;
    fcccpy(&hdr.fcc, "avih");
    hdr.cb = szAVIMAINHEADER - 8;
    hdr.MicroSecPerFrame = 2854;
    hdr.MaxBytesPerSec = 100000;
    hdr.PaddingGranularity = 0;
    hdr.Flags = 0;
    hdr.TotalFrames = usersettings.framecnt;
    hdr.InitialFrames = 0;
    hdr.Streams = 1;
    hdr.SuggestedBufferSize = 100000;
    hdr.Width = usersettings.width;
    hdr.Height = usersettings.height;
    hdr.Reserved[0] = hdr.Reserved[1] = hdr.Reserved[2] = hdr.Reserved[3] = 0;
    return hdr;
}

avi_list_h AviContainer::strlHeader() {
    avi_list_h hdr;
    fcccpy(&hdr.code, "LIST");
    hdr.listsize = calculateSize(STRL);
    fcccpy(&hdr.listtype, "strl");
    return hdr;
}

AVISTREAMHEADER AviContainer::strhHeader() {
    AVISTREAMHEADER hdr;
    fcccpy(&hdr.fcc, "strh");
    hdr.cb = szAVISTREAMHEADER - 8;
    fcccpy(&hdr.fccType, "vids");
    fcccpy(&hdr.fccHandler, 0x00000000);
    hdr.Flags = 0;
    hdr.Priority = 0;
    hdr.Language = 0;
    hdr.InitialFrames = 0;
    hdr.Scale = 1;
    hdr.Rate = 30;
    hdr.Start = 0;
    hdr.Length = 200;
    hdr.SuggestedBufferSize = 10000000;
    hdr.Quality = 0;
    hdr.SampleSize = 120;
    hdr.rcFrame.left = 0;
    hdr.rcFrame.right = usersettings.width;
    hdr.rcFrame.top = 0;
    hdr.rcFrame.bottom = usersettings.height;
    return hdr;
}

avi_chunk_h AviContainer::strfHeader_c() {
    avi_chunk_h hdr;
    fcccpy(&hdr.chunkID, "strf");
    hdr.chunkSize = szAVIBITMAPINFOHEADER;
    return hdr;
}

BITMAPINFOHEADER AviContainer::strfHeader_v() {
    BITMAPINFOHEADER hdr;
    hdr.Size = szAVIBITMAPINFOHEADER;
    hdr.Width = usersettings.width;
    hdr.Height = usersettings.height;
    hdr.Planes = 1;
    hdr.BitCount = 24;
    fcccpy(&hdr.Compression, 0x00000000);
    hdr.SizeImage = 0;
    hdr.XPelsPerMeter = 2835;
    hdr.YPelsPerMeter = hdr.XPelsPerMeter;
    hdr.ClrUsed = 0;
    hdr.ClrImportant = 0;
    return hdr;
}

avi_list_h AviContainer::moviHeader() {
    avi_list_h hdr;
    fcccpy(&hdr.code, "LIST");
    hdr.listsize = calculateSize(MOVI);
    fcccpy(&hdr.listtype, "movi");
    return hdr;
}