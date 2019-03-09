#pragma once
#include <string>
#include <iostream>
#include "HuffmanTree.hpp"

using std::cout;
using std::endl;
using std::string;

struct CharInfo
{
	CharInfo(size_t charCount = 0)
		: _charCount(charCount)
	{}
	CharInfo operator+(const CharInfo &c)
	{
		return CharInfo(_charCount + c._charCount);
	}
	bool operator>(const CharInfo &c)
	{
		return _charCount > c._charCount;
	}
	bool operator!=(const CharInfo &c)const
	{
		return _charCount != c._charCount;
	}
	char _ch;
	size_t _charCount;
	std::string _strCode;
};

class FileCompressHuffman
{
public:
	void CompressFile(const std::string & strFilePath);
	void UnCompressFile(const std::string &strFilePath);

private:
	void getHuffmanCode(HuffmanTreeNode<CharInfo> *pRoot);
	void writeCompressHeader(FILE* fOut);
	int getLastByte();

private:
	std::vector<CharInfo> _charInfo;
	std::string _FilePath;
	std::string _FileName;
	std::string sourseFileType;
};