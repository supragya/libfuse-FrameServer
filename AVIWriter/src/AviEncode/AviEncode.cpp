/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 * Reference:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd318189(v=vs.85).aspx
 * https://cdn.hackaday.io/files/274271173436768/avi.pdf
 *
 */

#include "AviEncode.h"


AviEncode::AviContainer::AviContainer(const char *filename, avi_usersettings usersettings) {
    output_filename = new char[1000];
    strcpy(output_filename, filename);
    this->usersettings = usersettings;

    file.open(filename, std::ios::binary | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "[AviContainer] Error: Cannot open file \"" << filename << "\"" << std::endl;
    }

    WriteHeaderSequence();
}

int AviEncode::AviContainer::WriteHeaderSequence() {

    // Compute sizes
    DWORD szList = sizeof(AviEncode::avi_list_h);
    DWORD szChunk = sizeof(AviEncode::avi_chunk_h);
    DWORD szFOURCC = sizeof(AviEncode::FOURCC);
    DWORD szDWORD = sizeof(DWORD);
    DWORD szAVIMAINHEADER = sizeof(AviEncode::AVIMAINHEADER);
    DWORD szAVISTREAMHEADER = sizeof(AviEncode::AVISTREAMHEADER);
    DWORD szAVIBITMAPINFOHEADER = sizeof(AviEncode::BITMAPINFOHEADER);

    // RIFF List
    AviEncode::avi_list_h riff;
    AviEncode::fcccpy(&riff.code, "RIFF");
    DWORD riffsize = szList - szFOURCC - szDWORD; //Riff list
    riffsize += szList; // hdr1 list header
    riffsize += szList; // avih
    riffsize += szAVIMAINHEADER; // AVIMAINHEADER
    riffsize += szList; // strl
    riffsize += szChunk; // strh
    riffsize += szAVISTREAMHEADER; // AVISTREAMHEADER
    riffsize += szChunk; // strf
    riffsize += szAVIBITMAPINFOHEADER; // BITMAPINFOHEADER
    riffsize += szList; // movi
    riffsize += szChunk * usersettings.framecnt; // for each frame, chunk
    riffsize += usersettings.height * usersettings.width * 3 * usersettings.framecnt; // frame data
    riff.listsize = riffsize;
    AviEncode::fcccpy(&riff.listtype, "AVI ");

    // avih list
    AviEncode::avi_list_h hdrl;
    AviEncode::fcccpy(&hdrl.code, "LIST");
    DWORD hdrlsize = szList - szFOURCC - szDWORD; // hdrl list
    hdrlsize += szList; // strh
    hdrlsize += szAVISTREAMHEADER; // AVISTREAMHEADER
    hdrlsize += szList; // stff
    hdrlsize += szAVIBITMAPINFOHEADER; // AVIBITMAPINFOHEADER
    hdrl.listsize = hdrlsize;
    AviEncode::fcccpy(&hdrl.listtype, "hdrl");

    AviEncode::AVIMAINHEADER MH;
    AviEncode::fcccpy(&MH.fcc, "avih");
    MH.cb = sizeof(AviEncode::AVIMAINHEADER) - 8;
    MH.dwMicroSecPerFrame = 2854;
    MH.dwMaxBytesPerSec = 100000;
    MH.dwPaddingGranularity = 0;
    MH.dwFlags = 0;
    MH.dwTotalFrames = usersettings.framecnt;
    MH.dwInitialFrames = 0;
    MH.dwStreams = 1;
    MH.dwSuggestedBufferSize = 100000;
    MH.dwWidth = usersettings.width;
    MH.dwHeight = usersettings.height;
    MH.dwReserved[0] = MH.dwReserved[1] = MH.dwReserved[2] = MH.dwReserved[3] = 0;

    // strl list
    AviEncode::avi_list_h strl;
    AviEncode::fcccpy(&strl.code, "LIST");
    DWORD strlsize = szList - szFOURCC - szDWORD;
    strlsize += szChunk;
    strlsize += szAVISTREAMHEADER;
    strlsize += szChunk;
    strlsize += szAVIBITMAPINFOHEADER;
    strl.listsize = strlsize;
    AviEncode::fcccpy(&strl.listtype, "strl");

    // Write stream information for stream 1 strh
    AviEncode::AVISTREAMHEADER SH;
    AviEncode::fcccpy(&SH.fcc, "strh");
    SH.cb = szAVISTREAMHEADER - 8;
    AviEncode::fcccpy(&SH.fccType, "vids");
    AviEncode::fcccpy(&SH.fccHandler, "RGB2");
    SH.dwFlags = 0;
    SH.wPriority = 0;
    SH.wLanguage = 0;
    SH.dwInitialFrames = 0;
    SH.dwScale = 1;
    SH.dwRate = 30;
    SH.dwStart = 0;
    SH.dwLength = 200;
    SH.dwSuggestedBufferSize = 10000000;
    SH.dwQuality = 0;
    SH.dwSampleSize = 120;
    SH.rcFrame.left = 0;
    SH.rcFrame.right = usersettings.width;
    SH.rcFrame.top = 0;
    SH.rcFrame.bottom = usersettings.height;

    // Stream 1 strf
    AviEncode::avi_chunk_h strf;
    AviEncode::fcccpy(&strf.chunkID, "strf");
    strf.chunkSize = sizeof(AviEncode::BITMAPINFOHEADER);

    AviEncode::BITMAPINFOHEADER SF;
    SF.biSize = szAVIBITMAPINFOHEADER;
    SF.biWidth = usersettings.width;
//    std::cout<<"While writing header: "<<usersettings.width<<" "<<usersettings.height<<std::endl;
    SF.biHeight = usersettings.height;
//    std::cout<<"op: "<<SF.biHeight<<" "<<SF.biWidth<<std::endl;
    SF.biPlanes = 1;
    SF.biBitCount = 24;
    AviEncode::fcccpy(&SF.biCompression, "YUYV");
    SF.biSizeImage = 0;
    SF.biXPelsPerMeter = 2835;
    SF.biYPelsPerMeter = SF.biXPelsPerMeter; // Square Pixels
    SF.biClrUsed = 0;
    SF.biClrImportant = 0;

    // List movi
    AviEncode::avi_list_h movi;
    AviEncode::fcccpy(&movi.code, "LIST");
    DWORD movisize = szList - szFOURCC - szDWORD;
    movisize += szChunk * usersettings.framecnt;
    movisize += usersettings.height * usersettings.width * 3 * usersettings.framecnt;
    movi.listsize = movisize;
    AviEncode::fcccpy(&movi.listtype, "movi");

    std::cout << "Writing header info at " << file.tellp() << " ";
    file.write((const char *) &riff, sizeof(riff));
    file.write((const char *) &hdrl, sizeof(hdrl));
    file.write((const char *) &MH, sizeof(MH));
    file.write((const char *) &strl, sizeof(strl));
    file.write((const char *) &SH, sizeof(SH));
    file.write((const char *) &strf, sizeof(strf));
    file.write((const char *) &SF, sizeof(SF));
    file.write((const char *) &movi, sizeof(movi));
    std::cout << "ending at " << file.tellp() << std::endl;
    return 0;
}

void AviEncode::fcccpy(AviEncode::FOURCC *fcc, std::string str) {
    for (int i = 0; i < 4; i++)
        fcc->byte[i] = str[i];
}

void AviEncode::AviContainer::AddFrame(char *framedata) {
    AviEncode::avi_chunk_h vidframe;
    AviEncode::fcccpy(&vidframe.chunkID, "00db");
    vidframe.chunkSize = usersettings.height * usersettings.width * 3;
    std::cout << "Writing frame at " << file.tellp() << " ";
    file.write((const char *) &vidframe, sizeof(vidframe));
    file.write((const char *) framedata, vidframe.chunkSize);
    std::cout << "ending at " << file.tellp() << " data: " << (int) framedata[0] << " " << (int) framedata[1] << " "
              << (int) framedata[2] << std::endl;
}

AviEncode::AviContainer::~AviContainer() {
    delete output_filename;
    file.close();
}

