//geometry cpp
//vector, matrices, and projection stuff

#include "geometry.h"
#include <math.h>


vec3 cross(vec3 a, vec3 b){
	vec3 cr(a.data[1]*b.data[2]-a.data[2]*b.data[1],
		a.data[2]*b.data[0]-a.data[0]*b.data[2],
		a.data[0]*b.data[1]-a.data[1]*b.data[0]);
	return cr;
}

//normalizes vector
vec3 normal(vec3 a){
	vec3 norm = a*sqrt(dot(a,a));
	return norm;
}

float dot(vec3 a, vec3 b){
	return a.data[0]*b.data[0]+a.data[1]*b.data[1]+a.data[2]*b.data[2];
}

vec3 operator+(vec3 a, vec3 b){
	vec3 s(a.data[0]+b.data[0],
			a.data[1]+b.data[1],
			a.data[2]+b.data[2]);
	return s;
}

vec3 operator*(vec3 a, float b){
	vec3 p(a.data[0]*b,
			a.data[1]*b,
			a.data[2]*b);
	return p;
}

vec3 operator-(vec3 a, vec3 b){
	vec3 d(a.data[0]-b.data[0],
			a.data[1]-b.data[1],
			a.data[2]-b.data[2]);
	return d;
}

mat4 perspective(float fovy, float aspect, float nr, float fr){
	float t = 1/tan(fovy*0.5f);
	float a = t/aspect;
	float b = t;
	float c = fr/(fr-nr);
	float d = 1;
	float e = -nr*fr/(fr-nr);

	mat4 proj;
	proj.data[0]=a;
	proj.data[1]=0;
	proj.data[2]=0;
	proj.data[3]=0;
	proj.data[4]=0;
	proj.data[5]=b;
	proj.data[6]=0;
	proj.data[7]=0;
	proj.data[8]=0;
	proj.data[9]=0;
	proj.data[10]=c;
	proj.data[11]=d;
	proj.data[12]=0;
	proj.data[13]=0;
	proj.data[14]=e;
	proj.data[15]=0;
	return proj;
}

mat4 lookAt(vec3 eye, vec3 at, vec3 up){
	vec3 z = normal(at-eye);
	vec3 x = normal(cross(up,z));
	vec3 y = cross(z,x);
	float a = -dot(x,eye);
	float b = -dot(y,eye);
	float c = -dot(z,eye);

	mat4 view;
	view.data[0]=x.data[0];
	view.data[1]=y.data[0];
	view.data[2]=z.data[0];
	view.data[3]=0;
	view.data[4]=x.data[1];
	view.data[5]=y.data[1];
	view.data[6]=z.data[1];
	view.data[7]=0;
	view.data[8]=x.data[2];
	view.data[9]=y.data[2];
	view.data[10]=z.data[2];
	view.data[11]=0;
	view.data[12]=a;
	view.data[13]=b;
	view.data[14]=c;
	view.data[15]=1;
	return view;
}	

mat4 operator*(mat4 a,mat4 b){
	mat4 product;
	product.data[0]=a.data[0]*b.data[0]+a.data[4]*b.data[1]+
					a.data[8]*b.data[2]+a.data[12]*b.data[3];
	product.data[1]=a.data[1]*b.data[0]+a.data[5]*b.data[1]+
					a.data[9]*b.data[2]+a.data[13]*b.data[3];
	product.data[2]=a.data[2]*b.data[0]+a.data[6]*b.data[1]+
					a.data[10]*b.data[2]+a.data[14]*b.data[3];
	product.data[3]=a.data[3]*b.data[0]+a.data[7]*b.data[1]+
					a.data[11]*b.data[2]+a.data[15]*b.data[3];
	product.data[4]=a.data[0]*b.data[4]+a.data[4]*b.data[5]+
					a.data[8]*b.data[6]+a.data[12]*b.data[7];
	product.data[5]=a.data[1]*b.data[4]+a.data[5]*b.data[5]+
					a.data[9]*b.data[6]+a.data[13]*b.data[7];
	product.data[6]=a.data[2]*b.data[4]+a.data[6]*b.data[5]+
					a.data[10]*b.data[6]+a.data[14]*b.data[7];
	product.data[7]=a.data[3]*b.data[4]+a.data[7]*b.data[5]+
					a.data[11]*b.data[6]+a.data[15]*b.data[7];
	product.data[8]=a.data[0]*b.data[8]+a.data[4]*b.data[9]+
					a.data[8]*b.data[10]+a.data[12]*b.data[11];
	product.data[9]=a.data[1]*b.data[8]+a.data[5]*b.data[9]+
					a.data[9]*b.data[10]+a.data[13]*b.data[11];
	product.data[10]=a.data[2]*b.data[8]+a.data[6]*b.data[9]+
					a.data[10]*b.data[10]+a.data[14]*b.data[11];
	product.data[11]=a.data[3]*b.data[8]+a.data[7]*b.data[9]+
					a.data[11]*b.data[10]+a.data[15]*b.data[11];
	product.data[12]=a.data[0]*b.data[12]+a.data[4]*b.data[13]+
					a.data[8]*b.data[14]+a.data[12]*b.data[15];
	product.data[13]=a.data[1]*b.data[12]+a.data[5]*b.data[13]+
					a.data[9]*b.data[14]+a.data[13]*b.data[15];
	product.data[14]=a.data[2]*b.data[12]+a.data[6]*b.data[13]+
					a.data[10]*b.data[14]+a.data[14]*b.data[15];
	product.data[15]=a.data[3]*b.data[12]+a.data[7]*b.data[13]+
					a.data[11]*b.data[14]+a.data[15]*b.data[15];
	return product;
}

vec4 operator*(mat4 a, vec4 b){
	vec4 p;
	p.data[0]=a.data[0]*b.data[0]+a.data[4]*b.data[1]+a.data[8]*b.data[2]+a.data[12]*b.data[3];
	p.data[1]=a.data[1]*b.data[0]+a.data[5]*b.data[1]+a.data[9]*b.data[2]+a.data[13]*b.data[3];
	p.data[2]=a.data[2]*b.data[0]+a.data[6]*b.data[1]+a.data[10]*b.data[2]+a.data[14]*b.data[3];
	p.data[3]=a.data[3]*b.data[0]+a.data[7]*b.data[1]+a.data[11]*b.data[2]+a.data[15]*b.data[3];
	return p;
}

void translate(mat4* model, vec3 t){
	model->data[0]+=model->data[3]*t.data[0];
	model->data[1]+=model->data[3]*t.data[1];
	model->data[2]+=model->data[3]*t.data[2];
	model->data[4]+=model->data[7]*t.data[0];
	model->data[5]+=model->data[7]*t.data[1];
	model->data[6]+=model->data[7]*t.data[2];
	model->data[8]+=model->data[11]*t.data[0];
	model->data[9]+=model->data[11]*t.data[1];
	model->data[10]+=model->data[11]*t.data[2];
	model->data[12]+=model->data[15]*t.data[0];
	model->data[13]+=model->data[15]*t.data[1];
	model->data[14]+=model->data[15]*t.data[2];
}

//angle in radians
void rotate(mat4* model, vec3 axis, float ang){
	float qr=cos(ang*0.5);
	float sineTerm = -sin(ang*0.5);
	float qi=sineTerm*axis.data[0];
	float qj=sineTerm*axis.data[1];
	float qk=sineTerm*axis.data[2];
	float a = 1-2*(qj*qj+qk*qk);
	float b = 2*(qi*qj+qk*qr);
	float c = 2*(qi*qk-qj*qr);
	float d = 2*(qi*qj-qk*qr);
	float e = 1-2*(qi*qi+qk*qk);
	float f = 2*(qj*qk+qi*qr);
	float g = 2*(qi*qk+qj*qr);
	float h = 2*(qj*qk-qi*qr);
	float i = 1-2*(qi*qi+qj*qj);
	mat4 prev = *model;
	model->data[0]=prev.data[0]*a+prev.data[1]*d+prev.data[2]*g;
	model->data[1]=prev.data[0]*b+prev.data[1]*e+prev.data[2]*h;
	model->data[2]=prev.data[0]*c+prev.data[1]*f+prev.data[2]*i;
	model->data[3]=prev.data[3];
	model->data[4]=prev.data[4]*a+prev.data[5]*d+prev.data[6]*g;
	model->data[5]=prev.data[4]*b+prev.data[5]*e+prev.data[6]*h;
	model->data[6]=prev.data[4]*c+prev.data[5]*f+prev.data[6]*i;
	model->data[7]=prev.data[7];
	model->data[8]=prev.data[8]*a+prev.data[9]*d+prev.data[10]*g;
	model->data[9]=prev.data[8]*b+prev.data[9]*e+prev.data[10]*h;
	model->data[10]=prev.data[8]*c+prev.data[9]*f+prev.data[10]*i;
	model->data[11]=prev.data[11];
	model->data[12]=prev.data[12]*a+prev.data[13]*d+prev.data[14]*g;
	model->data[13]=prev.data[12]*b+prev.data[13]*e+prev.data[14]*h;
	model->data[14]=prev.data[12]*c+prev.data[13]*f+prev.data[14]*i;
	model->data[15]=prev.data[15];
}
