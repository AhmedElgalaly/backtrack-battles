#pragma once
#include <vector>
#include <unordered_set>
#include <string>
#include <utility>
#include "pair_hash.cpp"

using namespace std;

class GameState
{
	// Private members were moved to the bottom because they depend on Player and CellStatus

public:
	// Player and CellStatus type enumerations
	enum class Player { PLAYER1, PLAYER2 };
	enum class CellStatus { EMPTY, PLAYER_1, PLAYER_2 };
	
	// Move structure to represent a move in the game
	struct Move
	{
		int fromRow;
		int fromCol;
		int toRow;
		int toCol;

		// Default constructor
		Move() : fromRow(-1), fromCol(-1), toRow(-1), toCol(-1) {}

		Move(int fromRow, int fromCol, int toRow, int toCol)
			: fromRow(fromRow), fromCol(fromCol), toRow(toRow), toCol(toCol) {
		}

		bool operator==(const Move& other) const;
	};

	// Adding hash support for GameState class
	struct GameStateHash {
		size_t operator()(const GameState& state) const;
	};


	// Constructor and Destructor
	explicit GameState(int size);
	GameState(const GameState& other);
	~GameState();

	// Copy assignment operator
	GameState& operator=(const GameState& other);

	// GameState comparison operator
	bool operator==(const GameState& other) const;

	// Game mechanics
	vector<Move> generateAllPossibleMoves() const;
	bool isWinning() const;
	bool isWinningForPlayer(Player player) const;
	GameState applyMove(int fromRow, int fromCol, int toRow, int toCol) const;
	// overloading the applyMove function to take Move object
	GameState applyMove(const Move& move) const;

	// Accessors
	Player getCurrentPlayer() const;
	int getSize() const;
	CellStatus getCellStatus(int row, int col) const;

	// View functions (for debugging if GUI is still in development)
	string toString() const;

	// Check if the move is valid
	bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;

	// Overloading isValidMove function to take Move object
	bool isValidMove(const Move& move) const;

private:
	// Core GameState attributes
	int size; // Size of the game board
	vector<vector<CellStatus>> boardGrid; // 2D vector representing the game board
	unordered_set<pair<int, int>,pair_hash> Player1Tokens; // Set of Player 1 tokens
	unordered_set<pair<int, int>,pair_hash> Player2Tokens; // Set of Player 2 tokens
	Player currentPlayer; // Current player

	// Helper functions
	void moveToken(int fromRow, int fromCol, int toRow, int toCol);
	void switchPlayer();

	// Boundary check
	bool isInBounds(int row, int col) const;

};

