/** @file cube.cpp
 *  @brief Implementation of the cube class.
 *  @details Contains methods for manipulating the cube state (rotations).
 */
#include <numeric>
#include <functional>
#include <iostream>
#include <vector>
#include "cube.h"
#include "debug.h"
using namespace std;
cube::cube()
{
    iota(cp.begin(), cp.end(), 0);
    iota(ep.begin(), ep.end(), 0);
    for(auto &v : co) v = 0;
    for(auto &v : eo) v = 0;
}
void cube::U() {
    int swp = cp[3];
    cp[3] = cp[2];
    cp[2] = cp[1];
    cp[1] = cp[0];
    cp[0] = swp;

    swp = ep[3];
    ep[3] = ep[2];
    ep[2] = ep[1];
    ep[1] = ep[0];
    ep[0] = swp;
}

void cube::D() {
    int swp = cp[7];
    cp[7] = cp[4];
    cp[4] = cp[5];
    cp[5] = cp[6];
    cp[6] = swp;

    swp = ep[11];
    ep[11] = ep[8];
    ep[8] = ep[9];
    ep[9] = ep[10];
    ep[10] = swp;

}

void cube::R() {
    co[cp[2]] = (co[cp[2]]+2)%3;
    co[cp[3]] = (co[cp[3]]+1)%3;
    co[cp[6]] = (co[cp[6]]+1)%3;
    co[cp[7]] = (co[cp[7]]+2)%3;

    int swp = cp[3];
    cp[3] = cp[7];
    cp[7] = cp[6];
    cp[6] = cp[2];
    cp[2] = swp;

    swp = ep[3];
    ep[3] = ep[7];
    ep[7] = ep[11];
    ep[11] = ep[6];
    ep[6] = swp;

}

void cube::L() {
    co[cp[0]] = (co[cp[0]]+2)%3;
    co[cp[1]] = (co[cp[1]]+1)%3;
    co[cp[4]] = (co[cp[4]]+1)%3;
    co[cp[5]] = (co[cp[5]]+2)%3;

    int swp = cp[0];
    cp[0] = cp[1];
    cp[1] = cp[5];
    cp[5] = cp[4];
    cp[4] = swp;

    swp = ep[1];
    ep[1] = ep[5];
    ep[5] = ep[9];
    ep[9] = ep[4];
    ep[4] = swp;

}

void cube::F() {
    co[cp[0]] = (co[cp[0]]+1)%3;
    co[cp[3]] = (co[cp[3]]+2)%3;
    co[cp[4]] = (co[cp[4]]+2)%3;
    co[cp[7]] = (co[cp[7]]+1)%3;

    eo[ep[0]] ^= 1;
    eo[ep[8]] ^= 1;
    eo[ep[4]] ^= 1;
    eo[ep[7]] ^= 1;

    int swp = cp[0];
    cp[0] = cp[4];
    cp[4] = cp[7];
    cp[7] = cp[3];
    cp[3] = swp;

    swp = ep[0];
    ep[0] = ep[4];
    ep[4] = ep[8];
    ep[8] = ep[7];
    ep[7] = swp;
}

void cube::B() {
    co[cp[1]] = (co[cp[1]]+2)%3;
    co[cp[2]] = (co[cp[2]]+1)%3;
    co[cp[5]] = (co[cp[5]]+1)%3;
    co[cp[6]] = (co[cp[6]]+2)%3;

    eo[ep[2]] ^= 1;
    eo[ep[6]] ^= 1;
    eo[ep[10]] ^= 1;
    eo[ep[5]] ^= 1;

    int swp = cp[1];
    cp[1] = cp[2];
    cp[2] = cp[6];
    cp[6] = cp[5];
    cp[5] = swp;

    swp = ep[2];
    ep[2] = ep[6];
    ep[6] = ep[10];
    ep[10] = ep[5];
    ep[5] = swp;

}
void cube::print()
{
    vector<pair<char, char> > edges = {
    {'W', 'B'}, {'W', 'R'}, {'W', 'G'}, {'W', 'O'},
    {'B', 'R'}, {'G', 'R'}, {'G', 'O'}, {'B', 'O'},
    {'Y', 'B'}, {'Y', 'R'}, {'Y', 'G'}, {'Y', 'O'}};

    vector<vector<char> > corners = {
    {'W', 'R', 'B'}, {'W', 'G', 'R'}, {'W', 'O', 'G'}, {'W', 'B', 'O'},
    {'Y', 'B', 'R'}, {'Y', 'R', 'G'}, {'Y', 'G', 'O'}, {'Y', 'O', 'B'}};

    auto p_cor = [&](int num, int face)
    {
        cout<<corners[cp[num]][(co[cp[num]]+face)%3]<<" ";
    };
    auto p_edg = [&](int num, int flip)
    {
        flip ^= eo[ep[num]];
        if(!flip)
            cout<<edges[ep[num]].first<<" ";
        else    
            cout<<edges[ep[num]].second<<" ";
    };
    // Upper face:
    cout<<"U: ";
    p_cor(3, 0); p_edg(0, 0); p_cor(0, 0); cout<<endl;
    p_edg(3, 0); cout<<"W "; p_edg(1, 0); cout<<endl;    
    p_cor(2, 0); p_edg(2, 0);  p_cor(1, 0); cout<<endl;

    // Bottom face:
    cout<<"D: ";
    p_cor(4, 0); p_edg(8, 0); p_cor(7, 0); cout<<endl;
    p_edg(9, 0); cout<<"Y "; p_edg(11, 0); cout<<endl;
    p_cor(5, 0); p_edg(10, 0); p_cor(6, 0); cout<<endl;

    // Front face:
    cout<<"F: ";
    p_cor(0, 2); p_edg(0, 1); p_cor(3, 1); cout<<endl;
    p_edg(4, 0); cout<<"B "; p_edg(7, 0); cout<<endl;
    p_cor(4, 1); p_edg(8, 1); p_cor(7, 2); cout<<endl;

    // Back face:
    cout<<"B: ";    
    p_cor(2, 2); p_edg(2, 1);  p_cor(1, 1); cout<<endl;
    p_edg(6, 0); cout<<"G ";   p_edg(5, 0); cout<<endl;
    p_cor(6, 1); p_edg(10, 1); p_cor(5, 2); cout<<endl;

    // Left face:
    cout<<"L: ";
    p_cor(1, 2); p_edg(1, 1); p_cor(0, 1); cout<<endl;
    p_edg(5, 1); cout<<"R "; p_edg(4, 1); cout<<endl;
    p_cor(5, 1); p_edg(9, 1); p_cor(4, 2); cout<<endl;

    // Right face:
    cout<<"R: ";
    p_cor(3, 2); p_edg(3, 1); p_cor(2, 1); cout<<endl;
    p_edg(7, 1); cout<<"O "; p_edg(6, 1); cout<<endl;
    p_cor(7, 1); p_edg(11, 1); p_cor(6, 2); cout<<endl;

    // Exact face orientation will depend on the Python visualizer specification
}
bool cube::read()
{
    vector<pair<char, char> > edges = {
        {'W', 'B'}, {'W', 'R'}, {'W', 'G'}, {'W', 'O'},
        {'B', 'R'}, {'G', 'R'}, {'G', 'O'}, {'B', 'O'},
        {'Y', 'B'}, {'Y', 'R'}, {'Y', 'G'}, {'Y', 'O'}};

    vector<vector<char> > corners = {
        {'W', 'R', 'B'}, {'W', 'G', 'R'}, {'W', 'O', 'G'}, {'W', 'B', 'O'},
        {'Y', 'B', 'R'}, {'Y', 'R', 'G'}, {'Y', 'G', 'O'}, {'Y', 'O', 'B'}};

    // Read input line by line, ignore potential "U:", "D:" etc. prefix
    vector<char> letters;
    string line;
    const string valid = "WBRGOY";
    while (getline(cin, line)) {
        size_t start = 0;
        // if the line starts with 'X:' (e.g. "U: "), skip these two characters
        if (line.size() >= 2 && isalpha((unsigned char)line[0]) && line[1] == ':')
            start = 2;
        // collect only characters that are valid colors
        for (size_t i = start; i < line.size(); ++i) {
            char c = line[i];
            if (valid.find(c) != string::npos) letters.push_back(c);
        }
    }
    debug(letters); 
    if (letters.size() < 54) {
        // invalid/incomplete input -> do not change the state
        return false;
    }

    // structures to store "read" colors in the order corresponding to print()
    array<array<char,3>,8> printedCorner;
    for (auto &arr : printedCorner) arr = {0,0,0};
    vector<array<pair<int,char>,2>> printedEdge(12);
    for (int i=0;i<12;i++) { printedEdge[i][0] = {-1,0}; printedEdge[i][1] = {-1,0}; }

    size_t idx = 0;
    auto take = [&](void)->char { return letters[idx++]; };

    auto pushEdge = [&](int pos, int flipParam, char ch){
        if (printedEdge[pos][0].first == -1) printedEdge[pos][0] = {flipParam, ch};
        else printedEdge[pos][1] = {flipParam, ch};
    };

    // Reconstruction of the order according to print()
    // U:
    printedCorner[3][0] = take(); pushEdge(0,0,take()); printedCorner[0][0] = take();
    pushEdge(3,0,take()); take(); /*center U*/ pushEdge(1,0,take());
    printedCorner[2][0] = take(); pushEdge(2,0,take()); printedCorner[1][0] = take();

    // D:
    printedCorner[4][0] = take(); pushEdge(8,0,take()); printedCorner[7][0] = take();
    pushEdge(9,0,take()); take(); /*center D*/ pushEdge(11,0,take());
    printedCorner[5][0] = take(); pushEdge(10,0,take()); printedCorner[6][0] = take();

    // F:
    printedCorner[0][2] = take(); pushEdge(0,1,take()); printedCorner[3][1] = take();
    pushEdge(4,0,take()); take(); /*center F*/ pushEdge(7,0,take());
    printedCorner[4][1] = take(); pushEdge(8,1,take()); printedCorner[7][2] = take();

    // B:
    printedCorner[2][2] = take(); pushEdge(2,1,take()); printedCorner[1][1] = take();
    pushEdge(6,0,take()); take(); /*center B*/ pushEdge(5,0,take());
    printedCorner[6][1] = take(); pushEdge(10,1,take()); printedCorner[5][2] = take();

    // L:
    printedCorner[1][2] = take(); pushEdge(1,1,take()); printedCorner[0][1] = take();
    pushEdge(5,1,take()); take(); /*center L*/ pushEdge(4,1,take());
    printedCorner[5][1] = take(); pushEdge(9,1,take()); printedCorner[4][2] = take();

    // R:
    printedCorner[3][2] = take(); pushEdge(3,1,take()); printedCorner[2][1] = take();
    pushEdge(7,1,take()); take(); /*center R*/ pushEdge(6,1,take());
    printedCorner[7][1] = take(); pushEdge(11,1,take()); printedCorner[6][2] = take();

    // 3) possibilities for corners
    vector<vector<pair<int,int>>> possCorner(8);
    for (int pos=0; pos<8; ++pos) {
        for (int cub=0; cub<8; ++cub) {
            for (int o=0; o<3; ++o) {
                bool ok = true;
                for (int f=0; f<3; ++f) {
                    char expected = corners[cub][(o+f)%3];
                    char got = printedCorner[pos][f];
                    if (got == 0 || got != expected) { ok = false; break; }
                }
                if (ok) possCorner[pos].push_back({cub,o});
            }
        }
    }

    array<int,8> new_cp; new_cp.fill(-1);
    array<int,8> new_co; new_co.fill(0);
    vector<char> usedC(8,0);

    function<bool(int)> dfsCorner = [&](int p)->bool{
        if (p==8) return true;
        for (auto pr : possCorner[p]) {
            int cub = pr.first, o = pr.second;
            if (usedC[cub]) continue;
            usedC[cub]=1;
            new_cp[p]=cub;
            new_co[cub]=o;
            if (dfsCorner(p+1)) return true;
            usedC[cub]=0;
            new_cp[p]=-1;
            new_co[cub]=0;
        }
        return false;
    };
    if (!dfsCorner(0)) return false;

    // 4) possibilities for edges
    vector<vector<pair<int,int>>> possEdge(12);
    for (int pos=0; pos<12; ++pos) {
        auto a = printedEdge[pos][0];
        auto b = printedEdge[pos][1];
        if (a.first == -1 || b.first == -1) return false;
        for (int e=0;e<12;++e) {
            for (int o=0;o<2;++o) {
                char expect1 = ( (a.first ^ o) ? edges[e].second : edges[e].first );
                char expect2 = ( (b.first ^ o) ? edges[e].second : edges[e].first );
                if (expect1 == a.second && expect2 == b.second) possEdge[pos].push_back({e,o});
            }
        }
    }

    array<int,12> new_ep; new_ep.fill(-1);
    array<int,12> new_eo; new_eo.fill(0);
    vector<char> usedE(12,0);

    function<bool(int)> dfsEdge = [&](int p)->bool{
        if (p==12) return true;
        for (auto pr : possEdge[p]) {
            int e = pr.first, o = pr.second;
            if (usedE[e]) continue;
            usedE[e]=1;
            new_ep[p]=e;
            new_eo[e]=o;
            if (dfsEdge(p+1)) return true;
            usedE[e]=0;
            new_ep[p]=-1;
            new_eo[e]=0;
        }
        return false;
    };
    if (!dfsEdge(0)) return false;

    // 5) assign reconstructed arrays to the cube state
    for (int i=0;i<8;i++) cp[i] = new_cp[i];
    for (int i=0;i<8;i++) co[i] = new_co[i];
    for (int i=0;i<12;i++) ep[i] = new_ep[i];
    for (int i=0;i<12;i++) eo[i] = new_eo[i];
    
    return true;
}
void cube::move(string id) {
    if(id=="U") U();
    if(id=="D") D();
    if(id=="R") R();
    if(id=="L") L();
    if(id=="F") F();
    if(id=="B") B();

    if(id=="Up") {U(); U(); U();}
    if(id=="Dp") {D(); D(); D();}
    if(id=="Rp") {R(); R(); R();}
    if(id=="Lp") {L(); L(); L();}
    if(id=="Fp") {F(); F(); F();}
    if(id=="Bp") {B(); B(); B();}

    if(id=="U2") {U(); U();}
    if(id=="D2") {D(); D();}
    if(id=="R2") {R(); R();}
    if(id=="L2") {L(); L();}
    if(id=="F2") {F(); F();}
    if(id=="B2") {B(); B();}
}
