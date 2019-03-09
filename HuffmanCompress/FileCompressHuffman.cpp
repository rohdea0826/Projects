#define _CRT_SECURE_NO_WARNINGS 1
#include "FileCompressHuffman.h"

void FileCompressHuffman::CompressFile(const std::string &strFilePath)
{
	_FilePath = strFilePath;
	//1. 获取源文件中每个字符出现的次数
	FILE* fIn = fopen(strFilePath.c_str(), "rb");//模式r只能读取文本文件
	if (nullptr == fIn)
	{
		std::cout << "打开文件失败！" << std::endl;
		return;
	}
	unsigned char *pReadBuff = new unsigned char[1024];
	_charInfo.resize(256);
	for (size_t i = 0; i < 256; i++)
	{
		_charInfo[i]._ch = (char)i;
	}
	while (1)
	{
		size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		if (rdSize == 0) break;
		for (size_t i = 0; i < rdSize; ++i)
		{
			++_charInfo[pReadBuff[i]]._charCount;
		}
	}

	//2. 以每个字符出现的次数为权值创建Huffman树
	HuffmanTree<CharInfo> ht;
	ht.CreathuffmanTree(_charInfo, CharInfo(0));

	//3. 根据Huffman树获取每个字符的编码
	getHuffmanCode(ht.getRoot());

	//4. 根据每个字符的编码重新改写源文件
	FILE* fOut = fopen("test.hcp", "wb");
	//在压缩文件开始位置先写入解压缩所需要的信息：
	//文件后缀， 每种字符的权值（次数)
	writeCompressHeader(fOut);

	fseek(fIn, 0, SEEK_SET);
	while (1)
	{
		unsigned char rdBuff[1024] = { 0 };
		size_t rdSize = fread(rdBuff, 1, 1024, fIn);
		if (rdSize == 0) break;
		int bitCount = 7;
		char buff = 0;
		for (size_t i = 0; i < rdSize; i++)
		{
			std::string code = _charInfo[rdBuff[i]]._strCode;

			for (size_t i = 0; i < code.size(); i++)
			{
				//TODO:
				if(code[i] != '0') 
				buff = buff |(1 << bitCount);
				if (--bitCount < 0)
				{
					putc(buff, fOut);
					buff = 0;
					bitCount = 7;
				}
			}
		}
		if (bitCount != 7) putc(buff, fOut);
	}
	fclose(fIn);
	fclose(fOut);
}

void FileCompressHuffman::UnCompressFile(const std::string & strFilePath)
{
	_FilePath = strFilePath;
	
	if (_FilePath.substr(_FilePath.rfind('.', _FilePath.size())) != ".hcp")
	{
		cout << "无法识别的压缩文件！" << endl;
		return;
	}
	//1. 提供压缩文件
	FILE* fIn = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "打开压缩文件失败！" << endl;
		return;
	}

	//2. 获取解压缩信息
	char buff[1024];
	fgets(buff, 1024, fIn);
	sourseFileType = buff[1];//后缀
	fgets(buff, 1024, fIn);
	int num = atoi(buff);
	for (int i = 0; i < num; i++)
	{
		fgets(buff, 1024, fIn);
		_charInfo[buff[0]]._charCount = atoi(buff + 2);
	}

	//3. 还原Huffman树

	HuffmanTree<CharInfo> ht;
	ht.CreathuffmanTree(_charInfo, CharInfo(0));
	HuffmanTreeNode<CharInfo> * pRoot = ht.getRoot();

	//4. 还原文件
	FILE* fOut = fopen("1.txt", "w");
	if (nullptr == fOut)
	{
		cout << "创建文件失败!" << endl;
	}
	int LastByte = getLastByte();
	while (1)
	{
		size_t rdSize = fread(buff, 1, 1024, fIn);
		if (rdSize <= 0) break;
		int bitCount;
		HuffmanTreeNode<CharInfo> *oldpRoot = pRoot;
		
		for (size_t i = 0; i < rdSize; i++)
		{
			bitCount = 7;
			int count = 0;
			int flag = 0;
			//此处暂时使用i+1 == rdSize判断文件结尾，若文件大于1024字节将出现问题
			//不知该如何判断最后一个字节
			//嗯，用i+1==rdSize和feof()一起判断应该就可以了
			if (i + 1 >= rdSize && feof(fIn))
			{
				count = 8 - getLastByte();
				flag = 1;
			}
		
			while (bitCount >= count)
			{
				if ((buff[i] & (1 << bitCount)) == 0)
				{
					if (pRoot->_pLeft)
						pRoot = pRoot->_pLeft;
				}
				else
				{
					if (pRoot->_pRight) pRoot = pRoot->_pRight;
				}
				if (pRoot->_pLeft == nullptr && pRoot->_pRight == nullptr)
				{
					putc(pRoot->_weight._ch, fOut);
					pRoot = oldpRoot;
				}
				--bitCount;
			}
			if (flag) break;
			
		}
	}
	fclose(fIn);
	fclose(fOut);
}

void FileCompressHuffman::getHuffmanCode(HuffmanTreeNode<CharInfo> *pRoot)
{
	if (nullptr == pRoot) return;

	getHuffmanCode(pRoot->_pLeft);
	getHuffmanCode(pRoot->_pRight);

	//将code信息保存到CharInfo.strCode中
	std::string strCode;
	HuffmanTreeNode<CharInfo> *pCur = pRoot;
	while (pCur)
	{
		HuffmanTreeNode<CharInfo> *pParent = pCur->_pParent;
		if (nullptr == pParent) break;
		if (pCur == pParent->_pLeft)
			strCode += "0";
		else
			strCode += "1";
		pCur = pCur->_pParent;
	}
	if (nullptr == pRoot->_pLeft && nullptr == pRoot->_pRight)
	{
		reverse(strCode.begin(), strCode.end());
		_charInfo[pRoot->_weight._ch]._strCode = strCode;
	}

	return;
}

//解压文件所需信息
//格式：	.源文件后缀
//			字符种类数
//			后面n行为还原Huffman树所需信息
void FileCompressHuffman::writeCompressHeader(FILE * fOut)
{
	//后缀
	std::string strHeader = _FilePath.substr(_FilePath.rfind('.', _FilePath.size()));
	strHeader += '\n';
	//解压文件所需权值信息
	size_t num = 0;
	for (size_t i = 0; i < 256; i++)
	{
		if (_charInfo[i]._charCount != 0) num++;
	}
	char buff[1024] = { 0 };
	strHeader += _itoa(num, buff, 10);
	strHeader += '\n';
	for (size_t i = 0; i < 256; i++)
	{
		if (_charInfo[i]._charCount != 0)
		{
			strHeader += _charInfo[i]._ch;
			strHeader += ',';
			strHeader += _itoa(_charInfo[i]._charCount, buff, 10);
			strHeader += '\n';
		}
	}
	fwrite(strHeader.c_str(), 1, strHeader.size(), fOut);
}

int FileCompressHuffman::getLastByte()
{
	int sum = 0;
	for (size_t i = 0; i < 256; i++)
	{
		if (_charInfo[i]._charCount != 0)
		{
			sum += (_charInfo[i]._charCount * _charInfo[i]._strCode.size());
		}
	}
	return sum % 8;
}