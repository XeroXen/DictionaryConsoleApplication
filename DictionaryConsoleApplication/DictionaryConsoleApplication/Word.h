//Jonathon Lewis
//5/16/2020
//Word.h
//The class object for a word. It holds the value of the word as well as definitions of the word.
//In retrospect, it may have been more efficient to use a map to hold words -> definitions, but the program is already done. Oops!

#pragma once

#include <string>
#include <iostream>
#include "LinkedList.h"

using namespace std;

//A basic word class. Holds onto the word and definitions, to be inserted into the AVL tree.
class Word {
private:
	string word;
	LinkedList<string> definitions;

public:
	Word() {
		//do nothing
	}

	Word(string word) {
		this->word = word;
	}

	Word(string word, LinkedList<string> def) {
		this->word = word;
		this->definitions = def;
	}

	string getWord() {
		return this->word;
	}

	void addDefinition(string def) {
		definitions.add(def);
	}

	int getNumDefinitions() {
		return definitions.getSize();
	}

	//These need to be comparable for the AVL tree.
	bool operator>(Word& otherWord) {
		return (this->word > otherWord.getWord());
	}

	bool operator<(Word& otherWord) {
		return (this->word < otherWord.getWord());
	}

	bool operator==(Word& otherWord) {
		return (this->word == otherWord.getWord());
	}

	//This is to index individual definitions.
	string operator[](int i) {
		return *(definitions[i]);
	}

	//prints the word in a different way than the stream insertion operator.
	void printThisWord() {
		cout << word << "\n-------Definitions-------\n";
		int numDef = getNumDefinitions();
		if (!numDef) cout << "No definitions found";
		else {
			for (int i = 0; i < numDef; i++) {
				cout << i + 1 << ") " << *(definitions[i]);
			}
		}
	}

	friend ostream & operator <<(ostream& out, Word toPrint);
};

//used for writing to a file.
ostream& operator <<(ostream& out, Word toPrint) {
	int numDefinitions = toPrint.definitions.getSize();
	out << toPrint.getWord() << endl << numDefinitions << endl;
	for (int i = 0; i < numDefinitions; i++) {
		out << toPrint[i] << endl;
	}

	return out;
}