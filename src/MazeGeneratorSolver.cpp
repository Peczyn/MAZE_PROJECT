#include "../header.h"


//Plik source zawierajacy funkcje dotyczace tworzenia labirytnu i rozwiazania


template<typename T>
void PrintMatrix(const vector<vector<T>>& matrix)
{
    for(auto &a : matrix)
    {
        for(auto &b: a) cout << b << ' ';
        cout << endl;
    }
    cout << endl;
}



MazeGenerator::MazeGenerator()
{

}


bool MazeGenerator::CreateMaze(int width, int height) {
    mWidth = width;
    mHeight = height;
    mSize = mWidth*mHeight;

    mMaze =  vector<vector<int>>(mHeight,vector<int>(mWidth,0));

    int x = dist6(rng)%mWidth;
    int y = dist6(rng)%mHeight;

    mStack.push(make_pair(x,y));
    mMaze[y][x]= CellVisited;
    mVisited=1;

    return true;
}

bool MazeGenerator::UpdateMaze() {
    auto offsetX = [&](int x)
    {
        return (mStack.top().first + x);
    };
    auto offsetY = [&](int y)
    {
        return (mStack.top().second + y);
    };


    if(mVisited < mWidth*mHeight)
    {
        vector<int> neighbours;

        if(mStack.top().second > 0 && (mMaze[offsetY(-1)][offsetX(0)] & CellVisited) ==0)
        {
            neighbours.push_back(0);
        }
        if(mStack.top().first < mWidth-1  && (mMaze[offsetY(0)][offsetX(1)] & CellVisited) ==0)
        {
            neighbours.push_back(1);
        }
        if(mStack.top().second < mHeight-1  && (mMaze[offsetY(1)][offsetX(0)] & CellVisited) ==0)
        {
            neighbours.push_back(2);
        }
        if(mStack.top().first > 0 && (mMaze[offsetY(0)][offsetX(-1)] & CellVisited) ==0)
        {
            neighbours.push_back(3);
        }


        if(!neighbours.empty())
        {
            int nextCellDir = neighbours[dist6(rng) % neighbours.size()];

            switch(nextCellDir)
            {
                case 0:
                    mMaze[offsetY(-1)][offsetX(0)] |= CellVisited | South;
                    mMaze[offsetY(0)][offsetX(0)]  |= North;
                    mStack.push(make_pair((mStack.top().first+0),(mStack.top().second-1)));
                    break;
                case 1:
                    mMaze[offsetY(0)][offsetX(1)] |= CellVisited | West;
                    mMaze[offsetY(0)][offsetX(0)]  |= East;
                    mStack.push(make_pair((mStack.top().first+1),(mStack.top().second+0)));
                    break;
                case 2:
                    mMaze[offsetY(1)][offsetX(0)] |= CellVisited | North;
                    mMaze[offsetY(0)][offsetX(0)]  |= South;
                    mStack.push(make_pair((mStack.top().first+0),(mStack.top().second+1)));
                    break;
                case 3:
                    mMaze[offsetY(0)][offsetX(-1)] |= CellVisited | East;
                    mMaze[offsetY(0)][offsetX(0)]  |= West;
                    mStack.push(make_pair((mStack.top().first-1),(mStack.top().second+0)));
                    break;
            }
            mVisited++;
        }
        else
        {
            mStack.pop();
        }
    }
    return true;
}

bool MazeGenerator::GenerateFullMaze(int width, int height) {
    srand((unsigned) time(nullptr));

    CreateMaze(width,height);

    while(mVisited!=mSize)
    {
        UpdateMaze();
    }



    for(int i=0; i<mHeight*3; i++)
    {
        vector<char> temp;
        for(int j=0; j<mWidth*3; j++)
        {
            temp.push_back('@');
        }
        matrix.push_back(temp);
    }

    for(int row =0; row<mHeight; row++)
    {
        for(int collumn=0; collumn<mWidth; collumn++) {
            matrix[row*3+1][collumn*3+1]=' ';
            if ((mMaze[row][collumn] & 0x01) == 1) {
                matrix[row*3][collumn*3 + 1] = ' ';
            }
            if ((mMaze[row][collumn] & 0x02) == 2) {
                matrix[row*3 + 1][collumn*3 + 2] = ' ';
            }
            if ((mMaze[row][collumn] & 0x04) == 4) {
                matrix[row*3 + 2][collumn*3 + 1] = ' ';
            }
            if ((mMaze[row][collumn] & 0x08) == 8) {
                matrix[row*3 + 1][collumn*3] = ' ';
            }
        }
    }
//    PrintMatrix(matrix);
    return true;
}


MazeSolver::MazeSolver(const MazeGenerator& x)
{
    matrix = x.matrix;
}



bool MazeSolver::CountDistance() {


    destinationX= matrix[0].size()-2;
    destinationY= matrix.size()-2;

    for(int row=0; row<matrix.size(); row++)
    {
        vector<float> temp;

        for(int collumn=0; collumn<matrix[0].size(); collumn++)
        {
            if(matrix[row][collumn]!='@')
            {
                temp.push_back(sqrt(pow(row-destinationX,2)+pow(collumn-destinationY,2)));
            }
            else
            {
                temp.push_back(-1);
            }

        }
        Distances.push_back(temp);
    }
    return true;
}


bool MazeSolver::FindSolutionMoves() {
    moves.push('X');

    startingPositionX = 1;
    startingPositionY = 1;

    int counter = 1;
    while(startingPositionX!=destinationX || startingPositionY!=destinationY){

        Distances[startingPositionY][startingPositionX]=-2;

        float directions[4]=  {-1,-1,-1,-1};

        if(startingPositionY!=0) directions[0] =  Distances[startingPositionY-1][startingPositionX];
        if(startingPositionX!= Distances[0].size()-1) directions[1] =  Distances[startingPositionY][startingPositionX+1];
        if(startingPositionY!= Distances.size()-1) directions[2] =  Distances[startingPositionY+1][startingPositionX];
        if(startingPositionX!=0) directions[3] =  Distances[startingPositionY][startingPositionX-1];

        float min= INT32_MAX;

        counter = 0;
        for(int i=0; i<4; i++)
        {
            if(directions[i]>=0) counter++;
            if(directions[i]<min && directions[i]>=0) min=directions[i];
        }

        if(counter==0)
        {
            if(moves.top()=='X') moves.pop();
            if(moves.empty()) return false;

            while(moves.top()!='X')
            {
                if      (moves.top()=='N')   startingPositionY++;
                else if (moves.top()=='E')   startingPositionX--;
                else if (moves.top()=='S')   startingPositionY--;
                else if (moves.top()=='W')   startingPositionX++;
                moves.pop();
            }
            continue;
        }


        if(counter>1) moves.push('X');

        if(directions[0]==min)
        {
            moves.push('N');
            startingPositionY--;
        }
        else if(directions[1]==min)
        {
            moves.push('E');
            startingPositionX++;
        }
        else if(directions[2]==min)
        {
            moves.push('S');
            startingPositionY++;
        }
        else if(directions[3]==min)
        {
            moves.push('W');
            startingPositionX--;
        }
    }
    return true;
}


bool MazeSolver::GenerateSolutionMatrix() {
    Solution = vector<vector<char>>(Distances.size(),vector<char>(Distances[0].size(),'.'));
    int a = destinationX;
    int b = destinationY;

    stack<char> tempMoves = moves;

    while(!tempMoves.empty())
    {
        Solution[b][a] = 'X';
        if(tempMoves.top()=='N') b++;
        else if(tempMoves.top()=='E') a--;
        else if(tempMoves.top()=='S')b--;
        else if(tempMoves.top()=='W')a++;
        tempMoves.pop();
    }
    Solution[b][a]= 'X';
    return true;
}

bool MazeSolver::GenerateSolutionForMaze() {

    CountDistance();
    FindSolutionMoves();

////Jezeli chcemy wyswietlac rozwiazanie i labriynt w terminalu to trzeba odkomentowac nastepne 3 linijki
//    GenerateSolutionMatrix();
//    PrintMatrix(matrix);
//    PrintMatrix(Solution);

    return true;
}

