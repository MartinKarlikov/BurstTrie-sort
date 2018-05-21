#pragma once
#include <stack>
#include <vector>
#include "helperFunctions.h"

#define ALPHABET_SIZE 53
#define BURST_SIZE 128

class BurstTrie
{

public:
	
	//default constructor.. demands that the root
	//will be a type - TrieNode- determining the first letter
	BurstTrie();
	
	//calls the desctructor of the root TrieNode which calls
	//the destructors of the elements in it resulting in
	//a "recursive" call of all the elemnts of the BurstTrie
	~BurstTrie();

	//the add function
	//continues on the length of the word
	//skips down on the TrieNode and adds the suffix in the desired bucket
	//or at some TrieNode if the word is a prefix of another already contained word
	void add(string);

	//result- if the given word is contained
	//in the BurstTrie
	bool contains(string) const;

	//result- gives us the count of the words contained
	//in the BurstTrie
	size_t wordCount()const;

	//the actual sorting algorithm which returns the sorted set of words
	//in any desired container given to the function
	//works fastest with a list
	template<typename T>
	T sortedConvert() const;

private:

	struct TrieNode;

	struct TNElement
	{

		~TNElement();

		void* next;

		char index;

		bool isBucket;

	};

	struct TrieNode
	{

		TrieNode();

		~TrieNode();

		TNElement* container;

		size_t count;

	};

	struct Bucket
	{
		vector<string> bucket;

		void addString(string);

		void sort();

		TrieNode* burst();
	};


	void fillStack(TrieNode*, stack<TNElement*>&) const;

	size_t count;

	TrieNode* root;

};

template<typename T>
inline T BurstTrie::sortedConvert() const
{
	TNElement* current;

	stack<TNElement*> dfsStack;

	T container;

	string cString;

	fillStack(root, dfsStack);

	while (!dfsStack.empty())
	{
		current = dfsStack.top();

		dfsStack.pop();

		if (current->index == 52)
		{

			if (cString.size() > 0)
				cString.erase(cString.size() - 1, 1);

			continue;

		}

		
		cString.append(1, parseIndex(current->index));

		if (current->isBucket)
		{

			Bucket* tempBucket = reinterpret_cast<Bucket*>(current->next);

			tempBucket->sort();

			for (vector<string>::iterator it = tempBucket->bucket.begin(); it != tempBucket->bucket.end(); ++it)
			{
				container.push_back(cString + *it);
			}

			cString.erase(cString.size() - 1, 1);

		}

		else
		{
			TrieNode* temp = reinterpret_cast<TrieNode*>(current->next);

			for (size_t i = 0; i < temp->count; ++i)
			{

				container.push_back(cString);

			}

			fillStack(temp, dfsStack);

		}

	}

	return container;
}
