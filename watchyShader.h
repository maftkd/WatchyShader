#ifndef WATCHYSHADER_H
#define WATCHYSHADER_H

#include <Watchy.h>
#include "lehmer.h"
#include "DSEG7_Classic_Regular_15.h"

class WatchyShader : public Watchy{
    public:
        WatchyShader();
        void drawWatchFace();
		void drawTime();
		void drawPixel(int16_t x,int16_t y,float intensity);
		void drawLineH(int16_t height, float intensity);
};

#endif
