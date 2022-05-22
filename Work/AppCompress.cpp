#include "StdAfx.h"
#include "AppCompress.h"
#include <algorithm>
#include <fstream>
using namespace std;

CAppCompress::CAppCompress(void)
{
	// Class Constructor
}

CAppCompress::~CAppCompress(void)
{
	// Class Destructor
	// Must call Final() function in the base class

	Final() ;
}

void CAppCompress::CustomInit(CView *pView) {
	// Add custom initialization code here
	// This initialization code will be called when this application is added to a processing task lists
}

void CAppCompress::CustomFinal(void) {
	// Add custom finalization code here
}

// --- Start of Custom Code --- //

unsigned char *CAppCompress::DifferentialCoding (int &predictorSize, unsigned char *&predictor, int dataSize, unsigned char *&data){

	predictorSize = height;
	
	unsigned char *compressedData = new unsigned char [dataSize];
	predictor = new unsigned char [predictorSize];
	
	int i, j, tempInt, diff, sDiff;
	int *squaredDiff = new int [5];

	for (j = 0; j < height; j++){
		for (i = 0; i< 5; i++){
			squaredDiff [i] = 0;
		}

		for (i = 1; i < width; i ++){
			tempInt = data [j * width];
			diff = data [j * width + i] - tempInt;
			squaredDiff [0] += diff * diff;
		}

		for (i = 1; i < width; i++){
			tempInt = data [j * width + i - 1];
			diff = data [j * width + i] - tempInt;
			squaredDiff [1] += diff * diff;
		}

		if (j == 0){
			// printf (TEXT ("Hello!????\r\n"));
			if (squaredDiff [0] <= squaredDiff [1])
				predictor [j] = 0;
			else
				predictor [j] = 1;
			continue;
		}

		for (i = 1; i < width; i++){
			tempInt = data [(j - 1) * width + i];
			diff = data [j * width + i] - tempInt;
			squaredDiff [2] += diff * diff;
		}

		for (i = 1; i < width; i++){
			tempInt = (data [j * width + i - 1] + data [(j - 1) * width + i]) >> 1;
			diff = data [j * width + i] - tempInt;
			squaredDiff [3] += diff * diff;
		}

		for (i = 1; i < width; i++){
			tempInt = data [j * width + i - 1] + data [(j - 1) * width + i] - data [(j - 1) * width + i - 1];

			diff = (data [j * width + i - 1] - tempInt) * (data [j * width + i - 1] - tempInt);

			if ((data [(j - 1) * width + i] - tempInt) * (data [(j - 1) * width + i] - tempInt) < diff){
				diff = (data [(j - 1) * width + i] - tempInt) * (data [(j - 1) * width + i] - tempInt);
			}

			if ((data [(j - 1) * width + i - 1] - tempInt) * (data [(j - 1) * width + i - 1] - tempInt) < diff){
				diff = (data [(j - 1) * width + i - 1] - tempInt) * (data [(j - 1) * width + i - 1] - tempInt);	
			}

			squaredDiff [4] += diff;
		}

		tempInt = squaredDiff [0];
		predictor [j] = 0;
		for (i = 1; i < 5; i ++){
			if (squaredDiff [i] < tempInt){
				predictor [j] = i;
				tempInt = squaredDiff [i];
			}
		}
	}

	

	delete [] squaredDiff;

	for (j = 0; j < height; j ++){
		compressedData [j * width] = data [j * width];
		switch (predictor [j]){

			case 0: for (i = 1; i < width; i++){
				tempInt = data [j * width];
				compressedData [j * width + i] = data [j * width + i] - tempInt;
			}
			break;

			case 1:	for (i = 1; i < width; i++){
				tempInt = data [j * width + i - 1];
				compressedData [j * width + i] = data [j * width + i] - tempInt;
			}
			break;

			case 2:	for (i = 1; i < width; i++){
				tempInt = data [(j - 1) * width + i];
				compressedData [j * width + i] = data [j * width + i] - tempInt;
			}
			break;

			case 3:	for (i = 1; i < width; i++){
				tempInt = (data [j * width + i - 1] + data [(j - 1) * width + i]) >> 1;
				compressedData [j * width + i] = data [j * width + i] - tempInt;
			}
			break;

			case 4:	for (i = 1; i < width; i++){

				tempInt = data [j * width + i - 1] + data [(j - 1) * width + i] - data [(j - 1) * width + i - 1];

				sDiff = (data [j * width + i - 1] - tempInt)  * (data [j * width + i - 1] - tempInt);
				diff = data [j * width + i] - data [j * width + i - 1];

				if ((data [(j - 1) * width + i] - tempInt) * (data [(j - 1) * width + i] - tempInt) < sDiff){
					sDiff = (data [(j - 1) * width + i] - tempInt) * (data [(j - 1) * width + i] - tempInt);
					diff = data [j * width + i] - data [(j - 1) * width + i];
				}

				if ((data [(j - 1) * width + i - 1] - tempInt) * (data [(j - 1) * width + i - 1] - tempInt) < sDiff){
					sDiff = (data [(j - 1) * width + i - 1] - tempInt) * (data [(j - 1) * width + i - 1] - tempInt);
					diff = data [j * width + i] - data [(j - 1) * width + i - 1];
				}
				compressedData [j * width + i] = diff;
			}
			break;

			default: printf (TEXT ("This is some error Line 136"));
			break;
		}
	}

	return compressedData;

}

void CAppCompress::MetaDataSizeInfo(int *metaData, unsigned char * info){

	int tester, i;

	for (i = 0; i < 6; i++){
		info [2 * i + 1] = 0;
		while ((metaData [i] & 0x1) != 1){
			metaData [i] >>= 1;
			info [2 * i + 1] += 1;
		}
		tester = 1;
		info [2 * i] = 1;
		while ((metaData [i] & tester) != metaData [i]){
			tester <<= 1;
			tester += 1;
			info [2 * i] += 1;
		}
	}
}

unsigned char CAppCompress::Push (unsigned char original, unsigned char &counter, unsigned char *buffer, int &bufferUsed, unsigned char value, unsigned char length){
	
	bufferUsed = -1;

	if (length > 8){
		if ((length - 8) >= counter){
			length -= counter;
			counter = 0;

			while (length >= 16){
				buffer [++bufferUsed] = 0;
				length -= 8;
			}

			if (length > 8){
				buffer [++bufferUsed] = 0;
				counter = 16 - length;
				length = 8;
			}
		}
		else{
			counter -= (length - 8);
			length = 8;
		}
	}

	if (length >= counter){
		if (bufferUsed == -1){
			original += (value >> (length - counter));
		}
		else{
			buffer [bufferUsed] += (value >> (length - counter));
		}
		
		length -= counter;
		counter = 8;

		counter -= length;
		buffer [++bufferUsed] = ((value << counter) & 0x00FF);
	}
	else{
		counter = counter - length;
		
		if (bufferUsed == -1){
			original += (value << counter);
		}
		else{
			buffer [bufferUsed] += (value << counter);
		}
	}
	
	return original;

}

unsigned char *CAppCompress::CompressChannel (unsigned char *channel, int size, int& compressedSize){
	unsigned char *compressedChannel = new unsigned char [size];
	unsigned char *buffer = new unsigned char [10];
	int i, index, bufferUsed, temp, bufferIndex;
	unsigned char counter;

	index = 0;
	bufferIndex = 0;
	counter = 8;
	bufferUsed = -1;

	compressedChannel [0] = 0;

	for (i = 0; i < size; i++) {
		if (channel [i] <= 32){
			if ((channel [i] >> 2) <= 1){
				switch (channel [i]){
					case 0: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 2, 2);
							break;

					case 1: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 6, 3);
							break;

					case 2: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 6, 4);
							break;

					case 3: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 4, 4);
							break;

					case 4: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 4, 5);
							break;

					case 5: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 6, 5);
							break;

					case 6: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 6, 6);
							break;

					case 7: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 4, 6);
							break;
				}
				
				if (bufferUsed != -1){
					bufferIndex = 0;
					while (bufferUsed >= 0){
						compressedChannel [++ index] = buffer [bufferIndex ++];
						bufferUsed--;
					}
				}
				
			continue;
			}

			if ((channel [i] >> 2) < 7){
				
				temp = channel [i] >> 2;

				compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, channel[i] - 4 * (temp - 1), temp * 2 + 3);

				if (bufferUsed != -1){
					bufferIndex = 0;
					while (bufferUsed >= 0){
						compressedChannel [++ index] = buffer [bufferIndex ++];
						bufferUsed--;
					}
				}
				
			continue;
			}

			if (channel [i] <= 29){
				compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, channel [i] - 14, 18);
			}
			else{
				compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, channel [i] - 26, 17);	
			}

			bufferIndex = 0;
			if (bufferUsed != -1){
				while (bufferUsed >= 0){
					compressedChannel [++ index] = buffer [bufferIndex ++];
					bufferUsed--;
				}
			}
			continue;
		}

		if (channel [i] >= 225){
			if (channel [i] >= 249){
				switch (channel [i]){
					case 255: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 7, 3);
							break;

					case 254: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 7, 4);
							break;

					case 253: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 5, 4);
							break;
				
					case 249: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 7, 6);
							break;

					case 250: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 5, 6);
							break;

					case 251: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 7, 5);
							break;

					case 252: compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 5, 5);
							break;
				}

				if (bufferUsed != -1){
					bufferIndex = 0;
						while (bufferUsed >= 0){
						compressedChannel [++ index] = buffer [bufferIndex ++];
						bufferUsed--;
					}
				}
				continue;		
			}

			if (channel [i] <= 228){
				compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, channel[i] - 221, 18);
				
				if (bufferUsed != -1){
					bufferIndex = 0;
					while (bufferUsed >= 0){
						compressedChannel [++ index] = buffer [bufferIndex ++];
						bufferUsed--;
					}
				}
		
				continue;
			}

			temp = (256 - channel [i]) >> 2;
			compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, channel [i] - (249 - temp * 4), 2*temp + 4);
			if (bufferUsed != -1){
				bufferIndex = 0;
				while (bufferUsed >= 0){
					compressedChannel [++ index] = buffer [bufferIndex ++];
					bufferUsed--;
				}
			}
			continue;
		}
		// if (i == 0 && channel [i] == 59) {printf (TEXT ("\r\nEUREKA --- %u\r\n bufferUsed: %i \r\nindex: %i \r\ncounter: %u \r\n"), channel [i], bufferUsed, index, counter);}
		compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, channel [i], 24);
		// if (i == 0 && channel [i] == 59) {printf (TEXT ("\r\nEUREKA --- %u\r\n bufferUsed: %i \r\nindex: %i \r\ncounter: %u \r\n"), channel [i], bufferUsed, index, counter);}
		if (bufferUsed != -1){
			bufferIndex = 0;
			while (bufferUsed >= 0){
				compressedChannel [++ index] = buffer [bufferIndex ++];
				bufferUsed--;
			}
		}
	}

	if (counter != 8){
		index ++;
	}

	compressedSize = index + 1;
	unsigned char *tempArray = new unsigned char [compressedSize];
	memcpy (tempArray, compressedChannel, index);
	delete [] compressedChannel;
	delete [] buffer;
	return tempArray;
}

// This function compresses input 24-bit image (8-8-8 format, in pInput pointer).
// This function shall allocate storage space for compressedData, and return it as a pointer.
// The input reference variable cDataSize, is also serve as an output variable to indicate the size (in bytes) of the compressed data.
unsigned char *CAppCompress::Compress(int &cDataSize) {

	// You can modify anything within this function, but you cannot change the function prototype.
	
	unsigned char *compressedData, *channelBlue, *channelGreen, *channelRed;
	unsigned char *differentialChannelBlue, *differentialChannelGreen, *differentialChannelRed;
	unsigned char *compressedChannelBlue, *compressedChannelGreen, *compressedChannelRed;
	unsigned char *predictorBlue, *predictorGreen, *predictorRed;

	int i = 0;
	int j;
	int size = width * height;
	
	int predictorSizeR, predictorSizeG, predictorSizeB;
	int compressedSizeR, compressedSizeG, compressedSizeB;

	// -------------------------------------------------- Separating 3 channels --------------------------------------------------

	channelBlue = new unsigned char [size];
	for (i = 0; i < size; i++){
		channelBlue [i] = pInput [i * 3];
	}
	
	channelGreen = new unsigned char [size];
	for (i = 0; i < size; i++){
		channelGreen [i] = pInput [i * 3 + 1];
	}

	channelRed = new unsigned char [size];
	for (i = 0; i < size; i++){
		channelRed [i] = pInput [i * 3 + 2];
	}

	// -------------------------------------------------- Differential Coding + Compression --------------------------------------------------

	differentialChannelBlue = DifferentialCoding (predictorSizeB, predictorBlue, size, channelBlue);
	differentialChannelGreen = DifferentialCoding (predictorSizeG, predictorGreen, size, channelGreen);
	differentialChannelRed = DifferentialCoding (predictorSizeR, predictorRed, size, channelRed);

	// original = new unsigned char [size];
	// memcpy (original, differentialChannelRed, size);

	delete [] channelRed;
	delete [] channelGreen;
	delete [] channelBlue;

	compressedSizeB = size;
	compressedSizeG = size;
	compressedSizeR = size;

	compressedChannelBlue = CompressChannel (differentialChannelBlue, size, compressedSizeB);
	compressedChannelGreen = CompressChannel (differentialChannelGreen, size, compressedSizeG);
	compressedChannelRed = CompressChannel (differentialChannelRed, size, compressedSizeR);

	// ofstream fout;
	// fout.open ("C:\\Users\\ArgensNg\\Desktop\\output2.txt");

	// for (i = 0; i < 50; i++){
	// 	fout << i << ": " << (int) compressedChannelRed [i] << endl;
	// }
	// fout.close ();

	delete [] differentialChannelBlue;
	delete [] differentialChannelRed;
	delete [] differentialChannelGreen;

	// -------------------------------------------------- Packing Meta Data --------------------------------------------------

	int * metaData = new int [6];
	metaData [0] = predictorSizeB;
	metaData [1] = predictorSizeG;
	metaData [2] = predictorSizeR;
	metaData [3] = compressedSizeB;
	metaData [4] = compressedSizeG;
	metaData [5] = compressedSizeR;
	unsigned char *info = new unsigned char [12];
	MetaDataSizeInfo (metaData, info);

	int no_of_bits = 0;
	for (i = 0; i < 6; i++){
		no_of_bits += info [i * 2];
	}
	int no_of_bytes = ((no_of_bits - 1) >> 3) + 1;


	cDataSize = predictorSizeB + predictorSizeG + predictorSizeR + compressedSizeB + compressedSizeG + compressedSizeR + 12 + no_of_bytes;
	// cDataSize = compressedSizeB + compressedSizeG + compressedSizeR;
	
	compressedData = new unsigned char [cDataSize];

	// printf (TEXT ("Predictor Size Blue: %u\r\n"), predictorSizeB);
	// printf (TEXT ("Predictor Size Green: %u\r\n"), predictorSizeG);
	// printf (TEXT ("Predictor Size Red: %u\r\n"), predictorSizeR);
	// printf (TEXT ("Compressed Data Size Blue: %u\r\n"), compressedSizeB);
	// printf (TEXT ("Compressed Data Size Green: %u\r\n"), compressedSizeG);
	// printf (TEXT ("Compressed Data Size Red: %u\r\n"), compressedSizeR);

	memcpy (compressedData, info, 12);

	int index = 12;
	unsigned counter = 8;
	unsigned char temp = 0;

	for (i = 0; i < 6; i++){
		if (info [i * 2] > counter){
			temp += (metaData [i] >> (info [i * 2] - counter));
			info [i * 2] -= counter;
			compressedData [index ++] = temp;
			temp = 0;
			counter = 8;

			while (info [i * 2] > 8){
				temp = (metaData [i] >> (info [i * 2] - 8)) & 0x00FF;
				info [i * 2] -= 8;
				compressedData [index ++] = temp;
				temp = 0;
			}

			counter = counter - info [i * 2];
			temp += ((metaData [i] << counter) & 0x00FF);
			compressedData [index] = temp;
		}
		else{
			counter = counter - info [i * 2];
			temp += (metaData [i] << counter);
		}
	}

	delete [] metaData;
	delete [] info;

	// -------------------------------------------------- Packing Predictor and Channels --------------------------------------------------

	unsigned char *loc = compressedData + 12 + no_of_bytes;
	// unsigned char *loc = compressedData;

	memcpy (loc, predictorBlue, predictorSizeB);
	loc = loc +predictorSizeB;

	memcpy (loc, predictorGreen, predictorSizeG);
	loc = loc + predictorSizeG;

	memcpy (loc, predictorRed, predictorSizeR);
	loc = loc + predictorSizeR;

	memcpy (loc, compressedChannelBlue, compressedSizeB);
	loc = loc + compressedSizeB;

	memcpy (loc, compressedChannelGreen, compressedSizeG);
	loc = loc + compressedSizeG;

	memcpy (loc, compressedChannelRed, compressedSizeR);
	loc = loc + compressedSizeR;

	delete [] predictorRed;
	delete [] predictorGreen;
	delete [] predictorBlue;
	delete [] compressedChannelRed;
	delete [] compressedChannelGreen;
	delete [] compressedChannelBlue;

	return compressedData;		// return the compressed data
}

void CAppCompress::DecompressChannel (unsigned char *compressedChannel, unsigned char *channel, int compressedSize, int size){
	int i, j, index;
	unsigned char counter, temp, shift, broken, cache, focus, found;
	
	const int bitmask = 0x00C0;

	i = 0;
	index = 0;
	counter = 0;
	shift = 0;
	found = false;

	broken = 0;
	cache = 0;

	temp = 0;

	while (i < compressedSize){
		focus = compressedChannel [i];
		if (!found){
			if (counter < 7){
				temp = (focus & (bitmask >> counter)) >> (6 - counter);
				counter += 2;
				if (counter == 8){
					counter = 0;
					i++;
				}
			}
			else{
				temp = (focus & (0x0001 << (7 - counter))) >> (7 - counter);
				temp <<= 1;

				counter = 1;
				i++;
				temp += ((compressedChannel [i] & 0x0080) >> 7);
			}
			if (temp == 0){
				shift ++;
				if (shift == 8){
					found = true;
				}
			}
			else{
				if (shift == 0 && temp == 2){
					channel [index ++] = 0;
					found = false;
					broken = false;
					cache = 0;
					continue;
				}
				found = true;
				broken = true;
				cache = temp;
			}
		}
		else{
			if (shift == 8){
				temp = focus << counter;
				i++;
				temp += (compressedChannel [i] >> (8 - counter));
				found = false;
				shift = 0;
				channel [index ++] = temp;
				continue;
			}

			if (shift == 0){
				if (cache == 3){
					temp = (focus & (0x0001 << (7 - counter))) >> (7 - counter);
					counter ++;
					if (counter == 8){
						counter = 0;
						i++;
					}

					if (temp == 1){
						channel [index ++] = 255;
					}
					else{
						channel [index ++] = 1;
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				if (cache == 1){
					if (counter == 7){
						temp = focus & 0x0001;
						temp <<= 1;
						i++;
						counter = 1;
						temp += ((compressedChannel [i] & 0x0080) >> 7);
					}
					else{
						temp = (focus & (bitmask >> counter)) >> (6 - counter);
						counter += 2;
						if (counter == 8){
							counter = 0;
							i ++;
						}
					}
					switch (temp){
						case 0: channel [index ++] = 3; break;
						case 1: channel [index ++] = 253; break;
						case 2: channel [index ++] = 2; break;
						case 3: channel [index ++] = 254; break;
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				printf (TEXT ("Somethings Wrong 710. Cache =  %u\r\n"), cache);
				continue;
			}

			if (shift == 1){
				if (cache == 1){
					if (counter == 7){
						temp = focus & 0x0001;
						temp <<= 1;
						i++;
						counter = 1;
						temp += ((compressedChannel [i] & 0x0080) >> 7);
					}
					else{
						temp = (focus & (bitmask >> counter)) >> (6 - counter);
						counter += 2;
						if (counter == 8){
							counter = 0;
							i ++;
						}
					}
					switch (temp){
						case 0: channel [index ++] = 7; break;
						case 1: channel [index ++] = 250; break;
						case 2: channel [index ++] = 6; break;
						case 3: channel [index ++] = 249; break;
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				if (cache == 3){
					temp = (focus & (0x0001 << (7 - counter))) >> (7 - counter);
					counter ++;
					if (counter == 8){
						counter = 0;
						i++;
					}

					if (temp == 1){
						channel [index ++] = 251;
					}
					else{
						channel [index ++] = 5;
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				if (cache == 2){
					temp = (focus & (0x0001 << (7 - counter))) >> (7 - counter);
					counter ++;
					if (counter == 8){
						counter = 0;
						i++;
					}

					if (temp == 1){
						channel [index ++] = 252;
					}
					else{
						channel [index ++] = 4;
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				printf (TEXT ("Somethings Wrong 781. Cache =  %u\r\n"), cache);
				continue;
			}

			if (shift == 7){

				if (cache == 1){
					if (counter == 7){
						temp = focus & 0x0001;
						temp <<= 1;
						i++;
						counter = 1;
						temp += ((compressedChannel [i] & 0x0080) >> 7);
					}
					else{
						temp = (focus & (bitmask >> counter)) >> (6 - counter);
						counter += 2;
						if (counter == 8){
							counter = 0;
							i ++;
						}
					}
					switch (temp){
						case 0: channel [index ++] = 225; break;
						case 1: channel [index ++] = 226; break;
						case 2: channel [index ++] = 227; break;
						case 3: channel [index ++] = 228; break;
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				if (cache == 2){
					temp = (focus & (0x0001 << (7 - counter))) >> (7 - counter);
					counter ++;
					if (counter == 8){
						counter = 0;
						i++;
					}

					if (temp == 1){
						channel [index ++] = 31;
					}
					else{
						channel [index ++] = 30;
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				if (cache == 3){
					temp = (focus & (0x0001 << (7 - counter))) >> (7 - counter);
					counter ++;
					if (counter == 8){
						counter = 0;
						i++;
					}

					if (temp == 0){
						channel [index ++] = 32;
					}
					else{
						temp <<= 1;
						temp += ((compressedChannel [i] & (0x0001 << (7 - counter))) >> (7 - counter));
						counter ++;
						if (counter == 8){
							counter = 0;
							i++;
						}

						if (temp == 3){
							channel [index ++] = 29;
						}
						else{
							channel [index ++] = 28;
						}
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				printf (TEXT ("Somethings Wrong 869. Cache =  %u\r\n"), cache);
				continue;			
			}

			if (shift > 1 && shift < 7){
				if (cache == 1){
					if (counter == 7){
						temp = focus & 0x0001;
						temp <<= 1;
						i++;
						counter = 1;
						temp += ((compressedChannel [i] & 0x0080) >> 7);;
					}
					else{
						temp = (focus & (bitmask >> counter)) >> (6 - counter);
						counter += 2;
						if (counter == 8){
							counter = 0;
							i ++;
						}
					}

					switch (temp){
						case 0: channel [index ++] = 245 - 4 * (shift - 2); break;
						case 1: channel [index ++] = 246 - 4 * (shift - 2); break;
						case 2: channel [index ++] = 247 - 4 * (shift - 2); break;
						case 3: channel [index ++] = 248 - 4 * (shift - 2); break;
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				if (cache == 3){
					temp = (focus & (0x0001 << (7 - counter))) >> (7 - counter);
					counter ++;
					if (counter == 8){
						counter = 0;
						i++;
					}

					if (temp == 1){
						channel [index ++] = 11 + 4 * (shift - 2);
					}
					else{
						channel [index ++] = 10 + 4 * (shift - 2);
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				if (cache == 2){
					temp = (focus & (0x0001 << (7 - counter))) >> (7 - counter);
					counter ++;
					if (counter == 8){
						counter = 0;
						i++;
					}

					if (temp == 1){
						channel [index ++] = 9 + 4 * (shift - 2);
					}
					else{
						channel [index ++] = 8 + 4 * (shift - 2);
					}
					found = false;
					broken = false;
					cache = 0;
					shift = 0;
					continue;
				}

				printf (TEXT ("Somethings Wrong 943. Cache =  %u\r\n"), cache);
				continue;
			}

			printf (TEXT ("Somethings Wrong 947. Shift =  %u\r\n"), cache);
			continue;
		}

		if (index >= size){
			// Cut off padding zeros
			break;
		}
	}
}

void CAppCompress::ChannelRestore (unsigned char *compressedChannel, unsigned char *channel, unsigned char *predictor, int size){
	
	int i, j, tempInt, diff, sDiff;

	for (j = 0; j < height; j ++){
		channel [j * width] = compressedChannel [j * width];
		switch (predictor [j]){

			case 0: for (i = 1; i < width; i++){
				tempInt = channel [j * width];
				channel [j * width + i] = compressedChannel [j * width + i] + tempInt;
			}
			break;

			case 1:	for (i = 1; i < width; i++){
				tempInt = channel [j * width + i - 1];
				channel [j * width + i] = compressedChannel [j * width + i] + tempInt;
			}
			break;

			case 2:	for (i = 1; i < width; i++){
				tempInt = channel [(j - 1) * width + i];
				channel [j * width + i] = compressedChannel [j * width + i] + tempInt;
			}
			break;

			case 3:	for (i = 1; i < width; i++){
				tempInt = (channel [j * width + i - 1] + channel [(j - 1) * width + i]) >> 1;
				channel [j * width + i] = compressedChannel [j * width + i] + tempInt;
			}
			break;

			case 4:	for (i = 1; i < width; i++){

				tempInt = channel [j * width + i - 1] + channel [(j - 1) * width + i] - channel [(j - 1) * width + i - 1];

				sDiff = (channel [j * width + i - 1] - tempInt)  * (channel [j * width + i - 1] - tempInt);
				diff = channel [j * width + i - 1];

				if ((channel [(j - 1) * width + i] - tempInt) * (channel [(j - 1) * width + i] - tempInt) < sDiff){
					sDiff = (channel [(j - 1) * width + i] - tempInt) * (channel [(j - 1) * width + i] - tempInt);
					diff = channel [(j - 1) * width + i];
				}

				if ((channel [(j - 1) * width + i - 1] - tempInt) * (channel [(j - 1) * width + i - 1] - tempInt) < sDiff){
					sDiff = (channel [(j - 1) * width + i - 1] - tempInt) * (channel [(j - 1) * width + i - 1] - tempInt);
					diff = channel [(j - 1) * width + i - 1];
				}

				channel [j * width + i] = diff + compressedChannel [j * width + i];
			}
			break;

			default: printf (TEXT ("This is some error Line 363"));
			break;
		}
	}
}
// This function takes in compressedData with size cDatasize, and decompresses it into 8-8-8 image.
// The decompressed image data should be stored into the uncompressedData buffer, with 8-8-8 image format
void CAppCompress::Decompress(unsigned char *compressedData, int cDataSize, unsigned char *uncompressedData) {

	// You can modify anything within this function, but you cannot change the function prototype.

	int predictorSizeR, predictorSizeG, predictorSizeB;
	int compressedSizeR, compressedSizeG, compressedSizeB;
	unsigned char *predictorBlue, *predictorGreen, *predictorRed;
	unsigned char *differentialChannelBlue, *differentialChannelGreen, *differentialChannelRed;
	unsigned char *compressedChannelBlue, *compressedChannelGreen, *compressedChannelRed;
	unsigned char *channelBlue, *channelGreen, *channelRed;


	int i, j, k;

	// -------------------------------------------------- Unpacking Meta Data --------------------------------------------------

	unsigned char * info = new unsigned char [12];
	int * metaData = new int [6];

	memcpy (info, compressedData, 12);

	int no_of_bits = 0;
	for (i = 0; i < 6; i++){
		no_of_bits += info [i * 2];
	}
	int no_of_bytes = ((no_of_bits - 1) >> 3) + 1;

	int index = 12;
	int counter = 8;

	for (i = 0; i < 6; i ++){
		if (info [i * 2] < counter){
			unsigned char mask = 1;
			for (j = 0; j < counter - 1; j++){
				mask <<= 1;
				mask += 1;
			}
			metaData [i] = (compressedData [index] & mask) >> (counter - info [i * 2]);
			counter -= info [i * 2];
		}
		else
		{
			unsigned char mask = 1;
			for (j = 0; j < counter - 1; j++){
				mask <<= 1;
				mask += 1;
			}
			metaData [i] = compressedData [index] & mask;
			info [i * 2] -= counter;
			metaData [i] <<= (info [i * 2]);
			counter = 8;
			index ++;
			while (info [i * 2] > 8){
				info [i * 2] -= 8;
				metaData [i] += (compressedData [index ++] << info [i * 2]);
			}
			counter = 8 - info [i * 2];
			metaData [i] += (compressedData [index] >> counter);

			if (counter == 0){
				counter = 8;
				index ++;
			}
		}
	}

	for (i = 0; i < 6; i ++){
		metaData [i] <<= info [i * 2 + 1];
	}

	delete [] info;

	predictorSizeB = metaData [0];
	predictorSizeG = metaData [1];
	predictorSizeR = metaData [2];
	compressedSizeB = metaData [3];
	compressedSizeG = metaData [4];
	compressedSizeR = metaData [5];

	delete [] metaData;

	// -------------------------------------------------- Unpacking Predictor --------------------------------------------------


	predictorBlue = new unsigned char [height];
	predictorGreen = new unsigned char [height];
	predictorRed = new unsigned char [height];

	unsigned char * loc = compressedData + 12 + no_of_bytes;

	memcpy (predictorBlue, loc, predictorSizeB);
	loc += predictorSizeB;

	memcpy (predictorGreen, loc, predictorSizeG);
	loc += predictorSizeG;

	memcpy (predictorRed, loc, predictorSizeR);
	loc += predictorSizeR;

	// -------------------------------------------------- Uncompressing Channels --------------------------------------------------


	compressedChannelBlue = new unsigned char [compressedSizeB];
	compressedChannelGreen = new unsigned char [compressedSizeG];
	compressedChannelRed = new unsigned char [compressedSizeR];

	memcpy (compressedChannelBlue, loc, compressedSizeB);
	loc += compressedSizeB;

	memcpy (compressedChannelGreen, loc, compressedSizeG);
	loc += compressedSizeG;

	memcpy (compressedChannelRed, loc, compressedSizeR);
	loc += compressedSizeR;

	differentialChannelBlue = new unsigned char [width * height];
	differentialChannelGreen = new unsigned char [width * height];
	differentialChannelRed = new unsigned char [width * height];

	DecompressChannel (compressedChannelBlue, differentialChannelBlue, compressedSizeB, width * height);
	DecompressChannel (compressedChannelGreen, differentialChannelGreen, compressedSizeG, width * height);
	DecompressChannel (compressedChannelRed, differentialChannelRed, compressedSizeR, width * height);

	// ofstream fout;
	// fout.open ("C:\\Users\\ArgensNg\\Desktop\\output.txt");
	// for (j = 0; j < height; j ++){
	// 	for (i = 0; i < width; i++){
	// 		if (differentialChannelRed[j * width + i] != original [j* width + i]){
	// 			fout << "( " << i << ", " << j << ") Original: " << (int) original[j* width + i] << " Outcome: " << (int) differentialChannelRed[j * width + i] << endl;
	// 		}
	// 	}
	// }
	
	// fout.close ();
	// fout.open ("C:\\Users\\ArgensNg\\Desktop\\output1.txt");

	// for (i = 0; i < 50; i++){
	// 	fout << i << ": " << (int) compressedChannelRed [i] << endl;
	// }
	// fout.close ();
	// delete [] original;

	delete [] compressedChannelBlue;
	delete [] compressedChannelGreen;
	delete [] compressedChannelRed;

	// -------------------------------------------------- Restoring Image --------------------------------------------------

	channelBlue = new unsigned char [width * height];
	channelGreen = new unsigned char [width * height];
	channelRed = new unsigned char [width * height];
	
	ChannelRestore (differentialChannelBlue, channelBlue, predictorBlue, width * height);
	ChannelRestore (differentialChannelGreen, channelGreen, predictorGreen, width * height);
	ChannelRestore (differentialChannelRed, channelRed, predictorRed, width * height);

	delete [] differentialChannelRed;
	delete [] differentialChannelBlue;
	delete [] differentialChannelGreen;
	delete [] predictorRed;
	delete [] predictorGreen;
	delete [] predictorBlue;


	for (i = 0; i < width * height; i++){
		uncompressedData [i * 3 + 0] = channelBlue [i];
		uncompressedData [i * 3 + 1] = channelGreen [i];
		uncompressedData [i * 3 + 2] = channelRed [i];
	}

	// -------------------------------------------------- Clear Memory --------------------------------------------------

	delete [] channelRed;
	delete [] channelGreen;
	delete [] channelBlue;
}

// --- End of Custom Code

void CAppCompress::Process(void) {

	// Don't change anything within this function.

	int i, cDataSize ;

	unsigned char *compressedData ;
	unsigned char *verifyCompressedData ;

	SetTitle(pOutput, _T("Lossless Decompressed Image")) ;

	compressedData = Compress(cDataSize) ;

	verifyCompressedData = new unsigned char [cDataSize] ;

	memcpy(verifyCompressedData, compressedData, cDataSize) ;

	delete [] compressedData ;

	Decompress(verifyCompressedData, cDataSize, pOutput) ;

	for(i = 0; i < width * height * 3; i++) {
		if(pInput[i] != pOutput[i]) {
			printf(_T("Caution: Decoded Image is not identical to the Original Image!\r\n")) ;
			break ;
		}
	}

	printf(_T("Original Size = %d, Compressed Size = %d, Compression Ratio = %2.2f\r\n"), width * height * 3, cDataSize, (double) width * height * 3 / cDataSize) ;

	PutDC(pOutput) ;
}
