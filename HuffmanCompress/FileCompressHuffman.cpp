#define _CRT_SECURE_NO_WARNINGS 1
#include "FileCompressHuffman.h"

void FileCompressHuffman::CompressFile(const std::string &strFilePath)
{
	_FilePath = strFilePath;
	//1. ��ȡԴ�ļ���ÿ���ַ����ֵĴ���
	FILE* fIn = fopen(strFilePath.c_str(), "rb");//ģʽrֻ�ܶ�ȡ�ı��ļ�
	if (nullptr == fIn)
	{
		std::cout << "���ļ�ʧ�ܣ�" << std::endl;
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

	//2. ��ÿ���ַ����ֵĴ���ΪȨֵ����Huffman��
	HuffmanTree<CharInfo> ht;
	ht.CreathuffmanTree(_charInfo, CharInfo(0));

	//3. ����Huffman����ȡÿ���ַ��ı���
	getHuffmanCode(ht.getRoot());

	//4. ����ÿ���ַ��ı������¸�дԴ�ļ�
	FILE* fOut = fopen("test.hcp", "wb");
	//��ѹ���ļ���ʼλ����д���ѹ������Ҫ����Ϣ��
	//�ļ���׺�� ÿ���ַ���Ȩֵ������)
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
		cout << "�޷�ʶ���ѹ���ļ���" << endl;
		return;
	}
	//1. �ṩѹ���ļ�
	FILE* fIn = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "��ѹ���ļ�ʧ�ܣ�" << endl;
		return;
	}

	//2. ��ȡ��ѹ����Ϣ
	char buff[1024];
	fgets(buff, 1024, fIn);
	sourseFileType = buff[1];//��׺
	fgets(buff, 1024, fIn);
	int num = atoi(buff);
	for (int i = 0; i < num; i++)
	{
		fgets(buff, 1024, fIn);
		_charInfo[buff[0]]._charCount = atoi(buff + 2);
	}

	//3. ��ԭHuffman��

	HuffmanTree<CharInfo> ht;
	ht.CreathuffmanTree(_charInfo, CharInfo(0));
	HuffmanTreeNode<CharInfo> * pRoot = ht.getRoot();

	//4. ��ԭ�ļ�
	FILE* fOut = fopen("1.txt", "w");
	if (nullptr == fOut)
	{
		cout << "�����ļ�ʧ��!" << endl;
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
			//�˴���ʱʹ��i+1 == rdSize�ж��ļ���β�����ļ�����1024�ֽڽ���������
			//��֪������ж����һ���ֽ�
			//�ţ���i+1==rdSize��feof()һ���ж�Ӧ�þͿ�����
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

	//��code��Ϣ���浽CharInfo.strCode��
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

//��ѹ�ļ�������Ϣ
//��ʽ��	.Դ�ļ���׺
//			�ַ�������
//			����n��Ϊ��ԭHuffman��������Ϣ
void FileCompressHuffman::writeCompressHeader(FILE * fOut)
{
	//��׺
	std::string strHeader = _FilePath.substr(_FilePath.rfind('.', _FilePath.size()));
	strHeader += '\n';
	//��ѹ�ļ�����Ȩֵ��Ϣ
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