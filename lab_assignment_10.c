#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26


struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count; 
};

struct TrieNode *createTrieNode() {
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode) {
        newNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            newNode->children[i] = NULL;
    }
    return newNode;
}


void insert(struct TrieNode *root, char *word) {
    struct TrieNode *node = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!node->children[index])
            node->children[index] = createTrieNode();
        node = node->children[index];
    }
    node->count++;
}


int numberOfOccurances(struct TrieNode *root, char *word) {
    struct TrieNode *node = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!node->children[index])
            return 0;
        node = node->children[index];
    }
    return node->count;
}


void deallocateTrie(struct TrieNode *node) {
    if (!node)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        deallocateTrie(node->children[i]);

    free(node);
}

int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file)
        return 0;

    char word[256];
    int count = 0;
    
    while (fscanf(file, "%255s", word) != EOF) {
        pInWords[count] = strdup(word);
        count++;
    }
    fclose(file);

    return count;
}

int main(void) {
    char *inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *root = createTrieNode();
    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
        free(inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(root, pWords[i]));
    }

    deallocateTrie(root);
    return 0;
}
