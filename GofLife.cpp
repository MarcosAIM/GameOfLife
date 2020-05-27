#include "GofLife.h"

/*
The void function populateWorld opens an input file and counts how many ROWS and
COLUMNS it has, by:
ROWS: all /n symbols +1
all_chars :all characters ie.(0,1,/n)
COLUMNS: ( all_chars - ROWS) / ROWS
Then it stores these numbers in the Global Variables ROWS and COLUMNS.

After it dynamically allocates memory to the Global bool pointer grid using ROWS
and COLUMNS as dimensions.
After allocating memory it resets the buffer to the beggining using seekg
function. Then it reads all 0,1 into the 2d bool array grid.
Finally it closes the file.

Receives: const char pointer. in main file name is grid_gun_fight.txt
Globals: fstream fin;
         int ROWS, COLUMNS;
         bool ** grid;
dynamic memory:
created: grid[ROWS][COLUMNS];
*/
void GofLife:: populateWorld (const char * file){
    std::fstream fin, fout;
    fin.open( file, std::ios::in ); // opens file
    if( !fin ) // tests if it did not open
    {
        std::cout << "Error: input file did not open.";
        exit( EXIT_FAILURE );
    }

    int number_of_chars = 0,// counts total num of chars including 0,1,\n
    col = 0, // to run through columns
    row = 0; // to run through rows

    while( fin.peek() != EOF ) // reads until it peeks EOF
    {
        fin.get( next_char ); // gets a char from input file stream
        if( next_char == '\n' ) // compares next_char to \n
            ROWS++; // counts a row when it sees a \n
        number_of_chars++; // adds one every char, counts all chars
    }
    //counts initial file rows and columns
    number_of_chars-=ROWS++; // subtracts all \n, then adds one to ROWS
    COLUMNS = number_of_chars/ROWS; // gets total number of columns
    //allocating memory for grid
    grid = new bool*[ROWS];
    for(int i = 0; i < ROWS; i++)
        grid[i] = new bool[COLUMNS];

    fin.seekg( 0, std::fstream::beg ); // resets position back to the beginning

    while( fin.peek() != EOF ) // reads file until it peeks EOF
    {
        fin.get( next_char ); // gets next char

        if ( next_char == '\n' ) // counts \n to rows and resets col number
        {
            row++;
            col = 0;
        }
        else
        {
            grid[row][col] = next_char != '0'; // sets bool to true or false
            col++;
        }
    }

    fin.close();
}

/*
The void function showWorld creates a dynamic array named display with size
COLUMNS. Then it opens a file named in the const char array TEMP_FILE_NAME,
outputting the bool grid array into the file.
It then outputs zeros and ones into the console using the diplay char array as
a display.
Using the logic that zeros with no neighbors who are 1 are not part of the
single boundary of dead cells instead it outputs a space.
After it releases and sets to null the dynamic arrays display and grid.

Globals: fstream fout;
         int ROWS, COLUMNS;
         bool ** grid;
         char display[COLUMNS];

dynamic memory:
created: display[COLUMNS];
released: display[COLUMNS],
          grid[ROWS][COLUMNS];
*/
void GofLife:: showWorld (){
    char * display;
    std::fstream fin, fout;
    display = new char[COLUMNS+1]; // dynamically allocates memory for display
    display[COLUMNS] = '\0'; // sets last element to null terminator

    fout.open(TEMP_FILE_NAME,std::ios::out); // opens file output
    if (!fout)
    {
        std::cout << "Error: output file did not open. ";
        exit(EXIT_FAILURE);
    } // checks if it opens


    for ( int r = 0; r < ROWS; r++ ) // traverses grid array
    {
        for( int c = 0; c < COLUMNS ; ++c )
        {
            if ( c+1 == COLUMNS ){
                if( r+1 == ROWS){
                    fout << grid[r][c]; // last element must be printed only
                } else{
                    fout << grid[r][c]<<'\n'; //last element in a row,start next row
                }
            }
            else
            {
                fout << grid[r][c]; // other elements
            }

            int type_of_cell =  typeOfCell( r,  c); // used in numOfNeighborsDisplay
            int number_of_neighbors = numOfNeighborsDisplay( type_of_cell, r,  c);
            if (!grid[r][c] && number_of_neighbors == 0)
                display[c] = ' '; // space if is dead and no neighbors
            else if(!grid[r][c]) // otherwise one or zero
                display[c] = '0';
            else
                display[c] = '1';

        }
        std::cout << display << std::endl; // displays all the 0 and 1 in the row
    }

    for(int i = 0; i < ROWS; i++) // releases grid memory
        delete [] grid[i];
    delete []grid;
    grid = nullptr;

    delete []display; // releases display memory
    display = nullptr;

    fout.close(); // closes file
}

/*
The function iterateGeneration iterates from grid to grid2 using the game of
life rules.
It allocates dynamic memory for two bool arrays, grid[ROWS][COLUMNS],
grid2[ROWS+2][COLUMNS+2].
Then it opens the temp file and it copies the file to the grid array.
It then copies the grid into grid2, however grid two surrounds the copied data
with zeroes.
Ex
        0000000
11111   0111110
11111   0111110
11111   0111110
        0000000

Then it uses grid2 to see if the cell is dead or alive and it stores it into
grid.
After it releases grid2.

Globals: fstream fin;
         int ROWS, COLUMNS;
         bool ** grid;
         bool ** grid2;

dynamic memory:
created:  grid2[ROWS][COLUMNS]
released: grid2[ROWS][COLUMNS];

*/
void GofLife:: iterateGeneration (){
    int row = 0, // used when reading file
    col = 0; // used when reading file
    std::fstream fin, fout;
    // dynamically creates grid array
    grid = new bool*[ROWS];
    for(int i = 0; i < ROWS; i++)
        grid[i] = new bool[COLUMNS];

    // dynamically creates grid2 array
    grid2 = new bool*[ROWS+2];
    for(int i = 0; i < ROWS+2; i++)
        grid2[i] = new bool[COLUMNS+2];

    fin.open(TEMP_FILE_NAME,std::ios::in); // opens file in input mode
    if (!fin)
    {
        std::cout << "Error: input file did not open. -on iteration ";
        exit(EXIT_FAILURE);
    }

    while(fin.peek() != EOF) // reads file to it peeks EOF
    {
        fin.get(next_char);

        if (next_char == '\n')
        {
            row++; // add row traverse through row
            col = 0; // resets col
        }
        else
        {
            grid[row][col] = next_char != '0'; // sets bool to true
            col++; // add col traverse through columns
        }
    }

    valuesToGrid2(); // assigns grid to grid 2

    for (int r=0;r<ROWS;r++)
    {
        for(int c=0;c<COLUMNS;++c)
        {
            int num_of_neighbors = numOfNeighbors(r, c); // determines neighbors
            grid[r][c] = liveOrDead(r, c,num_of_neighbors); // live or dead
        }
    }
    // releases grid2
    for(int i = 0; i < ROWS; i++)
        delete [] grid2[i];
    delete []grid2;
    grid2 = nullptr;

    fin.close();
}

/*
The void function valuesToGrid2 transfers grid values to grid2 and sets all
surrounding elements to zero.
*/
void GofLife:: valuesToGrid2(){
    for (int co = 0; co < COLUMNS+2; co++ )
    {
        grid2[0][co] = false;
        grid2[ROWS+1][co] = false;
    }

    for ( int ro = 1; ro < ROWS; ro ++ )
    {
        grid2[ro][0] = false;
    }

    for ( int ro = 0; ro < ROWS; ro++ )
    {
        for( int  co=0;co<COLUMNS;++co)
            grid2[ro+1][co+1] = grid[ro][co];
    }
}

/*
This function returns the number of neighbors. However the array must have a
surrounding layer of zeroes for this function to work.
It looks at the cell's 6 neighbors and determines how many are alive.
Receives: current row, current column.
Returns: number of alive neighbors
*/
int GofLife:: numOfNeighbors(int r, int c){
    int num_of_neighbors=0;

    num_of_neighbors+=grid2[r][c];
    num_of_neighbors+=grid2[r][c+1];
    num_of_neighbors+=grid2[r][c+2];
    num_of_neighbors+=grid2[r+1][c];
    num_of_neighbors+=grid2[r+1][c+2];
    num_of_neighbors+=grid2[r+2][c];
    num_of_neighbors+=grid2[r+2][c+1];
    num_of_neighbors+=grid2[r+2][c+2];

    return num_of_neighbors;
}

/*
This function determines if a cell is alive or dead by game of life rules
Alive: 3 neighbors,
       alive and 2 neighbors.
Dead: alive and 0,1, or more than 4 neighbors,
      dead and neighbors does not equal 3.

Receives: current state of cell via Global array grid2, and current row and
column.
Returns: bool, true or false.
*/
bool GofLife:: liveOrDead (int r, int c, int num_of_neighbors){
    if(num_of_neighbors == 3)
        return true;

    bool live_or_dead = grid2[r+1][c+1];

    if(live_or_dead && (num_of_neighbors == 0 || num_of_neighbors == 1 || num_of_neighbors >= 4))
        return false;
    else
        return live_or_dead;
}

/*
This function determines what type of cell, it is currently on. Used on grid
array. Since it doesn't have zeros surronding it.
Type of cells are described as follows
    15555552
    70000008
    70000008
    36666664
This is used to determine what neighbors each cell have.

Receives:  current row andcolumn.
Returns: int of type of cell.

*/
int GofLife:: typeOfCell(int r, int c) const{
    if ( r != 0 && c !=0 && r!= ROWS-1 && c != COLUMNS-1)
        return 0;
    else if(r == 0 && c == 0)
        return 1;
    else if (r == 0 && c == COLUMNS-1)
        return 2;
    else if (r == ROWS-1 && c == 0 )
        return 3;
    else if(r == ROWS-1 && c == COLUMNS-1 )
        return 4;
    else if (r == 0)
        return 5;
    else if (r == ROWS-1)
        return 6;
    else if (c == 0)
        return 7;
    else
        return 8;
}

/*
This function uses type of cell int and looks at the corresponding neighbors.
It is used on array with no surrounding zeros. Takes into account position of
cell.

Receives:  current row and column, type of cell.
Returns: number of neighbors.
*/
int GofLife:: numOfNeighborsDisplay(int type_of_cell,int r, int c){
    int num_of_neighbors=0;

    if(type_of_cell == 0)
    {
        num_of_neighbors+=grid[r-1][c-1];
        num_of_neighbors+=grid[r-1][c];
        num_of_neighbors+=grid[r-1][c+1];
        num_of_neighbors+=grid[r][c-1];
        num_of_neighbors+=grid[r][c+1];
        num_of_neighbors+=grid[r+1][c-1];
        num_of_neighbors+=grid[r+1][c];
        num_of_neighbors+=grid[r+1][c+1];

        return num_of_neighbors;
    }
    else if(type_of_cell == 5)
    {
        num_of_neighbors+=grid[r][c-1];
        num_of_neighbors+=grid[r][c+1];
        num_of_neighbors+=grid[r+1][c-1];
        num_of_neighbors+=grid[r+1][c];
        num_of_neighbors+=grid[r+1][c+1];

        return num_of_neighbors;
    }
    else if(type_of_cell == 6)
    {
        num_of_neighbors+=grid[r][c-1];
        num_of_neighbors+=grid[r][c+1];
        num_of_neighbors+=grid[r-1][c-1];
        num_of_neighbors+=grid[r-1][c];
        num_of_neighbors+=grid[r-1][c+1];

        return num_of_neighbors;
    }
    else if(type_of_cell == 7)
    {
        num_of_neighbors+=grid[r-1][c];
        num_of_neighbors+=grid[r-1][c+1];
        num_of_neighbors+=grid[r][c+1];
        num_of_neighbors+=grid[r+1][c];
        num_of_neighbors+=grid[r+1][c+1];

        return num_of_neighbors;
    }
    else if(type_of_cell == 8)
    {
        num_of_neighbors+=grid[r-1][c-1];
        num_of_neighbors+=grid[r-1][c];
        num_of_neighbors+=grid[r][c-1];
        num_of_neighbors+=grid[r+1][c-1];
        num_of_neighbors+=grid[r+1][c];

        return num_of_neighbors;
    }
    else if(type_of_cell == 1)
    {
        num_of_neighbors+=grid[r][c+1];
        num_of_neighbors+=grid[r+1][c];
        num_of_neighbors+=grid[r+1][c+1];

        return num_of_neighbors;
    }
    else if(type_of_cell == 2)
    {
        num_of_neighbors+=grid[r][c-1];
        num_of_neighbors+=grid[r+1][c];
        num_of_neighbors+=grid[r+1][c-1];

        return num_of_neighbors;
    }
    else if(type_of_cell == 3)
    {
        num_of_neighbors+=grid[r-1][c];
        num_of_neighbors+=grid[r-1][c+1];
        num_of_neighbors+=grid[r][c+1];

        return num_of_neighbors;
    }
    else
    {
        num_of_neighbors+=grid[r][c-1];
        num_of_neighbors+=grid[r-1][c];
        num_of_neighbors+=grid[r-1][c-1];

        return num_of_neighbors;
    }
}

GofLife::~GofLife() {
    remove(TEMP_FILE_NAME);
}

