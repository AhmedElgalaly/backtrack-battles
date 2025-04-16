#include "GameState.h"
#include <stdexcept>
#include <iostream>

GameState::GameState(int size)
{
	if (size < 3)
	{
		// for debugging purposes ONLY comment on push
		std::cerr << "Invalid size: " << size << std::endl;
		throw std::invalid_argument("Size must be at least 3.");
	}
	this->size = size;
	boardGrid.resize(size, vector<CellStatus>(size, CellStatus::EMPTY));
	currentPlayer = Player::PLAYER1;

	// Initialize player1 tokens on top border
	for (int col = 1; col < size-1; col++)
	{
		boardGrid[0][col] = CellStatus::PLAYER_1;
		Player1Tokens.insert({ 0, col });
	}

	// Initialize player2 tokens on left border
	for (int row = 1; row < size-1; row++)
	{
		boardGrid[row][0] = CellStatus::PLAYER_2;
		Player2Tokens.insert({ row, 0 });
	}
}

GameState::GameState(const GameState& other)
{
	this->size = other.size;
	this->boardGrid = other.boardGrid;
	this->currentPlayer = other.currentPlayer;
	this->Player1Tokens = other.Player1Tokens;
	this->Player2Tokens = other.Player2Tokens;
}

GameState::~GameState()
{
	// Destructor implementation
	// No dynamic memory allocation, so nothing to clean up
}

GameState& GameState::operator=(const GameState& other)
{
	// TODO: insert return statement here
	if (this != &other) {
		this->size = other.size;
		this->boardGrid = other.boardGrid;
		this->currentPlayer = other.currentPlayer;
		this->Player1Tokens = other.Player1Tokens;
		this->Player2Tokens = other.Player2Tokens;
	}
	return *this;
}

bool GameState::operator==(const GameState& other) const
{
	return boardGrid == other.boardGrid &&
			currentPlayer == other.currentPlayer &&
			Player1Tokens == other.Player1Tokens &&
			Player2Tokens == other.Player2Tokens;
}

// Generate all possible moves for the current player
vector<GameState::Move> GameState::generateAllPossibleMoves() const
{
	vector<Move> moves;

	if (currentPlayer == Player::PLAYER1) {
		for (const auto& token : Player1Tokens) {
			int row = token.first;
			int col = token.second;
			// Check all possible moves for Player 1
			
			// Regualr move down (1 step)
			if (row + 1 < size && boardGrid[row + 1][col] == CellStatus::EMPTY) {
				moves.emplace_back(row, col, row + 1, col);
			}

			// Jump move down (2 steps over opponent)
			if (row + 2 < size && boardGrid[row + 1][col] == CellStatus::PLAYER_2 && boardGrid[row + 2][col] == CellStatus::EMPTY) {
				moves.emplace_back(row, col, row + 2, col);
			}
		}
	}
	else {
		for (const auto& token : Player2Tokens) {
			int row = token.first;
			int col = token.second;
			// Check all possible moves for Player 2
			// Regular move right (1 step)
			if (col + 1 < size && boardGrid[row][col + 1] == CellStatus::EMPTY) {
				moves.emplace_back(row, col, row, col + 1);
			}
			// Jump move right (2 steps over opponent)
			if (col + 2 < size && boardGrid[row][col + 1] == CellStatus::PLAYER_1 && boardGrid[row][col + 2] == CellStatus::EMPTY) {
				moves.emplace_back(row, col, row, col + 2);
			}
		}
	}

	return moves;
}

bool GameState::isWinning() const
{
	if (currentPlayer == Player::PLAYER1) {
		// Check if Player 1 has reached the bottom row
		for (const auto& token : Player1Tokens) {
			if (token.first != size - 1) {
				return false;
			}
		}
		return true;
	}
	else {
		// Check if Player 2 has reached the right column
		for (const auto& token : Player2Tokens) {
			if (token.second != size - 1) {
				return false;
			}
		}
		return true;
		
	}
}

bool GameState::isWinningForPlayer(Player player) const
{
	if (player == Player::PLAYER1) {
		// Player 1 wins if all tokens are at the bottom
		for (const auto& token : Player1Tokens) {
			if (token.first != size - 1) return false;
		}
		return true;
	}
	else {
		// Player 2 wins if all tokens are at the right
		for (const auto& token : Player2Tokens) {
			if (token.second != size - 1) return false;
		}
		return true;
	}
}

GameState GameState::applyMove(int fromRow, int fromCol, int toRow, int toCol) const
{
	if (!isValidMove(fromRow, fromCol, toRow, toCol)) {
		// for debugging purposes ONLY comment on push
		std::cerr << "Invalid move attempted: ("
			<< fromRow << "," << fromCol << ") to ("
			<< toRow << "," << toCol << ")\n";
		throw std::invalid_argument("Invalid move");
	}
	GameState newState(*this); // Create a copy of the current state without referencing
	newState.moveToken(fromRow, fromCol, toRow, toCol);
	newState.switchPlayer(); // Switch the player after applying the move
	return newState;
}

GameState GameState::applyMove(const Move& move) const
{
	if (!isValidMove(move.fromRow, move.fromCol, move.toRow, move.toCol)) {
		// for debugging purposes ONLY comment on push
		std::cerr << "Invalid move attempted: ("
			<< move.fromRow << "," << move.fromCol << ") to ("
			<< move.toRow << "," << move.toCol << ")\n";
		throw std::invalid_argument("Invalid move");
	}
	GameState newState(*this); // Create a copy of the current state without referencing
	newState.moveToken(move.fromRow, move.fromCol, move.toRow, move.toCol);
	newState.switchPlayer(); // Switch the player after applying the move
	return newState;
}

// GameState::Player because Player is a nested enum class inside GameState
GameState::Player GameState::getCurrentPlayer() const
{
	return currentPlayer;
}

int GameState::getSize() const
{
	return size;
}
// GameState::CellStatus because CellStatus is a nested enum class inside GameState
GameState::CellStatus GameState::getCellStatus(int row, int col) const
{
	return boardGrid[row][col];
}

string GameState::toString() const
{
	string boardAsString;

	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			/*if (boardGrid[row][col] == CellStatus::EMPTY)
			{
				boardAsString += ". ";
			}
			else if (boardGrid[row][col] == CellStatus::PLAYER_1)
			{
				boardAsString += "X ";
			}
			else if (boardGrid[row][col] == CellStatus::PLAYER_2)
			{
				boardAsString += "O ";
			}*/

			// after searching we found that switch-case is faster than if-else in this case
			switch (boardGrid[row][col])
			{
			case CellStatus::EMPTY:
				boardAsString += ". ";
				break;
			case CellStatus::PLAYER_1:
				boardAsString += "X ";
				break;
			case CellStatus::PLAYER_2:
				boardAsString += "O ";
				break;
			}
		}
		boardAsString += "\n";
	}

	return boardAsString;
}

bool GameState::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const
{
	if (!isInBounds(fromRow, fromCol) || !isInBounds(toRow, toCol))
		return false;

	// Prevent moving to corners 
	if ((fromRow == 0 && fromCol == 0) || (fromRow == 0 && fromCol == size - 1) ||
		(fromRow == size - 1 && fromCol == 0) || (fromRow == size - 1 && fromCol == size - 1))
		return false;

	const CellStatus playerCell = (currentPlayer == Player::PLAYER1) ? CellStatus::PLAYER_1 : CellStatus::PLAYER_2;
	if (boardGrid[fromRow][fromCol] != playerCell)
		return false;

	if (currentPlayer == Player::PLAYER1) {
		// Must move down
		if (toCol != fromCol || toRow <= fromRow) return false;

		if (toRow == fromRow + 1) {
			// Regular move
			return boardGrid[toRow][toCol] == CellStatus::EMPTY;
		}
		else if (toRow == fromRow + 2) {
			// Jump move
			return boardGrid[fromRow + 1][fromCol] == CellStatus::PLAYER_2 &&
				boardGrid[toRow][toCol] == CellStatus::EMPTY;
		}
	}
	else { // Player 2
		// Must move right
		if (toRow != fromRow || toCol <= fromCol) return false;

		if (toCol == fromCol + 1) {
			// Regular move
			return boardGrid[toRow][toCol] == CellStatus::EMPTY;
		}
		else if (toCol == fromCol + 2) {
			// Jump move
			return boardGrid[fromRow][fromCol + 1] == CellStatus::PLAYER_1 &&
				boardGrid[toRow][toCol] == CellStatus::EMPTY;
		}
	}
	return false;
}

bool GameState::isValidMove(const Move& move) const
{
	return isValidMove(move.fromRow, move.fromCol, move.toRow, move.toCol);
}

void GameState::moveToken(int fromRow, int fromCol, int toRow, int toCol)
{
	if (boardGrid[fromRow][fromCol] == CellStatus::PLAYER_1)
	{
		Player1Tokens.erase({ fromRow, fromCol });
		Player1Tokens.insert({ toRow, toCol });
	}
	else {
		Player2Tokens.erase({ fromRow, fromCol });
		Player2Tokens.insert({ toRow, toCol });
	}

	boardGrid[toRow][toCol] = boardGrid[fromRow][fromCol];
	boardGrid[fromRow][fromCol] = CellStatus::EMPTY;
}

void GameState::switchPlayer()
{
	currentPlayer = (currentPlayer == Player::PLAYER1) ? Player::PLAYER2 : Player::PLAYER1;
}

bool GameState::isInBounds(int row, int col) const
{
	if (row < 0 || row >= size || col < 0 || col >= size)
	{
		return false;
	}

	return true;
}

bool GameState::Move::operator==(const Move& other) const
{
	return fromCol == other.fromCol &&
		fromRow == other.fromRow &&
		toCol == other.toCol &&
		toRow == other.toRow;
}

// Hash function for GameState
// This function generates a hash value for the GameState object
// because GameState is a complex object, we need to hash its attributes and to avoid collisions while making solve() function
size_t GameState::GameStateHash::operator()(const GameState& state) const
{
	size_t hashValue = 0;
	// Hash the size of the board
	for (const auto& row : state.boardGrid) {
		for (const auto& cell : row) {
			hashValue ^= static_cast<int>(cell) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
		}
	}

	// Hash the current player
	hashValue ^= static_cast<int>(state.currentPlayer);

	return hashValue;
}
