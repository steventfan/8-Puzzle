#include <iostream>
#include <string>
#include <vector>

const unsigned int solution[3][3] = {{1, 2, 3},{4, 5, 6},{7, 8, 0}};

class Node;

class Puzzle
{
    public:
        unsigned int puzzle[3][3];
        std::vector<Node *> storage;
        Puzzle():puzzle{{9, 9, 9},{9, 9, 9},{9, 9, 9}} {};
        bool solve(unsigned int &);
        Node * search(const unsigned int);
};

class Node
{
    friend class Puzzle;
    private:
        std::string operation;
        unsigned int cost;
        unsigned int heuristic;
        unsigned int array[3][3];
        Node * parent;
        Node(const unsigned int, const std::string, unsigned int, Node *);
        unsigned int calculate(unsigned int);// ***** Calculate heuristic *****
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
    if(!search(option))
    {
        std::cout << "No Solution Found\n" << std::endl;
    }
    while(1)
    {

    }

    return true;
}

Node * Puzzle::search(const unsigned int option)
{
    std::vector<Node *> nodes;
    std::vector<Node *> removal;

    nodes.push_back(new Node(option, "none", 0, 0));
    while(1)
    {
        if(nodes.empty())
        {
            break;
        }

        unsigned int i;
        unsigned int min = 0;

        for(i = 1; i < nodes.size(); i++)
        {
            if(nodes.at(i)->cost + nodes.at(i)->heuristic < nodes.at(min)->cost + nodes.at(min)->heuristic)
            {
                min = i;
            }
        }

        Node * node = nodes.at(min);

        storage.push_back(node);
        nodes.erase(nodes.begin() + min);

        unsigned int j;

        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++)
            {
                if(node->array[i][j] != solution[i][j])
                {
                    break;
                }
            }
        }
        if(i >= 3 && j >= 3)
        {
            return node;
        }
        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++)
            {
                if(node->array[i][j] == 0)
                {
                    break;
                }
            }
            if(j < 3)
            {
                break;
            }
        }
        if(i > 0)
        {
            nodes.push_back(new Node(option, "up", node->cost, node));
        }
        if(i < 2)
        {
            nodes.push_back(new Node(option, "down", node->cost, node));
        }
        if(j > 0)
        {
            nodes.push_back(new Node(option, "left", node->cost, node));
        }
        if(j < 2)
        {
            nodes.push_back(new Node(option, "right", node->cost, node));
        }
    }

    return 0;
}

Node::Node(const unsigned int option, const std::string operation, unsigned int cost, Node * parent)
{
    this->operation = operation;
    if(operation != "none")
    {
        unsigned int i;
        unsigned int j;

        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++)
            {
                if(array[i][j] == 0)
                {
                    break;
                }
            }
            if(j < 3)
            {
                break;
            }
        }
        if(operation == "up")
        {
            array[i][j] = array[i - 1][j];
            array[i - 1][j] = 0;
        }
        else if(operation == "down")
        {
            array[i][j] = array[i + 1][j];
            array[i + 1][j] = 0;
        }
        else if(operation == "left")
        {
            array[i][j] = array[i][j - 1];
            array[i][j - 1] = 0;
        }
        else if(operation == "right")
        {
            array[i][j] = array[i][j + 1];
            array[i][j + 1] = 0;
        }
    }
    if(parent != 0)
    {
        cost++;
    }
    this->cost = cost;

    unsigned int heuristic = 0;

    switch(option)
    {
        case 2:
            for(unsigned int i = 0; i < 3; i++)
            {
                for(unsigned int j = 0; j < 3; j++)
                {
                    if(array[i][j] != solution[i][j])
                    {
                        heuristic++;
                    }
                }
            }

            break;
        case 3:
            for(unsigned int i = 0; i < 3; i++)
            {
                for(unsigned int j = 0; j < 3; j++)
                {
                    for(unsigned int k = 0; k < 3; k++)
                    {
                        for(unsigned int l = 0; l < 3; l++)
                        {
                            if(solution[i][j] == array[k][l])
                            {
                                int row = i - k;
                                int column = j - l;

                                if(row < 0)
                                {
                                    row *= -1;
                                }
                                if(column < 0)
                                {
                                    column *= -1;
                                }
                                heuristic += row + column;
                            }
                        }
                    }
                }
            }

            break;
        default:
            break;
    }
    this->heuristic = heuristic;
    this->parent = parent;
}

unsigned int Node::calculate(unsigned int option)
{
    unsigned int heuristic = 0;

    switch(option)
    {
        case 2:
            for(unsigned int i = 0; i < 3; i++)
            {
                for(unsigned int j = 0; j < 3; j++)
                {
                    if(solution[i][j] != array[i][j])
                    {
                        heuristic++;
                    }
                }
            }

            break;
        case 3:
            for(unsigned int i = 0; i < 3; i++)
            {
                for(unsigned int j = 0; j < 3; j++)
                {
                    for(unsigned int k = 0; k < 3; k++)
                    {
                        for(unsigned int l = 0; l < 3; l++)
                        {
                            if(solution[i][j] == array[k][l])
                            {
                                int row = i - k;
                                int column = j - l;

                                if(row < 0)
                                {
                                    row *= -1;
                                }
                                if(column < 0)
                                {
                                    column *= -1;
                                }
                                heuristic += row + column;
                            }
                        }
                    }
                }
            }
            break;
        default:
            heuristic = 1;
    }

    return heuristic;
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