#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP
#include "imagesource/image_u32.h"
#include "hsv.hpp"
class image_processor{
    public:
        image_processor(){}
        void image_masking(image_u32_t *im,float x1,float x2,float y1,float y2);
        hsv_color_t rgb_to_hsv(uint32_t abgr); 
        void image_select(image_u32_t *im,max_min_hsv hsv);
};


#endif
