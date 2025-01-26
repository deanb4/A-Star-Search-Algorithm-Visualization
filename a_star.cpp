// create a window
// make a 2d array of squares or tile and draw it (white)
// Implmenet a star algo and then highlight the path on 
// made grid in green.

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <queue>
#include <cmath>
#include <limits>
#include <cassert>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <algorithm>

constexpr int gridSize = 20;

// Node Struct
struct Node{
    int x, y;
    double gCost, hCost;
    Node* parent;
    bool isObstacle;

    Node(int x, int y): x(x), y(y), gCost(std::numeric_limits<double>::infinity()), hCost(0), parent(nullptr), isObstacle(false){}

    double fCost() const{
        return gCost + hCost;
    }
};

struct CompareNode{
    bool operator()(Node* a , Node* b) const{
        return a->fCost() > b->fCost();
    }
};


// Utility functions for a star algo

//heruistic
double herisitic(Node* a, Node* b){
    return std::sqrt(pow(a->x-b->x,2)+pow(a->y-b->y,2));
}

//getneighbors
std::vector<Node*> getneighbors(Node* node, std::unordered_map<int,Node*>& nodes){
    std::vector<Node*> neighbors;
    std::vector<std::pair<int,int>> directions = {{0,1},{1,0},{0,-1},{-1,0},
                                                  {1,1},{-1,1},{-1,-1},{1,-1}}; // normal
    for (const auto& dir : directions){
        int newX = node->x + dir.first;
        int newY = node->y + dir.second;
        int key = abs(newY) * 1000 + abs(newX);
        if (nodes.find(key) != nodes.end() && !nodes[key]->isObstacle){
            neighbors.push_back(nodes[key]);
        }
    }

    return neighbors;
}


// update function
void drawBoard(int size,sf::RenderWindow& w,sf::RectangleShape (&sqs)[gridSize][gridSize], std::vector<Node*>& path, Node* start, Node* goal, std::unordered_map<int,Node*> nodes, 
                std::unordered_set<Node*> visited, Node* current,std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet){
    std::unordered_set<Node*> openSetNodes;
    auto openSetCopy = openSet; // Copy the priority queue to avoid modifying the original
    while (!openSetCopy.empty()) {
        openSetNodes.insert(openSetCopy.top());
        openSetCopy.pop();
    }
   
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            Node* node = nodes.at(j * 1000 + i);

            if (node == start) {
                sqs[i][j].setFillColor(sf::Color::Blue); // Start node
            } else if (node == goal) {
                sqs[i][j].setFillColor(sf::Color::Red); // Goal node
            } else if (node->isObstacle) {
                sqs[i][j].setFillColor(sf::Color::Black); // Obstacle
            } else if (std::find(path.begin(), path.end(), node) != path.end()) {
                sqs[i][j].setFillColor(sf::Color::Green); // Path
            } else if (visited.find(node) != visited.end()) {
                sqs[i][j].setFillColor(sf::Color(200, 200, 200)); // Visited nodes
            } else if (openSetNodes.find(node) != openSetNodes.end()) {
                sqs[i][j].setFillColor(sf::Color(0, 0, 255, 150)); // Nodes in open set
            } else {
                sqs[i][j].setFillColor(sf::Color::White); // Default
            }

            if (current && node == current) {
                sqs[i][j].setFillColor(sf::Color::Yellow); // Current node being explored
            }

            // Draw the square
            w.draw(sqs[i][j]);
        }
    }

    // Display the updated window
    w.display();

    // Add a delay to slow down the visualization
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); 

}




// a star algo
std::vector<Node*> aStar(Node* start, Node* goal, std::unordered_map<int,Node*>& nodes,sf::RenderWindow& w,sf::RectangleShape (&sqs)[gridSize][gridSize], int size,std::unordered_set<Node*> visited){
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    // std::unordered_set<Node*> openSetNodes;
    start->gCost = 0;
    start->hCost = herisitic(start, goal);
    openSet.push(start);
    std::vector<Node*> path;
    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();
   

        // If the current node is the goal, reconstruct the path
        if (current == goal) {
            while (current != nullptr) {
                path.push_back(current);
                w.clear();
                drawBoard(gridSize, w,sqs,path,start,goal,nodes,visited,current,openSet);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end()); 
            return path;
        }

        // Mark the current node as visited
        if (!current->isObstacle)
            visited.insert(current);

        // Explore neighbors
        for (Node* neighbor : getneighbors(current, nodes)) {
            if (visited.find(neighbor) != visited.end()) {
                continue;  // Skip already visited nodes
            }
            if (!neighbor->isObstacle && !current->isObstacle){
                double tentativeGCost = current->gCost + herisitic(current,neighbor); 
                if (tentativeGCost < neighbor->gCost) {
                    neighbor->parent = current;
                    neighbor->gCost = tentativeGCost;
                    neighbor->hCost = herisitic(neighbor, goal);
                    openSet.push(neighbor);
                }
            }
        }
        // Update the current path for visualization
        w.clear();
        drawBoard(gridSize, w,sqs,path,start,goal,nodes,visited,current,openSet);

    }

    // If no path is found, return an empty vector
    return {};

}


// Function to reset all nodes
void resetNodes(std::unordered_map<int, Node*>& nodes) {
    for (auto& [key, node] : nodes) {
        node->gCost = std::numeric_limits<double>::infinity();
        node->hCost = 0;
        node->parent = nullptr;
    }
}


int main(){
    // make map of nodes
    std::unordered_map<int,Node*> nodes;
    for (int y = 0; y < gridSize; y++){
        for (int x = 0; x < gridSize; x++){
            int key = y * 1000 + x;
            nodes[key] = new Node(x,y);
        }

    }
    Node* start = nodes[0]; 
    Node* goal = nodes[(gridSize-5) * 1000 + (gridSize-1)];
    std::unordered_set<Node*> visited;

    int windowSize = 600;
    int squareSize = windowSize/gridSize;
    sf::RenderWindow window(sf::VideoMode({windowSize,windowSize}), "Practice");
    sf::RectangleShape squares[gridSize][gridSize];
    for (int i = 0; i <gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            squares[i][j].setSize(sf::Vector2f(squareSize,squareSize));
            squares[i][j].setPosition(sf::Vector2f(i * squareSize, j * squareSize));
            squares[i][j].setFillColor(sf::Color::White);
            
        }
    }

    std::vector<Node*>  path;
    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                if (mouseButtonPressed->button == sf::Mouse::Button::Left || mouseButtonPressed->button == sf::Mouse::Button::Right){
                    path.clear();
                    int x =mouseButtonPressed->position.x / squareSize;
                    int y = mouseButtonPressed->position.y / squareSize;
                    if ( x >= 0 && x <gridSize && y >= 0 && y < gridSize){
                        int key = y * 1000 + x;
                        if (!nodes[key]->isObstacle){
                            resetNodes(nodes);
                            if (mouseButtonPressed->button == sf::Mouse::Button::Left){
                                start = nodes[key];
                            }
                            else{
                                goal = nodes[key];
                            }
                        }
                    }

                }
                if (mouseButtonPressed->button == sf::Mouse::Button::Middle){
                    int x = mouseButtonPressed->position.x / squareSize;
                    int y = mouseButtonPressed->position.y / squareSize;
                    if (x >= 0 && x <gridSize && y >= 0 && y < gridSize){
                        nodes[y*1000+x]->isObstacle = true;
                    }
                }  
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scan::Enter){
                    path = aStar(start,goal,nodes,window, squares, gridSize,visited);
                    window.clear();
                } else if (keyPressed->scancode == sf::Keyboard::Scan::R){
                    for (auto& [key,node]: nodes){
                        node->isObstacle = false;
                    }
                }
                
            }
                
        }
                  
        window.clear();
        drawBoard(squareSize, window,squares,path,start,goal,nodes,visited,nullptr,{});

    }


    return 0;

}

                        

                    