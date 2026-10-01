#include "cube.h"
#include "cph.h"
#include "lehmer_code.h"

cph::cph()
{
    cerr << "Starting to load cph.txt" << endl;    
    ifstream file("../data/cph.txt");
    if(!file) 
    {
        cerr << "cph data not generated!\n";
        exit(0);
    }
    for(int i = 0; i < 40320; i++)
        file >> _cph[i];
}

int cph::get_cph(cube& state)
{
    vector<int> cp(state.cp.begin(), state.cp.end());
    return _cph[lehmer_code(cp)];    
}
