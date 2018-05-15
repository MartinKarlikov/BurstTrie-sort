#include "BurstTrie.h"

BurstTrie::BurstTrie()
{
	root = new TrieNode;

	count = 0;
}

BurstTrie::~BurstTrie()
{

	delete root;

}

void BurstTrie::add(string toAdd)
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
bool BurstTrie::contains(string contained)
{
	char letter = parseChar(contained[0]);
	TNElement* current = &(root->container[letter]);

	while (contained.size()>0)
	{

		if (!current)
		{
			return false;
		}

		contained.erase(0, 1);
		if (contained.size()>0)
		{
			letter = parseChar(contained[0]);
		}

		if (current->isBucket)
		{

			Bucket* tempBucket = reinterpret_cast<Bucket*>(current->next);

			for (vector<string>::iterator it = tempBucket->bucket.begin(); it != tempBucket->bucket.end(); ++it)
			{

				if (contained == *it)
				{
					return true;
				}

			}

			return false;
	
		}

		else
		{
			TrieNode* tempNode = reinterpret_cast<TrieNode*>(current->next);

			if (contained.size()==0)
			{
				return tempNode->count;
			}

			current = &(tempNode->container[letter]);

		}



	}
}
size_t BurstTrie::wordCount() const
{
	return count;
}


void BurstTrie::fillStack(TrieNode * current, stack<TNElement*>& stack) const
{
	TNElement* temp;

	for (int i = 52; i >= 0; --i)
	{

		temp = &(current->container[i]);

		if (temp->next != nullptr || temp->index == 52)
		{

			stack.push(temp);
		}

	}
}

BurstTrie::TrieNode::TrieNode()
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

BurstTrie::TrieNode::~TrieNode()
{

	delete[] container;

}

void BurstTrie::Bucket::addString(string str)
{
	bucket.push_back(str);
}

void BurstTrie::Bucket::sort()
{
	std::sort(bucket.begin(), bucket.end());

}

BurstTrie::TrieNode* BurstTrie::Bucket::burst()
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

BurstTrie::TNElement::~TNElement()
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
