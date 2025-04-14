#pragma once
template<typename GameStateType>
struct StackNode
{
	GameStateType data;
	StackNode* next;
};

template<typename GameStateType>
class Stack
{
private:
	StackNode* topNode;

public:
	Stack();
	~Stack();
	void push(GameStateType& gameState);
	GameStateType pop();
	GameStateType top();
	bool isEmpty();


};