#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "cube.h"

using namespace std;

int main() 
{
    cube c;
    
    vector<string> moves = {"R", "U", "F", "L", "B", "D"};
    
    cout << "Generating cube state after each move: ";
    for(const auto& m : moves) {
        cout << m << " ";
        c.move(m);
    }
    cout << "\n\n";
    
    c.print();

    return 0;
}
