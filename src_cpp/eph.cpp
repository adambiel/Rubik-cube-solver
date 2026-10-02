#include <fstream>
#include <iostream>
#include "cube.h"
#include "eph.h"
#include "lehmer_code.h"
eph::eph()
{
	ifstream file("../data/eph.txt");
	cerr<<"Starting to load eph.txt"<<endl;
	if(!file) 
	{
		cerr<<"eph data not generated!\n";
		exit(0);
	}
	for(int i=0;i<479001600;i++)
	{
		if(i != 0 && i%100000000 == 0)
			cerr<<"Loaded next 100M entries"<<endl;
		file>>_eph[i];
	}
}
int eph::get_eph(cube& state)
{
	vector<int> ep(state.ep.begin(), state.ep.end());
	return _eph[lehmer_code(ep)];	
}
