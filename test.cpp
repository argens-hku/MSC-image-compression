#include <iostream>
#include <stdint.h>
using namespace std;

unsigned char Push (unsigned char original, unsigned char &counter, unsigned char *buffer, int &bufferUsed, unsigned char value, unsigned char length){
	
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

	 // if (value == 14) {cout << "bufferUsed: " << (int) bufferUsed << "; counter: " << (int) counter << "; len: " << (int)length<< endl;	}
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
		buffer [++bufferUsed]	= ((value << counter) & 0x00FF);
		// if (value == 14) cout <<" jlkhh: " << (int)buffer[bufferUsed] <<endl;
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
	

	cout << "bufferUsed: " << (int) bufferUsed << "; counter: " << (int) counter << endl;
		// if (value == 14) cout <<" jlkhh: " << (int)buffer[bufferUsed] <<endl;

	return original;

}

int main ()
{

	unsigned char counter;
	unsigned char *buffer, *compressedChannel;
	int bufferUsed, index, bufferIndex;

	buffer = new unsigned char [10];
	compressedChannel = new unsigned char [20];
	counter = 8;
	index = 0;
	bufferIndex = 0;

	compressedChannel [0] = 0;
	//0

	
	compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 81, 24);
	if (bufferUsed != -1){
		bufferIndex = 0;
		while (bufferUsed >= 0){
			compressedChannel [++ index] = buffer [bufferIndex ++];
			bufferUsed--;
		}
	}

	compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 6, 3);
	if (bufferUsed != -1){
		bufferIndex = 0;
		while (bufferUsed >= 0){
			compressedChannel [++ index] = buffer [bufferIndex ++];
			bufferUsed--;
		}
	}

	compressedChannel [index] = Push (compressedChannel [index], counter, buffer, bufferUsed, 4, 5);
	if (bufferUsed != -1){
		bufferIndex = 0;
		while (bufferUsed >= 0){
			compressedChannel [++ index] = buffer [bufferIndex ++];
			bufferUsed--;
		}
	}


	if (counter != 8){
		index ++;
	}
	for (int i = 0; i < index; i++){
		cout << (int) compressedChannel [i] << ", ";
	}

	cout << "END";

	delete [] compressedChannel;
	delete [] buffer;


	return 0;
}