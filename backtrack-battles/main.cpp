#include <iostream>
#include "GameState.h"
#include <limits>
#include <sstream>

// remove it if you don't need it
void printBoard(const GameState& state) {
    std::cout << "\nCurrent Board:\n";
	for (int col = 0; col < state.getSize(); col++)
		std::cout << col << " ";
	std::cout << "\n";
    std::cout << state.toString() << "\n";
}

void printHelp(int size) {
    std::cout << "\nHOW TO PLAY:\n";
    std::cout << "1. Enter moves as: from_row from_col to_row to_col\n";
    std::cout << "2. Player 1 () moves down\n";
    std::cout << "3. Player 2 () moves right\n";
    std::cout << "4. Valid coordinates: 0-" << size - 1 << "\n";
    std::cout << "5. Type 'help' to show this message\n";
    std::cout << "6. Type 'quit' to exit\n\n";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool processCommand(GameState& state, const std::string& input) {
    if (input == "help") {
        printHelp(state.getSize());
        return true;
    }
    if (input == "quit") {
        std::cout << "Game ended.\n";
        return false;
    }
    return true;
}

bool handlePlayerMove(GameState& state) {
    int fromRow, fromCol, toRow, toCol;
    while (true) {
        std::cout << "Player " << (state.getCurrentPlayer() == GameState::Player::PLAYER1 ? "1" : "2")
            << "'s turn. Enter move: ";

        std::string input;
        std::getline(std::cin, input);

        if (!processCommand(state, input)) return false;

        std::istringstream iss(input);
        if (iss >> fromRow >> fromCol >> toRow >> toCol) {
            try {
                // Store current player before the move
                GameState::Player currentPlayerBeforeMove = state.getCurrentPlayer();

                GameState newState = state.applyMove(fromRow, fromCol, toRow, toCol);
                state = newState;

                // Check if the move caused the current player (before switch) to win
                if (newState.isWinningForPlayer(currentPlayerBeforeMove)) {
                    printBoard(state);
                    std::cout << "Player " << (currentPlayerBeforeMove == GameState::Player::PLAYER1 ? "1" : "2")
                        << " wins!\n";
                    return false;
                }
                return true;
            }
            catch (...) {
                std::cout << "Invalid move! Try again or type 'help'.\n";
            }
        }
        else if (!input.empty()) {
            std::cout << "Invalid format! Use: row col row col\n";
        }
    }
}

int main() {
    const int BOARD_SIZE = 4;
    GameState state(BOARD_SIZE);

    std::cout << "=== TOKEN TACTICS ===\n";
    printHelp(BOARD_SIZE);

    while (true) {
        printBoard(state);

        if (!handlePlayerMove(state)) {
            break;
        }
    }

    return 0;
}