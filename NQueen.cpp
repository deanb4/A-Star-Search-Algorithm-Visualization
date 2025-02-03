#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <chrono>

const int N = 8;

bool isSafe(int[N][N], int,int );
bool solveNQ(int[N][N],int,sf::RenderWindow&,sf::RectangleShape[N][N],sf::Sprite&,int);
void drawBoard(sf::RenderWindow&,sf::RectangleShape,sf::Sprite&,int[N][N],int);

int main(){
    const int windowSize = 600;
    const int boardSize = 8;    // 8x8 board
    const int squareSize = windowSize / boardSize; // Size of each square
    sf::RenderWindow window(sf::VideoMode({windowSize, windowSize}), "N-Queens Visualizer");
     // Create a chessboard (8x8 grid of rectangles)
    sf::RectangleShape squares[boardSize][boardSize];
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            squares[row][col].setSize(sf::Vector2f(squareSize, squareSize));
            squares[row][col].setPosition(sf::Vector2f(col * squareSize, row * squareSize));

            // Alternate colors (light and dark)
            if ((row + col) % 2 == 0) {
                squares[row][col].setFillColor(sf::Color::White); // Light square
            } else {
                squares[row][col].setFillColor(sf::Color::Black); // Dark square
            }
        }
    }
    // Sprite texture for queen
    sf::Texture queen_texture("queen.png");
    sf::Sprite queen(queen_texture);
  
    // int queen_row = 3;
    // int queen_col = 3;
    // queen.setPosition(sf::Vector2f(queen_col * squareSize, queen_row * squareSize));
    
    queen.setScale({0.09f,0.115f});
    
    int board[N][N] = {0};

    if(!solveNQ(board,0,window,squares,queen,squareSize)){
        std::cout << "No Solution Exists" << std::endl;
    }

    while (window.isOpen()){
        while(const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();
         // Draw the chessboard
        for (int row = 0; row < boardSize; ++row) {
            for (int col = 0; col < boardSize; ++col) {
                window.draw(squares[row][col]);
            }
        }
        // window.draw(queen);
        // // window.draw(sprite);
        // window.display();
    }

    return 0;
}

void drawBoard(sf::RenderWindow& window,sf::RectangleShape squares[N][N],sf::Sprite& queen,int board[N][N],int squareSize){
    window.clear();

    //draw board
    for(int row = 0; row < N; row++){
        for(int col = 0; col < N; col++){
            window.draw(squares[row][col]);
        }
    }

    // draw the queens
    for(int row = 0; row < N; row++){
        for(int col = 0; col < N; col++){
            if(board[row][col] == 1){
                queen.setPosition(sf::Vector2f(static_cast<float>(col * squareSize), static_cast<float>(row * squareSize)));
                window.draw(queen);
            }
        }
    }

    window.display();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

}

bool isSafe(int board[N][N], int row, int col){
    int i,j;
    for(i = 0;i<col;i++){
        if(board[row][i]){
            return false;
        }
    }

    for(i = row, j = col; j>=0 && i>=0;i--,j--){
        if(board[i][j]){
            return false;
        }
    }

    for(i = row, j = col;j>=0 && i <N;i++,j--){
        if(board[i][j]){
            return false;
        }
    }

    return true;
}

          

bool solveNQ(int board[N][N], int col,sf::RenderWindow& window,sf::RectangleShape squares[N][N], sf::Sprite& queen, int squareSize){
    if(col >= N){
        return true;
    }
    for(int i = 0; i < N; i++){
        if(isSafe(board,i,col)){
            board[i][col] =1;
            
            drawBoard(window,squares,queen,board,squareSize);


            if(solveNQ(board,col+1,window,squares,queen,squareSize)){
                return true;
            }
            board[i][col] = 0;

            //update the visualization for backtracking
            drawBoard(window,squares,queen,board,squareSize);
        }
    }
    return false;
}