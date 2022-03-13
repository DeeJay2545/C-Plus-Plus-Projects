// Project #3
// David J. Morvay
// Joey Pezzano
// main.cc

#include <iostream>
#include "hexpathfinder.h"
#include "sampler.h"
#include "disjointset.h"
#include "stack.h"
using namespace std;

// Function Prototypes
void GenerateMaze(int32_t nR, int32_t nC, uint8_t (&maze)[50][50]);
void findPath(int32_t nR, int32_t nC, uint8_t (&maze)[50][50]);
int32_t encode(int32_t r, int32_t c, int32_t d, int32_t nR, int32_t nC);
void decode(int32_t e, int32_t nR, int32_t nC, int32_t &r, int32_t &c, int32_t &d);
// Changed int32_t to uint32_t
// Does this matter?

Stack<int32_t> s;

// this code will generate all six neighbors in a hex map
// Changed int32_t from constant to regular
int32_t
    dC[] = {0,1,1,0,-1,-1},
    dReven[] = {-1,-1,0,1,0,-1},
    dRodd[] = {-1,0,1,1,1,0};
int32_t
    *dR;

int main(int argc, char *argv[]) {
    int32_t
        nR, nC;
    
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " nRows nColumns" << endl;
        return 1;
    }
    
    nR = atoi(argv[1]);
    nC = atoi(argv[2]);
    
            // cout << nR << " " << nC << endl;
    
    uint8_t maze[50][50];
    GenerateMaze(nR, nC, maze);
    findPath(nR, nC, maze);
    printMaze(maze, nR, nC);
    
    return 0;
}

void GenerateMaze(int32_t nR, int32_t nC, uint8_t (&maze)[50][50]) {
    // Algorithm 4 - Generate a maze
    int i = 0;
    int32_t e, cell_1, r_1, c_1, dir_1, cell_2, r_2, c_2, shift, position;
    // Lines 3-7
    for (int r = 0; r < nR; r++) {
        for (int c = 0; c < nC; c++){
            maze[r][c] = 0b00111111;        // 63 = 0b00111111; marks all six walls present
        }
    }
    // Call functions
    DisjointSet ds(nR * nC);
    Sampler sampler(3 * nC * nR);
    
    i = 0;
    while ( i < (nR * nC - 1) ) {
        do {
            do {
                e = sampler.getSample();        // Call getSample
                decode(e, nR, nC, r_1, c_1, dir_1);     // Decode cell
                
                dR = (c_1 & 1) ? dRodd : dReven;        // Find cell adjacent to (r_1, c_1 in given direction
                r_2 = r_1 + dR[dir_1];
                c_2 = c_1 + dC[dir_1];
                
                position = maze[r_1][c_1];      // Shift maze[r_1][c_1] a specific amount of bits to find
                shift = position >> dir_1;      // if there is a wall in the given direction.
                shift = shift & 1;              // If shift == 0, we know there is no wall.
            
            // while e references an exterior or nonexistent wall
            } while ((r_2 < 0) || (r_2 >= nR) || (c_2 < 0) || (c_2 >= nC) || (shift == 0));
            
        cell_1 = encode(r_1, c_1, 0, nR, nC);       // Encode cell #1
        cell_2 = encode(r_2, c_2, 0, nR, nC);       // Encode cell #2
        } while (ds.find(cell_1) == ds.find(cell_2));   // While cell #1 == cell #2
        
        ds.join(cell_1, cell_2);        // Call DisjointSetUnion
        i++;
        
        // The following removes the wall between (r_1, c_1) and (r_2, c_2).
        // Notice the walls are opposite from each other.
        // This demonstrates that each wall needs to be removed twice in the puzzle.
        if (dir_1 == 0) {
            maze[r_1][c_1] = maze[r_1][c_1] ^ WALL_UP;
            maze[r_2][c_2] = maze[r_2][c_2] ^ WALL_DOWN;
        }
        else if (dir_1 == 1) {
            maze[r_1][c_1] = maze[r_1][c_1] ^ WALL_UP_RIGHT;
            maze[r_2][c_2] = maze[r_2][c_2] ^ WALL_DOWN_LEFT;
        }
        else if (dir_1 == 2) {
            maze[r_1][c_1] = maze[r_1][c_1] ^ WALL_DOWN_RIGHT;
            maze[r_2][c_2] = maze[r_2][c_2] ^ WALL_UP_LEFT;
        }
        else if (dir_1 == 3) {
            maze[r_1][c_1] = maze[r_1][c_1] ^ WALL_DOWN;
            maze[r_2][c_2] = maze[r_2][c_2] ^ WALL_UP;
        }
        else if (dir_1 == 4) {
            maze[r_1][c_1] = maze[r_1][c_1] ^ WALL_DOWN_LEFT;
            maze[r_2][c_2] = maze[r_2][c_2] ^ WALL_UP_RIGHT;
        }
        else {
            maze[r_1][c_1] = maze[r_1][c_1] ^ WALL_UP_LEFT;
            maze[r_2][c_2] = maze[r_2][c_2] ^ WALL_DOWN_RIGHT;
        }
    }
}


void findPath(int32_t nR, int32_t nC, uint8_t (&maze)[50][50]) {
    // Algorithm #5 - Maze solver
    int32_t e, r, c, d, r_, c_, dir_position, vis_position, dir_shift, vis_shift;
    
    
    s.push(encode(0,0,0,nR,nC));        // Push encode(0,0,0) onto the stack
    maze[0][0] = maze[0][0] | VISITED;      // Mark (0,0) as visited
    
    while (true) {
        e = s.pop();        // Pop stack - Moved from line 11 to line 4.5
        decode(e, nR, nC, r, c, d);     // Decode peek and put values in (r,c,d)
        
        if ((r == (nR - 1)) && (c == (nC - 1))) {       // Lines 6-8
            break;
        }
        
        if (d == 6) {       // Lines 9-11
            maze[r][c] = maze[r][c] | DEAD_END;
        }
        else {
            dR = (c & 1) ? dRodd : dReven;      // Find the next cell in direction d
            r_ = r + dR[d];                     // This process is exactly the same as Algorithm #4
            c_ = c + dC[d];
            s.push(encode(r,c,d+1,nR,nC));      // Replace encode(r,c,d) with encode(r,c,d+1) on top of stack
            
            dir_position = maze[r][c];          // Determine if a wall exists in direction d
            dir_shift = dir_position >> d;      // Same process as Algorithm #4
            dir_shift = dir_shift & 1;          // Shift bits and see if the lowest digit is turned on
            
            vis_position = maze[r_][c_];        // Determine if (r_,c_) is not marked as visited.
            vis_shift = vis_position >> 6;      // Again, shift bits to see if "visited" position in the
            vis_shift = vis_shift & 1;          // string is turned on or off.
            
            if ((dir_shift == 0) && (vis_shift == 0)) {     // Lines 15-17
                s.push(encode(r_,c_,0,nR,nC));
                maze[r_][c_] = maze[r_][c_] | VISITED;
            }
        }
    }
    /* // given position
    // this is the part where you need to visit all six neighbors
    dR = (c & 1) ? dRodd : dReven;
    for (int d = 0; d < 6; d++){
        neighborR = r + dR[d];
        neighborC = c + dC[d];
        // now, neightborR and neighborC specify a neighboring cell
    } */
}


int32_t encode(int32_t r, int32_t c, int32_t d, int32_t nR, int32_t nC) {
    return c + nC * (r + nR * d);
}

void decode(int32_t e, int32_t nR, int32_t nC, int32_t &r, int32_t &c, int32_t &d) {
    
    c = e % nC;
    e /= nC;
    r = e % nR;
    d = e / nR;
}

