// David Morvay
// Project #2
// main.cc
// The following code solves a blank
// Sudoku board based on a given input
// ** Updated file. I was using an older version of stack.h
// ** Replaced stack.h with the new file. Should work the same!

#include <iostream>
#include "stack.h"
using namespace std;

// Function Prototypes
int FindBest(uint16_t (&board)[9][9]);
int ValidChoice(uint16_t (&board)[9][9]);
int Solve(uint16_t (&board)[9][9]);
bool isValid(uint16_t cell, uint16_t i);
void markInvalid(uint16_t &cell, uint16_t i);
void DetermineSquare(int r, int c, int (&index)[4]);
int countBits(unsigned int n);
int overlap(uint16_t (&board)[9][9]);

// Constants
const uint16_t
    ZERO = 0x0000,
    VALUE_MASK = 0x000f,    // 1111
    VALID_MASK = 0x1ff0,    // 1111111110000
    IS_FILLED_IN = 0x8000,  // 1000000000000000
    IS_O_VALID = 0x2000;    // 10000000000000

int StackTopRow = 0, StackTopColumm = 0, StackTopCount = 0;

// Stacks
    Stack<int> r;
    Stack<int> c;
    Stack<int> counterBits;

// Main Function
int main() {
  uint16_t
    board[9][9];
    
    char ch;
    
    // Cin the board
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
            cin >> ch;
            if (ch == '.') {
                board[r][c] = 0;
            }
            else {
                board[r][c] = IS_FILLED_IN | (ch - '0');
            }
        }
    }
    
    cout << endl;
    // Cout the unsolved board
    for (int r=0; r<9; r++) {
         
        if ((r == 3) || (r==6)) {
            cout << endl;
        }
        else {}
        
        for (int c=0; c<9; c++) {
            cout << (board[r][c] & VALUE_MASK);
            
            if ((c == 2) || (c ==5 )) {
                cout << "  ";
            }
            else {}
            
        }
        cout << endl;
    }
    
    cout << endl << endl;
    cout << "Solving. Please wait...." << endl;
    
    // Call solve funciton
    Solve(board);
    
    return 0;
}






int Solve(uint16_t (&board)[9][9]) {
    uint16_t ChoiceValidMask = 0x0000, FilledSpots = 0x0000;
    int NoSolution = 0;
    int finderofbest = 0;
    int validChoice = 0;
    int ChoiceCounter = 0;
    int backtrack = 0, TryAgain = 0;
    int CountFilledSpots = 0, NumFilledSpots = 0, NumUnfilledSpots = 0, overlapmain = 0;
    
    Stack<int> SameLineR, SameLineC, SameLineR2, SameLineC2;
    int row, column, sizer, sizec, sizecount;
    
    
    //========================================================//
    // Find the number of filled and unfilled spots on the board
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
            FilledSpots = board[r][c] & IS_FILLED_IN;
            CountFilledSpots = countBits(FilledSpots);
            
            if (CountFilledSpots > 0) {
                NumFilledSpots++;
            }
            else {}
        }
    }
    // cout << NumFilledSpots << endl;
    
    NumUnfilledSpots = 81 - NumFilledSpots;

    // cout << NumUnfilledSpots << endl;
    //========================================================//
    
    // Select the best empty cell and place its location on the stack
    finderofbest = FindBest(board);
            // Find row, column, size, and testing functions
            row = r.peek();
            column = c.peek();
            StackTopRow = r.peek();
            StackTopColumm = c.peek();
            // cout << "Find best before manipulation: " << board[StackTopRow][StackTopColumm] << " Cordinates: " << StackTopRow << StackTopColumm << " "  << endl << endl;
            sizer = r.size();
            sizec = c.size();
    
    
    while (true) { //while1
    row = r.peek();
    column = c.peek();
      
        if ((finderofbest == 1) || (TryAgain == 1)) {
            TryAgain = 0;
            
            // Select the next valid choice for board[i][j]
            
            validChoice = ValidChoice(board);
            ChoiceValidMask = board[row][column] & VALID_MASK;
            ChoiceCounter = countBits(ChoiceValidMask);
            counterBits.push(ChoiceCounter);
        }
        else {}
        
        // Tesing Functions - Ignore
        // ------------------------------------- //
        StackTopRow = r.peek();
        StackTopColumm = c.peek();
        StackTopCount = counterBits.peek();
        sizer = r.size();
        sizec = c.size();
        sizecount = counterBits.size();
        // cout << "Top of stack after manipulation: " << board[StackTopRow][StackTopColumm] << " Cordinates: " << StackTopRow << StackTopColumm << " "  << StackTopCount << endl;
        // cout << sizer << " " << sizec << " " << sizecount << endl << endl;
        // ------------------------------------- //
        
       /* cout << endl;
        for (int r=0; r<9; r++) {
            for (int c=0; c<9; c++) {
                cout << (board[r][c] & VALUE_MASK);
            }
            cout << endl;
        } */
        
        // If no such choice exists then....
        if (board[row][column] == IS_FILLED_IN) {
            backtrack = 0;
            while (backtrack == 0) {
                // Mark board[i][j] as not filled in
                board[row][column] = board[row][column] ^ IS_FILLED_IN;
                
                // Pop the stack
                r.pop();
                c.pop();
                counterBits.pop();
                
                // Testing Functions - Ignore
                // ------------------------------------- //
                StackTopRow = r.peek();
                StackTopColumm = c.peek();
                StackTopCount = counterBits.peek();
                sizer = r.size();
                sizec = c.size();
                sizecount = counterBits.size();
                // cout << "Top of stack after pop: " << board[StackTopRow][StackTopColumm] << " Cordinates: " << StackTopRow << StackTopColumm << " "  << StackTopCount << endl;
                // cout << sizer << " " << sizec << " " << sizecount << endl << endl;
                // ------------------------------------- //
                
                row = r.peek();
                column = c.peek();
                ChoiceCounter = counterBits.peek();
                sizer = r.size();
                sizec = c.size();
                
                // Backtrack until found a valid spot
                if (ChoiceCounter > 0) {
                    validChoice = ValidChoice(board);
                    counterBits.pop();
                    ChoiceValidMask = board[row][column] & VALID_MASK;
                    ChoiceCounter = countBits(ChoiceValidMask);
                    counterBits.push(ChoiceCounter);
                    backtrack = 1;
                }
                else {}
                
                
                if ((sizer == 0) && (sizec == 0)) {
                    break;
                }
                else {}
            }

            sizer = r.size();
            sizec = c.size();
            
            // If the stack is empty then...
            if ((sizer == 0) && (sizec == 0)) {
                NoSolution = 1;
                break;
            }
            else {}
        }
    
    // Select the best empty cell and place its location on the stack
    finderofbest = FindBest(board);
        if (finderofbest == 0) {
            // cout << "Code red!" << endl << endl;
        }
        else {}
        
    sizer = r.size();
    sizec = c.size();
    row = r.peek();
    column = c.peek();
    

    // If no such cell exists then..
    // Break and output board or
    // Pop stack and go back to the beginnig
    if ((finderofbest == 0) && (sizer == NumUnfilledSpots))
    {
        overlapmain = overlap(board);
        if (overlapmain == 1) {
            break;
        }
        else {
            board[row][column] = board[row][column] ^ IS_FILLED_IN;
            r.pop();
            c.pop();
            counterBits.pop();
            counterBits.pop();
            TryAgain = 1;
        }
    }
    else {}

  } // End of while 1
    
    // Output the solved board
    cout << endl << endl;
    cout << "  Solution:" << endl;
    for (int r=0; r<9; r++) {
            
           if ((r == 3) || (r==6)) {
               cout << endl;
           }
           else {}
           
           for (int c=0; c<9; c++) {
               cout << (board[r][c] & VALUE_MASK);
               
               if ((c == 2) || (c ==5 )) {
                   cout << "  ";
               }
               else {}
               
           }
           cout << endl;
       }
    cout << endl;
    
    if (NoSolution == 1) {
        return false;
    }
    else {
        return true;
    }
}









int ValidChoice(uint16_t (&board)[9][9]) {
    // The following function determines the next best choice for a cell
    int i=1;
    int next = 0;
    int r1, c1;
    
    uint16_t
    mask = 0x0000,
    reset = 0x9FF0;
    
    
    uint16_t
    bitMask[9];
    
    bitMask[0] = 0x9000;
    bitMask[1] = 0x8800;
    bitMask[2] = 0x8400;
    bitMask[3] = 0x8200;
    bitMask[4] = 0x8100;
    bitMask[5] = 0x8080;
    bitMask[6] = 0x8040;
    bitMask[7] = 0x8020;
    bitMask[8] = 0x8010;
    
    uint16_t
    setDigit[9];
    
    setDigit[0] = 0x1001;
    setDigit[1] = 0x0802;
    setDigit[2] = 0x0403;
    setDigit[3] = 0x0204;
    setDigit[4] = 0x0105;
    setDigit[5] = 0x0086;
    setDigit[6] = 0x0047;
    setDigit[7] = 0x0028;
    setDigit[8] = 0x0019;
    
    r1 = r.peek();
    c1 = c.peek();
    // cout << board[r1][c1] << endl;
    
    board[r1][c1] = board[r1][c1] & reset;
    
    // Sets value of cell based on the first '1'
    // within the middle nine 1's.
    
    // If no such choice exists,
    // sets value to decimal 32768
    // and returns false.
    while (i == 1) {
        mask = bitMask[next] & board[r1][c1];
        
        if (bitMask[next] == mask) {
            board[r1][c1] = board[r1][c1] ^ setDigit[next];
            i = 0;
        }
        else {
            next++;
            if (next >= 9) {
                i = 0;
            }
            else {}
        }
    }
    
    // cout << "Best location after masking: " << endl;
    // cout << board[r1][c1] << endl;
    // cout << r1 << " " << c1 << endl << endl;
    
    
    if (next >= 9) {
        return false;
    }
    else {
        return true;
    }
}




int FindBest(uint16_t (&board)[9][9]) {
    // FindBest function determines the following...
    // Best location is pushed onto the stack
    // Best location is marked as filled in
    int valid = 0;
    
    int index[4];
    int index_r;
    int index_c;
    
    int low = 10;
    int count = 0;
    int rBest = 0;
    int cBest = 0;
    count = 0;
    
    // For each empty cell board do
        // Mark digits for empty cells
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
            if (board[r][c] < VALID_MASK)
            {
                // Mark digits by turning them on
                uint16_t mask_value;
                mask_value = board[r][c];
                mask_value = VALID_MASK | mask_value;
                board[r][c] = mask_value;
                
                // Testing
                // cout << board[r][c] << endl;
            }
            else {}
        }
    }
    
    
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
       // For each filled in cell, mark unfilled cells with valid choices
            if (board[r][c] > IS_FILLED_IN) {
                // Mark digits in rows as invalid choices
                for (int r2=0; r2<9; r2++) {
                    if (board[r2][c] < IS_FILLED_IN) {
                    valid = isValid(board[r2][c], board[r][c]);
                    if (valid == 1) {
                        markInvalid(board[r2][c], board[r][c]);
                   }
                    else {}
                    }
                   
                    else {}
                    
                }
                
                // Mark digits in columns as invalid choices
                for (int c2=0; c2<9; c2++) {
                    if (board[r][c2] < IS_FILLED_IN) {
                        valid = isValid(board[r][c2], board[r][c]);
                        
                        if (valid == 1) {
                            markInvalid(board[r][c2], board[r][c]);
                        }
                        else {}
                    }
                    
                    else {}
                }
                
                DetermineSquare(r, c, index);
                index_r = index[0];
                index_c = index[2];
                
                // Mark digits in 3 by 3 as invalid choices
                for (int index_r = index[0]; index_r < index[1]; index_r++) {
                    for (int index_c = index[2]; index_c < index[3]; index_c++) {
                        if (board[index_r][index_c] < IS_FILLED_IN) {
                        valid = isValid(board[index_r][index_c], board[r][c]);
                                
                                if (valid == 1) {
                                    markInvalid(board[index_r][index_c], board[r][c]);
                                }
                                else {}
                        }
                        else {}
                    }
                }
                }
            else {}
        }
    }
    
    
    // For empty cells, find the one that has the lowest countBits
    low = 10;
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
            if (board[r][c] < IS_FILLED_IN) {
                count = countBits(board[r][c]);
                
                if ((count < low)) {
                    low = count;
                    rBest = r;
                    cBest = c;
                }
                    
                else {}
            }
            else {}
            
        }
    }
    
    if (low == 10) {
        return false;
    }
    else {
    // Mark board[j][k] as filled in
    // Push (ibest, jbest) onto the stack
    board[rBest][cBest] = IS_FILLED_IN | board[rBest][cBest];
    r.push(rBest);
    c.push(cBest);
    return true;
    }
}






bool isValid(uint16_t cell, uint16_t i) {
    return ((cell & (IS_O_VALID >> i)) != 0);
}




void markInvalid(uint16_t &cell, uint16_t i) {
    cell = cell & ~(IS_O_VALID >> i);
}




// Counts number of 1's bits
int countBits(unsigned int n) {
  int count=0;
    while (n != 0) {
        count++;
        n &= n - 1;
    }
  return count;
}


int overlap(uint16_t (&board)[9][9]) {
    
    // The following function determines if
    // there is an overlap of numbers in a row, column,
    // or block in the final output.
    // Notice this funciton is very similar to FindBest
    
    uint16_t overlapfull = 0x0000, overlaphalf = 0x0000;
    int overlapcount = 0;
    int index[4];
    int index_r;
    int index_c;
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
                for (int r2=0; r2<9; r2++) {
                    if (r != r2) {
                        overlapfull = board[r][c] & VALUE_MASK;
                        overlaphalf = board[r2][c] & VALUE_MASK;
                        
                        if (overlaphalf == overlapfull) {
                            overlapcount++;
                        }
                        else {}
                    }
                    else {}
                }

                for (int c2=0; c2<9; c2++) {
                    if (c != c2) {
                        overlapfull = board[r][c] & VALUE_MASK;
                        overlaphalf = board[r][c2] & VALUE_MASK;
                                               
                                if (overlaphalf == overlapfull) {
                                    overlapcount++;
                                }
                                else {}
                    }
                    else {}
                    
                    }
                
                DetermineSquare(r, c, index);
                index_r = index[0];
                index_c = index[2];
                
                // Mark digits in 3 by 3 as invalid choices
                for (int index_r = index[0]; index_r < index[1]; index_r++) {
                    for (int index_c = index[2]; index_c < index[3]; index_c++) {
                        if ((r != index_r) && (c != index_c)) {
                            overlapfull = board[r][c] & VALUE_MASK;
                            overlaphalf = board[index_r][index_c] & VALUE_MASK;
                           
                           if (overlaphalf == overlapfull) {
                               overlapcount++;
                           }
                           else {}
                        }
                        else {}
                    }
                }
            }
        }
    // cout << "Overlap count: " << overlapcount << endl << endl;
    
    // If there was any overlap in the for loops above,
    // the overlap counter will increment.
    // However, if there is no overlap, the algorithm
    // will continue and the board is solved!
    
    if (overlapcount > 0) {
        return false;
    }
    else {
        return true;
    }
}



void DetermineSquare(int r, int c, int (&index)[4]) {
    
    // The following function determines the 3x3
    // sqaure board[i][j] is located in.
    // The function sends back the bounds in the following manner
    // index[0] = min row
    // index[1] = max row
    // index[2] = min column
    // index[3] = max column
    
    int index_r;
    int index_c;
    int m_dex_r;
    int m_dex_c;
    // Determine which 3x3 block the filled cell is in
        if (r<3) {
            if (c<3) {
                index_r = 0;
                index_c = 0;
                m_dex_r = 3;
                m_dex_c = 3;
            }
            else if ((c>=3)&(c<6)) {
                index_r = 0;
                index_c = 3;
                m_dex_r = 3;
                m_dex_c = 6;
            }
            else {
                index_r = 0;
                index_c = 6;
                m_dex_r = 3;
                m_dex_c = 9;
            }
        }
            
        else if ((r>=3)&(r<6)) {
            if (c<3) {
                index_r = 3;
                index_c = 0;
                m_dex_r = 6;
                m_dex_c = 3;
            }
            else if ((c>=3)&(c<6)) {
                index_r = 3;
                index_c = 3;
                m_dex_r = 6;
                m_dex_c = 6;
            }
                else {
                index_r = 3;
                index_c = 6;
                m_dex_r = 6;
                m_dex_c = 9;
            }
        }
            
        else {
            if (c<3) {
                index_r = 6;
                index_c = 0;
                m_dex_r = 9;
                m_dex_c = 3;
            }
            else if ((c>=3)&(c<6)) {
                index_r = 6;
                index_c = 3;
                m_dex_r = 9;
                m_dex_c = 6;
            }
                else {
                index_r = 6;
                index_c = 6;
                m_dex_r = 9;
                m_dex_c = 9;
            }
        }
    // END of IF statement
    index[0] = index_r;
    index[1] = m_dex_r;
    index[2] = index_c;
    index[3] = m_dex_c;
}
