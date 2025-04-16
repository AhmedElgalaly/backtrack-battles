#include "GameSolver.h"
#include <iostream>

bool GameSolver::isGoodState(const GameState& state)
{
	std::cerr << "Evaluating state:\n" << state.toString() << "\n";
	// Check memoization cache first
	if (memoizationCache.find(state) != memoizationCache.end())
	{
		currentBestMove.second = memoizationCache[state].bestMove;
		return memoizationCache[state].isGood;
	}

	// Base case: current Player has already won
	if (state.isWinningForPlayer(state.getCurrentPlayer())) {
		memoizationCache[state] = { true, GameState::Move(-1, -1, -1, -1) }; // No move needed
		return true;
	}

	// Generate all possible moves
	// used auto to avoid writing the full type whic is std::vector<GameState>
	auto moves = state.generateAllPossibleMoves();
	bool isGood = false;
	GameState::Move bestMove(-1, -1, -1, -1); // Initialize bestMove

	// Check if there are no moves available (not necessary because the base case is already checked but for safety)
	if (moves.empty()) {
		memoizationCache[state] = { false, GameState::Move(-1, -1, -1, -1) }; // No moves available
		return false;
	}

	// Recurisively check each move
	for (const auto& move : moves) {
		std::cerr << state.toString() << "\n";
		GameState nextState = state.applyMove(move);

		// recursively check if the opponent is in a bad state
		bool opponentIsGood = isGoodState(nextState);

		if (!opponentIsGood) {
			// If the opponent is in a bad state, we have a winning move
			memoizationCache[state] = { true, move };
			currentBestMove = { state, move };
			isGood = true;
			break; // No need to check further moves
		}
	}

	// if all moves lead to good states for the opponent, we are in a bad state
	if (!isGood) {
		memoizationCache[state] = { false, GameState::Move(-1, -1, -1, -1) }; // No winning move found
	}

	return isGood;
}

GameSolver::GameSolver(const GameState& initialState) :currentBestMove{ initialState, GameState::Move(-1, -1, -1, -1) }
{
}

bool GameSolver::solve()
{
	return isGoodState(currentBestMove.first);
}

std::pair<GameState, GameState::Move> GameSolver::getBestMove() const
{
	return currentBestMove;
}

bool GameSolver::hasWinningStrategy() const
{
	// Check if memoization cache is empty
	if (memoizationCache.empty())
	{
		return false; // No winning strategy found
	}
	return memoizationCache.at(currentBestMove.first).isGood;
}
