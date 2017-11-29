#pragma once

#include <iostream>
#include <crtdbg.h>
#include <list>
#include <string>
#include <bitset>
#include <vector>
#include <crtdbg.h>

#ifndef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)
#endif

using namespace std;

void Encoding();
void Decoding();

class node
{

public:
	int pixel;
	int frequency;
	//string code;

	node *leftlink = NULL;
	node *rightlink = NULL;

	node(int pixel, int frequency)
	{
		this->pixel = pixel;
		this->frequency = frequency;
	}
};

