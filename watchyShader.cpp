#include "watchyShader.h"

#define DARKMODE true

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

bool camInit=false;
bool updating=false;
uint16_t drawColor = 0xFFFF;
camera cam;
WatchyShader::WatchyShader(){} //constructor

float lerp(float a, float b, float t){
	return a+t*(b-a);
}

float inverseLerp(float a, float b, float v){
	return (v-a)/(b-a);
}

float smoothstep(float edgeA, float edgeB, float val){
	float il = inverseLerp(edgeA,edgeB,val);
	if(il<0)
		return 0;
	else if(il>1)
		return 1;
	return il;
}

void WatchyShader::initCam(){
	vec3 pos = vec3(0,0,0);
	vec3 forward = vec3(0,0,1);
	vec3 up = vec3(0,1,0);
	cam = camera(pos, forward, up, 1.57f, 1.0f, 0.1f, 20.0f);
	//cam = camera(pos, forward, up, 1.047f, 1.0f, 0.1f, 20.0f);
	camInit=true;
	//Serial.begin(9600);
}

void WatchyShader::drawPixel(int16_t x, int16_t y, float intensity){
	if(randInt(0,100)*0.01<intensity)
		display.drawPixel(x,y,drawColor);
}

void WatchyShader::drawLineH(int16_t xStart, int16_t height, int16_t width, float intensity){
	for(int x=xStart; x<xStart+width; x++){
		drawPixel(x,height,intensity);
	}
}

void WatchyShader::drawVertex(vec3 vert){
}

void WatchyShader::drawLine(vec3 a, vec3 b){
	for(int i=0; i<3; i++){
		display.drawLine((int)a.data[0]+i,(int)a.data[1],
				(int)b.data[0]+i,(int)b.data[1],drawColor);
	}
	for(int i=0; i<3; i++){
		display.drawLine((int)a.data[0],(int)a.data[1]+i,
				(int)b.data[0],(int)b.data[1]+i,drawColor);
	}
}

//inputs are in world space
void WatchyShader::drawTriangle(vec3 a, vec3 b, vec3 c, mat4 model){
	//transform points from world to watchy space
	vec3 wa = cam.localToWatchy(a,model);
	vec3 wb = cam.localToWatchy(b,model);
	vec3 wc = cam.localToWatchy(c,model);
	//draw some lines
	drawLine(wa,wb);
	drawLine(wb,wc);
	drawLine(wc,wa);
}

void WatchyShader::shadeTriangle(vec3 v0, vec3 v1, vec3 v2, mat4 model,float intensity){
	//transform points from world to watchy space
	vec3 p0 = cam.localToWatchy(v0,model);
	vec3 p1 = cam.localToWatchy(v1,model);
	vec3 p2 = cam.localToWatchy(v2,model);
	int16_t x0 = (int16_t)p0.data[0];
	int16_t y0 = (int16_t)p0.data[1];
	int16_t x1 = (int16_t)p1.data[0];
	int16_t y1 = (int16_t)p1.data[1];
	int16_t x2 = (int16_t)p2.data[0];
	int16_t y2 = (int16_t)p2.data[1];
	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}
	if (y1 > y2) {
		_swap_int16_t(y2, y1);
		_swap_int16_t(x2, x1);
	}
	if (y0 > y1) {
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}

	//startWrite();
	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		//writeFastHLine(a, y0, b - a + 1, color);
		//display.drawLine(a,y0,b,y0,0xFFFF);
		drawLineH(a,y0,b-a+1,intensity);
		//endWrite();
		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
	dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b)
			_swap_int16_t(a, b);
		//writeFastHLine(a, y, b - a + 1, color);
		//display.drawLine(a,y,b,y,0xFFFF);
		drawLineH(a,y,b-a+1,intensity);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = (int32_t)dx12 * (y - y1);
	sb = (int32_t)dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b)
			_swap_int16_t(a, b);
		//writeFastHLine(a, y, b - a + 1, color);
		//display.drawLine(a,y,b,y,0xFFFF);
		drawLineH(a,y,b-a+1,intensity);
	}
	//endWrite();
}


void WatchyShader::drawWatchFace(){
	if(updating)
		return;
	updating=true;
	//test code for triangle rendering and mvp transformation
	//uncomment for awesomeness
	if(!camInit)
		initCam();
	//init the model transform matrix as identity
	mat4 model;
	mat4 trans;
	mat4 quat;
	translate(&trans,vec3(0,0,1.5f));
	//test triangle
	vec3 t[3];
	t[0] = vec3(-1,-1,0);
	t[1] = vec3(0,1,0);
	t[2] = vec3(1,-1,0);
	//test normal
	vec4 localNorm = vec4(0,0,-1,1);
	vec4 worldNorm;
	//init the cube verts
	vec3 v[8];
	v[0] = vec3(-0.5,-0.5,-0.5);
	v[1] = vec3(-0.5,0.5,-0.5);
	v[2] = vec3(0.5,0.5,-0.5);
	v[3] = vec3(0.5,-0.5,-0.5);
	v[4] = vec3(-0.5,-0.5,0.5);
	v[5] = vec3(-0.5,0.5,0.5);
	v[6] = vec3(0.5,0.5,0.5);
	v[7] = vec3(0.5,-0.5,0.5);
	//init the local norms per face - hacky hack
	//vec4 because we only have a mat4*vec4, just set w to 1
	vec4 n[6];
	n[0] = vec4(0,0,-1,1);//front
	n[1] = vec4(0,0,1,1);//back
	n[2] = vec4(1,0,0,1);//right
	n[3] = vec4(-1,0,0,1);//left
	n[4] = vec4(0,1,0,1);//top
	n[5] = vec4(0,-1,0,1);//bottom
	float intensity;
	for(int i=0; i<20; i++){
		//draw time
		drawTime();
		//rotate cube
		rotate(&quat,worldUp,0.0785f);
		model=trans*quat;
		//draw cube
		//draw front face
		//calculate intensity assuming light source is at camera location
		worldNorm=quat*n[0];
		intensity=-dot(cam.forward,vec3(worldNorm.data));
		shadeTriangle(v[0],v[1],v[2],model,intensity);
		shadeTriangle(v[2],v[3],v[0],model,intensity);
		//draw back face
		worldNorm=quat*n[1];
		intensity=-dot(cam.forward,vec3(worldNorm.data));
		shadeTriangle(v[6],v[5],v[7],model,intensity);
		shadeTriangle(v[5],v[7],v[4],model,intensity);
		//draw right face
		worldNorm=quat*n[2];
		intensity=-dot(cam.forward,vec3(worldNorm.data));
		shadeTriangle(v[2],v[6],v[3],model,intensity);
		shadeTriangle(v[3],v[6],v[7],model,intensity);
		//draw left face
		worldNorm=quat*n[3];
		intensity=-dot(cam.forward,vec3(worldNorm.data));
		shadeTriangle(v[5],v[1],v[4],model,intensity);
		shadeTriangle(v[4],v[1],v[0],model,intensity);
		//draw top face
		worldNorm=quat*n[4];
		intensity=-dot(cam.forward,vec3(worldNorm.data));
		shadeTriangle(v[1],v[5],v[6],model,intensity);
		shadeTriangle(v[1],v[6],v[2],model,intensity);
		//draw bottom face
		worldNorm=quat*n[5];
		intensity=-dot(cam.forward,vec3(worldNorm.data));
		shadeTriangle(v[4],v[0],v[3],model,intensity);
		shadeTriangle(v[4],v[3],v[7],model,intensity);
		display.display(true); //partial refresh
	}
	updating=false;
}

void WatchyShader::drawTime(){
    //display.setFont(&DSEG7_Classic_Bold_53);
    display.setFont(&FreeSansOblique12pt7b);
	if(currentTime.Hour>=12)
	{
		display.setCursor(5, 53+60+65+10);
		display.setTextColor(0xFFFF);
		display.fillScreen(0x0000);
		drawColor = 0xFFFF;
	}
	else
	{
		display.setCursor(5, 20);
		display.setTextColor(0x0000);
		display.fillScreen(0xFFFF);
		drawColor = 0x0000;
	}

    if(currentTime.Hour < 10){
        display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }  
    display.println(currentTime.Minute);    
}
