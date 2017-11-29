#include "hoff.h"

using namespace std;

void tree_clear(node*& root_ptr)
{
	if (root_ptr->leftlink == NULL && root_ptr->rightlink == NULL)
	{
		delete root_ptr;
		return;
	}

	if (root_ptr->leftlink != NULL)
	{
		tree_clear(root_ptr->leftlink);
	}

	//code = "";

	if (root_ptr->rightlink != NULL)
	{
		tree_clear(root_ptr->rightlink);
	}
}

void MemFree2D(unsigned char **Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++)
	{
		delete[] Mem[n];
	}
	delete[] Mem;
}

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

string table[255];

int twoConvertTen(int num)
{
	int result = 0, mul = 1;
	while (num > 0)
	{
		if (num % 2)
			result += mul;

		mul *= 2;
		num /= 10;
	}

	return result;
}

static int getAbit(unsigned char x, int n)
{
	return (x & (1 << n)) >> n;
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

bool Compare(node a, node b)
{
	return a.frequency > b.frequency;
}

void codeReturn(node* root_ptr, string code)
{
	string previous_code = code;
	//cout << root_ptr << endl;
	if (root_ptr->leftlink == NULL && root_ptr->rightlink == NULL)
	{
		
		if (code == "")
			code = "0";

		table[root_ptr->pixel] = code;
		return;
	}

	if (root_ptr->leftlink != NULL)
	{
		code += "0";
		codeReturn(root_ptr->leftlink, code);
	}

	//code = "";

	if (root_ptr->rightlink != NULL)
	{
		code = previous_code + "1";
		codeReturn(root_ptr->rightlink, code);
	}
}

void Encoding()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	list<node> li;
	list<node>::iterator it;

	int height = 256;
	int width = 256;

	unsigned char **input_img;

	input_img = MemAlloc2D(height, width, 0);

	int count[255] = { 0 };
	bool listInputCompare[255] = { false };


	FILE *infile;

	fopen_s(&infile, "lena256.raw", "rb");

	for (int h = 0; h < height; h++)
	{
		fread(input_img[h], sizeof(unsigned char), width, infile);
	}
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			count[input_img[i][j]]++;
		}
	}
	

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			count[input_img[h][w]]++;
		}
	}

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			if (count[input_img[h][w]] != 0 && listInputCompare[input_img[h][w]] == false)
			{
				li.push_back(node(input_img[h][w], count[input_img[h][w]]));
				listInputCompare[input_img[h][w]] = true;
			}
		}
	}

	li.sort(Compare);

	node *tree = (node*)(malloc(height * width));
	int treeindex = 0;

	//////////tree배열에 list값 넣기
	for (it = li.begin(); it != li.end(); it++)
	{
		tree[treeindex].pixel = it->pixel;
		tree[treeindex].frequency = it->frequency;
		tree[treeindex].leftlink = NULL;
		tree[treeindex].rightlink = NULL;
		treeindex++;
	}

	/*for (int i = 0; i < li.size(); i++)
	{
		cout << i+1 << "번째 노드의 픽셀값 : " << tree[i].pixel << " " <<"빈도수 : "<< tree[i].frequency << endl;
	}*/

	node *newNode; 
	node *cursor;

	//////////부모 노드 생성
	for (treeindex = li.size(); treeindex > 1; treeindex--)
	{
		
		if (treeindex == li.size())
		{
			newNode = new node(0, 0);
			cursor = newNode;

			if (tree[treeindex - 1].frequency > tree[treeindex - 2].frequency)
			{
				newNode->leftlink = &tree[treeindex - 1];
				newNode->rightlink = &tree[treeindex - 2];
				newNode->pixel = -100;
				newNode->frequency = tree[treeindex - 1].frequency + tree[treeindex - 2].frequency;
			}

			else
			{
				newNode->leftlink = &tree[treeindex - 2];
				newNode->rightlink = &tree[treeindex - 1];
				newNode->pixel = -100;
				newNode->frequency = tree[treeindex - 1].frequency + tree[treeindex - 2].frequency;
			}
		}

		else
		{
			newNode = new node(0, 0);
			
			if (cursor->frequency > tree[treeindex - 2].frequency)
			{
				newNode->leftlink = cursor;
				newNode->rightlink = &tree[treeindex - 2];
				newNode->pixel = -100;
				newNode->frequency = cursor->frequency + tree[treeindex - 2].frequency;
			}

			else
			{
				newNode->leftlink = &tree[treeindex - 2];
				newNode->rightlink = cursor;
				newNode->pixel = -100;
				newNode->frequency = cursor->frequency + tree[treeindex - 2].frequency;
			}
			
			cursor = newNode;
		}

		/*
		cout << treeindex << " ::::::" << endl;
		cout << "pixel , frequncy:" << newNode->pixel << " " << newNode->frequency << endl;
		cout << "left:" << newNode->leftlink << " " << newNode->leftlink->pixel << " " << newNode->leftlink->frequency << endl;
		cout << "right:" << newNode->rightlink << " " << newNode->rightlink->pixel << " " << newNode->rightlink->frequency << endl;
		cout << "-----------------------------------------------------------" << endl;
		*/
	}

	//cout << newNode->pixel << " " << newNode->frequency << endl;

	node *root_ptr;

	root_ptr = cursor;


	string code = "";

	codeReturn(root_ptr, code);

	////////// 테이블에 코드 넣기

	int codecount = 0;

	for (int i = 0; i < 255; i++)
	{
		if (table[i] != "")
		{
			//cout << i << "     " << table[i] << endl;
			codecount++;
		}
	}

	cout << endl;

	FILE *outfile;
	fopen_s(&outfile, "comp.txt", "wb");

	unsigned short intcode;
	unsigned char intcodesize;
	

	////////////////////헤더 쓰기
	fwrite(&height, sizeof(int), 1, outfile);
	fwrite(&width, sizeof(int), 1, outfile);
	fwrite(&codecount, sizeof(int), 1, outfile);

	///////////////////역시 헤더
	for (int i = 0; i < 255; i++)
	{
		if (table[i] != "")
		{
			intcodesize = table[i].size();
			intcode = twoConvertTen(stoi(table[i]));

			fwrite(&i, sizeof(unsigned char), 1, outfile);
			fwrite(&intcodesize, sizeof(unsigned char), 1, outfile);
			fwrite(&intcode, sizeof(unsigned short), 1, outfile);
			//fwrite(&table[i], sizeof(string), 1, outfile);
		}
	}

	vector<unsigned char> codevalue;

	////////////벡터에 코드값들 넣기
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			for (int i = 0; i < table[input_img[h][w]].length(); i++)
			{
				codevalue.push_back(table[input_img[h][w]][i]);
			}
		}
	}

	int codesize = codevalue.size();
	fwrite(&codesize, sizeof(int), 1, outfile);

	/////////////////////각 픽셀에 대응하는 코드 값 넣기
	unsigned char *write_ptr = (unsigned char*)malloc(sizeof(unsigned char)*codesize);
	//unsigned char* write_ptr_rem = write_ptr;
	memset(write_ptr, 0, codesize);

	int bytesize;

	if (codesize % 8 == 0)
		bytesize = codesize / 8;
	else
		bytesize = codesize / 8 + 1;

	//cout << codesize << endl;
	//cout << write_ptr << endl;

	for (int i = 0; i < codesize;)
	{
		unsigned char item = 0;
		if (i == 0)
		{
			for (int j = 0; j < 8; j++)
			{
				if (j == 0)
				{
					*write_ptr = getAbit(codevalue[i], 0);
				}

				else
				{
					item = getAbit(codevalue[i], 0);

					*write_ptr = (*write_ptr << 1) | item;
				}
				i++;
			}
		}

		else
		{
			for (int j = 0; j < 8; j++)
			{
				item = getAbit(codevalue[i], 0);

				*write_ptr = *write_ptr << 1 | item;

				i++;

				//cout << i << endl;

				if (i == codesize)
				{
					if (i % 8 == 0) 
					{
						//*write_ptr = *write_ptr << 1 | item;
						
						break;
					}

					else 
					{
						*write_ptr = *write_ptr << (bytesize * 8 - codesize);
						break;
					}
				}
			}
		}
		
		fwrite(write_ptr, sizeof(unsigned char), 1, outfile);
		
	}

	/*
	FILE *in2out;
	fopen_s(&in2out, "in.raw", "wb");

	for (int h = 0; h < 40; h++)
	{
		fwrite(input_img[h], sizeof(unsigned char), 50, in2out);
	}
	*/
	
	delete write_ptr;
	//delete root_ptr;
	//delete cursor;
	delete[] tree;
	delete newNode;
	//tree_clear(root_ptr);


	MemFree2D(input_img, height);

	fclose(outfile);
	//fclose(in2out);
}
