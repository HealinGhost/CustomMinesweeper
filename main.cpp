#include <SFML/Graphics.hpp>
#include <iostream>

// Sweeper configuration
const int X = 8;
const int Y = 8;
const int bombs = 10;


void sweeper_generation(int minesweep[X][Y], int bombs);
void open_square(int showsquare[X][Y], int minesweep[X][Y], int x, int y);


// Main
int main()
{

    sf::RenderWindow window(sf::VideoMode(400, 400), "Minesweeper!");

    //Minesweeper square
    sf::RectangleShape MinesweeperSquare(sf::Vector2f(50.0f, 50.0f));
    
    //Textures
    sf::Texture MineSquare;
    MineSquare.loadFromFile("Sprites/SingleSquare1.png");
    MinesweeperSquare.setTexture(&MineSquare);
    float SquareSize =  MineSquare.getSize().x / 12.0f;
    
    int minesweep[8][8] = {0};
    int showSquare[8][8] = {0};
    sweeper_generation(minesweep, bombs);

    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            switch(evnt.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                    // Event when one of the mouse button is pressed
                case sf::Event::MouseButtonPressed:
                    if(evnt.mouseButton.button == sf::Mouse::Left)
                    {
                        if(showSquare[evnt.mouseButton.x / 50][evnt.mouseButton.y / 50] != 2)
                        {
                            open_square(showSquare, minesweep, evnt.mouseButton.x / 50, evnt.mouseButton.y / 50);
                        }
                    }
                    else if(evnt.mouseButton.button == sf::Mouse::Right)
                    {
                        if(showSquare[evnt.mouseButton.x / 50][evnt.mouseButton.y / 50] == 0)
                        {
                            showSquare[evnt.mouseButton.x / 50][evnt.mouseButton.y / 50] = 2;
                        }
                        else if(showSquare[evnt.mouseButton.x / 50][evnt.mouseButton.y / 50] == 2)
                        {
                            showSquare[evnt.mouseButton.x / 50][evnt.mouseButton.y / 50] = 0;
                        }
                    }
                    break;
            }
        }


        window.clear();
        
        for(int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                
                // If the square has been open, than show it
                MinesweeperSquare.setPosition(50.0f * i, 50.0f * j);
                if (showSquare[i][j] == 1)
                {
                    MinesweeperSquare.setTextureRect(sf::IntRect(SquareSize * minesweep[i][j], 0, 8, 8));
                }
                // If the square hasnt been opened yet, show closed square
                else if(showSquare[i][j] == 2)
                {
                    MinesweeperSquare.setTextureRect(sf::IntRect(SquareSize * 10, 0, 8, 8));
                }
                else
                {
                    MinesweeperSquare.setTextureRect(sf::IntRect(SquareSize * 11, 0, 8, 8));
                }
                
                window.draw(MinesweeperSquare);
            }
        }
        window.display();
    }
    return 0;
}

// Generation for minesweeper tilebox
void sweeper_generation(int minesweep[X][Y], int bombs)
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

// Function, defining what happens, when a square is opened
void open_square(int showsquare[X][Y], int minesweep[X][Y], int x, int y)
{
    showsquare[x][y] = 1;
    // If a square was blank, it opens all the square next to it
    if(minesweep[x][y] == 0)
    {
        //Left
        if(x - 1 >= 0 && showsquare[x - 1][y] == 0)
        {
            open_square(showsquare, minesweep, x - 1, y);
        }
        //Right
        if(x + 1 < X && showsquare[x + 1][y] == 0)
        {
            open_square(showsquare, minesweep, x + 1, y);
        }
        //Down
        if(y - 1 >= 0 && showsquare[x][y - 1] == 0)
        {
            open_square(showsquare, minesweep, x, y - 1);
        }
        //Up
        if(y + 1 < Y && showsquare[x][y + 1] == 0)
        {
            open_square(showsquare, minesweep, x, y + 1);
        }
        //Up right
        if(y + 1 < Y && x + 1 < X && showsquare[x + 1][y + 1] == 0)
        {
            open_square(showsquare, minesweep, x + 1, y + 1);
        }      
        //Up left
        if(y + 1 < Y && x - 1 >= 0 && showsquare[x - 1][y + 1] == 0)
        {
            open_square(showsquare, minesweep, x - 1, y + 1);
        }
        //Down right
        if(y - 1 >= 0 && x + 1 < X && showsquare[x + 1][y - 1] == 0)
        {
            open_square(showsquare, minesweep, x + 1, y - 1);
        }
        //Down left
        if(y - 1 >= 0 && x - 1 >= 0 && showsquare[x - 1][y - 1] == 0)
        {
            open_square(showsquare, minesweep, x - 1, y - 1);
        }
    }
    return;
}