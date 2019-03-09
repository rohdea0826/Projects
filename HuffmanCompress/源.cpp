#include "FileCompressHuffman.h"

int main()
{
	FileCompressHuffman f;
	f.CompressFile("test.txt");
	f.UnCompressFile("test.hcp");
	return 0;
}