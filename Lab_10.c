#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    int count;
    struct Trie *children[26];
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* newNode = malloc(sizeof(struct Trie));
    for (int i = 0; i < 26; i++)
    {
        newNode->children[i] = NULL;
    }
    newNode->count = 0;
    return newNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie* currentNode = pTrie;
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a'; // Assuming only lowercase alphabets (a-z) are considered
        if (currentNode->children[index] == NULL)
        {
            currentNode->children[index] = createTrie();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie* currentNode = pTrie;
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a'; // Assuming only lowercase alphabets (a-z) are considered
        if (currentNode->children[index] == NULL)
        {
            return 0;
        }
        currentNode = currentNode->children[index];
    }
    return currentNode->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < 26; i++)
    {
        if (pTrie->children[i] != NULL)
        {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
    return NULL;
}


// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        perror("fopen");
        return 0;
    }

    int numWords;
    if (fscanf(file, "%d", &numWords) != 1)
    {
        printf("Error reading the number of words from the file.\n");
        fclose(file);
        return 0;
    }

    int i;
    char word[256];
    for (i = 0; i < numWords && fscanf(file, "%s", word) == 1; i++)
    {
        pInWords[i] = strdup(word);
    }

    fclose(file);
    return i;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
    for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}