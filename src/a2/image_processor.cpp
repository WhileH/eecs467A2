#include "image_processor.hpp"
#include <algorithm>
#include <cmath>
#include <cfloat>
void image_processor::image_masking(image_u32_t *im,float x1,float x2,float y1, float y2){
    uint32_t black = 0xff000000;
    for (int y = 0; y < im->height; ++y) {
        for (int x = 0; x < im->width; ++x) {
            if(x<x1 || x>x2 || y<(im->height-y2) || y>(im->height-y1)){
                im->buf[y*im->stride + x] = black;
            }
        }
    }  
}

void image_processor::blob_detection(image_u32_t *im, float x1,float x2,float y1,float y2,max_min_hsv red,max_min_hsv green){
    uint32_t black = 0xff000000;
    hsv_color_t tmp_hsv;
    hsv_color_t max_red_hsv = red.get_max_HSV();
    hsv_color_t min_red_hsv = red.get_min_HSV();
    hsv_color_t max_green_hsv = green.get_max_HSV();
    hsv_color_t min_green_hsv = green.get_min_HSV();
    image_masking(im,x1,x2,y1,y2);
    for(int y = (int)(im->height - y2);y<=(int)(im->height-y1);++y){
        for(int x = (int)x1;x<=(int)x2;++x){
            tmp_hsv = rgb_to_hsv(im->buf[y*im->stride + x]);    
            if(is_hsv_in_range(tmp_hsv,max_red_hsv,min_red_hsv) || is_hsv_in_range(tmp_hsv,max_green_hsv,min_green_hsv)){
                continue;
            }
            else{
                im->buf[y*im->stride + x] = black;
            }
        }
    }
}

void image_processor::image_select(image_u32_t *im,max_min_hsv hsv){
    hsv_color_t max_hsv = hsv.get_max_HSV();
    hsv_color_t min_hsv = hsv.get_min_HSV();
    hsv_color_t tmp_hsv;
    uint32_t light_purple = 0xffff73df;
    for (int y = 0; y < im->height; ++y) {
        for (int x = 0; x < im->width; ++x) {
            tmp_hsv = rgb_to_hsv(im->buf[y*im->stride + x]);    
            if(is_hsv_in_range(tmp_hsv,max_hsv,min_hsv)) {
                im->buf[y*im->stride + x] = light_purple; 
            }
        }
    }  
}


hsv_color_t image_processor::rgb_to_hsv(uint32_t abgr){
    hsv_color_t hsv_tmp;
    uint8_t r = (uint8_t)  abgr;
    uint8_t g = (uint8_t) (abgr >> 8);
    uint8_t b = (uint8_t) (abgr >> 16);
    float r_prime = r/255.0;
    float g_prime = g/255.0;
    float b_prime = b/255.0;
    float cmax = std::max(r_prime,std::max(g_prime,b_prime));
    float cmin = std::min(r_prime,std::min(g_prime,b_prime));
    float delta = cmax - cmin;
    hsv_tmp.V = cmax;
    if(fabs(cmax - 0.00) > FLT_EPSILON){
        hsv_tmp.S = delta/cmax;
    }
    else{
        //r = g = b = 0
        hsv_tmp.H = -1;
        hsv_tmp.S = 0;
    }
    if(fabs(cmax - r_prime) < FLT_EPSILON){
        hsv_tmp.H = (g_prime - b_prime) / delta; 
    }
    else if(fabs(cmax - g_prime) < FLT_EPSILON){
        hsv_tmp.H = (2 + b_prime - r_prime) / delta;
    }
    else{
        hsv_tmp.H = (4 + r_prime - g_prime) / delta;
    }
    hsv_tmp.H *= 60.0;
    if(hsv_tmp.H < 0){
        hsv_tmp.H += 360.0;
    }
    return hsv_tmp;
} 

bool image_processor::is_hsv_in_range(hsv_color_t hsv,hsv_color_t max,hsv_color_t min){
    return hsv.H <= max.H && hsv.S <= max.S && hsv.V <= max.V &&
            hsv.H >= min.H && hsv.S >= min.S && hsv.V >= min.V;
}
