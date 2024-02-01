#ifndef MAZE_PROJECT_HEADER_H
#define MAZE_PROJECT_HEADER_H

#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <SFML/Graphics.hpp>

using namespace std;


float cellSize = 5.0f;
const float cellScale = 2.0f;

sf::Color custom(0x5E5E5EFF); // 0xF6E7CBFF
sf::Color custom2(0xA9DEF9FF); //0xA9DEF9FF
sf::RectangleShape cell(sf::Vector2f(cellSize*cellScale, cellSize*cellScale));



class MazeGenerator{
public:
    int mWidth;
    int mHeight;

private:
    int mVisited;
    int mSize;
    vector<vector<int>> mMaze;
    stack<pair<int, int>> mStack;

    enum{
        North = 0x01,
        East = 0x02,
        South = 0x04,
        West = 0x08,
        CellVisited = 0x10,
    };


public:
    vector<vector<char>> matrix;
    MazeGenerator();

private:
    virtual bool CreateMaze(int width, int height);
    virtual bool UpdateMaze();

public:
    virtual bool GenerateFullMaze(int width, int height);
};


class MazeSolver : public MazeGenerator
{
private:
    int startingPositionX;
    int startingPositionY;
    int destinationX;
    int destinationY;




    vector<vector<float>> Distances;
public:
    stack<char> moves;
    vector<vector<char>> Solution;
    explicit MazeSolver(const MazeGenerator& x);


private:
    virtual bool CountDistance();
    virtual bool FindSolutionMoves();
    virtual bool GenerateSolutionMatrix();


public:
    virtual bool GenerateSolutionForMaze();
};


class Display: public MazeGenerator
{
public:
    sf::RenderWindow window;
    sf::RenderTexture renderTexture;

    unsigned int Width;
    unsigned int Height;

public:
    Display();
};

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist6(1,100);


void drawSolution(MazeSolver &y, sf::RenderTexture &renderTexture, float &i ,float &j);


#endif //MAZE_PROJECT_HEADER_H