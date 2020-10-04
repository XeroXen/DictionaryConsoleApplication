//Jonathon Lewis
//5/16/2020
//LinkedList.h
//My implementation of a linked list, it's pretty straightforward and not super efficient. Can be indexed with the [] operator, the search is linear though.
//This is used primarily to hold definitions of words, because they're small. It was originally being used to hold all the words before they're written to file, but that took
//way too long compared to a vector (an hour versus half a minute for all A-Z english words), so I unfortunately had to scrap that part in the interest of getting this done on time.

#pragma once
template <class T>
class LinkedList {
	struct Node {
		T payload;
		Node* next;
	};

private:

	int size;
	Node* root;

	Node* createNode(T toAdd) {
		Node* newNode = new Node;
		newNode->payload = toAdd;
		newNode->next = NULL;

		return newNode;
	}

public:

	LinkedList() {
		size = 0;
	}

	void add(T toAdd) {
		Node* newNode = createNode(toAdd);
		Node* iter = root;
		if (!iter) {
			root = newNode;
			size++;
			return;
		}

		else {
			while (iter->next) iter = iter->next;
			iter->next = newNode;
			size++;
		}
	}

	int getSize() {
		return this->size;
	}

	//will need delete later

	//Allows you to index the linked list.
	T* operator[](int i) {
		if (i > (size - 1)) {
			return nullptr;
		}
		else {
			int count = 0;
			Node* iter = root;
			while (count < i) {
				iter = iter->next;
				count++;
			}

			return &(iter->payload);
		}
	}
};