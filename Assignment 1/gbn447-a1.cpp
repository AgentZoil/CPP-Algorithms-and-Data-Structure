/*
Student Number: 7547821
Student Name: Gia Bach Nhu
Assignment 1
CSCI203
*/


#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

//Assume that the input file contains no more than 50,000 different words
const int MAX_WORDS = 50000;
const int WORD_LENGTH = 25;

// Structure to store word data and its frequency
struct WordData {
    char word[WORD_LENGTH];
    int frequency;
};

// Read and process the input file
void readAndProcessFile(const char *filename, WordData *wordDataArray, int &numWords);

// Insert word data into the array
void insertWordData(WordData *wordDataArray, int &numWords, const char *word);

// Sort the word data array by frequency and lexicographical order
void sortWordDataArray(WordData *wordDataArray, int numWords);

// Display the results
void displayResults(WordData *wordDataArray, int numWords);

int main() {
    char filename[20];
    WordData wordDataArray[MAX_WORDS];
    int numWords = 0;

    cout << "Please enter the name of the input file: ";
    cin >> filename;

    // Read, process, sort, and display the results
    readAndProcessFile(filename, wordDataArray, numWords);
    sortWordDataArray(wordDataArray, numWords);
    displayResults(wordDataArray, numWords);

    return 0;
}

// Read and process the input file
void readAndProcessFile(const char *filename, WordData *wordDataArray, int &numWords) {
    ifstream inputFile(filename);
    char word[WORD_LENGTH];

    if (!inputFile) {
        cerr << "Error opening file " << filename << ". Program will exit." << endl;
        exit(1);
    }

    while (inputFile >> word) {
        // Remove punctuation and convert to lowercase
        int len = strlen(word);
        int j = 0;
        for (int i = 0; i < len; i++) {
            if (isalpha(word[i])) {
                word[j] = tolower(word[i]);
                j++;
            }
        }
        word[j] = '\0';
        if (j > 0) {
            insertWordData(wordDataArray, numWords, word);
        }
    }

    inputFile.close();
}

// Insert word data into the array
void insertWordData(WordData *wordDataArray, int &numWords, const char *word) {
    for (int i = 0; i < numWords; i++) {
        if (strcmp(wordDataArray[i].word, word) == 0) {
            wordDataArray[i].frequency++;
            return;
        }
    }

    if (numWords < MAX_WORDS) {
        strcpy(wordDataArray[numWords].word, word);
        wordDataArray[numWords].frequency = 1;
        numWords++;
    }
}

// Sort the word data array by frequency and lexicographical order
void sortWordDataArray(WordData *wordDataArray, int numWords) {
    for (int i = 0; i < numWords - 1; i++) {
        for (int j = 0; j < numWords - i - 1; j++) {
            if (wordDataArray[j].frequency < wordDataArray[j + 1].frequency ||
                (wordDataArray[j].frequency == wordDataArray[j + 1].frequency && strcmp(wordDataArray[j].word, wordDataArray[j + 1].word) > 0)) {
                swap(wordDataArray[j], wordDataArray[j + 1]);
            }
        }
    }
}

// Display the results
void displayResults(WordData *wordDataArray, int numWords) {
    cout << "First ten words in the sorted list, along with their counts: " << endl;
    for (int i = 0; i < min(10, numWords); i++) {
        cout << wordDataArray[i].word << " : " << wordDataArray[i].frequency << endl;
    }

    cout << " " << endl;
    cout << "Last ten words in the sorted list, along with their counts: " << endl;
    for (int i = max(0, numWords - 10); i < numWords; i++) {
        cout << wordDataArray[i].word << " : " << wordDataArray[i].frequency << endl;
    }

    cout << " " << endl;
    cout << "Unique words: " << endl;
    for (int i = 0; i < numWords; i++) {
        if (wordDataArray[i].frequency == 1) {
            cout << wordDataArray[i].word << " : " << wordDataArray[i].frequency << endl;
        }
    }
}
