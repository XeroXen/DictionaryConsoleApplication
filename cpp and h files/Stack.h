//Jonathon Lewis
//5/16/2020
//Queue.h
//A template-class wrapper for a basic stack, supports push, pop, top, and empty.
//Probably will get very little use in the actual program, but I implemented it as a third data structure.

#pragma once


template<class T>
//basic implementation of a queue.
class Stack {
	struct Node {
		Node* next;
		T payload;
	};

private:
	//stores the top of the stack.
	Node* top;
	int size;

	//creates a new node when necessary.
	Node* createNode(T payload) {
		Node* newNode = new Node;
		newNode->next = NULL;
		newNode->payload = payload;

		return newNode;
	}

public:

	Stack() {
		top = NULL;
		size = 0;
	}

	//pushes new data to the top of the stack.
	void push(T toPush) {
		Node* newNode = createNode(toPush);

		if (!top) {
			top = newNode;
			top->next = NULL;
			size++;
		}

		else {
			newNode->next = top;
			top = newNode;
			size++;
		}
	}
	
	//returns what's on the top of the stack
	T topOfStack() {
		return top->payload;
	}

	//pops the top bit of data off the stack.
	void popThis() {
		Node* oldTop = top;
		top = top->next;
		size--;

		delete oldTop;
	}

	bool empty() {
		return (size == 0);
	}
};