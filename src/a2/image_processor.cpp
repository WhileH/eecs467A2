#include "image_processor.hpp"

void image_processor::image_masking(image_u32_t *im,float x1,float x2,float y1, float y2){
    uint8_t a = 0xff;
    uint8_t r = 0x00; 
    uint8_t g = 0x00;
    uint8_t b = 0x00;
    for (int y = 0; y < im->height; y++) {
        for (int x = 0; x < im->width; x++) {
            if(x<x1 || x>x2 || y<(480-y2) || y>(480-y1)){
                im->buf[y*im->stride + x] = (a & 0xff) << 24 | (b & 0xff) << 16 | (g & 0xff) << 8 | r;
            }
        }
    }  
}
