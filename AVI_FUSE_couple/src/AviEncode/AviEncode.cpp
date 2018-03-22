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
#include "AviSizes.h"
using namespace AviEncode;

AviContainer::AviContainer(const char *filename, avi_usersettings usersettings) {
    output_filename = new char[1000];
    strcpy(output_filename, filename);
    this->usersettings = usersettings;

    file.open(filename, std::ios::binary | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "[AviContainer] Error: Cannot open file \"" << filename << "\"" << std::endl;
    }

    hdrBuffer = new char[MAXHEADERBUFLEN];
    std::cout<<szList<<" "<<szList<<" "<<szChunk<<" "<<szFOURCC<<" "<<sz_uint32_t<<" "<<std::endl;
    WriteHeaderSequence();
}

int AviContainer::WriteHeaderSequence() {

    avi_list_h riff = riffHeader();
    avi_list_h hdrl = hdrlHeader();
    AVIMAINHEADER mh = avihHeader();
    avi_list_h strl = strlHeader();
    AVISTREAMHEADER strh = strhHeader();
    avi_chunk_h strf_c = strfHeader_c();
    BITMAPINFOHEADER strf_v = strfHeader_v();
    avi_list_h movi = moviHeader();

    hdrBufferLen = 0;

    memcpy((void*)hdrBuffer, (const void *) &riff, szList);
    hdrBufferLen += szList;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&hdrl, szList);
    hdrBufferLen += szList;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&mh, szAVIMAINHEADER);
    hdrBufferLen += szAVIMAINHEADER;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&strl, szList);
    hdrBufferLen += szList;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&strh, szAVISTREAMHEADER);
    hdrBufferLen += szAVISTREAMHEADER;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&strf_c, szChunk);
    hdrBufferLen += szChunk;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&strf_v, szAVIBITMAPINFOHEADER);
    hdrBufferLen += szAVIBITMAPINFOHEADER;

    memcpy((void*)(hdrBuffer+hdrBufferLen), (const void *)&movi, szList);
    hdrBufferLen += szList;

    std::cout<<hdrBuffer<<std::endl;
    std::cout<<hdrBufferLen<<std::endl;
    std::cout<<szAVISTREAMHEADER<<std::endl;

    std::cout << "Writing header info at " << file.tellp() << " ";
    file.write(hdrBuffer, hdrBufferLen);
    std::cout << "ending at " << file.tellp() << std::endl;

    return 0;
}

void fcccpy(FOURCC *fcc, std::string str) {
    for (int i = 0; i < 4; i++)
        fcc->byte[i] = str[i];
}

void fcccpy(FOURCC *fcc, int hexval) {
    for (int i = 3; i >= 0; i--) {
        fcc->byte[i] = hexval % 16;
        hexval = hexval >> 4;
    }
}

void AviContainer::AddFrame(char *framedata) {
    avi_chunk_h vidframe;
    fcccpy(&vidframe.chunkID, "00db");
    vidframe.chunkSize = calculateSize(VIDFRAME);

    std::cout << "Writing frame at " << file.tellp() << " ";
    file.write((const char *) &vidframe, sizeof(vidframe));
    file.write((const char *) framedata, vidframe.chunkSize);

    std::cout << "ending at " << file.tellp() << " data: " << (int) framedata[0] << " " << (int) framedata[1] << " "
              << (int) framedata[2] << std::endl;
}

AviContainer::~AviContainer() {
    delete hdrBuffer;
    delete output_filename;
    file.close();
}

