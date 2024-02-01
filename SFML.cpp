#include "header.h"

Display::Display()
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