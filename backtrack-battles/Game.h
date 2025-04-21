#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "GameState.h"
#include "GameSolver.h"

const int CELL_SIZE = 80;

class Game {
private:
    GameState state;
    bool gameOver;
    GameState::Player winner;
    int boardSize;
    sf::Font font;

public:
    Game(int size) : state(size), gameOver(false), winner(GameState::Player::PLAYER1),
        boardSize(size* CELL_SIZE) {
        if (!font.openFromFile("Arial.ttf")) {
            std::cerr << "Failed to load font\n";
        }
    }

    void handleClick(int mouseX, int mouseY) {
        if (gameOver || state.getCurrentPlayer() != GameState::Player::PLAYER2) return;

        int col = mouseX / CELL_SIZE;
        int row = mouseY / CELL_SIZE;

        // Generate all possible moves for human player (PLAYER2)
        auto moves = state.generateAllPossibleMoves();
        for (const auto& move : moves) {
            if (move.fromRow == row && move.fromCol == col) {

                try {
                    state = state.applyMove(move);
                    checkWinCondition();
                    if (!gameOver) {
                        // CPU makes move
                        cpuMove();
                    }
                    return;
                }
                catch (...) {
                    // Invalid move
                }
            }
        }
    }

    void cpuMove() {
        if (state.getCurrentPlayer() == GameState::Player::PLAYER1) {
            // 1. Explicit type declaration for clarity
            GameState::Move bestMove(-1, -1, -1, -1);
            int highestPriority = INT_MIN;

            // 2. Direct move evaluation with early exit
            const std::vector<GameState::Move> possibleMoves = state.generateAllPossibleMoves();

            // 3. Simple heuristic: prioritize most progressive moves
            for (const GameState::Move& move : possibleMoves) {
                // Calculate move priority (custom heuristic)
                int priority = calculateMovePriority(move);

                if (priority > highestPriority) {
                    highestPriority = priority;
                    bestMove = move;
                }
            }

            // 4. Fallback to first valid move if no better option
            if (bestMove.fromRow == -1 && !possibleMoves.empty()) {
                bestMove = possibleMoves.front();
            }

            // 5. Apply the selected move
            if (bestMove.fromRow != -1) {
                state = state.applyMove(bestMove);
                checkWinCondition();
            }
        }
    }
    int calculateMovePriority(const GameState::Move& move) {
        // Reward vertical progress for Player 1 (CPU)
        if (state.getCurrentPlayer() == GameState::Player::PLAYER1) {
            return (move.toRow - move.fromRow) * 2; // Prioritize bigger jumps
        }
        // Reward horizontal progress for Player 2
        else {
            return (move.toCol - move.fromCol) * 2;
        }
    }

    void checkWinCondition() {
        if (state.isWinningForPlayer(GameState::Player::PLAYER1)) {
            winner = GameState::Player::PLAYER1;
            gameOver = true;
        }
        else if (state.isWinningForPlayer(GameState::Player::PLAYER2)) {
            winner = GameState::Player::PLAYER2;
            gameOver = true;
        }
    }

    void draw(sf::RenderWindow& window) {
        // Draw board grid
        for (int row = 0; row < state.getSize(); ++row) {
            for (int col = 0; col < state.getSize(); ++col) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                cell.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
                cell.setFillColor(getCellColor(row, col));
                window.draw(cell);
            }
        }

        // Draw tokens
        for (int row = 0; row < state.getSize(); ++row) {
            for (int col = 0; col < state.getSize(); ++col) {
                auto status = state.getCellStatus(row, col);
                if (status != GameState::CellStatus::EMPTY) {
                    sf::CircleShape circle(CELL_SIZE / 2 - 10);
                    circle.setPosition(sf::Vector2f(col * CELL_SIZE + 10, row * CELL_SIZE + 10));
                    circle.setFillColor(status == GameState::CellStatus::PLAYER_1 ?
                        sf::Color::Red : sf::Color::Green);
                    window.draw(circle);
                }
            }
        }

        // Draw game over text
        if (gameOver) {
            sf::Font font;
            if (font.openFromFile("Arial.ttf")) {
                sf::Text winText(font,"",50);
                winText.setFont(font);
                winText.setCharacterSize(50);
                winText.setFillColor(sf::Color::Black);

                if (winner == GameState::Player::PLAYER1)
                    winText.setString("CPU Wins!");
                else if (winner == GameState::Player::PLAYER2)
                    winText.setString("You Win!");

                // Center the text
                sf::FloatRect bounds = winText.getLocalBounds();
                winText.setPosition(sf::Vector2f(
                    (boardSize - bounds.size.x) / 2.f,
                    (boardSize - bounds.size.y) / 2.f
                ));
                window.draw(winText);
            }
            else {
                std::cerr << "Failed to load font for game over message\n";
            }
        }
    }
    void run() {
        sf::RenderWindow window(sf::VideoMode(sf::Vector2u(boardSize, boardSize)), "Token Tactics");

        // Initial CPU move if first player
        if (state.getCurrentPlayer() == GameState::Player::PLAYER1) {
            cpuMove();
        }

        while (window.isOpen()) {
            while (optional event = window.pollEvent()) {
                if (!event) continue;

                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);
                    handleClick(mouse.x, mouse.y);
                }
            }

            window.clear(sf::Color::White);
            draw(window);
            window.display();
        }
    }


private:
    sf::Color getCellColor(int row, int col) const {
        // Your existing cell coloring logic
        if ((row == 0 && col == 0) || (row == 0 && col == state.getSize() - 1) ||
            (row == state.getSize() - 1 && col == 0) || (row == state.getSize() - 1 && col == state.getSize() - 1)) {
            return sf::Color::Black;
        }
        return (row > 0 && row < state.getSize() - 1 && col > 0 && col < state.getSize() - 1) ?
            sf::Color(220, 255, 220) : sf::Color(200, 200, 200);
    }
};