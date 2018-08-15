#include <iostream>
#include <iomanip>
using namespace std;

struct Space
{
    public:
    int value;
    bool canHold[9];
    bool lastChanged;
    bool wasTested[9];
};
struct Board
{
    public:
    Space members[9][9];
    Space backTrack[9][9];  // holds copies of all values, before changing anything
    bool wasChanged;        // true if any function was successful
    bool solved;
    bool isNewCopy;  // restarts wasTested for a new copy
    int rowZero;     // the first zero is in this row
    int colZero;     // the first zero is in this column
    int valueIndex;  // Remember the last value tried on this board
};

// Prototypes for boards
void flashSudoku(Board);
void easySudoku(Board);
void mediumSudoku(Board);

// Functional prototypes
void printBoard(Board);
void initiateBoard(Board &, int);
void populateBoard(Board &, int [81]);
void writeOnBoard(Board &);
bool check(Board);
void makeCopy(Board &, Board);
void firstZero(Board &);
bool filled(Board);
void backTrack(Board &);
void menu(Board &);
void solved(Board &);

// Prototypes for techniques
bool uniqueCandidate(Board &);
bool soleCandidate(Board &);
bool magic(Board &);

// Declare board
Board puzzle;

int main()
{
    puzzle.solved = false;

    menu(puzzle);
    writeOnBoard(puzzle);

    while (puzzle.solved != true)
    {
        writeOnBoard(puzzle);
        cin.get();

        if (soleCandidate(puzzle))
        {
            cout << "Sole Candidate\n";
            puzzle.wasChanged = true;
        }

        if (puzzle.wasChanged != true)
        {
            if (uniqueCandidate(puzzle))
            {
                cout << "Unique Candidate\n";
                puzzle.wasChanged = true;
            }
        }

        if (puzzle.wasChanged != true)
        {
            if (magic(puzzle))
            {
                puzzle.wasChanged = true;
            }
        }

        printBoard(puzzle);
        puzzle.wasChanged = false;

        if (check(puzzle))
        {
            solved(puzzle);
        }
    }
}

void solved(Board &brd)
{
    cout << "Solved!!!\n";
    brd.solved = true;
}

void menu(Board &brd)
{
    cout << "Select the level of difficulty:\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
    cout << "4. Expert\n";
    cout << "5. World's Hardest\n";
    cout << "6. Out of this world\n";

    int choice;
    cin >> choice;

    initiateBoard(brd, choice);
    printBoard(brd);
    cout << "\nPress Enter to solve...\n";
    cin.ignore();
    cin.get();
}

bool magic(Board &brd)
{
    int tries = 0;
    int index = 0;
    Board *copies = NULL;
    copies = new Board[81];

    copies[0].isNewCopy = true;
    makeCopy(copies[0], brd);
    writeOnBoard(copies[0]);

    while ((brd.solved != true) || (tries < 81))
    {
        // Try each value for the first zero space in the first board.
        for (int v = 0; v < 9; v++)
        {
            firstZero(copies[tries]);
            int r = copies[tries].rowZero;
            int c = copies[tries].colZero;
            copies[tries].valueIndex = v;
            writeOnBoard(copies[tries]);

         //   cout << "v = " << v << endl;
            if((copies[tries].members[r][c].canHold[v])
               && (!(copies[tries].members[r][c].wasTested[v])))
            {
                copies[tries].members[r][c].value = v + 1;
                copies[tries].members[r][c].wasTested[v] = true;
                copies[tries].members[r][c].lastChanged = true;

                index++;
                cout << "Attempt index [" << setw(4) << index << "]: (";
                for (int i = 0; i < (tries); i++)
                {
                    cout << copies[i].valueIndex + 1 << ", ";
                }
                cout << copies[tries].valueIndex + 1 << ")\n";

                copies[tries].solved = false;
                while (copies[tries].solved != true)
                {
                    writeOnBoard(copies[tries]);

                    if (soleCandidate(copies[tries]))
                    {
                        copies[tries].wasChanged = true;
                    }

                    if (copies[tries].wasChanged != true)
                    {
                        if (uniqueCandidate(copies[tries]))
                        {
                            copies[tries].wasChanged = true;
                        }
                    }

                    if (copies[tries].wasChanged != true)
                    {
                        if (check(copies[tries]))
                        {
                            cout << "MAGIC!!!\n";
                            copies[tries].solved = true;
                            brd.members[r][c].value = v + 1;
                            brd.members[r][c].lastChanged = true;
                            brd.wasChanged = true;
                            cin.get();
                            return true;
                        }
                        else if (filled(copies[tries]))
                        {
                            backTrack(copies[tries]);
                            copies[tries].solved = true;
                        }
                        // for not dead end, create another puzzle
                        else if (!(filled(copies[tries])))
                        {
                            copies[tries].isNewCopy = true;
                            makeCopy(copies[tries + 1], copies[tries]);
                            tries++;
                            v = -1;
                            copies[tries].solved = true;
                        }
                    }
                    copies[tries].wasChanged = false;
                }
            }
            copies[tries].valueIndex++;
        }
        tries--;
        if (tries < 0)
        {
            cout << "ERROR!!!";
            cin.get();
        }
        backTrack(copies[tries]);
    }
    delete [] copies;

    return false;
}

void backTrack(Board &brd)
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            brd.members[r][c].value = brd.backTrack[r][c].value;
        }
    }
}

bool filled(Board brd)
{
    int counter = 0;
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            for (int v = 0; v < 9; v++)
            {
                if (brd.members[r][c].canHold[v])
                {
                    counter++;
                }
            }
        }
    }
    if (counter == 0)
        return true;
    else if (counter > 0)
        return false;
    else
        cout << "Error\n";

    return false;
}

void firstZero(Board &brd)
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (brd.members[r][c].value == 0)
            {
                brd.rowZero = r;
                brd.colZero = c;
                return;
            }
        }
    }
}

void makeCopy(Board &replica, Board brd)
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            replica.members[r][c].value = brd.members[r][c].value;
            replica.backTrack[r][c].value = brd.members[r][c].value;
            for (int v = 0; v < 9; v++)
            {
                replica.members[r][c].canHold[v] = brd.members[r][c].canHold[v];
                // New copies need fresh possibilities
                if (replica.isNewCopy)
                {
                    replica.members[r][c].wasTested[v] = false;
                }
            }
        }
    }
    if (replica.isNewCopy)
    {
        replica.valueIndex = 0;
    }
}

void initiateBoard(Board &brd, int difficulty)
{
    int easy[81] = {0,0,0,0,5,0,0,7,2,
                    5,0,3,2,1,0,0,0,8,
                    0,7,0,0,0,8,3,5,0,
                    0,0,9,0,6,2,0,4,5,
                    7,6,0,0,3,0,0,1,0,
                    1,0,0,0,9,4,0,3,0,
                    0,4,1,0,0,6,0,0,7,
                    6,0,0,0,7,9,0,8,0,
                    9,0,0,4,8,1,0,2,0};

    int medium[81] = {0,0,0,0,6,0,2,0,0,
                      9,0,0,0,3,4,1,6,0,
                      1,0,0,0,0,0,3,0,0,
                      0,0,5,4,0,8,9,0,0,
                      0,0,0,0,5,0,0,7,1,
                      2,0,1,0,0,9,0,0,5,
                      3,7,0,6,0,0,0,0,0,
                      0,0,8,0,0,1,7,4,0,
                      5,0,0,0,0,0,8,2,0};

    int hard[81] = {7,0,0,3,0,0,9,0,0,
                    0,0,6,0,8,0,0,0,1,
                    9,0,0,7,5,0,0,6,2,
                    0,0,4,0,0,9,0,0,3,
                    0,0,0,5,0,0,0,0,7,
                    0,0,0,1,0,0,6,4,0,
                    0,0,0,4,0,0,0,5,6,
                    0,6,9,0,0,0,0,0,0,
                    5,0,0,2,0,0,0,7,0};

    int expert[81] = {0,0,0,0,0,0,3,0,7,
                      0,0,4,9,0,0,0,8,0,
                      3,0,0,1,0,0,0,4,6,
                      0,9,0,0,5,0,0,1,0,
                      1,0,0,3,0,0,0,0,0,
                      0,0,8,0,0,6,7,0,0,
                      9,0,0,0,0,0,0,0,1,
                      0,0,0,0,0,0,0,0,5,
                      0,5,0,6,0,7,4,0,8};

    int worldsHardest[81] = {8,0,0,0,0,0,0,0,0,
                             0,0,3,6,0,0,0,0,0,
                             0,7,0,0,9,0,2,0,0,
                             0,5,0,0,0,7,0,0,0,
                             0,0,0,0,4,5,7,0,0,
                             0,0,0,1,0,0,0,3,0,
                             0,0,1,0,0,0,0,6,8,
                             0,0,8,5,0,0,0,1,0,
                             0,9,0,0,0,0,4,0,0};

    int difficult[81] = {0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,3,0,8,5,
                         0,0,1,0,2,0,0,0,0,
                         0,0,0,5,0,7,0,0,0,
                         0,0,4,0,0,0,1,0,0,
                         0,9,0,0,0,0,0,0,0,
                         5,0,0,0,0,0,0,7,3,
                         0,0,2,0,1,0,0,0,0,
                         0,0,0,0,4,0,0,0,9};

    switch (difficulty)
    {
        case 1: populateBoard(brd, easy);
        break;
        case 2: populateBoard(brd, medium);
        break;
        case 3: populateBoard(brd, hard);
        break;
        case 4: populateBoard(brd, expert);
        break;
        case 5: populateBoard(brd, worldsHardest);
        break;
        case 6: populateBoard(brd, difficult);
        break;

        default: cout << "Enter a valid puzzle." << endl;
    }
}

void populateBoard(Board &brd, int puzzle[81])
{
    // Missing number
    int missing[9] = {0,0,0,0,0,0,0,0,0};
    // Count the number for each value.
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (brd.members[r][c].value != 0)
            {
                missing[brd.members[r][c].value - 1]++;
            }
        }
    }
    for (int v = 0; v < 9; v++)
    {
        // if something is missing
        if (missing[v] == 0)
        {
            for (int r = 0; r < 9; r++)
            {
                for (int c = 0; c < 9; c++)
                {
                    brd.members[r][c].canHold[v] = true;
                }
            }
        }
    }

    for (int s = 0; s < 81; s++)
    {
        int r = s/9;
        int c = s%9;
        // Set all values
        brd.members[r][c].value = puzzle[s];
        // If a space is full, it cannot hold anything
        if (brd.members[r][c].value != 0)
        {
            for (int h = 0; h < 10; h++)
            {
                brd.members[r][c].canHold[h] = false;
            }
        }
    }
}

void printBoard(Board brd)
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << "|";
        for (int col = 3; col < 6; col++)
        {
            if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << "|";
        for (int col = 6; col < 9; col++)
        {
            if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << endl;
    }
    cout << "----------------------------\n";
    for (int row = 3; row < 6; row++)
    {
        for (int col = 0; col < 3; col++)
        {
             if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << "|";
        for (int col = 3; col < 6; col++)
        {
            if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << "|";
        for (int col = 6; col < 9; col++)
        {
            if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << endl;
    }
    cout << "----------------------------\n";
    for (int row = 6; row < 9; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << "|";
        for (int col = 3; col < 6; col++)
        {
            if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << "|";
        for (int col = 6; col < 9; col++)
        {
            if (brd.members[row][col].lastChanged)
            {
                cout << "[" << brd.members[row][col].value << "]";
            }
            else
            {
                cout << " " << brd.members[row][col].value << " ";
            }
        }
        cout << endl;
    }
}

void writeOnBoard(Board &brd)
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            brd.members[r][c].lastChanged = false;
            for (int v = 0; v < 9; v++)
            {
                brd.members[r][c].canHold[v] = true;
            }
        }
    }

    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (brd.members[r][c].value != 0)
            {
                for (int v = 0; v < 9; v++)
                {
                    brd.members[r][c].canHold[v] = false;
                }
            }
        }
    }

    // check for completely missing number
    // if so, set all non-filled squares to possible

   bool in[9];
   for (int v = 0; v < 9; v++)
        in[v] = true;

   for (int r = 0; r < 9; r++)
   {
       for (int c = 0; c < 9; c++)
       {
           if (brd.members[r][c].value != 0)
           {
               in[brd.members[r][c].value - 1 ] = false;
           }
       }
       for (int c = 0; c < 9; c++)
       {
           if (brd.members[r][c].value == 0)
           {
               for (int v = 0; v < 9; v++)
               {
                    if (in[v] != true)
                    {
                        brd.members[r][c].canHold[v] = in[v];
                    }
               }
           }
       }
       for (int v = 0; v < 9; v++)
          in[v] = true;
   }

   for (int c = 0; c < 9; c++)
   {
       for (int r = 0; r < 9; r++)
       {
           if (brd.members[r][c].value != 0)
           {
               in[brd.members[r][c].value - 1 ] = false;
           }
       }
       for (int r = 0; r < 9; r++)
       {
           if (brd.members[r][c].value == 0)
           {
               for (int v = 0; v < 9; v++)
               {
                   if (in[v] != true)
                   {
                        brd.members[r][c].canHold[v] = false;
                   }
               }
           }
       }
       for (int v = 0; v < 9; v++)
            in[v] = true;
   }

    for (int R = 0; R < 3; R++)
    {
        for (int C = 0; C < 3; C++)
        {
            for (int r = (3*R); r < (3 + (3*R)); r++)
            {
                for (int c = (3*C); c < (3+ (3*C)); c++)
                {
                    if (brd.members[r][c].value != 0)
                    {
                        in[brd.members[r][c].value - 1 ] = false;
                    }
                }
            }
            for (int r = (3*R); r < (3 + (3*R)); r++)
            {
                for (int c = (3*C); c < (3+ (3*C)); c++)
                {
                    if (brd.members[r][c].value == 0)
                    {
                        for (int v = 0; v < 9; v++)
                        {
                            if (in[v] != true)
                            {
                                brd.members[r][c].canHold[v] = false;
                            }
                        }
                    }
                }
            }
            for (int v = 0; v < 9; v++)
            in[v] = true;
        }
    }
}

bool soleCandidate(Board &brd)
{
    // Check for any space with only possible.
    int counter = 0;
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            for (int v = 0; v < 9; v++)
            {
                if (brd.members[r][c].canHold[v] != false)
                    counter++;
            }
            if (counter == 1)
            {
                for (int v = 0; v < 9; v++)
                {
                    if (brd.members[r][c].canHold[v] != false)
                    {
                        brd.members[r][c].value = v + 1;
                        brd.members[r][c].lastChanged = true;
                        brd.wasChanged = true;
                      //  cout << "Sole Candidate\n";
                        return true;
                    }
                }
            }
            counter = 0;
        }
    }
    return false;
}

bool check(Board brd)
{
    // Check row sums
    int total = 0;
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            total += brd.members[row][col].value;
        }
        if (total != 45)
            return false;
        total = 0;
    }
    // Check column sums
    for (int col = 0; col < 9; col++)
    {
        for (int row = 0; row < 9; row++)
        {
            total += brd.members[row][col].value;
        }
        if (total != 45)
            return false;
        total = 0;
    }
    // Check square sums
    for (int addRow = 0; addRow < 3; addRow++)
    {
        for (int addCol = 0; addCol < 3; addCol++)
        {
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                    total += brd.members[row][col].value;
            }
            if (total != 45)
                return false;
            total = 0;
        }
    }
    return true;
}

bool uniqueCandidate(Board &brd)
{
    // initialize
    int in[9];
    for (int v = 0; v <9; v++)
        in[v] = 0;


    for (int R = 0; R < 3; R++)
    {
        for (int C = 0; C < 3; C++)
        {
            for (int r = (3*R); r < (3 + (3*R)); r++)
            {
                for (int c = (3*C); c < (3 + (3*C)); c++)
                {
                    if (brd.members[r][c].value == 0)
                    {
                        for (int v = 0; v < 9; v++)
                        {
                            if (brd.members[r][c].canHold[v])
                            {
                                in[v]++;
                            }
                        }
                    }
                }
            }
            for (int v = 0; v < 9; v++)
            {
                if (in[v] == 1)
                {
                    for (int r = (3*R); r < (3 + (3*R)); r++)
                    {
                        for (int c = (3*C); c < (3 + (3*C)); c++)
                        {
                            if (brd.members[r][c].canHold[v])
                            {
                                brd.members[r][c].value = v + 1;
                                brd.members[r][c].lastChanged = true;
                                brd.wasChanged = true;
                             //   cout << "Unique\n";
                                return true;
                            }
                        }
                    }
                }
            }
            for (int v = 0; v < 9; v++)
            {
                in[v] = 0;
            }
        }
    }
    return false;
}

