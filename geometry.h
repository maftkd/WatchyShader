#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string.h>


struct vec3{
	float data[3];

	vec3(){}

	vec3(float x, float y, float z){
		data[0]=x;
		data[1]=y;
		data[2]=z;
	}

	void operator=(vec3 a){
		memcpy(data,a.data,3*4);
	}
};
const vec3 worldUp = vec3(0,1,0);

struct vec4{
	float data[4];

	vec4(){}

	vec4(vec3 a, float w){
		memcpy(data,a.data,3*4);
		data[3]=w;
	}
};

//these matrices are column major. i.e
//[0 4 8 12]
//[1 5 9 13]
//[2 6 10 14]
//[3 7 11 15]
struct mat4{
	float data[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

	mat4(){}

	void operator=(mat4 a){
		memcpy(data,a.data,16*4);
	}
};


vec4 operator*(mat4 a, vec4 b);
mat4 operator*(mat4 a, mat4 b);
vec3 operator+(vec3 a, vec3 b);
vec3 operator*(vec3 a, float b);
vec3 operator-(vec3 a, vec3 b);
vec3 cross(vec3 a, vec3 b);
vec3 normal(vec3 a);
float dot(vec3 a, vec3 b);
mat4 perspective(float fovy, float aspect, float nr, float fr);
mat4 lookAt(vec3 eye, vec3 at, vec3 up);
void translate(mat4* model, vec3 t);
void rotate(mat4* model, vec3 axis, float ang);

struct camera{
	vec3 pos;
	vec3 forward;
	vec3 right;
	vec3 up;
	mat4 proj;
	mat4 view;
	float nr;
	float fr;
	float fovy;
	float aspect;

	camera(){}

	camera(vec3 p, vec3 f, vec3 u, float fy, float as, float n, float fa){
		pos=p;
		forward=f;
		up=u;
		right=cross(up,forward);
		fovy=fy;
		aspect=as;
		nr=n;
		fr=fa;
		proj=perspective(fy,as,n,fa);
		view = lookAt(
				pos,
				pos+forward,
				worldUp);
	}	

	vec3 worldToNDC(vec3 world){
		mat4 vp = proj*view;
		vec4 point = vec4(world,1);
		vec4 c = vp*point;
		vec3 ndc = vec3(c.data[0]/c.data[3],
				c.data[1]/c.data[3],
				c.data[2]/c.data[3]);
		return ndc;
	}
	vec3 worldToWatchy(vec3 world){
		mat4 vp = proj*view;
		vec4 point = vec4(world,1);
		vec4 c = vp*point;
		vec3 ndc = vec3(c.data[0]/c.data[3],
				c.data[1]/c.data[3],
				c.data[2]/c.data[3]);
		vec3 w = vec3((ndc.data[0]+1)*0.5*199,
				(1-(ndc.data[1]+1)*0.5)*199,
				ndc.data[2]);
		return w;
	}
	vec3 localToWatchy(vec3 world, mat4 model){
		mat4 mvp = proj*view*model;
		vec4 point = vec4(world,1);
		vec4 c = mvp*point;
		vec3 ndc = vec3(c.data[0]/c.data[3],
				c.data[1]/c.data[3],
				c.data[2]/c.data[3]);
		vec3 w = vec3((ndc.data[0]+1)*0.5*199,
				(1-(ndc.data[1]+1)*0.5)*199,
				ndc.data[2]);
		return w;
	}
};
#endif
