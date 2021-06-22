#include "watchyShader.h"

#define DARKMODE true

WatchyShader::WatchyShader(){} //constructor

void WatchyShader::drawPixel(int16_t x, int16_t y, float intensity){
	if(randInt(0,100)*0.01<intensity)
		display.drawPixel(x,y,0xFFFF);
}

void WatchyShader::drawLineH(int16_t height, float intensity){
	for(int x=0; x<200; x++){
		drawPixel(x,height,intensity);
	}
}

void WatchyShader::drawWatchFace(){
	//draw a bg / skybox type thing
	for(int y=0; y<200; y++)
		drawLineH(y,1.0-y/199.0f);

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
