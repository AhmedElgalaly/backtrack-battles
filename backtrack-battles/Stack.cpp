#include <stdexcept>

/**

    @class   Stack
	@brief   Stack class for managing game states.
	@details Stack class that provides methods to push, pop, and access the top game state
	using linkedlist for dynamic memory management.
	@member topNode - Pointer to the top node of the stack.
	@methods push, pop, top, isEmpty - Methods to manipulate the stack except for isEmpty.
	@destructor - Destructor to clean up the stack when it goes out of scope and to prevent memory leaks.
	@tparam  GameStateType - The type of the game state stored in the stack.

**/
template<typename GameStateType>
class Stack
{
private:

/**
	@struct StackNode
	@brief  structure representing a node in the stack.
	@details This structure contains a game state and a pointer to the next node in the stack.
	Generic programming GameStateType type is used to make it data type-independent.
	@tparam GameStateType - The type of the game state stored in the stack node.
**/
	struct StackNode{
		GameStateType gameState;
		StackNode* next;
	};
	StackNode* topNode;


public:
	Stack() {
		topNode = nullptr;
	}

	bool isEmpty() const{
		return topNode == nullptr;
	}

	~Stack() {
		while (!isEmpty()) {
			StackNode* temp = topNode;
			topNode = topNode->next;
			delete temp;
		}
	}

	void push(const GameStateType& gameState) {
		StackNode* newNode = new StackNode{ gameState, topNode };
		/*newNode->gameState = gameState;
		newNode->next = topNode;*/
		topNode = newNode;
	}

	GameStateType& pop() {
		if (isEmpty()) {
			throw std::out_of_range("Stack is empty");
		}
		StackNode* temp = topNode;
		GameStateType data = topNode->gameState;
		topNode = topNode->next;
		delete temp;
		return data;
	}

	GameStateType& top() const {
		if (isEmpty()) {
			throw std::out_of_range("Stack is empty");
		}
		return topNode->gameState;
	}

};