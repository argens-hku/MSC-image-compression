#include "StdAfx.h"
#include "AppConvert.h"

CAppConvert::CAppConvert(void)
{
	// Class Constructor
	mode = 0 ;
}

CAppConvert::~CAppConvert(void)
{
	// Class Destructor
	// Must call Final() function in the base class

	Final() ;
}

void CAppConvert::CustomInit(CView *pView) {
	// Add custom initialization code here
	// This initialization code will be called when this application is added to a processing task lists
	if(mode == 0) {
		CreateTunnel(width, height, &pOutput420) ;
	}
}

void CAppConvert::CustomFinal(void) {
	// Add custom finalization code here

	RemoveTunnel(pOutput420) ;
}

// This function converts input RGB image to a YUV image.
void CAppConvert::RGBtoYUV(unsigned char *pRGB, unsigned char *pYUV) {

	// You should implement RGB to YUV conversion here.


	// Sample code start - You may delete these sample code
	// The following code illustrates generation of grayscale images YUV images

	int i, j;
	unsigned char r, g, b;

	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			b = pRGB[(i + j * width) * 3] ;
			g = pRGB[(i + j * width) * 3 + 1] ;
			r = pRGB[(i + j * width) * 3 + 2] ;
			
			pYUV[(i + j * width) * 3 + 0] = unsigned char (0.299000 * r + 0.587000 * g + 0.114000 * b);
			pYUV[(i + j * width) * 3 + 1] = unsigned char (-0.168736 * r -0.331264 * g + 0.500000 * b + 128);
			pYUV[(i + j * width) * 3 + 2] = unsigned char (0.500000 * r -0.418688 * g -0.081312 * b + 128);
		}
	}

	// TCHAR t [100] = TEXT("%c: %u; ");
	// for (i = 138103; i < 138123; i ++){
	// 	printf (t, 'r', pRGB [(i * 3 + 2)]);
	// 	printf (t, 'g', pRGB [(i * 3 + 1)]);
	// 	printf (t, 'b', pRGB [(i * 3)]);
	// 	printf (t, 'y', pYUV [(i * 3 + 0)]);
	// 	printf (t, 'u', pYUV[(i * 3 + 1)]);
	// 	printf (t, 'v', pYUV [(i * 3 + 2)]);
	// 	printf (TEXT (" ---%u--- \r\n"), i);
	// }


	// Sample Code end
}

// This function converts input YUV image to a RGB image.
void CAppConvert::YUVtoRGB(unsigned char *pYUV, unsigned char *pRGB) {

	// You should implement YUV to RGB conversion here.

	// Sample code start - You may delete these sample code
	int i, j ;
	unsigned char y, u, v;
	float f;

	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {

			y = pYUV[(i + j * width) * 3 + 0];
			u = pYUV[(i + j * width) * 3 + 1];
			v = pYUV[(i + j * width) * 3 + 2];

			f = y + 1.4075 * (v - 128);
			pRGB[(i + j * width) * 3 + 2] = unsigned char (f < 0 ? 0 : f > 255 ? 255 : f); //r

			f = y - 0.3455 * (u - 128) - 0.7169 * (v - 128);
			pRGB[(i + j * width) * 3 + 1] = unsigned char (f < 0 ? 0 : f > 255 ? 255 : f); //g

			f = y + 1.7790 * (u - 128);
			pRGB[(i + j * width) * 3 + 0] = unsigned char (f < 0 ? 0 : f > 255 ? 255 : f); //b
		}
	}

	// printf (TEXT("width: %u; height: %u"), width, height);

	// TCHAR t [100] = TEXT("%c: %u; ");
	// for (i = 138103; i < 138123; i ++){
	// 	printf (t, 'r', pRGB [(i * 3 + 2)]);
	// 	printf (t, 'g', pRGB [(i * 3 + 1)]);
	// 	printf (t, 'b', pRGB [(i * 3)]);
	// 	printf (t, 'y', pYUV [(i * 3 + 0)]);
	// 	printf (t, 'u', pYUV[(i * 3 + 1)]);
	// 	printf (t, 'v', pYUV [(i * 3 + 2)]);
	// 	printf (TEXT (" ---%u--- \r\n"), i);
	// }

	// Sample Code end

	unsigned int *output;
	output = new unsigned int [96];
	for (i = 0; i < 96;i ++){
		output [i] = 0;
	}
	unsigned int temp;

	for (i = 0; i < height * width; i++){
		temp = pRGB [i*3 + 2] / 16; //r
		output [temp] += 1;

		temp = pRGB [i*3 + 1] / 16; //g
		output [16 + temp] += 1;

		temp = pRGB [i*3 + 0] / 16; //b
		output [32 + temp] += 1;

		temp = pYUV [i*3 + 0] / 16; //y
		output [48 + temp] += 1;

		temp = pYUV [i*3 + 1] / 16; //u
		output [64 + temp] += 1;

		temp = pYUV [i*3 + 2] / 16; //v
		output [80 + temp] += 1;
	}

	// for (j = 0; j < 6; j ++){
	// 	switch (j)
	// 	{
	// 		case 0:
	// 			printf (TEXT("R: "));
	// 			break;
	// 		case 1:
	// 			printf (TEXT("G: "));
	// 			break;
	// 		case 2:
	// 			printf (TEXT("B: "));
	// 			break;
	// 		case 3:
	// 			printf (TEXT("Y: "));
	// 			break;
	// 		case 4:
	// 			printf (TEXT("U: "));
	// 			break;
	// 		case 5:
	// 			printf (TEXT("V: "));
	// 			break;
	// 	}

	// 	int sum = 0;
	// 	for (i = 0; i < 16; i++){
	// 		printf (TEXT("%u "), output [j * 16 + i]);
	// 		sum += output [j * 16 + i];
	// 	}

	// 	printf (TEXT("              ||%u   \r\n")), sum;
	// }

	delete output;

}



void CAppConvert::YUVtoYUV420(unsigned char *pYUV, unsigned char *pYUV420) {

	int i, j ;
	int sum ;
	int si0, si1, sj0, sj1 ;

	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			pYUV420[(i + j * width) * 3] = pYUV[(i + j * width) * 3] ;
		}
	}

	for(j = 0; j < height; j+=2) {
		sj0 = j ;
		sj1 = (j + 1 < height) ? j + 1 : j ;
	
		for(i = 0; i < width; i+=2) {
			si0 = i ;
			si1 = (i + 1 < width) ? i + 1 : i ;

			sum =  pYUV[(si0 + sj0 * width) * 3 + 1] ; 
			sum += pYUV[(si1 + sj0 * width) * 3 + 1] ; 
			sum += pYUV[(si0 + sj1 * width) * 3 + 1] ; 
			sum += pYUV[(si1 + sj1 * width) * 3 + 1] ; 

			sum = sum / 4 ;

			pYUV420[(si0 + sj0 * width) * 3 + 1] = sum ;
			pYUV420[(si1 + sj0 * width) * 3 + 1] = sum ;
			pYUV420[(si0 + sj1 * width) * 3 + 1] = sum ;
			pYUV420[(si1 + sj1 * width) * 3 + 1] = sum ;

			sum =  pYUV[(si0 + sj0 * width) * 3 + 2] ; 
			sum += pYUV[(si1 + sj0 * width) * 3 + 2] ; 
			sum += pYUV[(si0 + sj1 * width) * 3 + 2] ; 
			sum += pYUV[(si1 + sj1 * width) * 3 + 2] ; 

			sum = sum / 4 ;

			pYUV420[(si0 + sj0 * width) * 3 + 2] = sum ;
			pYUV420[(si1 + sj0 * width) * 3 + 2] = sum ;
			pYUV420[(si0 + sj1 * width) * 3 + 2] = sum ;
			pYUV420[(si1 + sj1 * width) * 3 + 2] = sum ;
		}
	}
}

void CAppConvert::Process(void) {

	// Don't change anything within this function.

	unsigned char *yuvBuf = new unsigned char[width * height * 3] ;
	unsigned char *yuv420Buf = new unsigned char[width * height * 3] ;

	RGBtoYUV(pInput, yuvBuf) ;

	if(mode == 0) {
		SetTitle(pOutput, _T("YUV444")) ;
		SetTitle(pOutput420, _T("YUV420")) ;

		YUVtoYUV420(yuvBuf, yuv420Buf) ;

		YUVtoRGB(yuvBuf, pOutput) ;
		YUVtoRGB(yuv420Buf, pOutput420) ;
	} else if(mode == 1) {
		SetTitle(pOutput, _T("Y")) ;
	
		int i ;

		for(i = 0; i < width * height; i++) {
			yuvBuf[i * 3 + 1] = 128 ;
			yuvBuf[i * 3 + 2] = 128 ;
		}

		YUVtoRGB(yuvBuf, pOutput) ;
	} else if(mode == 2) {
		SetTitle(pOutput, _T("U")) ;

		int i ;

		for(i = 0; i < width * height; i++) {
			yuvBuf[i * 3 + 0] = 128 ;
			yuvBuf[i * 3 + 2] = 128 ;
		}

		YUVtoRGB(yuvBuf, pOutput) ;
	} else if(mode == 3) {
		SetTitle(pOutput, _T("V")) ;

		int i ;

		for(i = 0; i < width * height; i++) {
			yuvBuf[i * 3 + 0] = 128 ;
			yuvBuf[i * 3 + 1] = 128 ;
		}

		YUVtoRGB(yuvBuf, pOutput) ;
	}

	delete [] yuvBuf ;
	delete [] yuv420Buf ;

	PutDC(pOutput) ;
	if(mode == 0) {
		PutDC(pOutput420) ;
	}
}
