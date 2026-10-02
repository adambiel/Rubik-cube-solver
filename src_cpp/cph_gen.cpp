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
	vector<int> distance(40320, -1);
	while(!q.empty())
	{
		auto [akt, dist] = q.front();
		q.pop();
		vector<int> akt_cp(akt.cp.begin(), akt.cp.end());
		distance[lehmer_code(akt_cp)] = dist;
		for(auto& v : moves)
		{
			cube nw = akt;
			nw.move(v);
			vector<int> cp(nw.cp.begin(), nw.cp.end());
			if(distance[lehmer_code(cp)] == -1)
			{
				distance[lehmer_code(cp)] = dist+1;
				q.push(make_pair(nw, dist+1));
			}
		}
	}
	for(int i=0;i<40320;i++)
		cout<<distance[i]<<"\n";
}
