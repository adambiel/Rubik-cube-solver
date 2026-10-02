#include <fstream>
#include <iostream>
#include "cube.h"
#include "eoh.h"
eoh::eoh()
{
	cerr<<"Starting to load eoh.txt"<<endl;
	ifstream data("../data/eoh.txt");
	if(!data)
	{
		cerr<<"eoh data not generated!\n";
		return;
	}
	for(int i=0;i<4096;i++) data>>_eoh[i];
}
int eoh::get_eoh(cube& state)
{	
	auto to_int = [&](cube& obj)
	{
		int res = 0;
		for(int i=0;i<obj.eo.size();i++)
			res += obj.eo[i]*(1<<i);
		return res;
	};
	return _eoh[to_int(state)];
}
