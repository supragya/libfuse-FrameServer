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

    hdrBuffer = new char[MAXHEADERBUFLEN];
    init_sizes();
    std::cout<<sz.List<<" "<<sz.Chunk<<" "<<sz.FOURCC<<" "<<sz._uint32_t<<" "<<std::endl;
    WriteHeaderSequence();
}

int AviEncode::AviContainer::WriteHeaderSequence() {

    AviEncode::avi_list_h riff = riffHeader();
    AviEncode::avi_list_h hdrl = hdrlHeader();
    AviEncode::AVIMAINHEADER mh = avihHeader();
    AviEncode::avi_list_h strl = strlHeader();
    AviEncode::AVISTREAMHEADER strh = strhHeader();
    AviEncode::avi_chunk_h strf_c = strfHeader_c();
    AviEncode::BITMAPINFOHEADER strf_v = strfHeader_v();
    AviEncode::avi_list_h movi = moviHeader();

    hdrBufferLen = 0;

    memcpy((void*)hdrBuffer, (const void *) &riff, sz.List);
    hdrBufferLen += sz.List;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&hdrl, sz.List);
    hdrBufferLen += sz.List;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&mh, sz.AVIMAINHEADER);
    hdrBufferLen += sz.AVIMAINHEADER;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&strl, sz.List);
    hdrBufferLen += sz.List;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&strh, sz.AVISTREAMHEADER);
    hdrBufferLen += sz.AVISTREAMHEADER;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&strf_c, sz.Chunk);
    hdrBufferLen += sz.Chunk;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&strf_v, sz.AVIBITMAPINFOHEADER);
    hdrBufferLen += sz.AVIBITMAPINFOHEADER;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&movi, sz.List);
    hdrBufferLen += sz.List;

    std::cout<<hdrBuffer<<std::endl;
    std::cout<<hdrBufferLen<<std::endl;
    std::cout<<sz.AVISTREAMHEADER<<std::endl;

    std::cout << "Writing header info at " << file.tellp() << " ";
    file.write((const char *) &hdrBuffer, hdrBufferLen);
    std::cout << "ending at " << file.tellp() << std::endl;

    return 0;
}

void AviEncode::fcccpy(AviEncode::FOURCC *fcc, std::string str) {
    for (int i = 0; i < 4; i++)
        fcc->byte[i] = str[i];
}

void AviEncode::fcccpy(AviEncode::FOURCC *fcc, int hexval) {
    for (int i = 3; i >= 0; i--) {
        fcc->byte[i] = hexval % 16;
        hexval = hexval >> 4;
    }
}

void AviEncode::AviContainer::AddFrame(char *framedata) {
    AviEncode::avi_chunk_h vidframe;
    AviEncode::fcccpy(&vidframe.chunkID, "00db");
    vidframe.chunkSize = calculateSize(VIDFRAME);

    std::cout << "Writing frame at " << file.tellp() << " ";
    file.write((const char *) &vidframe, sizeof(vidframe));
    file.write((const char *) framedata, vidframe.chunkSize);

    std::cout << "ending at " << file.tellp() << " data: " << (int) framedata[0] << " " << (int) framedata[1] << " "
              << (int) framedata[2] << std::endl;
}


AviEncode::AviContainer::~AviContainer() {
    delete hdrBuffer;
    delete output_filename;
    file.close();
}

