/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * libfuse-FrameServer - (https://github.com/supragya/libfuse-FrameServer)
 *
 */

#include "AviHeaders.h"
#include "AviEncode.h"


AviEncode::AviContainer::AviContainer(const char *filename, unsigned int mx_buflen) {
    output_filename = (char *) malloc(sizeof(char) * strlen(filename));
    strcpy(output_filename, filename);
    MaxBufLen = mx_buflen;
    writeBuffer = (char *) malloc(sizeof(char) * MaxBufLen);
    BufLen = 0;
    file.open(filename, std::ios::binary | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "[AviContainer] Error: Cannot open file \"" << filename << "\"" << std::endl;
    }

    is_avimainheaderset = false;
}

int AviEncode::AviContainer::flushBuffer() {
    file.write(writeBuffer, BufLen);
    BufLen = 0;
    return 0;
}

int AviEncode::AviContainer::WriteHeaderSequence() {
    if (!is_avimainheaderset) {
        std::cerr << "[AviContainer] Error: Cannot write header sequence, unset headers! status: ";

        // Show status flags
        std::cerr << is_avimainheaderset << std::endl;

        return 1;
    }
    AviEncode::avi_list_h list;

    AviEncode::setFourCC(&list.code, "RIFF");
    list.listsize = 0;
    AviEncode::setFourCC(&list.listtype, "AVI ");
    addtobuffer((char *) &list, sizeof(AviEncode::avi_list_h));

    AviEncode::setFourCC(&list.code, "LIST");
    list.listsize = 0;
    AviEncode::setFourCC(&list.listtype, "hdr1");
    addtobuffer((char *) &list, sizeof(AviEncode::avi_list_h));

    addtobuffer((char *) &avimainheader, sizeof(avimainheader));

    return 0;
}

int AviEncode::AviContainer::addtobuffer(char *content, unsigned int size) {
    memcpy(writeBuffer + BufLen, content, size);
    BufLen += size;
    return 0;
}

void AviEncode::AviContainer::setAviMainHeader(FourCC code, uint32_t usecperframe, uint32_t maxbytepersec,
                                               uint32_t paddinggranularity, uint32_t flags, uint32_t totalframes,
                                               uint32_t initialframes, uint32_t streams, uint32_t suggestedbufsize,
                                               uint32_t width, uint32_t height) {

    AviEncode::setFourCC(&avimainheader.fcc, (char *) &code);
    avimainheader.cb = sizeof(AviEncode::AviContainer::AVIMAINHEADER) - sizeof(AviEncode::FourCC);
    avimainheader.dwMicroSecPerFrame = usecperframe;
    avimainheader.dwMaxBytesPerSec = maxbytepersec;
    avimainheader.dwPaddingGranularity = paddinggranularity;
    avimainheader.dwFlags = flags;
    avimainheader.dwTotalFrames = totalframes;
    avimainheader.dwInitialFrames = initialframes;
    avimainheader.dwStreams = streams;
    avimainheader.dwSuggestedBufferSize = suggestedbufsize;
    avimainheader.dwWidth = width;
    avimainheader.dwHeight = height;

    is_avimainheaderset = true;
}

void AviEncode::AviContainer::showBuffer() {
    std::cout << "[AviContainer] Buffer: ";
    for (unsigned int i = 0; i < BufLen; i++)
        std::cout << (int) writeBuffer[i];
    std::cout << std::endl;
}

AviEncode::AviContainer::~AviContainer() {
    free(output_filename);
    free(writeBuffer);
    file.close();
}

AviEncode::FourCC AviEncode::createFourCC(char *c) {
    FourCC result;
    memcpy(&result, c, sizeof(FourCC));
    return result;
}

void AviEncode::setFourCC(FourCC *fcc, char *c) {
    memcpy(fcc, c, sizeof(FourCC));
}