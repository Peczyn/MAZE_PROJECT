#include <iostream>
#include "SFML/Graphics.hpp"
#include </Users/marcin/CLionProjects/maze_solving_project/main.cpp>
using namespace std;

float cellSize = 5.0f;
const float cellScale = 2.0f;

sf::Color custom(0x5E5E5EFF); // 0xF6E7CBFF
sf::Color custom2(0xA9DEF9FF); //0xA9DEF9FF
sf::RectangleShape cell(sf::Vector2f(cellSize*cellScale, cellSize*cellScale));


class Display: public MazeGenerator
{
public:
    sf::RenderWindow window;
    sf::RenderTexture renderTexture;

    unsigned int Width;
    unsigned int Height;

public:
    Display()
    {
//        sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
//        Width = desktopMode.width/2-100;
//        Height = desktopMode.height/2-100;
//        window.create(sf::VideoMode(desktopMode.width, desktopMode.height), "Labirynt");

        Width=600;
        Height=600;
        window.create(sf::VideoMode(Width, Height), "Labirynt");

        window.clear();

    }
};






void getGridTexture(vector<vector<char>> grid, sf::RenderTexture &renderTexture);

void drawSolution(MazeSolver &y, sf::RenderTexture &renderTexture, float &i ,float &j);



int main()
{
    MazeGenerator x;
    MazeSolver y(x);

    Display display;

    sf::Vector2i mousePos;

    //MIEJSCA POD TEKST
    sf::RectangleShape inputField1(sf::Vector2f(display.Width-display.Width/4 , 100));
    inputField1.setFillColor(sf::Color::Black);
    inputField1.setOutlineThickness(5);
    inputField1.setOutlineColor(sf::Color::Red);
    inputField1.setPosition(display.Width/8, display.Height/4+30);

    sf::RectangleShape inputField2(sf::Vector2f(display.Width-display.Width/4 , 100));
    inputField2.setFillColor(sf::Color::Black);
    inputField2.setOutlineThickness(1);
    inputField2.setOutlineColor(sf::Color::Blue);
    inputField2.setPosition(display.Width/8, display.Height/2+30);

    //CZCZIONKA
    sf::Font font;
    if (!font.loadFromFile("../fonts/Satoshi-Regular.ttf")) {
        std::cerr << "Cannot load font" << std::endl;
        return -1;
    }
    font.setSmooth(true);

    //TEKST
    sf::Text mainText("Maze Solver", font, 110);
    mainText.setFillColor(sf::Color(0xF6E7CBFF));
    mainText.setPosition(5,display.Height/32);

    sf::Text inputText1("Width: ", font, 90);
    inputText1.setFillColor(sf::Color::White);
    inputText1.setPosition(inputField1.getPosition().x+5,inputField1.getPosition().y-15);

    sf::Text inputText2("Heigth: ", font, 90);
    inputText2.setFillColor(sf::Color::White);
    inputText2.setPosition(inputField2.getPosition().x+5,inputField2.getPosition().y-15);

    //ZMIENNE PRZECHOWUJACE WPISANY TEKST/LICZBY
    string sizeX;
    string sizeY;


    //ZMIENNE KONTROLUJACE PETLE
    bool menuIsRunning = true;
    bool renderingTexture = false;
    bool isRunning = false;
    bool a = true;



    float j;
    float i;
    int test = 0;



    int ColorCounter = 0;
    int temp;

    int viewScale = 1;



    sf::View view = display.window.getDefaultView();

    sf::RectangleShape cellBEGIN(sf::Vector2f(cellSize*cellScale, cellSize*cellScale));
    sf::RectangleShape cellEND(sf::Vector2f(cellSize*cellScale, cellSize*cellScale));

    bool zoomedIn;


    while(display.window.isOpen()) {//display.window.isOpen()
        sf::Event event;
        sf::Sprite sprite;

        while (display.window.pollEvent(event))
        {
            //ZAMYKANIE OKNA PRZEZ ESC LUB ZAMKNIECIE MYSZKA
            if (event.type == sf::Event::Closed) {
                display.window.close();
            }
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                display.window.close();
            }
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K && viewScale>1)
            {
                zoomedIn = !zoomedIn;
            }




            //GDY WPISUJEMY TEKST/LICZBY
            if(event.type == sf::Event::TextEntered && event.text.unicode < 128)
            {
                char typedChar = static_cast<char>(event.text.unicode);

                if(test==0) {
                    if (typedChar == '\b' && !inputText1.getString().isEmpty()) {
                        string currentText = inputText1.getString().toAnsiString();
                        currentText.pop_back();
                        inputText1.setString(currentText);

                        sizeX.pop_back();
                    }
                    else if (isnumber(typedChar)) {
                        sizeX.push_back(typedChar);
                        inputText1.setString(inputText1.getString() + typedChar);

                    }
                    else if (typedChar == '\n') {
                        test++;
                        inputField1.setOutlineThickness(1);
                        inputField2.setOutlineThickness(5);
                    }
                }
                else if(test==1) {
                    if (typedChar == '\b' && !inputText2.getString().isEmpty()) {
                        string currentText = inputText2.getString().toAnsiString();
                        currentText.pop_back();
                        sizeY.pop_back();
                        inputText2.setString(currentText);
                    }
                    else if (isnumber(typedChar)) {

                        inputText2.setString(inputText2.getString() + typedChar);
                        sizeY.push_back(typedChar);
                    }
                    else if (typedChar == '\n') {
                        test++;
                        menuIsRunning = false;
                        renderingTexture = true;
                        inputField2.setOutlineThickness(1);
                    }
                }
                else {

                    if (typedChar == '\n') {
                        isRunning = !isRunning;
                    }
                    else if (typedChar == ' ') {
                        display.window.close();

                        custom2 = sf::Color(0xA9DEF9FF);

                        main();
                    }
                }









            }



        }

        if(menuIsRunning) //Wyświetla Menu
        {
            display.window.clear();
            display.window.draw(inputField1);
            display.window.draw(inputField2);

            display.window.draw(mainText);
            display.window.draw(inputText1);
            display.window.draw(inputText2);
            display.window.display();
        }

        if(renderingTexture)
        {
            display.window.clear();
            int d = stoi(sizeX);
            sizeX = "";
            int e = stoi(sizeY);
            sizeY = "";

//            cellSize=min(display.Width/3/d,display.Height/3/e)+0.5;


            x.GenerateFullMaze(d,e);
            y.matrix = x.matrix;
            y.GenerateSolutionForMaze();

            j=x.mWidth*3;
            i=x.mHeight*3;


            temp =  y.moves.size()/20;
//            display.window.setFramerateLimit(y.moves.size()/6);


            cellBEGIN.setPosition(cellSize - cellSize/cellScale, cellSize - cellSize/cellScale);
            cellBEGIN.setFillColor(sf::Color(0xDB2955FF));
            cellEND.setPosition((x.mWidth*3-2)*cellSize-cellSize/cellScale, (x.mHeight*3-2)*cellSize-cellSize/cellScale);
            cellEND.setFillColor(sf::Color(0xDB2955FF));

            display.Height= x.mHeight*3*cellSize;
            display.Width = x.mWidth*3*cellSize;

            while(display.Width/viewScale>1300 || display.Height/viewScale>800)
            {
                viewScale++;
            }

            display.window.setSize({display.Width/viewScale,display.Height/viewScale});



            display.renderTexture.create(display.Width, display.Height);



            getGridTexture(x.matrix, display.renderTexture);
            display.renderTexture.draw(cellBEGIN);
            display.renderTexture.draw(cellEND);
            sprite.setTexture(display.renderTexture.getTexture());





            view.setCenter(display.renderTexture.getDefaultView().getCenter());    // Ustaw środek widoku na (400, 300)
            view.setSize(display.renderTexture.getSize().x+10, display.renderTexture.getSize().y+10);      // Ustaw rozmiar widoku na (800, 600)
//            view.setViewport(sf::FloatRect(0.25f, 0.25f, 0.5f, 0.5f)); // Ustaw widok na środkową część okna
//            view.setRotation(90);        // Obróć widok o 45 stopni
//            view.zoom(0.25f);             // Oddal widok o połowę


            display.window.setView(view);
            display.window.draw(sprite);

            display.window.display(); ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            renderingTexture=false;


        }





        mousePos = sf::Mouse::getPosition(display.window);
        if (zoomedIn && display.window.hasFocus())
        {
            sf::View zoomedView(sf::FloatRect(mousePos.x*viewScale -0.5* view.getSize().x/viewScale+2 ,mousePos.y*viewScale -0.5*view.getSize().y/viewScale+2 ,view.getSize().x/viewScale+2,view.getSize().y/viewScale+2));
            display.window.setView(zoomedView);
        }
        else
        {
            display.window.setView(view);
        }






        if(isRunning){ //Wyświetla labirynt

            display.window.clear();

            //to dziala git

            display.renderTexture.draw(cellBEGIN);
            display.renderTexture.draw(cellEND);

            //starting pointy i,j
            drawSolution(y, display.renderTexture, i, j);




            sprite.setTexture(display.renderTexture.getTexture());
            display.window.draw(sprite);

            display.window.display();






            ColorCounter++;
            if(ColorCounter==temp && ColorCounter<y.moves.size())
            {
                custom2.r *=1.009; //1.015
                custom2.g *= 0.93; //0.94
                custom2.b *= 1.003; //off
                ColorCounter=0;
            }
        }


    }

    return 0;
}






void getGridTexture(vector<vector<char>> grid, sf::RenderTexture &renderTexture) {
    renderTexture.clear();
    for(int f=0; f<grid.size(); f++)
    {
        for(int g=0; g<grid[0].size(); g++)
        {
            if (grid[f][g] != '@') {

                sf::RectangleShape cell(sf::Vector2f(cellSize*cellScale, cellSize*cellScale));

                cell.setPosition(g * cellSize-cellSize/cellScale, f * cellSize-cellSize/cellScale);
                cell.setFillColor(custom);

                renderTexture.draw(cell);
            }
        }
    }
}

void drawSolution(MazeSolver &y, sf::RenderTexture &renderTexture, float &i ,float &j) {
    if(!y.moves.empty())
    {
        if(y.moves.top() == 'N')
        {
            i++;
        }
        else if(y.moves.top() == 'E')
        {
            j--;
        }
        else if(y.moves.top() == 'S')
        {
            i--;
        }
        else if(y.moves.top() == 'W')
        {
            j++;
        }
        else if(y.moves.top()=='X')
        {
            y.moves.pop();
            return;
        }

        cell.setPosition((j-2) * cellSize-cellSize/cellScale, (i-2) * cellSize-cellSize/cellScale);
        cell.setFillColor(custom2);

        renderTexture.draw(cell);




        y.moves.pop();
    }
}