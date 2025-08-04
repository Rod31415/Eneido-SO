#ifndef IMAGb_H
#define IMAGb_H
#include "types.h"
#include "vga.h"
#include "functions.h"

typedef struct BitMapHeader{
        unsigned char   b_signature[2]; 
        uint32      b_fileSize;
        uint32      b_reserved;
        uint32      b_dataOffset;
        uint32      b_infoSize;
        uint32      b_width;
        uint32      b_height;
        uint16      b_planes;
        uint16      b_bitCount;
        uint32      b_compression;
        uint32      b_imageSize;
        uint32      b_XpixelPerM;
        uint32      b_YpixelPerM;
        uint32      b_colorsUsed;
        uint32      b_colorsImp;
}__attribute__((packed))BitMapHeader_t;




void initViewer(uint32 addressImage);

#endif