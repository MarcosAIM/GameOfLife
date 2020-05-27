//
// Created by izaguirre on 5/26/2020.
//

#ifndef UNTITLED_GOFLIFE_H
#define UNTITLED_GOFLIFE_H

#include <fstream>
#include <iostream>
#include <cstdio>

class GofLife {
private:
    int ROWS = 0;         //stores the number of rows in the grid
    int COLUMNS = 0;      //stores the number of columns in the grid
    bool ** grid = nullptr;     // used for initial grid and passing grid to grid2
    bool ** grid2 = nullptr;    // used to iterate next generation
    char next_char = '0';   // used for reading from file next char
    char TEMP_FILE_NAME[14] = "temp_file.txt";

    int numOfNeighbors(int , int ); // counts neighbors
    bool liveOrDead (int , int , int ); // determines if alive or dead
    void valuesToGrid2(); // copies values to second grid
    int typeOfCell(int r, int c) const; // sees what type of cell
    int numOfNeighborsDisplay(int type_of_cell,int r, int c); // counts neighbors

public:
    void populateWorld(const char * file); // populates initial array
    void showWorld(); // displays array
    void iterateGeneration(); // applies rules to array


    ~GofLife();
};


#endif //UNTITLED_GOFLIFE_H
