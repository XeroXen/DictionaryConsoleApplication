//Jonathon Lewis
//5/16/2020
//FinalProject.cpp
//My final data structures project. Not as ambitious as originally planned, thanks to bugfixing...

#include <iostream>
#include <fstream>
#include <string>
#include "AVLTree.h"
#include "LinkedList.h"
#include "Word.h"
#include "Stack.h"
#include <vector> //used at the very end to write to file, my linkedlist is way too slow comparatively, and I didn't have time to improve.

using namespace std;

//a tolower for strings, returns the lowercase representation of a string.
string tolower(string input) {
    string toReturn;
    for (int i = 0; i < input.size(); i++) {
        toReturn.push_back(tolower(input[i]));
    }

    return toReturn;
}

//gets input and returns the lowercase representation of the word entered.
Word getWordInput(string prompt) {
    std::cout << prompt;
    string input;
    cin >> input;

    cin.ignore(INT_MAX, '\n');

    return Word(tolower(input));
}

//will get the input for a definition.
string getDefinitionInput(string prompt) {
    std::cout << prompt;
    string input;
    getline(cin, input);

    return tolower(input);
}

int main()
{
    AVLTree<Word> dictionary;
    ifstream createDictionary;
    createDictionary.open("dictionary.txt");

    std::cout << "Loading dictionary. This will take some time, please wait...";
    while (!(createDictionary.peek() == EOF)) {
        string word, num;
        LinkedList<string> definitions;
        getline(createDictionary, word);
        getline(createDictionary, num);
        for (int i = 0; i < stoi(num); i++) {
            string definition;
            getline(createDictionary, definition);

            definitions.add(definition);
        }

        //Add this word to dictionary. This takes a while because it has to make all the objects and then put them into place in a (rather large) BST.
        //Note that we aren't sorting the tree at all, this program assumes that the tree is already sorted. The one included with the project IS indeed sorted
        //which took quite a bit of time because it's every word from A-Z in the english language. If you decide to test with your own set of words, ensure
        //that dictionary.add(word, false) is replaced with dictionary.add(word)
        Word thisWord(word, definitions);
        dictionary.add(thisWord, false);
    }

    //create main menu, to add/search for words.
    int input;
    Stack<string> operationsPerformed; //in a more developed program, can be used to undo actions.

    //this handles the menu, very basic.
    do {
        std::cout << "\n\n1) Search for word\n2) Add Word\n3) Add Definition\n0) Exit Program\n\nEnter a command: ";

        while (!(cin >> input) || input < 0 || input > 3) {
            std::cout << "Invalid input, try again: ";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }

        cin.ignore(INT_MAX, '\n');

        Word wordInput;
        string defInput;
        Word* found;

        switch (input) {
        case 1:
            //searches for the word in the AVLTree, and prints it + its definitions if found.
            wordInput = getWordInput("\nEnter a word to search for: ");
            found = dictionary.search(wordInput);
            if (found) {
                std::cout << endl;
                found->printThisWord();
            }
            else std::cout << "\nWord not found.";
            operationsPerformed.push("Searched for word " + wordInput.getWord());
            break;
        case 2:
            //Adds a word to the dictionary if it doesn't already exist.
            wordInput = getWordInput("\nEnter a word to add: ");
            found = dictionary.search(wordInput);
            if (!found) {
                std::cout << "Adding " << wordInput << " to dictionary.";
                dictionary.add(wordInput);
                operationsPerformed.push("Added " + wordInput.getWord() + " to the dictionary.");
            }
            else std::cout << "Word already exists in dictionary.";
            operationsPerformed.push("Attempted to add " + wordInput.getWord() + " to dictionary, but it already existed.");
            break;
        case 3:
            //Adds a definition to a word as long as that word exists.
            wordInput = getWordInput("\nEnter a word to add a definition to: ");
            found = dictionary.search(wordInput);
            if (!found) {
                std::cout << "\nWord not found.";
                operationsPerformed.push("Attempted to add a definition to the word " + wordInput.getWord() + ", but it didn't exist.");
            }
            else {
                defInput = getDefinitionInput("Enter a definition to add to the word: ");
                found->addDefinition(defInput);
                std::cout << "Definition added successfully";
                operationsPerformed.push("Added definition \"" + defInput + "\" to word " + wordInput.getWord());
            }
            break;
        case 0:
            break;
        }
    } while (input != 0);
    
    cout << "Listing performed operations in reverse order...\n\n";
    while (!(operationsPerformed.empty())) {
        string operation = operationsPerformed.topOfStack();
        cout << operation << endl;
        operationsPerformed.popThis();
    }

    //Saves the dictionary to a file. Used to be way faster when it was just string literals, but
    //introducting the Word object slowed it down SIGNIFICANTLY.
    std::cout << "\nSaving changes to dictionary...";

    //First we generate a linkedlist of words (which is rather fast)
    vector<Word> words;
    dictionary.vectorPreorder(words);

    //Then we write them to the file. It's not super fast, but faster than reading.
    ofstream outputFile;
    outputFile.open("dictionary.txt");
    int size = words.size();
    for (int i = 0; i < size; i++) {

        int k = words[i].getNumDefinitions(); //save this so we're only ever doing this operation once (it adds up with very large datasets...)
        outputFile << words[i];
    }

    std::cout << "\nDone!";
}
