#include "GameSolver.h"
#include <iostream>

// this function is replaced by the processNextFrame function to meet the requirement
// of the stack-based backtracking for dfs in the project requirements on the e-learning
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

bool GameSolver::processNextFrame()
{
	if (stateStack.isEmpty()) return false;

	StackFrame& frame = stateStack.top();

	if (frame.evaluated) {
		stateStack.pop();
		return true;
	}

	// Check memoization using dynamic board state
	auto memoIt = memoizationCache.find(frame.state);
	if (memoIt != memoizationCache.end()) {
		currentBestMove.second = memoIt->second.bestMove;
		stateStack.pop();
		return true;
	}

	// Dynamic win condition check
	const int currentSize = frame.state.getSize();
	if (frame.state.isWinningForPlayer(frame.state.getCurrentPlayer())) {
		memoizationCache[frame.state] = { true, GameState::Move(-1,-1,-1,-1) };
		currentBestMove = { frame.state, GameState::Move(-1,-1,-1,-1) };
		stateStack.pop();
		return true;
	}

	// Process moves using board size-agnostic generation
	if (frame.moveIndex < frame.moves.size()) {
		GameState::Move move = frame.moves[frame.moveIndex++];
		GameState nextState = frame.state.applyMove(move);
		stateStack.push(StackFrame(nextState, nextState.generateAllPossibleMoves()));
		return true;
	}

	// Evaluate all possible moves dynamically
	bool isGood = false;
	GameState::Move bestMove(-1, -1, -1, -1);

	// Check all possible subsequent states
	for (const auto& move : frame.moves) {
		GameState nextState = frame.state.applyMove(move);
		auto it = memoizationCache.find(nextState);

		// Size-agnostic win potential check
		if (it != memoizationCache.end()) {
			if (!it->second.isGood) {
				isGood = true;
				bestMove = move;
				break;
			}
		}
		else {
			// Handle unexplored states
			StackFrame newFrame(nextState, nextState.generateAllPossibleMoves());
			stateStack.push(newFrame);
			return true;
		}
	}

	// Store results for current state
	memoizationCache[frame.state] = { isGood, bestMove };
	if (isGood) {
		currentBestMove = { frame.state, bestMove };
	}

	frame.evaluated = true;
	return true;
}

GameSolver::GameSolver(const GameState& initialState) :currentBestMove{ initialState, GameState::Move(-1, -1, -1, -1) }
{
	stateStack.push(StackFrame(initialState, initialState.generateAllPossibleMoves()));
}

bool GameSolver::solve()
{
	/*return isGoodState(currentBestMove.first);*/
	while (!stateStack.isEmpty()) {
		if (!processNextFrame()) {
			break; // No more frames to process
		}
	}

	// return the result of the last processed frame
	return memoizationCache[currentBestMove.first].isGood;
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

GameSolver::StackFrame::StackFrame(GameState s, vector<GameState::Move> m) : state(s), moveIndex(0), moves(m), evaluated(false)
{
}
