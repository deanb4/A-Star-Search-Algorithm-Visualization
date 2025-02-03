#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
// constants
const int NUM_RECTANGLES = 50;

// First Create A window then the rectangles with random heights 

void swap(sf::RectangleShape&, sf::RectangleShape&);
void bubbleSort(std::vector<sf::RectangleShape>&);

int main(){
    //Create a window 
    sf::RenderWindow window(sf::VideoMode({800,600}), "Bubble Sort Visualizer");
    // vector of rectangles
    std::vector<sf::RectangleShape> rectangles;
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    
    // Rectangle width is proportional to number of rectangles
    float rectangleWidth = windowWidth / NUM_RECTANGLES;

    // populate the vector with random height rectangles
    for (int i = 0; i < NUM_RECTANGLES;i++){
        float randomHeight = static_cast<float>(rand() % static_cast<int>(windowHeight)+5);
        //create rectangle
        sf::RectangleShape rectangle(sf::Vector2f(rectangleWidth-2, randomHeight));
        rectangle.setFillColor(sf::Color::Green);

        // set the position
        rectangle.setPosition({i * rectangleWidth, windowHeight - randomHeight});

        rectangles.push_back(rectangle);

    }
 
    //main event
    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        // clear the window and set to black
        bubbleSort(rectangles);
        // Perform one sorting step (one swap) per frame


        window.clear(sf::Color::Black);
   

        
        // Draw all the rectangles
        for (auto& rectangle:rectangles){
            window.draw(rectangle);
        }
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // display the rendered frame
   
     
    }

    return 0;
}

void swap(sf::RectangleShape& a, sf::RectangleShape& b){
    float temp = a.getSize().y;
    a.setSize(sf::Vector2f(a.getSize().x, b.getSize().y));
    b.setSize(sf::Vector2f(b.getSize().x, temp));
    // Swap positions
    sf::Vector2f tempY = a.getPosition();
    a.setPosition(sf::Vector2f(a.getPosition().x,b.getPosition().y));
    b.setPosition(sf::Vector2f(b.getPosition().x,tempY.y));

}

void bubbleSort(std::vector<sf::RectangleShape>& rectangle_vec) {
    static int i = 0;  // static index to track current sorting step
    bool swapped = false;

    if (i < rectangle_vec.size() - 1) {
        // Perform one step of bubble sort
        if (rectangle_vec[i].getSize().y > rectangle_vec[i + 1].getSize().y) {
            swap(rectangle_vec[i], rectangle_vec[i + 1]);
            swapped = true;
        }
        i++;
    }
    
    // If we reached the end of the array, start over
    if (i >= rectangle_vec.size() - 1) {
        i = 0;
    }
}



//MISTAKE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!___________________------------------------------------
// void bubbleSort(std::vector<sf::RectangleShape>& rectangle_vec,sf::RenderWindow& window){
//     bool switched = false;
//     for(int i = 0; i < rectangle_vec.size() && !switched;i++){
//         switched = true;
//         for(int j = 0;j <= rectangle_vec.size()-1-i;j++){
//             if (rectangle_vec[j].getSize().y > rectangle_vec[j+1].getSize().y){
//                 swap(rectangle_vec[j],rectangle_vec[j+1]);
//                 // sf::RectangleShape temp = rectangle_vec[j]; 
//                 // rectangle_vec[j] = rectangle_vec[j+1];
//                 // rectangle_vec[j+1] = temp;
//                 switched = false;
//                  // Visualize the sorting step
//                 }
//                 window.clear(sf::Color::Black);
//                 for (auto& rectangle : rectangle_vec) {
//                     window.draw(rectangle);
//                 }
//                 window.display();

//                 // Pause for visualization
//                 std::this_thread::sleep_for(std::chrono::milliseconds(50));
//         }
//     }    
// }



