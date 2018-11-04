#include <iostream>
#include <vector>

class Puzzle
{
    public:
        unsigned int puzzle[3][3];
        Puzzle():puzzle{{9, 9, 9},{9, 9, 9},{9, 9, 9}} {};
        bool solve(unsigned int &);
};

class Node
{
    friend class Puzzle;
    private:
        unsigned int cost;
        unsigned int path;
        unsigned int array[3][3];
        Node * parent;
        Node();
};

bool Puzzle::solve(unsigned int & option)
{
    switch(option)
    {
        case 1:
            std::cout << "Solving using Uniform Cost Search" << std::endl;
            break;
        case 2:
            std::cout << "Solving using A* with Misplaced Tile Heuristic" << std::endl;
            break;
        case 3:
            std::cout << "Solving using A* with Manhattan Distance Heuristic" << std::endl;
            break;
        default:
            return false;
    }

    return true;
}

Node::Node()
{

}

int main()
{
    std::cout << "----- Welcome to the 8 Puzzle Solver -----" << std::endl;

    while(1)
    {
        Puzzle * puzzle = new Puzzle();

        std::cout << "----- Puzzle Set Up -----\n" << std::endl;
        for(unsigned int i = 0; i < 3; i++)// ***** Initialize puzzle *****
        {
            for(int j = 0; j < 3; j++)
            {
                unsigned int value;

                for(unsigned int k = 0; k < 3; k++)
                {
                    for(unsigned int l = 0; l < 3; l++)
                    {
                        std::cout << '[';
                        if(puzzle->puzzle[k][l] == 9)
                        {
                            std::cout << ' ';
                        }
                        else
                        {
                            std::cout << puzzle->puzzle[k][l];
                        }
                        std::cout << ']';
                    }
                    std::cout << std::endl;
                }

                std::cout << "[Puzzle Initialization]\n" << "Input a value from 1 to 8 for integers or 0 for blank" << std::endl << "> ";
                std::cin >> value;
                if(value <= 8)
                {
                    unsigned int k;
                    unsigned int l;

                    for(k = 0; k <= i; k++)// ***** Check for duplicate values *****
                    {
                        for(l = 0; l < 3; l++)
                        {
                            if(puzzle->puzzle[k][l] == value)
                            {
                                break;
                            }
                        }
                        if(l < 3)
                        {
                            break;
                        }
                    }
                    if(l < 3)
                    {
                        std::cout << "[Error] Duplicate Value\n";
                        j--;
                    }
                    else
                    {
                        puzzle->puzzle[i][j] = value;
                    }
                }
                else
                {
                    std::cout << "[Error] Invalid Input\n";
                    j--;
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
        while(1)
        {
            unsigned int value;

            std::cout << "[Enter Solver Algorithm]\n" << "[1] Uniform Cost Search\n" << "[2] A* with Misplaced Tile Heuristic\n" << "[3] A* with Manhattan Distance Heuristic" << std::endl << "> ";
            std::cin >> value;
            if(!puzzle->solve(value))
            {
                std::cout << "[Error] Invalid Input\n" << std::endl;
            }
        }
    }

    return 0;
}