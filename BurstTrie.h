#pragma once
#include <stack>
#include <vector>
#include "helperFunctions.h"

#define ALPHABET_SIZE 53
#define BURST_SIZE 2

class BurstTrie
{

public:

	BurstTrie();

	~BurstTrie();

	void add(string);

	bool contains(string);

	size_t wordCount()const;

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
