#include <SFML/Graphics.hpp>
#include <iostream>

// Sweeper configuration
const int bombConstant = 8;
const int uiSpace = 50;

void sweeper_generation(int minesweep[], int bombs, int avoidSquare);
void open_square(int showsquare[], int minesweep[], int &remSquares, int pos);
void clearTheField(int Array[]);


// Main
int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 450), "Minesweeper!", sf::Style::Titlebar | sf::Style::Close);

    //Minesweeper square
    sf::RectangleShape MinesweeperSquare(sf::Vector2f(50.0f, 50.0f));
    sf::RectangleShape Ui(sf::Vector2f(400, float(uiSpace)));
    sf::RectangleShape CounterNumber(sf::Vector2f(25.0f, 50.0f));
    
    //Textures
    sf::Texture MineSquareSprite;
    sf::Texture UiSprite;
    sf::Texture GreenNumberSprite;
    MineSquareSprite.loadFromFile("Sprites/SingleSquare1.png");
    UiSprite.loadFromFile("Sprites/ui.png");
    GreenNumberSprite.loadFromFile("Sprites/GreenNumbers.png");
    MinesweeperSquare.setTexture(&MineSquareSprite);
    Ui.setTexture(&UiSprite);
    CounterNumber.setTexture(&GreenNumberSprite);
    float SquareSize =  MineSquareSprite.getSize().x / 12.0f; // a single minesweeper square size
    int remSquares = 64 - bombConstant; // Integer counting the remaining squares
    bool gameOver = false; // Bool that defines if the game should be stopped
    bool minesweepIsEmpty = true; // Bool that defines if the minesweeper has not been generated
    int minesweep[64] = {0}; // Array responsible for storring square information, such as( bomb/how many bombs next to it)
    int showSquare[64] = {0}; // Array defining if a particular square should be oppened
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
                    // Event when mouse Button is pressed not in the Ui
                    if(evnt.mouseButton.y >= uiSpace)
                    {
                        if(evnt.mouseButton.button == sf::Mouse::Left)
                        {
                            // Generates the field in such a way that the first square will never be a bomb
                            if(minesweepIsEmpty)
                            {
                                sweeper_generation(minesweep, bombConstant, (evnt.mouseButton.x / 50) + (evnt.mouseButton.y - uiSpace) / 50 * 8);
                                minesweepIsEmpty = false;
                            }
                            if(showSquare[(evnt.mouseButton.x / 50) + (evnt.mouseButton.y - uiSpace) / 50 * 8] != 2 && gameOver == false)
                            {
                                open_square(showSquare, minesweep, remSquares, (evnt.mouseButton.x / 50) + (evnt.mouseButton.y - uiSpace) / 50 * 8);
                                if(minesweep[(evnt.mouseButton.x / 50) + (evnt.mouseButton.y - uiSpace) / 50 * 8] == 9)
                                {
                                    gameOver = true;
                                }
                            }
                        }
                        // Placing flags
                        else if(evnt.mouseButton.button == sf::Mouse::Right && gameOver == false)
                        {
                            if(showSquare[evnt.mouseButton.x / 50 + (evnt.mouseButton.y - uiSpace) / 50 * 8] == 0)
                            {
                                showSquare[evnt.mouseButton.x / 50 + (evnt.mouseButton.y - uiSpace) / 50 * 8] = 2;
                            }
                            else if(showSquare[evnt.mouseButton.x / 50 + (evnt.mouseButton.y - uiSpace) / 50 * 8] == 2)
                            {
                                showSquare[evnt.mouseButton.x / 50 + (evnt.mouseButton.y - uiSpace) / 50 * 8] = 0;
                            }
                        }
                    }
                    // When mouse button is pressed in the Ui
                    else
                    {
                        if(evnt.mouseButton.button == sf::Mouse::Left)
                        {
                            //Restart button
                            if(evnt.mouseButton.x >= 350)
                            {
                                gameOver = false;
                                remSquares = 64 - bombConstant; // Resets the counter
                                clearTheField(showSquare);// Hides all of the squares
                                minesweepIsEmpty = true; // Set the bool to true,cause the array needs to be refreshed
                            }
                        }
                    }
                break;        
            }   
        }


        window.clear();

        //Drawing the UI
        window.draw(Ui);
        CounterNumber.setTextureRect(sf::IntRect(SquareSize * (remSquares / 10), 0, 8, 8));
        CounterNumber.setPosition(0,0);
        window.draw(CounterNumber);
        CounterNumber.setTextureRect(sf::IntRect(SquareSize * (remSquares % 10), 0, 8, 8));
        CounterNumber.setPosition(25.0f, 0);
        window.draw(CounterNumber);


        // Drawing the minesweeper layout
        for(int i = 0; i < 64; i++){
                
            // If the square has been open, than show it
            MinesweeperSquare.setPosition(50.0f * (i % 8), (50.0f * (i / 8)) + uiSpace);
            if (showSquare[i] == 1)
            {
                MinesweeperSquare.setTextureRect(sf::IntRect(SquareSize * minesweep[i], 0, 8, 8));
            }
            // If the square hasnt been opened yet, show closed square
            else if(showSquare[i] == 2)
            {
                MinesweeperSquare.setTextureRect(sf::IntRect(SquareSize * 10, 0, 8, 8));
            }
            else
            {
                MinesweeperSquare.setTextureRect(sf::IntRect(SquareSize * 11, 0, 8, 8));
            }
            window.draw(MinesweeperSquare);
        }
        window.display();
    }
    return 0;
}

// Generation for minesweeper tilebox
void sweeper_generation(int minesweep[], int bombs, int avoidSquare)
{
    
    clearTheField(minesweep); // Turns every square into 0

    // Bomb generation
    while(bombs > 0)
    {
        for(int i = 0; i < 64; i++)
        {
            // Modulus 16 produced 16 different options and 0 means that the square is empty
            if(std::rand() % 16 == 0 && minesweep[i] == 0 && bombs > 0 && i != avoidSquare)
            {
                minesweep[i] = 9;
                bombs--;
            }
        }
    }
    // Number generation
    for(int i = 0; i < 64; i++)
    {
        int counter;
        // If the square is not a bomb count the squares
        if(minesweep[i] != 9)
        {
            counter = 0;
            //Left
            if(i - 1 >= 0 && minesweep[i - 1] == 9 && i % 8 != 0)
            {
                counter++;
            }
            //Right
            if(i + 1 < 64 && minesweep[i + 1] == 9 && i % 8 != 7)
            {
                counter++;
            }
            //Down
            if(i + 8 < 64 && minesweep[i + 8] == 9)
            {
                counter++;
            }
            //Up
            if(i - 8 >= 0 && minesweep[i - 8] == 9)
            {
                counter++;
            }
            //Up right
            if(i - 7 >= 0 &&  i % 8 != 7 && minesweep[i - 7] == 9)
            {
                counter++;
            }
            //Up left
            if(i - 9 >= 0 && i % 8 != 0 && minesweep[i - 9] == 9)
            {
                counter++;
            }
            //Down right
            if(i + 9 < 64 && i % 8 != 7 && minesweep[i + 9] == 9)
            {
                 counter++;
            }
            //Down left
            if(i + 7 < 64 && i % 8 != 0 && minesweep[i + 7] == 9)
            {
                counter++;
            }
            minesweep[i] = counter;
        }
    }
    return;
}

// Function, defining what happens, when a square is opened
void open_square(int showsquare[], int minesweep[], int &remSquares, int pos)
{
    if(showsquare[pos] != 1)
    {
        showsquare[pos] = 1;
        // if not bomb, then do not lower the remainign square counter.
        if(minesweep[pos] != 9)
        {
            remSquares--;
        }
    }
    
    // If a square was blank, it opens all the square next to it
    if(minesweep[pos] == 0)
    {
        //Left
        if(pos % 8 != 0 && showsquare[pos - 1] == 0)
        {
            open_square(showsquare, minesweep, remSquares, pos - 1);
        }
        //Right
        if(pos % 8 != 7 && showsquare[pos + 1] == 0)
        {
            open_square(showsquare, minesweep, remSquares, pos + 1);
        }
        //Down
        if(pos + 8 < 64 && showsquare[pos + 8] == 0)
        {
            open_square(showsquare, minesweep, remSquares, pos + 8);
        }
        //Up
        if(pos - 8 >= 0 && showsquare[pos - 8] == 0)
        {
            open_square(showsquare, minesweep, remSquares, pos - 8);
        }
        //Up right
        if(pos % 8 != 7 && pos - 7 >= 0 && showsquare[pos - 7] == 0)
        {
            open_square(showsquare, minesweep, remSquares, pos - 7);
        }      
        //Up left
        if(pos % 8 != 0 && pos - 9 >= 0 && showsquare[pos - 9] == 0)
        {
            open_square(showsquare, minesweep, remSquares, pos - 9);
        }
        //Down right
        if(pos % 8 != 7 && pos + 9 < 64 && showsquare[pos + 9] == 0)
        {
            open_square(showsquare, minesweep, remSquares, pos + 9);
        }
        //Down left
        if(pos % 8 != 0 && pos + 7 < 64 && showsquare[pos + 7] == 0)
        {
            open_square(showsquare, minesweep, remSquares, pos + 7);
        }
    }
    return;
}

// Turns all of the arrays positions into 0
void clearTheField(int Array[])
{
    for(int i = 0; i < 64; i++)
    {
        Array[i] = 0;
    }
    return;
}