#include "Trie.h"

Trie::Trie()
{
	root = new TrieNode;

	count = 0;
}

Trie::~Trie()
{

	delete root;

}

void Trie::add(string toAdd)
{
	++count;

	char letter = parseChar(toAdd[0]);

	TNElement* current = &root->container[letter];

	while (toAdd.size() > 0)
	{


		toAdd.erase(0, 1);

		if (toAdd.size() > 0)
		{ 
		
			letter = parseChar(toAdd[0]);

		}
			

		if (current->isBucket)
		{

			Bucket* tempBucket = reinterpret_cast<Bucket*>(current->next);

			if (tempBucket->bucket.size() + 1  > BURST_SIZE)
			{

				TrieNode* tempNode = tempBucket->burst();

				delete tempBucket;

				current->next = tempNode;

				current->isBucket = false;

				current = &(tempNode->container[letter]);

				if (toAdd.size()==0)
				{
					tempNode->count++;
					
					return;
				}

				continue;

			}
			else
			{
				tempBucket->addString(toAdd);
			}

			return;
		}

		else
		{

			if (current->next == nullptr)
			{

				current->next = new Bucket;

				current->isBucket = true;

				reinterpret_cast<Bucket*>(current->next)->addString(toAdd);

				return;

			}

			TrieNode* tempNode = reinterpret_cast<TrieNode*>(current->next);

			if (toAdd.size() == 0)
			{

				tempNode->count++;

				return;

			}

			if (tempNode->container[letter].next == nullptr)
			{
				toAdd.erase(0, 1);

				Bucket* newBucket = new Bucket;

				newBucket->addString(toAdd);

				tempNode->container[letter].next = newBucket;

				tempNode->container[letter].isBucket = true;

				return;

			}

			else
			{

				current = &(tempNode->container[letter]);

			}
		}

	}

}
size_t Trie::wordCount() const
{
	return count;
}


void Trie::fillStack(TrieNode * current, stack<TNElement>& stack) const
{
	TNElement temp;

	for (int i = 52; i >= 0; --i)
	{

		temp = current->container[i];

		if (temp.next != nullptr || temp.index == 52)
		{

			stack.push(current->container[i]);
		}

	}
}

Trie::TrieNode::TrieNode()
{
	container = new TNElement[ALPHABET_SIZE];

	count = 0;

	for (size_t i = 0; i <= 52; ++i)
	{

		container[i].index = i;

		container[i].next = nullptr;

		container[i].isBucket = false;

	}
}

Trie::TrieNode::~TrieNode()
{

	delete[] container;

}

void Trie::Bucket::addString(string str)
{
	bucket.push_back(str);
}

void Trie::Bucket::sort()
{
	std::sort(bucket.begin(), bucket.end());

}

Trie::TrieNode* Trie::Bucket::burst()
{
	TrieNode* temp = new TrieNode;
	vector<string>::iterator it;


	for (it = bucket.begin(); it != bucket.end(); ++it)
	{

		if (*it == "")
		{
			temp->count++;
			continue;
		}

		size_t index = parseChar((*it)[0]);

		string tempStr = *it;

		if (temp->container[index].next == nullptr)
		{

			temp->container[index].next = new Bucket;

			temp->container[index].isBucket = true;

		}

		tempStr.erase(0, 1);

		reinterpret_cast<Bucket*>(temp->container[index].next)->addString(tempStr);


	}

	return temp;
}

Trie::TNElement::~TNElement()
{
	if (isBucket)
	{
		delete reinterpret_cast<Bucket*>(next);
	}

	else
	{
		delete reinterpret_cast<TrieNode*>(next);
	}
}
