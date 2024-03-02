#include "../header.h"


//Plik source zawierajacy funkcje dotyczace wyswietlania oraz biblioteki SFML


Display::Display()
{
    Width=600;
    Height=600;

    window.create(sf::VideoMode(Width, Height), "Maze");
    window.clear();
}


void getGridTexture(vector<vector<char>> grid, sf::RenderTexture &renderTexture) {
    renderTexture.clear();
    for(int col=0; col<grid.size(); col++)
    {
        for(int row=0; row<grid[0].size(); row++)
        {
            if (grid[col][row] != '@') {

                sf::RectangleShape cell(sf::Vector2f(cellSize*cellScale, cellSize*cellScale));

                cell.setPosition(row * cellSize-cellSize/cellScale, col * cellSize-cellSize/cellScale);
                cell.setFillColor(custom);

                renderTexture.draw(cell);
            }
        }
    }
}

void drawSolution(MazeSolver &mazeSolution, sf::RenderTexture &renderTexture, float &startingPointHeigth , float &startingPointWidth) {

    if(!mazeSolution.moves.empty()) //Wypisuje tylko ruchy rozwiazania zapisane na stosie moves
    {
        if(mazeSolution.moves.top() == 'N')
        {
            startingPointHeigth++;
        }
        else if(mazeSolution.moves.top() == 'E')
        {
            startingPointWidth--;
        }
        else if(mazeSolution.moves.top() == 'S')
        {
            startingPointHeigth--;
        }
        else if(mazeSolution.moves.top() == 'W')
        {
            startingPointWidth++;
        }
        else if(mazeSolution.moves.top() == 'X')
        {
            mazeSolution.moves.pop();
            return;
        }

        cell.setPosition((startingPointWidth - 2) * cellSize - cellSize / cellScale, (startingPointHeigth - 2) * cellSize - cellSize / cellScale);
        cell.setFillColor(custom2);

        renderTexture.draw(cell);

        mazeSolution.moves.pop();
    }
}