#pragma once
#include<string>
#include<array>
#include<cstdint>
using namespace std;
class cube
{
public:
	array<uint8_t, 8> cp; //permutacja rogow
	array<uint8_t, 8> co; //orientacja rogow
	array<uint8_t, 12> ep; //permutacja krawedzi
	array<uint8_t, 12> eo; //orientacja krawedzi
	//kostka jest reprezentowana wzgledem orientacji: niebieski center z przodu, biały center u góry, pomarańczowy center z prawej
	cube();
	
	void R();
	void L();
	void U();
	void D();
	void B();
	void F();
	
	bool read();
	void print();
	void move(string id);
};
