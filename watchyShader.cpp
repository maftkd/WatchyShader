#include "watchyShader.h"

#define DARKMODE true

bool camInit=false;
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
}

void WatchyShader::drawPixel(int16_t x, int16_t y, float intensity){
	if(randInt(0,100)*0.01<intensity)
		display.drawPixel(x,y,0xFFFF);
}

void WatchyShader::drawLineH(int16_t height, float intensity){
	for(int x=0; x<200; x++){
		drawPixel(x,height,intensity);
	}
}

void WatchyShader::drawVertex(vec3 vert){
}

void WatchyShader::drawLine(vec3 a, vec3 b){
	display.drawLine((int)a.data[0],(int)a.data[1],
			(int)b.data[0],(int)b.data[1],drawColor);
}

//inputs are in world space
void WatchyShader::drawTriangle(vec3 a, vec3 b, vec3 c, mat4 model){
	//transform points from world to watchy space
	vec3 wa = cam.localToWatchy(a,model);
	vec3 wb = cam.localToWatchy(b,model);
	vec3 wc = cam.localToWatchy(c,model);
	/*
	vec3 wa = cam.worldToWatchy(a);
	vec3 wb = cam.worldToWatchy(b);
	vec3 wc = cam.worldToWatchy(c);
	*/
	//check bounds - maybe we don't need to worry about this
	/*
	if(x1<0 || x1>199 || y1<0 || y1>199 || 
		x2<0 || x2>199 || y2<0 || y2>199 || 
		x3<0 || x3>199 || y3<0 || y3>199)
	{
		Serial.println("not drawing because one of the vertices was outside the clip space");
		return;
	}
	*/
	//draw some lines - clockwise
	drawLine(wa,wb);
	drawLine(wb,wc);
	drawLine(wc,wa);
}


void WatchyShader::drawWatchFace(){
	//test code for triangle rendering and mvp transformation
	/*
	if(!camInit)
		initCam();
	//init the model transform matrix as identity
	mat4 model;
	mat4 trans;
	mat4 quat;
	translate(&trans,vec3(0,0,1.0f));
	//init the vertices
	vec3 foo = vec3(0,0.25,0);
	vec3 foo2 = vec3(0.25,-0.25,0);
	vec3 foo3 = vec3(-0.25,-0.25,0);
	for(int i=0; i<20; i++){
		//clear to black
		display.fillScreen(0x0000);
		//draw time
		drawTime();
		//translate triangle
		//translate(&model,vec3(0,0,0.5f));
		rotate(&quat,worldUp,0.3145f);
		model=trans*quat;
		//draw triangle
		drawTriangle(foo,foo2,foo3,model);
		display.display(true); //partial refresh
	}
	*/
	
	setSeed(currentTime.Minute);
	//clear to black
	display.fillScreen(0x0000);
	//draw gradient
	for(int y=199; y>=0; y--){
		float v = inverseLerp(199,0,y);
		drawLineH(y,v);
	}
	//draw time
	drawTime();
}

void WatchyShader::drawTime(){
    display.setFont(&DSEG7_Classic_Bold_53);
	if(currentTime.Hour>=12)
	{
		display.setCursor(5, 53+60+65+10);
		display.setTextColor(0xFFFF);
	}
	else
	{
		display.setCursor(5, 53+15);
		display.setTextColor(0x0000);
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
