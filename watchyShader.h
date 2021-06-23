#ifndef WATCHYSHADER_H
#define WATCHYSHADER_H

#include <Watchy.h>
#include "lehmer.h"
#include "geometry.h"
#include "DSEG7_Classic_Regular_15.h"

class WatchyShader : public Watchy{
    public:
        WatchyShader();
		void initCam();
        void drawWatchFace();
		void drawTime();
		void drawVertex(vec3 vert);
		void drawLine(vec3 a, vec3 b);
		void drawTriangle(vec3 a, vec3 b, vec3 c,mat4 model);
		void drawPixel(int16_t x,int16_t y,float intensity);
		void drawLineH(int16_t height, float intensity);
};

#endif
