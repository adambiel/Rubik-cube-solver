/** @file ida_star.cpp
 *  @brief Implementation of the IDA* algorithm.
 *  @details Contains state space search logic to find a solution.
 */
#include <cassert>
#include<vector>
#include<string>
#include "cube.h"
#include "debug.h"
#include "eoh.h"
#include "eph.h"
#include "cph.h"

using namespace std;

typedef long long LL;

/// Constant representing "infinity" used in IDA*
#define inf 1000000000
vector<string> moves;

/**
 * @brief List of all allowed moves in phase 1 of the algorithm.
 *
 * Contains the full set of face turns (90°, -90°, 180°).
 */
vector<string> moves_phase1 = {
"U", 
"D", 
"R", 
"L", 
"F", 
"B",
};

/**
* @brief List of allowed moves in phase 2 of the algorithm.
*
*/
vector<string> moves_phase2 = {
    "U", "Up","U2",
    "D", "Dp","D2",
    "R2","L2","F2","B2"
};

/// Edge Orientation Heuristic
eoh EOH;

/// Corner Permutation Heuristic
cph CPH;

/// Edge Permutation Heuristic
eph EPH;

/**
 * @brief Checks if the cube is solved in phase 2.
 *
 */
bool is_goal_phase2(cube& c) {

    /// Check corner validity
    for (int i = 0; i < 8; i++) {
        if (c.cp[i] != i) return false; // corner in the wrong position
        if (c.co[i] != 0) return false; // invalid corner orientation
    }

    /// Check edge validity
    for (int i = 0; i < 12; i++) {
        if (c.ep[i] != i) return false; // edge in the wrong position
        if (c.eo[i] != 0) return false; // invalid edge orientation
    }

    /// All elements are in the correct places
    return true;
}

/**
 * @brief Checks if the cube meets the phase 1 end conditions.
 *
 */
bool is_goal_phase1(cube& c) {

    /// Check corner orientation
    for (int i = 0; i < 8; i++) {
        if (c.co[i] != 0)
            return false;
    }

    /// Check edge orientation
    for (int i = 0; i < 12; i++) {
        if (c.eo[i] != 0)
            return false;
    }

    /// Check if UD edges are located in the UD layers
    for (int pos = 0; pos < 12; pos++) {
        int edge = c.ep[pos];

        bool is_ud_edge = (edge <= 3) || (edge >= 8); // UD-type edge
        bool is_ud_pos  = (pos  <= 3) || (pos  >= 8); // UD layer position

        if (is_ud_edge && !is_ud_pos)
            return false;
    }

    return true;
}

/**
 * @brief Calculates the heuristic value for a given cube state.
 *
 */
int getheuristic(cube &node, int stage){

    if(stage==1)
        return EOH.get_eoh(node); // heuristic for phase 1

    /// Maximum of corner and edge heuristics in phase 2
    return max(CPH.get_cph(node), EPH.get_eph(node));
}

/**
 * @brief Recursive IDA* search function.
 *
 * @return The smallest exceeded cost or -1 if a solution is found
 */
int search(vector<string>& seq, cube node, int price, int bound, int stage){

    int f = price + getheuristic(node, stage);
    
    if(f > bound) return f;  // search range exceeded
    if(stage==1 and is_goal_phase1(node)) return -1;  // solution found
    if(stage==2 and is_goal_phase2(node)) return -1;
    
    int mint = inf, move_count = 6;
    if(stage == 2) move_count = 10;
    for (int i = 0; i < move_count; i++){
        if(!seq.empty() && seq.back()[0] == moves[i][0]) continue;
        cube newcube=node;
        seq.push_back(moves[i]);
        newcube.move(moves[i]);
        int t = search(seq, newcube, price+1, bound, stage);
        if(t == -1) return t;
        mint = min(mint, t);
        
        seq.pop_back();
    
    }
    return mint;
    
}

/**
 * @brief Implementation of the IDA* algorithm for a given phase.
 *
 * @param root Initial state of the cube
 * @param stage Algorithm phase (1 or 2)
 * @return Pair: (move list, solution depth)
 */
vector<string> ida_star(cube root, int stage){
    if(stage == 1)
        moves = moves_phase1;
    else
        moves = moves_phase2;
    if(stage == 2)
        assert(is_goal_phase1(root) == 1);
    int bound=getheuristic(root, stage); // set initial search bound
    
    vector<string> seq;    
    while(true){
        int t = search(seq, root, 0, bound, stage); 
        if(t == -1) return seq;
        if(t >= inf) return seq;
        bound = t;
        debug(bound);
    }
}
