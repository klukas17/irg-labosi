//
// Created by mihael on 25.05.22..
//

#ifndef IRGLAB_TEXTURE_H
#define IRGLAB_TEXTURE_H

#include "string"

class Texture {
public:
    unsigned int texture;

    Texture(unsigned char* d, int w, int h, int n);
    ~Texture();
};


#endif //IRGLAB_TEXTURE_H
