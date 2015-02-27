#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP
#include "imagesource/image_u32.h"

class image_processor{
    public:
        image_processor(){}
        void image_masking(image_u32_t *im,float x1,float x2,float y1,float y2);
};


#endif
