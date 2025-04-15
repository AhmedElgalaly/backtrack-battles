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

	// Core recursive solving function
	bool isGoodState(const GameState& state);

	// Memoization Cache were we store the results of previously computed states
	std::unordered_map<GameState, StateResult, GameState::GameStateHash> memoizationCache;

	// Current best moves found
	std::pair<GameState, GameState::Move> bestMoves;

	// State stack for backtracking
	Stack<GameState> stateStack;

public:
	// Constructor
	explicit GameSolver(const GameState& initialState);

	// Main solving function
	bool solve();

	// Get the best move found
	std::pair<int, int> getBestMove() const;

	// Check if a winning startegy exists
	bool hasWinningStrategy() const;

};

