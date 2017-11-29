#include "hoffDec.h"

void MemFreeDEC(unsigned char **Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++)
	{
		delete[] Mem[n];
	}
	delete[] Mem;
}

unsigned char** MemAllocDEC(int nHeight, int nWidth, unsigned char nInitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

int tenConvertTwo(int num)
{
	int result = 0;

	for (int i = 1; num > 0; i *= 10)
	{
		int binary = num % 2;
		result += binary*i;
		num /= 2;
	}

	return result;
}

static char *ucharToBinary(unsigned char *i)
{
	static char s[8 + 1] = { '0', };
	int count = 8;

	do {
		s[--count] = '0' + (char)(*i & 1);
		*i = *i >> 1;
	} while (count);

	return s;
}

static int getAbit(unsigned char x, int n)
{
	return (x & (1 << n)) >> n;
}

void Decoding()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int *sizeHeight = new int[1];		//10
	int *sizeWidth = new int[1];		//10
	int *codecount = new int[1];		//4
	int *codesize = new int[1];			//190
	int bytesize;

	string table[255];

	FILE * infile;

	fopen_s(&infile, "comp.txt", "rb");

	fread(sizeHeight, sizeof(int), 1, infile);
	fread(sizeWidth, sizeof(int), 1, infile);
	fread(codecount, sizeof(int), 1, infile);

	unsigned char *tablepixel = new unsigned char[*codecount];
	unsigned char *intcodesize = new unsigned char[*codecount];
	unsigned short *intcode = new unsigned short[*codecount];
	//string *tablecode = new string[*codecount];

	for (int i = 0; i < *codecount; i++)
	{
		fread(&tablepixel[i], sizeof(unsigned char), 1, infile);
		fread(&intcodesize[i], sizeof(unsigned char), 1, infile);
		fread(&intcode[i], sizeof(unsigned short), 1, infile);
		//fread(&tablecode[i], sizeof(string), 1, infile);
	}


	//////////////////////테이블만들기 시작
	for (int i = 0; i < *codecount; i++)
	{
		table[tablepixel[i]] = to_string(tenConvertTwo(intcode[i]));
	}

	string zeroadd = "";

	for (int i = 0; i < *codecount; i++)
	{
		zeroadd = "";
		if (table[tablepixel[i]].size() != intcodesize[i])
		{
			for (int j = 0; j < intcodesize[i] - table[tablepixel[i]].size(); j++)
			{
				zeroadd += "0";
			}
			table[tablepixel[i]] = zeroadd + table[tablepixel[i]];
		}
	}

	fread(codesize, sizeof(int), 1, infile);

	if (*codesize % 8 == 0)
		bytesize = *codesize / 8;
	else
		bytesize = *codesize / 8 + 1;
	
	unsigned char *code = new unsigned char[bytesize];

	fread(code, sizeof(unsigned char), bytesize, infile);

	vector<unsigned char> codevalue;

	unsigned char result;
	
	for (int i = 0; i < bytesize; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			result = getAbit((code[i] >> (7 - j)), 0);

			codevalue.push_back(result);
		}
	}

	unsigned char **output_img;

	output_img = MemAllocDEC(*sizeHeight, *sizeWidth, 0);

	FILE *outDec;
	fopen_s(&outDec, "output.raw", "wb");

	string codestring = "";
	bool writeOrnot = false;
	
	int i = 0;

	for (int h = 0; h < *sizeHeight; h++)
	{
		for (int w = 0; w < *sizeWidth; w++)
		{
			for (; i < *codesize; i++)
			{
				writeOrnot = false;

				if (codevalue[i] == 0)
					codestring += "0";
				else
					codestring += "1";

				//codestring += codevalue[i]; 

				for (int j = 0; j < *codecount; j++)
				{
					if (codestring == table[tablepixel[j]])
					{
						codestring = "";
						output_img[h][w] = tablepixel[j];
						writeOrnot = true;
						i++;
						break;
					}
				}
				if (writeOrnot)
					break;
			}
			//cout << (int)output_img[h][w];
		}
		//cout << endl;
	}

	for (int h = 0; h < *sizeHeight; h++)
	{
		fwrite(output_img[h], sizeof(unsigned char), *sizeWidth, outDec);
	}

	MemFreeDEC(output_img, *sizeHeight);
	fclose(infile);
	fclose(outDec);
	
	delete[] sizeHeight;
	delete[] sizeWidth;
	delete[] codecount;
	delete[] codesize;
	delete[] tablepixel;
	delete[] intcodesize;
	delete[] intcode;
	delete[] code;
	
	cout << "asdasd" << endl;
}

