#include <iostream>
#include <sstream>
#include <limits>
#include "GameState.h"
#include "GameSolver.h"
#include "Menu.h"

// Function declarations
void runTerminalVersion();
void runGUIVersion();
void printTerminalBoard(const GameState& state);
void printTerminalHelp(int size);
void clearTerminalInputBuffer();
bool processTerminalCommand(GameState& state, const std::string& input);
bool handleTerminalPlayerMove(GameState& state);

int main() {
    std::cout << "=== TOKEN TACTICS ===\n";
    std::cout << "Select game mode:\n";
    std::cout << "1. Terminal Version (Text-based)\n";
    std::cout << "2. GUI Version (SFML)\n";
    std::cout << "Enter choice (1 or 2): ";

    int choice;
    while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter 1 or 2: ";
    }

    // Clear input buffer before proceeding
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1) {
        runTerminalVersion();
    }
    else {
        runGUIVersion();
    }

    return 0;
}

void runGUIVersion() {
    Menu menu;
    menu.run();
}


void runTerminalVersion() {
    const int BOARD_SIZE = 4;
    GameState state(BOARD_SIZE);

    std::cout << "=== TERMINAL VERSION ===\n";
    printTerminalHelp(BOARD_SIZE);

    while (true) {
        printTerminalBoard(state);

        if (!handleTerminalPlayerMove(state)) {
            break;
        }
    }
}

void printTerminalBoard(const GameState& state) {
    std::cout << "\nCurrent Board:\n";
    // Print column numbers
    for (int col = 0; col < state.getSize(); col++) {
        std::cout << col << " ";
    }
    std::cout << "\n";

    std::cout << state.toString() << "\n";
    std::cout << "Current Player: "
        << (state.getCurrentPlayer() == GameState::Player::PLAYER1 ? "1 (CPU)" : "2 (Human)") << "\n";
}

void printTerminalHelp(int size) {
    std::cout << "\nHOW TO PLAY (Terminal Version):\n";
    std::cout << "1. Enter moves as: from_row from_col to_row to_col\n";
    std::cout << "2. Player 1 (X) is CPU (moves automatically)\n";
    std::cout << "3. Player 2 (O) is Human (you)\n";
    std::cout << "4. Valid coordinates: 0-" << size - 1 << "\n";
    std::cout << "5. Type 'help' to show this message\n";
    std::cout << "6. Type 'quit' to exit\n\n";
}

void clearTerminalInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool processTerminalCommand(GameState& state, const std::string& input) {
    if (input == "help") {
        printTerminalHelp(state.getSize());
        return true;
    }
    if (input == "quit") {
        std::cout << "Game ended.\n";
        return false;
    }
    return true;
}

bool handleTerminalPlayerMove(GameState& state) {
    int fromRow, fromCol, toRow, toCol;
    while (true) {
        std::cout << "Player "
            << (state.getCurrentPlayer() == GameState::Player::PLAYER1 ? "1 (CPU)" : "2 (Human)")
            << "'s turn. Enter move: ";

        std::string input;
        std::getline(std::cin, input);

        if (!processTerminalCommand(state, input)) {
            return false;
        }

        std::istringstream iss(input);
        if (iss >> fromRow >> fromCol >> toRow >> toCol) {
            try {
                GameState::Player currentPlayerBeforeMove = state.getCurrentPlayer();
                GameState newState = state.applyMove(fromRow, fromCol, toRow, toCol);
                state = newState;

                if (newState.isWinningForPlayer(currentPlayerBeforeMove)) {
                    printTerminalBoard(state);
                    std::cout << "Player "
                        << (currentPlayerBeforeMove == GameState::Player::PLAYER1 ? "1 (CPU)" : "2 (Human)")
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