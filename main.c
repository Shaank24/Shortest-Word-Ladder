// Program 5: Find the Shortest Word Ladder
// Course: CS 211, Fall 2023, UIC
// System: Advanced zyLab
// Author: Shaan Kohli

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

// counts number of words of size wordSize
// return count if filename is valid, otherwise returns -1
int countWordsOfLength(char* filename, int wordSize) { 
 
    // opens file and check that it opened correctly
    FILE* fileOpened = fopen(filename, "r");
    if (fileOpened == NULL) {
        return -1;
    }

    // initializes count and array to store words
    int count = 0;
    char word[101];

    // reads each word from the file
    while (fscanf(fileOpened, "%101s", word) == 1) {
        int wordLength = strlen(word);

        // increments count if the current word size matches the word size in the function parameter
        if (wordLength == wordSize) {
            count++;
        }
    }

    fclose(fileOpened);
    
    return count;
}

// fills word array with words of size wordSize and contains numWords words
// return true if this is done succesfully, otherwise returns false
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 

    // opens file and checks that it opened correctly
    FILE* fileOpened = fopen(filename, "r");
    if (fileOpened == NULL) {
        return false;
    }

    // initializes count and array for words
    int count = 0;
    char word[100];

    // reads all the words in the file
    while (fscanf(fileOpened, "%s", word) == 1) {

        // checks that the word length matches wordSize
        if (strlen(word) == wordSize) {

            // copies word if the count remains less than numWords
            if (count < numWords) {
                strncpy(words[count], word, wordSize + 1);
                count++;
            }

            // closes file and returns false if count is >= wordSize
            else {
                fclose(fileOpened);
                return false;
            }
        }

    }

    fclose(fileOpened);

    // returns either true/false depending on if the counter matches numWords
    return count == numWords;
}

// uses binary search to find aWord
// returns index of aWord if found, otherwise returns -1
int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    
    // loops while low index is not greater than the high index
    while (loInd <= hiInd) {

        // finds the middle index and compares the word at that index with aWord
        int midIndex = loInd + (hiInd - loInd) / 2;
        int compares = strcmp(words[midIndex], aWord);

        // this means we have found the index of the word we are searching for
        if (compares == 0) {
            return midIndex;
        }

        // this means that the index is to the right of the middle index so we change the low index
        else if (compares < 0) {
            loInd = midIndex + 1;
        }

        // this means that the index is to the left of the middle index so we change the high index
        else {
            hiInd = midIndex - 1;
        }
    }
    
    // word is not found
    return -1; 
}

// frees all space allocated for words
void freeWords(char** words, int numWords) {
    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

// inserts a new word at the front of the ladder
void insertWordAtFront(WordNode** ladder, char* newWord) {

    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));

    // sets the word of the new node
    newNode->myWord = newWord;

    // sets the new node's next to be the ladder(the head node)
    newNode->next = *ladder;

    // sets the new head node to be the new node that was just inserted
    *ladder = newNode;

}

// returns the number of words in the ladder list
int getLadderHeight(WordNode* ladder) {

    // initializes height and creates variable to store ladder
    int height = 0;
    WordNode* currNode = ladder;

    // traverses through linked list until the end and increments height counter
    while (currNode != NULL) {
        height++;
        currNode = currNode->next;
    }

    return height; 
}

// creates a copy of a given ladder
WordNode* copyLadder(WordNode* ladder) {

    // returns NULL if the ladder is empty
    if (ladder == NULL) {
        return NULL;
    }

    // allocates new space for the ladderCopy
    WordNode* ladderCopy = malloc(sizeof(WordNode));

    
    if (ladderCopy != NULL) {
        // sets myWord of the ladder copy to be the same as the original ladder's myWord
        ladderCopy->myWord = ladder->myWord;

        // recursively updates every word in ladderCopy
        ladderCopy->next = copyLadder(ladder->next);
    }

    return ladderCopy;
}

// frees the ladder
void freeLadder(WordNode* ladder) {

    WordNode* curr = ladder;

    // temporarily stores the current node, moves to the next node, and frees the temp stored node
    while (curr != NULL) {
        WordNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
}

// inserts a new ladder at the end of the list
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
  
    // allocates memory for new ladderNode and sets topWord
    LadderNode* newNode = malloc(sizeof(LadderNode));
    newNode->topWord = newLadder;
    newNode->next = NULL;

    // handles edge case if list is empty
    if (*list == NULL) {
        *list = newNode;
    }
    else {
        // finds the last node in the list and sets the next to the new ladderNode
        LadderNode* curr = *list;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
}

// gets back the ladder in the front of the list and removes it from the list
WordNode* popLadderFromFront(LadderNode** list) {
 
    // checks if list is empty
    if (*list == NULL) {
        return NULL;
    }

    // stores old head node and the ladder to be popped
    LadderNode* oldHeadNode = *list;
    WordNode* poppedLadder = oldHeadNode->topWord;

    // updates the head of the list to the next node and frees the old head node memory
    *list = oldHeadNode->next;
    free(oldHeadNode);

    return poppedLadder;   
}

// frees the ladder list
void freeLadderList(LadderNode* myList) {

    LadderNode* currLadderNode = myList;
    LadderNode* temp;

    while (currLadderNode != NULL) {
        // frees the ladder first
        freeLadder(currLadderNode->topWord);

        // moves to the next ladderNode
        temp = currLadderNode;
        currLadderNode = currLadderNode->next;

        // frees the current ladderNode
        free(temp);
    }
}

// implements algorithm to find the shorted word ladder between a given start and final word
WordNode* findShortestWordLadder(char** words, bool* usedWord, int numWords, int wordSize, char* startWord, char* finalWord ) {

    // initializes "queue" for word ladders
    LadderNode* queueForWord = NULL;

    // creates a starting ladder and inserts it
    WordNode* startLadder = (WordNode*)malloc(sizeof(WordNode));
    startLadder->myWord = startWord;
    startLadder->next = NULL;
    insertLadderAtBack(&queueForWord, startLadder);

    // while there are ladders in the queue, continue
    while (queueForWord != NULL) {

        // dequeues ladder and gets last word from it
        WordNode* currLadder = popLadderFromFront(&queueForWord);
        char* currWord = currLadder->myWord;

        // if last word in ladder is final word, free remaining ladders and return the correct ladder
        if (strcmp(currWord, finalWord) == 0) {
            freeLadderList(queueForWord);
            return currLadder;
        }

        // checks each word in the word list
        for (int i = 0; i < numWords; i++) {

            // checks that the word is not used and the right length
            if (!usedWord[i] && strlen(words[i]) == wordSize) {
                int diffCount = 0;

                // compares every character of the word with the currWord
                for (int j = 0; j < wordSize; j++) {
                    if (currWord[j] != words[i][j]) {
                        diffCount++;
                    }
                    if (diffCount > 1) {
                        break;
                    }
                }

                // checks if the words only differ by 1 character
                if (diffCount == 1) {

                    // copies curr ladder and appends the new word
                    WordNode* newLadder = copyLadder(currLadder);
                    insertWordAtFront(&newLadder, words[i]);
                    insertLadderAtBack(&queueForWord, newLadder);
                    usedWord[i] = true;
                }
            }
        }

        // frees the memory of the current ladder 
        freeLadder(currLadder);
    }

    // returns NULL only if there is no solution 
    return NULL;
}

// interactive user-input to set a word; 
//  ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

// TEST FUNCTION
void test_countWordsOfLength() {
    if (countWordsOfLength("sampleDict.txt", 3) != 7) {
        printf("countWordsOfLength does not output the correct number of 3 letter words\n");
    }
    if (countWordsOfLength("sampleDict.txt", 4) != 4) {
        printf("countWordsOfLength does not output the correct number of 4 letter words\n");
    }
    if (countWordsOfLength("sampleDict.txt", 5) != 2) {
        printf("countWordsOfLength does not output the correct number of 5 letter words\n");
    }
    
    printf("countWordsOfLength is functioning correctly\n");
}

// TEST FUNCTION
void test_buildWordArray() {
    int wordSize = 3;
    int numWords = 7;
    char file[] = "sampleDict.txt";
    char** words = malloc(numWords * sizeof(char*));

    for (int i = 0; i < numWords; i++) {
        words[i] = malloc((wordSize + 1) * sizeof(char));
    }
    
    bool result = buildWordArray(file, words, numWords, wordSize);

    if (result == true) {
        printf("buildWordArray PASSED!!!\n");
    }
    else {
        printf("buildWordArray FAILED!!!\n");
    }

    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

// TEST FUNCTION
void test_findWord() {
    char* wordsArray[] = {"apple", "banana", "cherry", "grape"};
    char* wordToFind = "apple";

    int idx = findWord(wordsArray, wordToFind, 0, 3);
    if (idx != -1) {
        printf("THE WORD %s IS FOUND AT INDEX %d\n", wordToFind, idx);
    }
    else {
        printf("DID NOT FIND WORD\n");
    }
}

// HELPER TEST FUNCTION
void printWords(WordNode* ladder) {
    WordNode* currNode = ladder;

    while (currNode != NULL) {
        printf("%s ", currNode->myWord);
        currNode = currNode->next;
    }
    printf("\n");
}

// TEST FUNCTION
void test_insertWordAtFront() {
    WordNode* ladder = NULL;

    char word1[] = "hello";
    char* word1Dup = malloc(strlen(word1) + 1);
    strcpy(word1Dup, word1);
    insertWordAtFront(&ladder, word1Dup);
    printWords(ladder);

    char word2[] = "world";
    char* word2Dup = malloc(strlen(word2) + 1);
    strcpy(word2Dup, word2);
    insertWordAtFront(&ladder, word2Dup);
    printWords(ladder);

    char word3[] = "test";
    char* word3Dup = malloc(strlen(word3) + 1);
    strcpy(word3Dup, word3);
    insertWordAtFront(&ladder, word3Dup);
    printWords(ladder);

    WordNode* currNode;
    while (ladder != NULL) {
        currNode = ladder;
        ladder = ladder->next;
        free(currNode->myWord);
        free(currNode);
    }   
}

// TEST FUNCTION
void test_getLadderHeight() {
    WordNode* ladder = NULL;

    for (int i = 0; i < 3; i++) {
        WordNode* newNode = malloc(sizeof(WordNode));
        newNode->myWord = malloc(5);
        sprintf(newNode->myWord, "Word%d", i);
        newNode->next = ladder;
        ladder = newNode;
    }

    int height = getLadderHeight(ladder);
    if (height == 3) {
        printf("getLadderHeight PASSED!!!\n");
    }
    else {
        printf("getLadderHeight FAILED!!!\n");
    }

    WordNode* curr = ladder;
    while (curr != NULL) {
        WordNode* freeThis = curr;
        curr = curr->next;
        free(freeThis->myWord);
        free(freeThis);
    }
}

// TEST FUNCTION
void test_copyLadder() {
    WordNode* origLadder = NULL;

    char* word1 = "hello";
    insertWordAtFront(&origLadder, word1);

    char* word2 = "world";
    insertWordAtFront(&origLadder, word2);

    char* word3 = "test";
    insertWordAtFront(&origLadder, word3);

    WordNode* copiedLadder = copyLadder(origLadder);
 
    printf("Original Ladder: ");
    printWords(origLadder);

    printf("Copied Ladder: ");
    printWords(copiedLadder);
    
    origLadder->myWord = "change";
    printf("After change, original ladder is: ");
    printWords(origLadder);

    printf("After change, copied ladder is: ");
    printWords(copiedLadder);
}

// TEST FUNCTION
void test_insertLadderAtBack() {
    LadderNode* ladderList = NULL;

    WordNode* ladder = NULL;

    char* word1 = "hello";
    insertWordAtFront(&ladder, word1);

    insertLadderAtBack(&ladderList, ladder);

    if (ladderList == NULL || ladderList->topWord != ladder) {
        printf("insertLadderAtBack FAILED\n");
    }
    else {
        printf("insertLadderAtBack PASSED\n");
    }

    WordNode* anotherLadder = NULL;

    char* word2 = "world";
    insertWordAtFront(&anotherLadder, word2);

    insertLadderAtBack(&ladderList, anotherLadder);

    LadderNode* lastNode = ladderList->next;

    if (lastNode == NULL || lastNode->topWord != anotherLadder) {
        printf("insertLadderAtBack FAILED\n");
    }
    else {
        printf("insertLadderAtBack PASSED\n");
    }
}

// HELPER TEST FUNCTION
void printLadderList(LadderNode* list) {
    LadderNode* curr = list;
    while (curr != NULL) {
        printWords(curr->topWord);
        printf("\n");
        curr = curr->next;
    }
}

// TEST FUNCTION
void test_popLadderFromFront() {
    LadderNode* list = NULL;

    WordNode* ladder1 = NULL;
    insertWordAtFront(&ladder1, "apple");
    insertWordAtFront(&ladder1, "grape");
    insertWordAtFront(&ladder1, "orange");

    WordNode* ladder2 = NULL;
    insertWordAtFront(&ladder2, "banana");
    insertWordAtFront(&ladder2, "peach");
    insertWordAtFront(&ladder2, "watermelon");

    WordNode* ladder3 = NULL;
    insertWordAtFront(&ladder3, "cherry");
    insertWordAtFront(&ladder3, "blackberry");
    insertWordAtFront(&ladder3, "strawberry");

    insertLadderAtBack(&list, ladder1);
    insertLadderAtBack(&list, ladder2);
    insertLadderAtBack(&list, ladder3);

    printf("Initial List:\n");
    printLadderList(list);

    for (int i = 0; i < 3; i++) {
        WordNode* poppedLadder = popLadderFromFront(&list);
        printf("\nPopped Ladder: ");
        printWords(poppedLadder);

        printf("\nRemaining list:\n");
        printLadderList(list);
    }
}

// TEST FUNCTION
void test_findShortestWordLadder() {
    char* words[] = {"data", "date", "gate", "gave", "cave", "cove", "code"};
    int numWords = sizeof(words) / sizeof(words[0]);

    bool usedWord[numWords];

    for (int i = 0; i < numWords; i++) {
        usedWord[i] = false;
    }

    char startWord[] = "data";
    char finalWord[] = "code";

    WordNode* resultLadder = findShortestWordLadder(words, usedWord, numWords, 4, startWord, finalWord);

    if (resultLadder != NULL) {
        printf("findShortestWordLadder PASSED!!! Shortest Word Ladder found:\n");
        WordNode* curr = resultLadder;

        while (curr != NULL) {
            printf("%s ", curr->myWord);
            curr = curr->next;
        }
        printf("\n");
    }
    else {
        printf("No word ladder found.\n");
    }
}

//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main()
//-----------------------------------------------------
int main() {
    srand((int)time(0));

    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
