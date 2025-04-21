#pragma once
#include <unordered_map>
#include "Stack.cpp"
#include <vector>
#include <utility>
// #include <stdexcept> // Uncomment if you need to throw exceptions
#include "GameState.h"

class GameSolver
{
private:
	// Memoization structure
	struct StateResult
	{
		bool isGood;
		GameState::Move bestMove;
	};

	// Stack-based backtracking state
	struct StackFrame {
		GameState state;
		size_t moveIndex; // Current move being processed
		vector<GameState::Move> moves;
		bool evaluated;

		// Constructor
		StackFrame() = default;
		explicit StackFrame(GameState s, vector<GameState::Move> m);
	};

	// Core recursive solving function using backtracking with memoization and minimax decision making
	bool isGoodState(const GameState& state);

	// Memoization Cache were we store the results of previously computed states
	std::unordered_map<GameState, StateResult, GameState::GameStateHash> memoizationCache;

	// Current best moves found
	std::pair<GameState, GameState::Move> currentBestMove;

	// State stack for backtracking
	Stack<StackFrame> stateStack;

	bool processNextFrame();

public:
	// Constructor
	explicit GameSolver(const GameState& initialState);

	// Main solving function
	bool solve();

	// Get the best move found
	std::pair<GameState, GameState::Move> getBestMove() const;

	// Check if a winning startegy exists
	bool hasWinningStrategy() const;

};

