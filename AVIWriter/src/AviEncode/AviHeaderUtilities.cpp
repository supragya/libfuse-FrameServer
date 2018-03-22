/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#include "AviEncode.h"

uint32_t AviEncode::AviContainer::calculateSize(sizeType type) {
    uint32_t size = 0;
    switch (type) {
        case RIFF:
            size = sz.List - sz.FOURCC - sz._uint32_t  //Riff list
                   + sz.List                     // hdr1 list header
                   + sz.List                     // avih
                   + sz.AVIMAINHEADER            // AVIMAINHEADER
                   + sz.List                     // strl
                   + sz.Chunk                    // strh
                   + sz.AVISTREAMHEADER          // AVISTREAMHEADER
                   + sz.Chunk                    // strf
                   + sz.AVIBITMAPINFOHEADER      // BITMAPINFOHEADER
                   + sz.List                     // movi
                   + sz.Chunk * usersettings.framecnt    // for each frame, chunk
                   + usersettings.height * usersettings.width * 3 * usersettings.framecnt; // frame data
            break;
        case HDRL:
            size = sz.List - sz.FOURCC - sz._uint32_t  // hdrl list
                   + sz.List                     // strh
                   + sz.AVISTREAMHEADER          // AVISTREAMHEADER
                   + sz.List                     // strf
                   + sz.AVIBITMAPINFOHEADER;     // AVIBITMAPINFOHEADER
            break;
        case STRL:
            size = sz.List - sz.FOURCC - sz._uint32_t
                   + sz.Chunk
                   + sz.AVISTREAMHEADER
                   + sz.Chunk
                   + sz.AVIBITMAPINFOHEADER;
            break;
        case MOVI:
            size = sz.List - sz.FOURCC - sz._uint32_t
                   + sz.Chunk * usersettings.framecnt
                   + usersettings.height * usersettings.width * 3 * usersettings.framecnt;
            break;
        case VIDFRAME:
            size = usersettings.height * usersettings.width * 3;
            break;
    }
    return size;
}


AviEncode::avi_list_h AviEncode::AviContainer::riffHeader() {
    AviEncode::avi_list_h hdr;
    AviEncode::fcccpy(&hdr.code, "RIFF");
    hdr.listsize = calculateSize(RIFF);
    AviEncode::fcccpy(&hdr.listtype, "AVI ");
    return hdr;
}

AviEncode::avi_list_h AviEncode::AviContainer::hdrlHeader() {
    AviEncode::avi_list_h hdr;
    AviEncode::fcccpy(&hdr.code, "LIST");
    hdr.listsize = calculateSize(HDRL);
    AviEncode::fcccpy(&hdr.listtype, "hdrl");
    return hdr;
}

AviEncode::AVIMAINHEADER AviEncode::AviContainer::avihHeader() {
    AviEncode::AVIMAINHEADER hdr;
    AviEncode::fcccpy(&hdr.fcc, "avih");
    hdr.cb = sz.AVIMAINHEADER - 8;
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

AviEncode::avi_list_h AviEncode::AviContainer::strlHeader() {
    AviEncode::avi_list_h hdr;
    AviEncode::fcccpy(&hdr.code, "LIST");
    hdr.listsize = calculateSize(STRL);
    AviEncode::fcccpy(&hdr.listtype, "strl");
    return hdr;
}

AviEncode::AVISTREAMHEADER AviEncode::AviContainer::strhHeader() {
    AviEncode::AVISTREAMHEADER hdr;
    AviEncode::fcccpy(&hdr.fcc, "strh");
    hdr.cb = sz.AVISTREAMHEADER - 8;
    AviEncode::fcccpy(&hdr.fccType, "vids");
    AviEncode::fcccpy(&hdr.fccHandler, 0x00000000);
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

AviEncode::avi_chunk_h AviEncode::AviContainer::strfHeader_c() {
    AviEncode::avi_chunk_h hdr;
    AviEncode::fcccpy(&hdr.chunkID, "strf");
    hdr.chunkSize = sz.AVIBITMAPINFOHEADER;
    return hdr;
}

AviEncode::BITMAPINFOHEADER AviEncode::AviContainer::strfHeader_v() {
    AviEncode::BITMAPINFOHEADER hdr;
    hdr.Size = sz.AVIBITMAPINFOHEADER;
    hdr.Width = usersettings.width;
    hdr.Height = usersettings.height;
    hdr.Planes = 1;
    hdr.BitCount = 24;
    AviEncode::fcccpy(&hdr.Compression, 0x00000000);
    hdr.SizeImage = 0;
    hdr.XPelsPerMeter = 2835;
    hdr.YPelsPerMeter = hdr.XPelsPerMeter;
    hdr.ClrUsed = 0;
    hdr.ClrImportant = 0;
    return hdr;
}

AviEncode::avi_list_h AviEncode::AviContainer::moviHeader() {
    AviEncode::avi_list_h hdr;
    AviEncode::fcccpy(&hdr.code, "LIST");
    hdr.listsize = calculateSize(MOVI);
    AviEncode::fcccpy(&hdr.listtype, "movi");
    return hdr;
}