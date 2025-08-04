#include "headers/image.h"



uint8 mapPalette[256];


uint8 mapColor(uint8 color){


return mapPalette[color];
}


void initViewer(uint32 addressImage){

BitMapHeader* head=(BitMapHeader*)addressImage;

uint32 off=head->b_dataOffset+addressImage;
uint32* paletteOff=(uint32*)(0x36+addressImage);
uint32 width=head->b_width;
uint32 height=head->b_height;

uint32 paddX=(640-width)/2;
uint32 paddY=(480-height)/2;

/*for(uint32 i=0;i<256;i++){
    uint32 bestColor;
    uint32 minDist=(uint32)(-1);

    for(uint32 j=0;j<256;j++){
        uint32 dr=palette_13h[i][0]-paletteOff[j*4+0];
        uint32 dg=palette_13h[i][1]-paletteOff[j*4+1];
        uint32 db=palette_13h[i][2]-paletteOff[j*4+2];

        uint32 dist=dr*dr + dg*dg + db*db;
        if(dist<minDist){
                minDist=dist;
                bestColor=j;
        }
        
    }
    mapPalette[i]=bestColor;
    
}
*/
//changePalette((uint32)paletteOff,false);



cls(0x00);

uint32 padded=(width+3)&~0x3;

//(((uint32*)(off+j*(padded)+i))[0]

for(uint32 j=0;j<height;j++){
    for(uint32 i=0;i<width;i++){
        uint32 a=paletteOff[((uint8*)(off+j*padded+i))[0]];
    draw_pixel(paddX+i,paddY+height-j,a);
    }
}
refresh();

}
