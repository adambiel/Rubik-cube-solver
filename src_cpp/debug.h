// modul udostepniajacy makro debug(X) 
// pozwalajacy wypisac na wyjscie diagnostyczne dowolna liczbe obiektow ponizej postaci:
// zmienna / kontener iterowalny / para zlozona z dowolnej kombinacji dwoch powyzszych
#include<bits/stdc++.h>
using namespace std;
#ifdef DEBUG
auto&operator <<(auto& o, pair<auto, auto> p) {return o<<"{"<<p.fi<<", "<<p.se<<"}";}
auto operator <<(auto& o, auto x)->decltype(x.end(), o) {o<<"{"; for(auto& v : x) o<<v<<", "; return o<<"}";}
#define debug(X...) cerr<<"["#X"]: ", [](auto...$){((cerr<<$<<"; "),...)<<endl;}(X)
#else
#define debug(X...) {}
#endif
