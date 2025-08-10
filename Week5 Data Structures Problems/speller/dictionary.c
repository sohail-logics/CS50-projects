// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // for strcasecmp

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table.
// Chosen large enough to reduce collisions for provided dictionaries.
const unsigned int N = 65536;

// Hash table
static node *table[N];

// Number of words loaded into dictionary
static unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    if (word == NULL)
    {
        return false;
    }

    // Hash the word (case-insensitive)
    unsigned int index = hash(word);

    // Traverse linked list at this bucket
    node *cursor = table[index];
    while (cursor != NULL)
    {
        // Case-insensitive comparison
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number (djb2, case-insensitive)
unsigned int hash(const char *word)
{
    // djb2 by Dan Bernstein, adapted to be case-insensitive
    unsigned long hash = 5381;
    for (int i = 0; word[i] != '\0'; i++)
    {
        unsigned char c = (unsigned char) word[i];
        c = (unsigned char) tolower(c);
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return (unsigned int)(hash % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Initialize table (not strictly necessary because static global is zeroed,
    // but safe to explicitly initialize)
    for (unsigned int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    word_count = 0;

    // Buffer to hold each word
    char word[LENGTH + 1];

    // Read words from file, one per line
    while (fscanf(file, "%45s", word) != EOF)
    {
        // Allocate node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            // Allocation failed: clean up and return false
            fclose(file);
            unload();
            return false;
        }

        // Copy word into node
        strcpy(new_node->word, word);

        // Compute hash index
        unsigned int index = hash(word);

        // Insert node at head of list for this bucket
        new_node->next = table[index];
        table[index] = new_node;

        // Increment word count
        word_count++;
    }

    // Close dictionary file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        table[i] = NULL;
    }

    // Reset word count
    word_count = 0;

    return true;
}
