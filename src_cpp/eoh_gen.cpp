#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include "debug.h"
#include "cube.h"
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cube solved;
	queue<pair<cube, int> > q;
	q.push(make_pair(solved, 0));
	auto to_int = [&](cube& obj)
	{
		int res = 0;
		for(int i=0;i<obj.eo.size();i++)
			res += obj.eo[i]*(1<<i);
		return res;
	};
	const int all_states = 4096;
	const int inf = 1e9;
	vector<int> opt(all_states, inf);
	map<array<uint8_t, 12>, int> visited;
	int cnt = 0;
	vector<string> moves = {"U", "D", "R", "L", "F", "B"};
	while(!q.empty())
	{
		auto [akt, dist] = q.front();
		q.pop();
		if(opt[to_int(akt)] == inf)
			cnt++;
		opt[to_int(akt)] = min(dist, opt[to_int(akt)]);
		if(cnt == all_states/2) break;
		visited[akt.ep] = 1;
		for(auto& v : moves)
		{
			cube nxt = akt;
			nxt.move(v);
			if(visited[nxt.ep] == 0)
				q.push(make_pair(nxt, dist+1));
		}
	}
	for(int i=0;i<all_states;i++)
		cout<<opt[i]<<"\n";
}
