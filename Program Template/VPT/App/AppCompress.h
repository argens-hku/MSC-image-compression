#pragma once
#include "../processing.h"

class CAppCompress :
	public CProcessing
{
public:
	// Add variables here

public:
	CAppCompress(void);
	~CAppCompress(void);
	// Add methods here

	unsigned char *Compress(int &cDataSize) ;
	void Decompress(unsigned char *compressedData, int cDataSize, unsigned char *deCompressedData) ;

public:
	void CustomInit(CView *pView) ;
	void Process(void) ;
	void CustomFinal(void) ;

private:
	unsigned char *DifferentialCoding (int &predictorSize, unsigned char *&predictor, int dataSize, unsigned char *&data);
	unsigned char Push (unsigned char original, unsigned char &counter, unsigned char *buffer, int &bufferUsed, unsigned char value, unsigned char length);
	unsigned char *CompressChannel (unsigned char *channel, int size, int& compressedSize);
	void DecompressChannel (unsigned char *compressedChannel, unsigned char *channel, int compressedSize, int size);
	void MetaDataSizeInfo(int *metaData, unsigned char * info);
	void ChannelRestore (unsigned char *compressedChannel, unsigned char *channel, unsigned char *predictor, int size);
};
