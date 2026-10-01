/** @file main.cpp
 *  @brief Main file of the solver program.
 *  @details Handles command line arguments and controls the solving process.
 */
#include "debug.h"
#include "cube.h"
#include "ida_star.h"
#include <chrono>

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        cerr << "Heuristics loaded" << endl;
        cube state;
        if (!state.read()) {
            cerr << "Error: Failed to load cube state (invalid cube configuration?)" << endl;
            return 1;
        }
        cerr << "Cube state loaded successfully" << endl;
        auto seq = ida_star(state, 1);
        for(auto& v : seq)
        {        
            state.print();
            cout << "---" << endl;
            state.move(v);
            cerr << v << " ";
        }
        seq = ida_star(state, 2);
        for(auto& v : seq)
        {
            state.print();
            cout << "---" << endl;
            state.move(v);
            cerr << v << " ";
        }
        state.print();
        return 0;
    }
    if(argc == 2 && strcmp(argv[1], "--moves") == 0)
    {
        #ifndef DEBUG
        cout << "Enter the number of moves: ";
        #endif
        int n;
        cin >> n;
        assert(0 <= n && n <= 1000);
        cube state;
        for(int i = 0; i < n; i++)
        {
            #ifndef DEBUG
            cout << "Enter move: ";
            #endif
            string move;
            cin >> move;
            state.move(move);
        }
        #ifndef DEBUG
        cout << "Starting phase one\n";
        #endif
        auto solving_seq = ida_star(state, 1);
        #ifndef DEBUG
        cout << "Phase one completed\n";
        cout << "Phase one moves: \n";
        #endif
        for(auto& v : solving_seq)
            state.move(v), cout << v << " ";
        #ifndef DEBUG 
        cout << "Starting phase two\n";
        #endif
        solving_seq = ida_star(state, 2);
        #ifndef DEBUG
        cout << "Phase two completed\n";
        cout << "Phase two moves: \n";
        #endif    
        for(auto& v : solving_seq)
            cout << v << " ";
        return 0;
    }
    if(argc == 2 && strcmp(argv[1], "--test") == 0)
    {
        int t;
        cout << "Enter the number of tests: " << endl;
        cin >> t;
        for(int s = 0; s < t; s++)
        {
            int n = rand() % 100;
            cube state;
            vector<string> moves = {"R", "U", "D", "F", "B", "L"};
            vector<string> scramble;
            for(int i = 0; i < n; i++)
            {
                int id = rand() % 6;
                state.move(moves[id]);
                scramble.push_back(moves[id]);
            }
            auto start = chrono::steady_clock::now();
            vector<string> seq = ida_star(state, 1);
            for(auto& v : seq) state.move(v);
            seq = ida_star(state, 2);
            auto end = chrono::steady_clock::now();
            for(auto& v : seq) state.move(v);
            bool ok = 1;
            for(int i = 0; i < 12; i++)
                if(!(state.ep[i] == i && state.eo[i] == 0))
                    ok = false;
            for(int i = 0; i < 8; i++)
                if(!(state.cp[i] == i && state.co[i] == 0))
                    ok = false;
            if(!ok)
            {
                cout << "Test " << s << " failed. ";
                cout << "The scrambling sequence is: ";
                for(auto& v : scramble) cout << v << " ";
                cout << endl;
                return 0;
            }
            chrono::duration<double> elapsed = end - start;
            cout << "Test " << s << " passed in time: " << elapsed.count() << endl;
        }
        return 0;
    }
    cerr << "Unrecognized flags!\n";
}
