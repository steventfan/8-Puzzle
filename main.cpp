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

        Puzzle();
        ~Puzzle();
        Node * search(unsigned int);
    private:
        std::vector<Node *> storage;
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
    depth = 0;
    queue = 0;
    total = 0;
}

Puzzle::~Puzzle()
{
    while(!storage.empty())
    {
        delete storage.back();

        storage.pop_back();
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
        }
    }
    do
    {
        if(queue < nodes.size())
        {
            queue = nodes.size();
        }

        unsigned int min = 0;

        for(unsigned int i = 1; i < length; i++)
        {
            if(nodes.at(i)->cost + nodes.at(i)->heuristic < nodes.at(min)->cost + nodes.at(min)->heuristic)
            {
                min = i;
            }
        }

        Node * node = nodes.at(min);

        nodes.erase(nodes.begin() + min);
        storage.push_back(node);
        if(node->state == goal)
        {
            while(!nodes.empty())
            {
                storage.push_back(nodes.back());
                nodes.pop_back();
            }

            return node;
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
        if(i > 0)
        {
            nodes.push_back(new Node(option, node->state, "up", node->cost, node));
        }
        if(i < length - 1)
        {
            nodes.push_back(new Node(option, node->state, "down", node->cost, node));
        }
        if(j > 0)
        {
            nodes.push_back(new Node(option, node->state, "left", node->cost, node));
        }
        if(j < length - 1)
        {
            nodes.push_back(new Node(option, node->state, "right", node->cost, node));
        }
    } while (!nodes.empty());

    return 0;
}

Node::Node(unsigned int option, std::vector<std::vector<int>> state, std::string operation, unsigned int cost, Node * parent):state(length, std::vector<int>(length))
{
    unsigned int i;
    unsigned int j;

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < length; j++)
        {
            if(state.at(i).at(j) == 0)
            {
                break;
            }
        }
        if(j < length)
        {
            break;
        }
    }
    if(operation == "up")
    {
        state.at(i).at(j) = state.at(i - 1).at(j);
        state.at(i - 1).at(j) = 0;
    }
    else if(operation == "down")
    {
        state.at(i).at(j) = state.at(i + 1).at(j);
        state.at(i + 1).at(j) = 0;
    }
    else if(operation == "left")
    {
        state.at(i).at(j) = state.at(i).at(j - 1);
        state.at(i).at(j - 1) = 0;
    }
    else if(operation == "right")
    {
        state.at(i).at(j) = state.at(i).at(j + 1);
        state.at(i).at(j + 1) = 0;
    }
    this->state = state;
    this->operation = operation;
    if(parent != 0)
    {
        cost++;
    }
    this->cost = cost;

    unsigned int goal[length][length];
    unsigned int count = 1;

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < length; j++)
        {
            if(count < length * length)
            {
                goal[i][j] = count;
            }
            else
            {
                goal[i][j] = 0;
            }
        }
    }

    unsigned int heuristic = 0;

    if(option == 2)
    {
        for(i = 0; i < length; i++)
        {
            for(j = 0; j < length; j++)
            {
                if(this->state.at(i).at(j) != goal[i][j])
                {
                    heuristic++;
                }
            }
        }
    }
    else if(option == 3)
    {
        for(i = 0; i < length; i++)
        {
            for(j = 0; j < length; j++)
            {
                for(unsigned int k = 0; k < length; k++)
                {
                    for(unsigned int l = 0; l < length; l++)
                    {
                        if(goal[i][j] == this->state.at(k).at(l))
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

        unsigned int option;

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
                        if(goal)
                        {
                            std::vector<Node *> trace(1, goal);
                            Node * pointer = goal->parent;

                            while(trace.back()->parent != 0)
                            {
                                trace.push_back(pointer);
                                pointer = pointer->parent;
                            }
                            puzzle->depth = trace.size();
                            std::cout << std::endl;
                            while(!trace.empty())
                            {
                                if(trace.back()->parent == 0)
                                {
                                    std::cout << "Initial State:" << std::endl;
                                }
                                else
                                {
                                    std::cout << "Move blank " << trace.back()->operation << std::endl;
                                    std::cout << "Expanding state with g(n) = " << trace.back()->cost << " and h(n) = " << trace.back()->heuristic << ':' << std::endl;
                                    for(unsigned int i = 0; i < length; i++)
                                    {
                                        for(unsigned int j = 0; j < length; j++)
                                        {
                                            std::cout << '[';
                                            if(trace.back()->state.at(i).at(j) == 0)
                                            {
                                                std::cout << '0';
                                            }
                                            else
                                            {
                                                std::cout << trace.back()->state.at(i).at(j);
                                            }
                                            std::cout << ']';
                                        }
                                        std::cout << std::endl;
                                    }
                                }
                                trace.pop_back();
                            }
                            std::cout << std::endl << "The depth of the goal node is " << puzzle->depth;
                        }
                        else
                        {
                            std::cout << "Solution trace unavailable due to no solution\n";
                        }
                        std::cout << "The maximum number of nodes in the queue at any one time was " << puzzle->queue << " nodes" << std::endl;
                        std::cout << "The search algorithm expanded a total of " << puzzle->total << " nodes";
                    }
                    else if(option == 2)
                    {
                        break;
                    }
                    else if(option == 3)
                    {
                        option = 0;

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