#include "GameSolver.h"
#include <chrono>
#include <iostream>
#include <cassert>

using namespace std;
using namespace std::chrono;

template<typename Duration>
void printTestResult(const string& testName, bool passed, Duration duration) {
    cout << (passed ? "[PASS] " : "[FAIL] ") << testName << " - "
        << duration_cast<milliseconds>(duration).count() << "ms\n";
}

// Test 1: Immediate win in 1 move (3x3 board)
void testImmediateWin() {
    auto start = high_resolution_clock::now();

    GameState state(3);
    // Setup: Player 1 can win immediately by moving off board
    state = state.applyMove(GameState::Move(0, 1, 1, 1)); // P1 down
    state = state.applyMove(GameState::Move(1, 0, 1, 1)); // P2 right (blocks)

    // Now P1 can jump off board (1,1)->(3,1)
    GameSolver solver(state);
    bool result = solver.solve();

    auto stop = high_resolution_clock::now();
    assert(result == true);
    auto bestMove = solver.getBestMove().second;
    assert(bestMove.fromRow == 1 && bestMove.fromCol == 1 &&
        bestMove.toRow == 3 && bestMove.toCol == 1); // Off board
    printTestResult("Immediate Win Detection", true, stop - start);
}

// Test 2: Forced win in 3 moves (4x4 board)
void testForcedWin() {
    auto start = high_resolution_clock::now();

    GameState state(4);
    /* Initial state:
       . X X .
       O . . .
       O . . .
       . . . . */

    GameSolver solver(state);
    bool result = solver.solve();

    auto stop = high_resolution_clock::now();
    assert(result == true); // Player 1 should have forced win

    // Verify the recommended first move is valid
    auto firstMove = solver.getBestMove().second;
    GameState nextState = state.applyMove(firstMove);
    assert(nextState.isValidMove(firstMove));

    printTestResult("Forced Win Detection", true, stop - start);
}

// Test 3: No possible win (blocked early)
void testNoPossibleWin() {
    auto start = high_resolution_clock::now();

    GameState state(3);
    // Setup completely blocked position
    state = state.applyMove(GameState::Move(0, 1, 1, 1)); // P1
    state = state.applyMove(GameState::Move(1, 0, 1, 1)); // P2 blocks

    GameSolver solver(state);
    bool result = solver.solve();

    auto stop = high_resolution_clock::now();
    assert(result == false); // No winning moves exist
    printTestResult("No Win Detection", true, stop - start);
}

// Test 4: Win by exiting all tokens
void testMultiTokenExit() {
    auto start = high_resolution_clock::now();

    GameState state(4);
    /* Setup:
       . X X .
       . . . .
       . . . .
       . . . . */

       // Move both Player 1 tokens to exit
    state = state.applyMove(GameState::Move(0, 1, 1, 1));
    state = state.applyMove(GameState::Move(1, 1, 2, 1));
    state = state.applyMove(GameState::Move(2, 1, 4, 1)); // Exit first token

    state = state.applyMove(GameState::Move(0, 2, 1, 2));
    state = state.applyMove(GameState::Move(1, 2, 2, 2));
    state = state.applyMove(GameState::Move(2, 2, 4, 2)); // Exit second token

    assert(state.isWinningForPlayer(GameState::Player::PLAYER1));

    auto stop = high_resolution_clock::now();
    printTestResult("Multi-Token Exit Win", true, stop - start);
}

int main() {
    try {
        cout << "=== Testing GameSolver::solve() ===\n";

        testImmediateWin();
        testForcedWin();
        testNoPossibleWin();
        testMultiTokenExit();

        cout << "All solver tests passed successfully!\n";
        return 0;
    }
    catch (const exception& e) {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
}