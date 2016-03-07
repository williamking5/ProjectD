#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"mathlib.h"
#include"functionset.h"
#define STEP 10000000

typedef unsigned char byte;
typedef unsigned short dbyte;
typedef unsigned long dword;
typedef unsigned short word;
#pragma pack(push,2)
struct tagBITMAPFILEHEADER{
	dbyte bfType;
	dword bfSize;
	word bfReserved1;
	word bfReserved2;
	dword bfOffBits;

	dword biSize;
	dword biWidth;
	dword biHeight;
	word biPlanes;
	word biBitCount;
	dword biCompression;
	dword biSizeImage;
	dword biXPelsPerMeter;
	dword biYPelsPerMeter;
	dword biClrUsed;
	dword biClrImportant;
} BMPFILEHEADER;
#pragma(pop)
struct tagBITMAPFILEHEADER *bmp_p;
FILE *fd;
static unsigned char *file_p=NULL;
int Init_bmp_head(int width,int height){
	bmp_p=&BMPFILEHEADER;
	bmp_p->bfType=0x4d42;
	bmp_p->bfReserved1=0x0;
	bmp_p->bfReserved2=0x0;
	bmp_p->bfOffBits=0x36;

	bmp_p->biSize=0x28;
	bmp_p->biWidth=width;
	bmp_p->biHeight=height;
	bmp_p->biPlanes=0x01;
	bmp_p->biBitCount=24;
	bmp_p->biCompression=0;
	bmp_p->biSizeImage=width*height*3;

	bmp_p->bfSize=bmp_p->biSizeImage+54;
	
	bmp_p->biXPelsPerMeter=0x60;
	bmp_p->biYPelsPerMeter=0x60;
	bmp_p->biClrUsed=0;
	bmp_p->biClrImportant=0;
	return bmp_p->bfSize;
}
int distance_x=0,distance_y=0;
int width,height,red=0,green=0,blue=0;
int font[26][16][16];
int getpoint(int x,int y,int width){
	x+=distance_x;
	y+=distance_y;
	x=(x+18)%width;
	return y*width+x;
}
void drawpoint_easy(int x,int y,int width,int height){
	int limit=width*height+53;
	int location;
	location=getpoint(x,y,width);
	if (location<limit&&location>0)
		*(file_p+location*3)= *(file_p+location*3+1)=*(file_p+location*3+2)=0;
}
void paint(double a,double b,double y1,double y2,int width,int height,double(*func)(double)){
	double step=(b-a)/STEP,start,x,y;
	unsigned char* file_p_tmp=file_p;
	int location,location_x,location_y,limit=width*height-1;
	for (start=a;start<b;start+=step){
		x=start;
		y=(*func)(x);
		if (y<y1||y>y2) continue;
		x-=a;
		y-=y1;
		x/=(b-a);
		y/=(y2-y1);
		x*=width;
		y*=height;

		location_x=(int) (x+0.5);
		location_x=(location_x+18)%width;
		location_y=(int) (y+0.5);
		location=location_y*width+location_x;
		if (location>limit) continue;

		*(file_p_tmp+location*3)=blue;
		*(file_p_tmp+location*3+1)=green;
		*(file_p_tmp+location*3+2)=red;
	}
}
void drawword(int x){
	int i,j;
	for (i=15;i>=0;i--)
		for (j=0;j<16;j++)
			if (font[x][i][j]) {
				drawpoint_easy(j,15-i,width,height);
				drawpoint_easy(j-1,15-i,width,height);
				drawpoint_easy(j,16-i,width,height);
				drawpoint_easy(j+1,15-i,width,height);
				drawpoint_easy(j,14-i,width,height);
			}
}
int main(){
	printf("Please Input the width and height");
	scanf("%d %d",&width,&height);
	static char* file_name=NULL;
	static long file_length;
	file_length=Init_bmp_head(width,height);
	unsigned char *file_p_tmp=NULL;
	unsigned char *byte_copy_p=NULL;
	unsigned char byte_copy=0;

	int i=0;
	file_name="shiyan.bmp";
	file_p=(unsigned char*)malloc(sizeof (unsigned char)*file_length);
	file_p_tmp=file_p;

	for (i=0;i<file_length;i++){
		if (i%3==2) *file_p_tmp=0xff;
		if (i%3==1) *file_p_tmp=0xff;
		if (i%3==0) *file_p_tmp=0xff;
		file_p_tmp++;
	}

	double a,b;
	printf("Please input the range of x\n");
	scanf("%lf%lf",&a,&b);
	double y1,y2;
	printf("Please input the range of y\n");
	scanf("%lf%lf",&y1,&y2);
	if (y1>=y2){
		printf("Error range of y");
		return 0;
	}

paint(a,b,y1,y2,width,height,f1);
paint(a,b,y1,y2,width,height,f2);

	int j,k;
	memset(font,0,sizeof font);
	char s[30],filename,tmp[30];
	FILE* font_fp;
	for (i=0;i<26;i++){
		filename='A'+i;
		sprintf(tmp,"font/%c",filename);
		font_fp=fopen(tmp,"r");
		for (j=0;j<16;j++){
			fgets(s,29,font_fp);
			for (k=0;k<16;k++)
				font[i][j][k]=(s[k]==' '?0:1);
		}

		fclose(font_fp);
	}
		
	byte_copy_p=(unsigned char*) bmp_p;
	file_p_tmp=file_p;
	for (i=0;i<54;i++){
		*file_p_tmp=*byte_copy_p;
		file_p_tmp++;
		byte_copy_p++;
	}

	fd=fopen(file_name,"w");
	fwrite(file_p,file_length,1,fd);
	free(file_p);
	fclose(fd);
	printf("memeda\n");
	getchar();
	return 0;
}
