/*
 * FILE: m3.cpp
 * PROJECT: DS3
 * PROGRAMMER:MANREET THIND (Student ID: [8982315]
 * FIRSTVERSION: 07/16/24
 * DESCRIPTION :
 * This program handles searching and displaying information to  compare the efficiency of searching a sorted linked list and
 * a hash table. 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 127//maximum size of hash table
#define MAX_LENGTH 30//maximum length of name
#define MAX_SEARCH 5//maximum search allowed

typedef struct Node 
{
	char* name;//pointer to the name string
	struct Node* next;//pointer to next node in linked list
}Node;

typedef struct	HashTable
{
	Node* buckets[TABLE_SIZE];//Array of pointers to head of list
}HashTable;

/*
 * Function: hashFunction()
 * Parameter:
 * const char* str: pointer to a C-style string to be hashed.
 * Return Value:
 * int: the index of the hash table bucket.
 * Description: This function computes a hash value for the input string.
 */
int hashFunction(const char* str)
{
	unsigned int hash = 0;
	while (*str)
	{
		hash = (hash << 5) + *str++;
	}
	return hash % TABLE_SIZE;
}

/*
 * Function: toLowerCase()
 * Parameter:
 * char* str: pointer to a C-style string to be converted to lowercase.
 * Return Value: void
 * Description: This function converts all characters in the string to lowercase.
 */
void toLowerCase(char* str)
{
	for (int i = 0; str[i]; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] = str[i] + ('a' - 'A');
		}
	}
}

/*
 * Function: strDuplicate()
 * Parameter:
 * const char* str: pointer to a C-style string to be duplicated.
 * Return Value:
 * char*: pointer to the newly duplicated string.
 * Description: This function creates a new string that is a duplicate of the input string.
 */
char* strDuplicate(const char* str)
{
	if (str == NULL)
	{
		return NULL;
	}

	size_t len = strlen(str) + 1;
	//Allocating memory for duplicated str
	char* dup = (char*)malloc(strlen(str) + 1);
	if (dup == NULL)
	{
		fprintf(stderr, "Mmory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	//Copy string
	errno_t err = strcpy_s(dup, len, str);
	if (err != 0)
	{
		fprintf(stderr, "Error copyingthe string\n");
		free(dup);
		exit(EXIT_FAILURE);
	}

	return dup;
}

/*
 * Function: createNode()
 * Parameter:
 * const char* name: pointer to a C-style string containing the name.
 * Return Value:
 * Node*: pointer to the newly created Node.
 * Description: This function creates a new node with the given name.
 */
Node* createNode(const char* name)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	newNode->name = strDuplicate(name);
	newNode->next = NULL;
	return newNode;
}

/*
 * Function: insertSortedLinkedList()
 * Parameter:
 * Node** head: double pointer to the head of the linked list.
 * const char* name: pointer to a C-style string containing the name.
 * Return Value: void
 * Description: This function inserts a new node into the linked list in sorted order.
 */
void insertSortedLinkedList(Node** head, const char* name)
{
	Node* newNode = createNode(name);
	if (*head == NULL || strcmp((*head)->name, name) > 0)
	{
		newNode->next = *head;
		*head = newNode;
	}
	else
	{
		Node* current = *head;
		while (current->next != NULL && strcmp(current->next->name, name) < 0)
		{
			current = current->next;
		}
		newNode->next = current->next;
		current->next = newNode;
	}
}

/*
 * Function: searchLinkedList()
 * Parameter:
 * const char* searchName: pointer to a C-style string containing the name to search for.
 * Node* head: pointer to the head of the linked list.
 * int* comparisonCount: pointer to an integer to track the number of comparisons.
 * Return Value:
 * Node*: pointer to the node with the matching name, or NULL if not found.
 * Description: This function searches for a node with the given name in the linked list.
 */
Node* searchLinkedList(const char* searchName, Node* head, int* comparisonCount)
{
	Node* current = head;
	*comparisonCount = 0;

	while (current != NULL)
	{
		(*comparisonCount)++;
		if (strcmp(current->name, searchName) == 0)
		{
			return current;
		}
		current = current->next;
	}

	return NULL;
}

/*
 * Function: initializeHashTable()
 * Parameter: void
 * Return Value:
 * HashTable*: pointer to the initialized hash table.
 * Description: This function initializes a new hash table with empty buckets.
 */
HashTable* initializeHashTable(void)
{
	HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
	if (hashTable == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		hashTable->buckets[i] = NULL;
	}

	return hashTable;
}

/*
 * Function: insertHashTable()
 * Parameter:
 * HashTable* ht: pointer to the hash table.
 * const char* name: pointer to a C-style string containing the name.
 * Return Value: void
 * Description: This function inserts a new name into the hash table.
 */
void insertHashTable(HashTable* h, const char* name)
{
	int index = hashFunction(name);
	insertSortedLinkedList(&(h->buckets[index]), name);
}

/*
 * Function: searchHashTable()
 * Parameter:
 * HashTable* ht: pointer to the hash table.
 * const char* searchName: pointer to a C-style string containing the name to search for.
 * int* comparisonCount: pointer to an integer to track the number of comparisons.
 * Return Value:
 * Node*: pointer to the node with the matching name, or NULL if not found.
 * Description: This function searches for a name in the hash table.
 */
Node* searchHashTable(HashTable* h, const char* searchName, int* comparisonCount)
{
	int index = hashFunction(searchName);
	return searchLinkedList(searchName, h->buckets[index], comparisonCount);
}

/*
 * Function: freeLinkedList()
 * Parameter:
 * Node* head: pointer to the head of the linked list.
 * Return Value: void
 * Description: This function frees the memory allocated for the linked list.
 */
void freeLinkedList(Node* head)
{
	Node* temp;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->name);
		free(temp);
	}
}

/*
 * Function: freeHashTable()
 * Parameter:
 * HashTable* ht: pointer to the hash table.
 * Return Value: void
 * Description: This function frees the memory allocated for the hash table.
 */
void freeHashTable(HashTable* h)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		freeLinkedList(h->buckets[i]);
	}
	free(h);
}

int main()
{
	FILE* file;
	errno_t err = fopen_s(&file, "names.txt", "r");//open file for reading
	if (err != 0 || file == NULL)
	{
		fprintf(stderr, "Could not open file\n");
		return 1;
	}
	HashTable* hashTable = initializeHashTable();//initializing the hash table
	Node* sortedListHead = NULL;
	char name[MAX_LENGTH + 1];

	//Read names from files and insert into hash table
	while (fgets(name, sizeof(name), file))
	{
		name[strcspn(name, "\n")] = 0;
		toLowerCase(name);
		insertHashTable(hashTable, name);//insertion into hash table
		insertSortedLinkedList(&sortedListHead, name);//insertion to linked list
	}

	fclose(file);

	int ttlComparisonCountLinkedList = 0;
	int ttlComparisonCountHashTable = 0;
	char searchName[MAX_LENGTH + 1];
	int searchCount = 0;

	//Search loop
	while (searchCount < MAX_SEARCH)
	{
		printf("Enter a name to search (or type 'exit' to quit): ");
		fgets(searchName, sizeof(searchName), stdin);
		searchName[strcspn(searchName, "\n")] = 0;

		//Get out of loop if user enters exit
		if (strcmp(searchName, "exit") == 0)
		{
			break;
		}

		toLowerCase(searchName);

		int compCountLinkedList = 0;
		Node* resultLinkedList = searchLinkedList(searchName, sortedListHead, &compCountLinkedList);

		//displaying result for linked list
		if (resultLinkedList)
		{
			printf("\t%s was found in the linked list with %d comparisons\n", searchName, compCountLinkedList);
		}
		else
		{
			printf("\t%s was NOT found in the linked list in %d comparisons\n", searchName, compCountLinkedList);
		}
		ttlComparisonCountLinkedList += compCountLinkedList;

		int compCountHashTable = 0;
		Node* resultHashTable = searchHashTable(hashTable, searchName, &compCountHashTable);

		//displaying results for hash table
		if (resultHashTable)
		{
			printf("\t%s was found in the hash table bucket with %d comparisons\n", searchName, compCountHashTable);
		}
		else
		{
			printf("\t%s was NOT found in the hash table bucket with %d comparisons\n", searchName, compCountHashTable);
		}
		ttlComparisonCountHashTable += compCountHashTable;

		searchCount++;
	}

	//displaying the summary output
	printf("\n\n\n");
	printf("Total number of searches:%d\n", searchCount);
	printf("Total comparisons in sorted linked list: %d\n", ttlComparisonCountLinkedList);
	printf("Total comparisons in hash Table: %d\n", ttlComparisonCountHashTable);

	freeLinkedList(sortedListHead);//free memory for sorted list
	freeHashTable(hashTable);//free memory for hash table

	return 0;
}
