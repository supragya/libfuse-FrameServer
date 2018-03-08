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

    file.open(filename, std::ios::binary | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "[AviContainer] Error: Cannot open file \"" << filename << "\"" << std::endl;
    }

    WriteHeaderSequence();
}

int AviEncode::AviContainer::WriteHeaderSequence() {

    // RIFF List
    AviEncode::avi_list_h riff;
    AviEncode::fcccpy(&riff.code, "RIFF");
    riff.listsize = 1000000; // TODO: calculate filesize here
    AviEncode::fcccpy(&riff.listtype, "AVI ");

    // avih list
    AviEncode::avi_list_h hdrl;
    AviEncode::fcccpy(&hdrl.code, "LIST");
    hdrl.listsize = sizeof(AviEncode::avi_list_h) - 8 +
                    sizeof(AviEncode::AVIMAINHEADER) + sizeof(AviEncode::avi_list_h) +
                    sizeof(AviEncode::AVISTREAMHEADER) +
                    sizeof(AviEncode::BITMAPINFOHEADER);    // NOTE: Are we correct here?
    AviEncode::fcccpy(&hdrl.listtype, "hdr1");

    // Write main header for the AVI
    AviEncode::avi_chunk_h avih;
    AviEncode::fcccpy(&avih.chunkID, "avih");
    avih.chunkSize = sizeof(AviEncode::AVIMAINHEADER);

    AviEncode::AVIMAINHEADER MH;
    AviEncode::fcccpy(&MH.fcc, "avih");
    MH.cb = sizeof(AviEncode::AVIMAINHEADER) - 8;
    MH.dwMicroSecPerFrame = 1000000.0 / usersettings.fps;
    MH.dwMaxBytesPerSec = 10000;
    MH.dwPaddingGranularity = 2;
    MH.dwFlags = 0;
    MH.dwTotalFrames = 200; // TODO: FrameCount computation
    MH.dwInitialFrames = 0;
    MH.dwStreams = 1;
    MH.dwSuggestedBufferSize = 100000;
    MH.dwWidth = usersettings.width;
    MH.dwHeight = usersettings.height;
    MH.dwReserved[0] = MH.dwReserved[1] = MH.dwReserved[2] = MH.dwReserved[3] = 0;

    // strl list
    AviEncode::avi_list_h strl;
    AviEncode::fcccpy(&strl.code, "LIST");
    strl.listsize = sizeof(AviEncode::avi_list_h) - 8 +
                    sizeof(AviEncode::avi_list_h) + sizeof(AviEncode::AVISTREAMHEADER) +
                    sizeof(AviEncode::BITMAPINFOHEADER);    // NOTE: Are we correct here?
    AviEncode::fcccpy(&strl.listtype, "strl");

    // Write stream information for stream 1 strh
    AviEncode::avi_chunk_h strh;
    AviEncode::fcccpy(&strh.chunkID, "strh");
    strh.chunkSize = sizeof(AviEncode::AVISTREAMHEADER);

    AviEncode::AVISTREAMHEADER SH;
    AviEncode::fcccpy(&SH.fcc, "strh");
    SH.cb = sizeof(AviEncode::AVISTREAMHEADER) - 8;
    AviEncode::fcccpy(&SH.fccType, "vids");
    AviEncode::fcccpy(&SH.fccHandler, "    ");      // NOTE: Are we correct here?
    SH.dwFlags = 0;
    SH.wPriority = 1;
    SH.wLanguage = 0;
    SH.dwInitialFrames = 0;
    SH.dwScale = 1;
    SH.dwRate = 24;
    SH.dwLength = 200;  // NOTE: Are we correct here?
    SH.dwSuggestedBufferSize = 0;   // NOTE: Performance
    SH.dwQuality = -1;
    SH.dwSampleSize = 0; //We can change this to some value given that frames are constant size
    SH.rcFrame.left = 0;
    SH.rcFrame.right = usersettings.width;
    SH.rcFrame.top = 0;
    SH.rcFrame.bottom = usersettings.height;

    // Stream 1 strf
    AviEncode::avi_chunk_h strf;
    AviEncode::fcccpy(&strf.chunkID, "strf");
    strf.chunkSize = sizeof(AviEncode::BITMAPINFOHEADER);

    AviEncode::BITMAPINFOHEADER SF;
    SF.biSize = sizeof(AviEncode::BITMAPINFOHEADER);
    SF.biWidth = usersettings.width;
    SF.biHeight = -usersettings.height;
    SF.biPlanes = 1;
    SF.biBitCount = 3;
    SF.biCompression = 0; // NOTE: Really need help here (need BI_RGB)
    SF.biSizeImage = 0;
    SF.biXPelsPerMeter = (long) (300 / 2.54) * 100; // 300 PPI
    SF.biYPelsPerMeter = SF.biXPelsPerMeter; // Square Pixels
    SF.biClrUsed = 0;
    SF.biClrImportant = 0;

    // List movi
    AviEncode::avi_list_h movi;
    AviEncode::fcccpy(&movi.code, "LIST");
    movi.listsize = 1000000; // TODO: Calculate here the frame data length
    AviEncode::fcccpy(&movi.listtype, "movi");

    file.write((const char *) &riff, sizeof(riff));
    file.write((const char *) &hdrl, sizeof(hdrl));
    file.write((const char *) &avih, sizeof(avih));
    file.write((const char *) &MH, sizeof(MH));
    file.write((const char *) &strl, sizeof(strl));
    file.write((const char *) &strh, sizeof(strh));
    file.write((const char *) &SH, sizeof(SH));
    file.write((const char *) &strf, sizeof(strf));
    file.write((const char *) &SF, sizeof(SF));
    file.write((const char *) &movi, sizeof(movi));
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
    file.write((const char *)&vidframe, sizeof(vidframe));
    file.write((const char *)&framedata, vidframe.chunkSize);
}

AviEncode::AviContainer::~AviContainer() {
    delete output_filename;
    file.close();
}

