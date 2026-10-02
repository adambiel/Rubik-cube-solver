#include "debug.h"
#include "cube.h"
#include "lehmer_code.h"
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cube state;

	vector<string> moves = {"U", "Up", "U2", "Dp", "D2", "D", "R2", "L2", "F2", "B2"};
	queue<pair<cube, int> > q;
	q.push(make_pair(state, 0));
	vector<int> distance(479001600, -1);
	while(!q.empty())
	{
		auto [akt, dist] = q.front();
		q.pop();
		vector<int> akt_ep(akt.ep.begin(), akt.ep.end());
		distance[lehmer_code(akt_ep)] = dist;
		for(auto& v : moves)
		{
			cube nw = akt;
			nw.move(v);
			vector<int> ep(nw.ep.begin(), nw.ep.end());
			if(distance[lehmer_code(ep)] == -1)
			{
				distance[lehmer_code(ep)] = dist+1;
				q.push(make_pair(nw, dist+1));
			}
		}
	}
	for(int i=0;i<479001600;i++)
		cout<<distance[i]<<"\n";
}

