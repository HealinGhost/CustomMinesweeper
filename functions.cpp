#include <iostream>

class func
{
    public:
        void sweeper_generation(int *minesweep, int bombs, int X, int Y);
};
// Generation for minesweeper tilebox
func::sweeper_generation(int *minesweep, int bombs, int X, int Y)
{
    // Bomb generation
    while(bombs > 0)
    {
        for(int i = 0; i < X; i++)
        {
            for(int j = 0; j < Y; j++)
            {
                // Modulus 16 produced 16 different options and 0 means that the square is empty
                if(std::rand() % 16 == 0 && minesweep[i][j] == 0 && bombs > 0)
                {
                    minesweep[i][j] = 9;
                    bombs--;
                }
            }
        }
    }
    // Number generation
    for(int i = 0; i < X; i++)
    {
        for(int j = 0; j < Y; j++)
        {
            int counter;
            // If the square is not a bomb count the squares
            if(minesweep[i][j] != 9)
            {
                counter = 0;
                //Left
                if(i - 1 >= 0 && minesweep[i - 1][j] == 9)
                {
                    counter++;
                }
                //Right
                if(i + 1 < X && minesweep[i + 1][j] == 9)
                {
                    counter++;
                }
                //Down
                if(j - 1 >= 0 && minesweep[i][j - 1] == 9)
                {
                    counter++;
                }
                //Up
                if(j + 1 < Y && minesweep[i][j + 1] == 9)
                {
                    counter++;
                }
                //Up right
                if(j + 1 < Y && i + 1 < X && minesweep[i + 1][j + 1] == 9)
                {
                    counter++;
                }
                
                //Up left
                if(j + 1 < Y && i - 1 >= 0 && minesweep[i - 1][j + 1] == 9)
                {
                    counter++;
                }

                //Down right
                if(j - 1 >= 0 && i + 1 < X && minesweep[i + 1][j - 1] == 9)
                {
                    counter++;
                }
                //Down left
                if(j - 1 >= 0 && i - 1 >= 0 && minesweep[i - 1][j - 1] == 9)
                {
                    counter++;
                }
                minesweep[i][j] = counter;
            }
        }
    }
    return;
}