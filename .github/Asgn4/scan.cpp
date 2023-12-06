#include <iostream>
#include <string>
#include <sstream>
using namespace std;
string *getcom(){
    string input;
    
    // Get the input line from the user
    getline(cin, input);

    // Create an input string stream to tokenize the input
    istringstream iss(input);

    // Set a maximum number of words (adjust as needed)
    const int maxWords = 256;
    string *words = new string[maxWords];

    int wordCount = 0;

    string word;
    while (iss >> word && wordCount < maxWords) {
        // Tokenize the input by space (whitespace) using the >> operator
        words[wordCount] = word;
        wordCount++;
    }


    return words;
}

