#include <iostream>
#include <string>
#include <vector>

unsigned int length = 3;

class Node;

class Puzzle
{
    public:
        std::vector<std::vector<int>> initial;
        unsigned int depth;
        unsigned int queue;
        unsigned int total;
        std::vector<Node *> duplicate;

        Puzzle();
        ~Puzzle();
        Node * search(unsigned int);
};

class Node
{
    friend class Puzzle;
    public:
        std::vector<std::vector<int>> state;
        std::string operation;
        unsigned int cost;
        unsigned int heuristic;
        Node * parent;
    private:
        Node(unsigned int, std::vector<std::vector<int>>, std::string, unsigned int, Node *);
};

Puzzle::Puzzle():initial(length ,std::vector<int>(length))
{
    for(unsigned int i = 0; i < length; i++)
    {
        for(unsigned int j = 0; j < length; j++)
        {
            initial.at(i).at(j) = -1;
        }
    }
}

Puzzle::~Puzzle()
{
    while(!duplicate.empty())
    {
        delete duplicate.back();

        duplicate.pop_back();
    }
}

Node * Puzzle::search(unsigned int option)
{
    if(option == 1)
    {
        std::cout << "Solving using Uniform Cost Search\n";
    }
    else if(option == 2)
    {
        std::cout << "Solving using A* with Misplaced Tile Heuristic\n";
    }
    else
    {
        std::cout << "Solving using A* with Manhattan Distance Heuristic\n";
    }
    std::cout << std::endl;

    std::vector<Node *> nodes(1, new Node(option, initial, "", 0, 0));

    duplicate.push_back(nodes.back());

    std::vector<std::vector<int>> goal(length, std::vector<int>(length));
    unsigned int count = 1;

    for(unsigned int i = 0; i < length; i++)
    {
        for(unsigned int j = 0; j < length; j++)
        {
            if(count < length * length)
            {
                goal.at(i).at(j) = count;
            }
            else
            {
                goal.at(i).at(j) = 0;
            }
            count++;
        }
    }
    queue = 1;
    if(nodes.back()->state == goal)
    {
        return nodes.back();
    }
    total = 0;
    do
    {
        Node * node = nodes.front();

        nodes.erase(nodes.begin());
        if(length == 3 && node->cost > 31 || length == 4 && node->cost > 80)
        {
            break;
        }
        total++;

        unsigned int i;
        unsigned int j;

        for(i = 0; i < length; i++)
        {
            for(j = 0; j < length; j++)
            {
                if(node->state.at(i).at(j) == 0)
                {
                    break;
                }
            }
            if(j < length)
            {
                break;
            }
        }

        std::vector<std::vector<int>> state = node->state;
        unsigned int l;
        Node * temp;
        Node * found = 0;

        for(unsigned int k = 0; k < 4; k++)
        {
            std::string operation = "";

            state = node->state;
            if(k == 0 && i > 0 && node->operation != "down")
            {
                operation = "up";
                state.at(i).at(j) = state.at(i - 1).at(j);
                state.at(i - 1).at(j) = 0;
            }
            else if(k == 1 && i < length - 1 && node->operation != "up")
            {
                operation = "down";
                state.at(i).at(j) = state.at(i + 1).at(j);
                state.at(i + 1).at(j) = 0;
            }
            else if(k == 2 && j > 0 && node->operation != "right")
            {
                operation = "left";
                state.at(i).at(j) = state.at(i).at(j - 1);
                state.at(i).at(j - 1) = 0;
            }
            else if(k == 3 && j < length - 1 && node->operation != "left")
            {
                operation = "right";
                state.at(i).at(j) = state.at(i).at(j + 1);
                state.at(i).at(j + 1) = 0;
            }
            for(l = 0; l < duplicate.size(); l++)
            {
                if(state == duplicate.at(l)->state)
                {
                    break;
                }
            }
            if(l == duplicate.size())
            {
                temp = new Node(option, state, operation, node->cost, node);

                for(l = 0; l < nodes.size(); l++)
                {
                    if(temp->cost + temp->heuristic < nodes.at(l)->cost + nodes.at(l)->heuristic)
                    {
                        break;
                    }
                }
                if(l == nodes.size())
                {
                    nodes.push_back(temp);
                }
                else
                {
                    nodes.insert(nodes.begin() + l, temp);
                }
                if(queue < nodes.size())
                {
                    queue = nodes.size();
                }
                for(l = 0; l < duplicate.size(); l++)
                {
                    if(temp->cost + temp->heuristic < duplicate.at(l)->cost + duplicate.at(l)->heuristic)
                    {
                        break;
                    }
                }
                if(l == duplicate.size())
                {
                    duplicate.push_back(temp);
                }
                else
                {
                    duplicate.insert(duplicate.begin() + l, temp);
                }
                if(temp->state == goal)
                {
                    found = temp;
                }
            }
        }
        if(found)
        {
            return found;
        }
    } while (!nodes.empty());

    return 0;
}

Node::Node(unsigned int option, std::vector<std::vector<int>> state, std::string operation, unsigned int cost, Node * parent):state(length, std::vector<int>(length))
{
    this->state = state;
    this->operation = operation;
    if(parent != 0)
    {
        cost++;
    }
    this->cost = cost;

    unsigned int goal[length][length];
    unsigned int count = 1;

    for(unsigned int i = 0; i < length; i++)
    {
        for(unsigned int j = 0; j < length; j++)
        {
            if(count < length * length)
            {
                goal[i][j] = count;
            }
            else
            {
                goal[i][j] = 0;
            }
            count++;
        }
    }

    unsigned int heuristic = 0;

    if(option == 2)
    {
        for(unsigned int i = 0; i < length; i++)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                if(state.at(i).at(j) != goal[i][j] && state.at(i).at(j) != 0)
                {
                    heuristic++;
                }
            }
        }
    }
    else if(option == 3)
    {
        for(unsigned int i = 0; i < length; i++)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                for(unsigned int k = 0; k < length; k++)
                {
                    for(unsigned int l = 0; l < length; l++)
                    {
                        if(goal[i][j] == state.at(k).at(l) && state.at(k).at(l) != 0)
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
    }
    this->heuristic = heuristic;
    this->parent = parent;
}

int main()
{
    std::cout << "----- Welcome to the Puzzle Solver -----\n" << std::endl;

    bool status = true;

    while(status)
    {
        Puzzle * puzzle = new Puzzle();

        for(unsigned int i = 0; i < length; i++)// ***** Initialize puzzle *****
        {
            for(int j = 0; j < length; j++)
            {
                for(unsigned int k = 0; k < length; k++)// ***** Check for duplicate values *****
                {
                    for(unsigned int l = 0; l < length; l++)
                    {
                        std::cout << '[';
                        if(puzzle->initial.at(k).at(l) == -1 )
                        {
                            std::cout << '-';
                        }
                        else if(puzzle->initial.at(k).at(l) == 0)
                        {
                            std::cout << ' ';
                        }
                        else
                        {
                            std::cout << puzzle->initial.at(k).at(l);
                        }
                        std::cout << ']';
                    }
                    std::cout << std::endl;
                }

                unsigned int value;

                std::cout << "----- Puzzle Initialization -----\n";
                std::cout << "Input a value from 1 to " << length * length - 1 << " for integers or 0 for blank" << std::endl;
                std::cout << "> ";
                std::cin >> value;
                if(value < length * length)
                {
                    unsigned int k;
                    unsigned int l;

                    for(k = 0; k < length; k++)
                    {
                        for(l = 0; l < length; l++)
                        {
                            if(puzzle->initial.at(k).at(l) == value)
                            {
                                break;
                            }
                        }
                        if(l < length)
                        {
                            break;
                        }
                    }
                    if(l < length)
                    {
                        std::cout << "[Error] Duplicate Value\n";
                        j--;
                    }
                    else
                    {
                        puzzle->initial.at(i).at(j) = value;
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
        for(unsigned int i = 0; i < length; i++)
        {
            for(unsigned int j = 0; j < length; j++)
            {
                std::cout << '[';
                if(puzzle->initial.at(i).at(j) == 0)
                {
                    std::cout << ' ';
                }
                else
                {
                    std::cout << puzzle->initial.at(i).at(j);
                }
                std::cout << ']';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        int option = -1;

        while(option)
        {
            std::cout << "----- Enter Solver Algorithm -----\n";
            std::cout << "[1] Uniform Cost Search\n";
            std::cout << "[2] A* with Misplaced Tile Heuristic\n";
            std::cout << "[3] A* with Manhattan Distance Heuristic" << std::endl;
            std::cout << "> ";
            std::cin >> option;
            if(option != 0 && option < 4)
            {
                Node * goal = puzzle->search(option);

                if(goal)
                {
                    std::cout << "[Solution Found]\n" << std::endl;
                }
                else
                {
                    std::cout << "[No Solution]\n" << std::endl;
                }
                while(1)
                {
                    std::cout << "----- Enter Option -----\n";
                    std::cout << "[1] Output Solution Trace (if applicable)\n";
                    std::cout << "[2] Choose Another Search Algorithm\n";
                    std::cout << "[3] Start New Puzzle\n";
                    std::cout << "[4] Exit Program" << std::endl;
                    std::cout << "> ";
                    std::cin >> option;
                    if(option == 1)
                    {
                        std::cout << std::endl;
                        if(goal)
                        {
                            std::vector<Node *> trace(1, goal);
                            Node * pointer = goal->parent;

                            puzzle->depth = goal->cost;
                            while(pointer != 0)
                            {
                                trace.push_back(pointer);
                                pointer = pointer->parent;
                            }
                            while(!trace.empty())
                            {
                                if(trace.back()->parent == 0)
                                {
                                    std::cout << "Initial State:" << std::endl;
                                }
                                else
                                {
                                    std::cout << "Move blank " << trace.back()->operation << std::endl;
                                    if(trace.front() != trace.back())
                                    {
                                        std::cout << "Expanding state with g(n) = " << trace.back()->cost << " and h(n) = " << trace.back()->heuristic << ':' << std::endl;
                                    }
                                }
                                for(unsigned int i = 0; i < length; i++)
                                {
                                    for(unsigned int j = 0; j < length; j++)
                                    {
                                        std::cout << '[';
                                        if(trace.back()->state.at(i).at(j) == 0)
                                        {
                                            std::cout << ' ';
                                        }
                                        else
                                        {
                                            std::cout << trace.back()->state.at(i).at(j);
                                        }
                                        std::cout << ']';
                                    }
                                    std::cout << std::endl;
                                }
                                std::cout << std::endl;
                                trace.pop_back();
                            }
                            std::cout << "The depth of the goal node is " << puzzle->depth;
                        }
                        else
                        {
                            std::cout << "Solution trace unavailable due to no solution\n";
                        }
                        std::cout << std::endl << "The maximum number of nodes in the queue at any one time was " << puzzle->queue << " nodes" << std::endl;
                        std::cout << "The search algorithm expanded a total of " << puzzle->total << " nodes\n" << std::endl;
                    }
                    else if(option == 2)
                    {
                        while(!puzzle->duplicate.empty())
                        {
                            delete puzzle->duplicate.back();

                            puzzle->duplicate.pop_back();
                        }
                        std::cout << std::endl;

                        break;
                    }
                    else if(option == 3)
                    {
                        while(!puzzle->duplicate.empty())
                        {
                            delete puzzle->duplicate.back();
                            
                            puzzle->duplicate.pop_back();
                        }
                        option = 0;
                        std::cout << std::endl;

                        break;
                    }
                    else if(option == 4)
                    {
                        option = 0;
                        status = false;

                        break;
                    }
                    else
                    {
                        std::cout << "[Error] Invalid Input\n" << std::endl;
                    }
                }
            }
            else
            {
                std::cout << "[Error] Invalid Input\n" << std::endl;
                option = -1;
            }
        }

        delete puzzle;
    }

    return 0;
}