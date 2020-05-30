//
//  main.cpp
//  Conway
//
//  Created by Jon Castillo on 23/5/20.
//  Copyright © 2020 Jon Castillo. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unistd.h>


#include <unordered_map>
using namespace std;

class GameBoard {
private:

    typedef vector<bool> Row_t;
    vector<Row_t> m_board;

    
    virtual int countNeighbors(int row, int col);
    
public:
    typedef unordered_map<int,bool> AffectedColumns_t;
    typedef unordered_map<int,AffectedColumns_t> AffectedParts_t;
    AffectedParts_t affectedParts;

    GameBoard(int maxRow, int maxCol);

    virtual void set(int row, int col, bool value);
    virtual bool get(int row, int col);
    virtual bool applyRules(int row, int col);
    virtual ~GameBoard();
};

class Game {
private:
    unique_ptr<GameBoard> p_GameBoardSource;
    unique_ptr<GameBoard> p_GameBoardDestination;
    int m_maxRow;
    int m_maxCol;
    
    virtual void flip();

public:
    Game(int row, int col);
    virtual ~Game();

    virtual void next();
    virtual void setBoard(unique_ptr<GameBoard>& source);
    virtual void display();
};

Game::Game(int row, int col) {
    m_maxRow = row;
    m_maxCol = col;
    p_GameBoardSource = make_unique<GameBoard>(m_maxRow,m_maxCol);
    p_GameBoardDestination = make_unique<GameBoard>(m_maxRow,m_maxCol);
}

Game::~Game() {
    
}

void Game::flip() {
    p_GameBoardSource = move(p_GameBoardDestination);
    p_GameBoardDestination = make_unique<GameBoard>(m_maxRow, m_maxCol);
}

void Game::next() {

    auto affectedParts = p_GameBoardSource->affectedParts;
    for ( auto& it : affectedParts ) {
        for (auto& it2 : it.second) {
            int i = it.first;
            int j = it2.first;
            int toTest = it2.second;
            
            if (toTest) {
                //cout << "Tested: " << i << ", " << j << endl;
                bool nextState = p_GameBoardSource->applyRules(i,j);
                p_GameBoardDestination->set(i,j,nextState);
            }
        }
    }
    
    
    flip();
    display();
}

void Game::setBoard(unique_ptr<GameBoard>& source) {
    p_GameBoardSource = std::move(source);
}

void Game::display() {
    system("clear");

    for (int i = 0; i < m_maxRow ; i++){
        for (int j = 0 ; j < m_maxCol ; j++) {
            if (p_GameBoardSource->get(i,j) == true) {
                cout << "█";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
    
    usleep(100000);
}

GameBoard::GameBoard(int maxRow, int maxCol)
    : m_board(maxRow, Row_t(maxCol)){}

int GameBoard::countNeighbors(int row, int col) {
    size_t maxRow = m_board.size();
    size_t maxCol = m_board[0].size();
    
    int count = 0;

    // is North Exist
    if (row > 0) {
        // count += hasNorth();
        count += m_board[row-1][col];
    }
    
    // is NorthEast Exist
    if (row > 0 && col < (maxCol -1)) {
        // count += hasNorthEast();
        count += m_board[row-1][col+1];
    }
    
    // is East Exist
    if (col < (maxCol - 1)) {
        // count += hasEast();
        count += m_board[row][col+1];
    }
    // is SouthEast Exist
    if (row < (maxRow - 1) && col < (maxCol - 1)) {
        // count += hasSouthEast();
        count += m_board[row+1][col+1];
    }

    // is South Exist
    if (row < (maxRow - 1)) {
        // count += hasSouth();
        count += m_board[row+1][col];
    }

    // is SouthWest Exist
    if (row < (maxRow -1) && col > 0) {
        // count += hasSouthWest();
        count += m_board[row+1][col-1];
    }

    // is West Exist
    if (col > 0) {
        // count += hasWest();
        count += m_board[row][col-1];
    }

    // is NorthWest Exist
    if (row > 0 && col > 0) {
        // count += hasNorthWest();
        count += m_board[row-1][col-1];
    }
    return count;
}


void GameBoard::set(int row, int col, bool value) {
    if (row < m_board.size() && col < m_board[0].size()) {
        m_board[row][col] = value;

        
        if (value == true) {
            affectedParts[row][col] = true;

            size_t maxRow = m_board.size();
            size_t maxCol = m_board[0].size();
            
            // is North Exist
            if (row > 0) {
                // count += hasNorth();
                affectedParts[row-1][col] = true;
            }
            
            // is NorthEast Exist
            if (row > 0 && col < (maxCol -1)) {
                // count += hasNorthEast();
                affectedParts[row-1][col+1] = true;
            }
            
            // is East Exist
            if (col < (maxCol - 1)) {
                // count += hasEast();
                affectedParts[row][col+1] = true;
            }
            // is SouthEast Exist
            if (row < (maxRow - 1) && col < (maxCol - 1)) {
                // count += hasSouthEast();
                affectedParts[row+1][col+1] = true;
            }
            
            // is South Exist
            if (row < (maxRow - 1)) {
                // count += hasSouth();
                affectedParts[row+1][col] = true;
            }
            
            // is SouthWest Exist
            if (row < (maxRow -1) && col > 0) {
                // count += hasSouthWest();
                affectedParts[row+1][col-1] = true;
            }
            
            // is West Exist
            if (col > 0) {
                // count += hasWest();
                affectedParts[row][col-1] = true;
            }
            
            // is NorthWest Exist
            if (row > 0 && col > 0) {
                // count += hasNorthWest();
                affectedParts[row-1][col-1] = true;
            }
        }
    }
}

bool GameBoard::get(int row, int col) {
    if (row < m_board.size() && col < m_board[0].size()) {
        return m_board[row][col];
    }
    else {
        throw (out_of_range("GameBoard::get - out of range"));
    }
}

bool GameBoard::applyRules(int row, int col) {
    bool isAlive = m_board[row][col];
    int countN = countNeighbors(row, col);
    if (isAlive) {
        if (countN <= 1) {
            // isolation
            return false;
        } else if (countN <= 3 ) {
            // survival
            return true;
        } else {
            // overpopulation
            return false;
        }
    } else {
        if (countN == 3) {
            // revival
            return true;
        }
    }
    return isAlive;
}
GameBoard::~GameBoard() {}


int main(int argc, const char * argv[]) {
    // insert code here...

    
#if 0
#define MAX_ROW (5)
#define MAX_COL (5)

    Game myGame(MAX_ROW, MAX_COL);
    
    unique_ptr<GameBoard> p_initialBoard = make_unique<GameBoard>(MAX_ROW,MAX_COL);

    p_initialBoard->set(1,2,true);
    p_initialBoard->set(2,2,true);
    p_initialBoard->set(3,2,true);

    p_initialBoard->set(2,6,true);
    p_initialBoard->set(2,7,true);
    p_initialBoard->set(2,8,true);
#else
#define MAX_ROW (17)
#define MAX_COL (17)
    Game myGame(MAX_ROW, MAX_COL);
    
    unique_ptr<GameBoard> p_initialBoard = make_unique<GameBoard>(MAX_ROW,MAX_COL);

    

    p_initialBoard->set(4,2,true);
    p_initialBoard->set(5,2,true);
    p_initialBoard->set(6,2,true);
    p_initialBoard->set(10,2,true);
    p_initialBoard->set(11,2,true);
    p_initialBoard->set(12,2,true);

    p_initialBoard->set(4,7,true);
    p_initialBoard->set(5,7,true);
    p_initialBoard->set(6,7,true);
    p_initialBoard->set(10,7,true);
    p_initialBoard->set(11,7,true);
    p_initialBoard->set(12,7,true);
    
    p_initialBoard->set(4,9,true);
    p_initialBoard->set(5,9,true);
    p_initialBoard->set(6,9,true);
    p_initialBoard->set(10,9,true);
    p_initialBoard->set(11,9,true);
    p_initialBoard->set(12,9,true);

    p_initialBoard->set(4,14,true);
    p_initialBoard->set(5,14,true);
    p_initialBoard->set(6,14,true);
    p_initialBoard->set(10,14,true);
    p_initialBoard->set(11,14,true);
    p_initialBoard->set(12,14,true);

    
    p_initialBoard->set(2,4,true);
    p_initialBoard->set(2,5,true);
    p_initialBoard->set(2,6,true);
    p_initialBoard->set(2,10,true);
    p_initialBoard->set(2,11,true);
    p_initialBoard->set(2,12,true);

    p_initialBoard->set(7,4,true);
    p_initialBoard->set(7,5,true);
    p_initialBoard->set(7,6,true);
    p_initialBoard->set(7,10,true);
    p_initialBoard->set(7,11,true);
    p_initialBoard->set(7,12,true);

    p_initialBoard->set(9,4,true);
    p_initialBoard->set(9,5,true);
    p_initialBoard->set(9,6,true);
    p_initialBoard->set(9,10,true);
    p_initialBoard->set(9,11,true);
    p_initialBoard->set(9,12,true);

    p_initialBoard->set(14,4,true);
    p_initialBoard->set(14,5,true);
    p_initialBoard->set(14,6,true);
    p_initialBoard->set(14,10,true);
    p_initialBoard->set(14,11,true);
    p_initialBoard->set(14,12,true);

#endif

    myGame.setBoard(p_initialBoard);
    myGame.display();
    for (auto i = 0 ; i < 100 ; i++)
        myGame.next();


    std::cout << "Hello, World!\n";
    return 0;
}
