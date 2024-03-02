#include "src/SFML.cpp"
#include "src/MazeGeneratorSolver.cpp"


using namespace std;






int main() {
    MazeGenerator maze;
    MazeSolver mazeSolution(maze);

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
    if (!font.loadFromFile("fonts/Satoshi-Regular.ttf")) {
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
    string mazeWidthInput;
    string mazeHeigthInput;


    //ZMIENNE KONTROLUJACE PETLE
    bool menuIsRunning = true;
    bool renderingTexture = false;
    bool isRunning = false;
    bool zoomedIn = false;
    int enteringTextNR = 0;

    float startingPointWidth;
    float startingPointHeigth;




    int ColorCounter = 0;
    int colorSwitchCounter;

    int viewScale = 1;



    sf::View view = display.window.getDefaultView();

    sf::RectangleShape cellBEGIN(sf::Vector2f(cellSize*cellScale, cellSize*cellScale));
    sf::RectangleShape cellEND(sf::Vector2f(cellSize*cellScale, cellSize*cellScale));

    sf::Event event;
    sf::Sprite sprite;



    while(display.window.isOpen()) {
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

                if(enteringTextNR == 0) {
                    if (typedChar == '\b' && !inputText1.getString().isEmpty()) {
                        string currentText = inputText1.getString().toAnsiString();
                        currentText.pop_back();
                        inputText1.setString(currentText);

                        mazeWidthInput.pop_back();
                    }
                    else if (isnumber(typedChar)) {
                        mazeWidthInput.push_back(typedChar);
                        inputText1.setString(inputText1.getString() + typedChar);

                    }
                    else if (typedChar == '\n') {
                        enteringTextNR++;
                        inputField1.setOutlineThickness(1);
                        inputField2.setOutlineThickness(5);
                    }
                }
                else if(enteringTextNR == 1) {
                    if (typedChar == '\b' && !inputText2.getString().isEmpty()) {
                        string currentText = inputText2.getString().toAnsiString();
                        currentText.pop_back();
                        mazeHeigthInput.pop_back();
                        inputText2.setString(currentText);
                    }
                    else if (isnumber(typedChar)) {

                        inputText2.setString(inputText2.getString() + typedChar);
                        mazeHeigthInput.push_back(typedChar);
                    }
                    else if (typedChar == '\n') {
                        enteringTextNR++;
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
            int mazeWidth = stoi(mazeWidthInput);
            mazeWidthInput = "";
            int mazeHeigth = stoi(mazeHeigthInput);
            mazeHeigthInput = "";

            maze.GenerateFullMaze(mazeWidth, mazeHeigth);
            mazeSolution.matrix = maze.matrix;
            mazeSolution.GenerateSolutionForMaze();

            startingPointWidth= maze.mWidth * 3;
            startingPointHeigth= maze.mHeight * 3;


            colorSwitchCounter = mazeSolution.moves.size() / 20;
            display.window.setFramerateLimit(mazeSolution.moves.size()/6);


            cellBEGIN.setPosition(cellSize - cellSize/cellScale, cellSize - cellSize/cellScale);
            cellBEGIN.setFillColor(sf::Color(0xDB2955FF));
            cellEND.setPosition((maze.mWidth * 3 - 2) * cellSize - cellSize / cellScale, (maze.mHeight * 3 - 2) * cellSize - cellSize / cellScale);
            cellEND.setFillColor(sf::Color(0xDB2955FF));

            display.Height= maze.mHeight * 3 * cellSize;
            display.Width = maze.mWidth * 3 * cellSize;

            while(display.Width/viewScale>1300 || display.Height/viewScale>800)
            {
                viewScale++;
            }

            display.window.setSize({display.Width/viewScale,display.Height/viewScale});



            display.renderTexture.create(display.Width, display.Height);



            getGridTexture(maze.matrix, display.renderTexture);
            display.renderTexture.draw(cellBEGIN);
            display.renderTexture.draw(cellEND);
            sprite.setTexture(display.renderTexture.getTexture());


            view.setCenter(display.renderTexture.getDefaultView().getCenter());
            view.setSize(display.renderTexture.getSize().x+10, display.renderTexture.getSize().y+10);

            display.window.setView(view);
            display.window.draw(sprite);

            display.window.display();

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

            display.renderTexture.draw(cellBEGIN);
            display.renderTexture.draw(cellEND);

            drawSolution(mazeSolution, display.renderTexture, startingPointHeigth, startingPointWidth);

            sprite.setTexture(display.renderTexture.getTexture());
            display.window.draw(sprite);

            display.window.display();


            ColorCounter++;
            if(ColorCounter == colorSwitchCounter && ColorCounter < mazeSolution.moves.size())
            {
                custom2.r *= 1.009;
                custom2.g *= 0.93;
                custom2.b *= 1.003;
                ColorCounter=0;
            }
        }


    }



    return 0;
}
