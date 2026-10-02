#include "lehmer_code.h"
#include "cube.h"
#include <bits/stdc++.h>
constexpr int fact[12] = {
    1, 1, 2, 6, 24, 120, 720, 5040,
    40320, 362880, 3628800, 39916800
};
int lehmer_code(vector<int>& perm) 
{
	const unsigned int sz = perm.size();
	int used = 0;
	int res = 0;
	for(int i=0;i<sz;i++)
	{
		int p = perm[i];
		int smaller = p - __builtin_popcount(used & ((1<<p)-1));
		res += smaller * fact[sz-1-i];
		used |= (1<<p);
	}
	return res;
}
