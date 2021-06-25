#ifndef WATCHYSHADER_H
#define WATCHYSHADER_H

#include <Watchy.h>
#include "lehmer.h"
#include "geometry.h"
#include "DSEG7_Classic_Regular_15.h"
#include "FreeSansOblique12pt7b.h"

class WatchyShader : public Watchy{
    public:
        WatchyShader();
		void initCam();
        void drawWatchFace();
		void drawTime();
		void drawVertex(vec3 vert);
		void drawLine(vec3 a, vec3 b);
		void drawTriangle(vec3 a, vec3 b, vec3 c,mat4 model);
		void shadeTriangle(vec3 a, vec3 b, vec3 c,mat4 model,float intensity);
		void drawPixel(int16_t x,int16_t y,float intensity);
		void drawLineH(int16_t xStart, int16_t height, int16_t width, float intensity);
};

#endif
