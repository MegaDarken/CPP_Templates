#pragma once

const int returnErrorValue = -1;

template <typename Type>
struct ListEntry
{
	Type* value;

	ListEntry* previousEntry;
	ListEntry* nextEntry;
};

template <typename Type>
class MultiList
{

private:
	int _length;

	ListEntry<Type>* _firstEntry;
	ListEntry<Type>* _lastEntry;

	//Current?

	//Internal Continuity
	template <typename Type>
	int MultiList<Type>::fixLast()
	{
		//When Last is Null

		//Are There any entries? 
		if (this->_firstEntry != nullptr)
		{
			//Check first goes to last
			ListEntry* current = this->_firstEntry;

			int timeoutCounter = 0;

			//Traverse each entry
			while (current->nextEntry != nullptr && (timeoutCounter < this->_length * 2))
			{
				timeoutCounter++;

				current = *current->nextEntry;
			}

			//When at end 
			if (current->nextEntry == nullptr)
			{
				this->_lastEntry = *current;
			}
			else
			{
				//Loop occoured?
				return -2;
			}
		}
		else
		{
			//O length or missing front
			return -1;
		}

		return 0;//No issues
	};

	template <typename Type>
	int MultiList<Type>::fixFirst()
	{
		//When First is Null

		//Are There any entries? 
		if (this->_lastEntry != nullptr)
		{
			ListEntry* current = this->_lastEntry;

			int timeoutCounter = 0;

			//Traverse each entry
			while (current->previousEntry != nullptr && (timeoutCounter < this->_length * 2))
			{
				timeoutCounter++;

				current = *current->previousEntry;
			}

			//When at beginning
			if (current->previousEntry == nullptr)
			{
				this->_firstEntry = *current;
			}
			else
			{
				//Loop occoured?
				return -2;
			}
		}
		else
		{
			//O length or missing front
			return returnErrorValue;
		}

		return 0;//No issues
	};

	//int checkForwards();
	//int checkBackwards();
	template <typename Type>
	int MultiList<Type>::checkContinuity()
	{
		//Are There any entries 
		bool firstNull = this->_firstEntry == nullptr;
		bool lastNull = this->_lastEntry == nullptr;

		if (firstNull != lastNull) //Logical XOR
		{
			if (firstNull) {
				this->fixFirst();
			}
			else if (lastNull) {
				this->fixLast();
			}

			return returnErrorValue;
		}

		//Fully Connected?

		return 0;//No issues
	};
	
public:
	template <typename Type>
	MultiList<Type>::MultiList()
	{
		//No entries
		this->_firstEntry = nullptr;
		this->_lastEntry = nullptr;

		this->_length = 0;
	};

	//Get Ends
	template <typename Type>
	Type* MultiList<Type>::GetFirst();
	{
		return this->_firstEntry;
	};

	template <typename Type>
	Type* MultiList<Type>::GetLast();
	{
		return this->_lastEntry;
	}

	//CRUD
	template <typename Type>
	void MultiList<Type>::Append(Type* newValue)//Create
	{
		//Create new Entry
		ListEntry* valueEntry = new ListEntry;

		//place value in entry
		valueEntry->value = newValue;

		//First Entry?
		if (this->_firstEntry == nullptr && this->_lastEntry == nullptr)
		{
			//First is on both ends
			this->_firstEntry = *valueEntry;
			this->_lastEntry = *valueEntry;
		}
		else if (this->_lastEntry != nullptr)
		{
			//original last is given next entry
			this->_lastEntry->nextEntry = *valueEntry;
			//new entry is give last as previous
			valueEntry->previousEntry = *this->_lastEntry;

			//new is now last entry 
			this->_lastEntry = *valueEntry;

			this->_length++;
		}
		else
		{
			//Continuity Issue
			this->checkContinuity();

			return returnErrorValue;
		}

		return 0;
	};

	template <typename Type>
	void MultiList<Type>::Insert(Type* newValue, int index)
	{
		//in range
		if (this->_length <= index)
		{
			Append(*newValue);
		}
		else
		{
			//Create new Entry
			ListEntry* valueEntry = new ListEntry;

			//place value in entry
			valueEntry->value = newValue;

			//First Entry?
			if (this->_firstEntry == nullptr && this->_lastEntry == nullptr)
			{
				//First is on both ends
				this->_firstEntry = *valueEntry;
				this->_lastEntry = *valueEntry;
			}
			else
			{
				int currentIndex;
				ListEntry* currentEntry = nullptr;

				//Find place in list
				//Which Half
				if (index > (this->_length * 0.5))
				{
					currentIndex = 0;
					currentEntry = *this->_firstEntry;
				}
				else
				{
					currentIndex = this->_length;
					currentEntry = *this->_lastEntry;
				}

				while (currentIndex != index)
				{
					if (currentIndex < index)
					{
						currentEntry = *currentEntry->nextEntry;
						currentIndex++;
					}
					else if (currentIndex > index)
					{
						currentEntry = *currentEntry->previousEntry;
						currentIndex--;
					}
				}

				valueEntry->nextEntry = *currentEntry;
				valueEntry->previousEntry = *currentEntry->previousEntry;
				currentEntry->previousEntry = *valueEntry;
				//a
			}
		}

	};


	template <typename Type>
	Type* MultiList<Type>::GetEntry(int index)//Read
	{
		int currentIndex;
		ListEntry* currentEntry = nullptr;

		//Find place in list
		//Which Half
		if (index > (this->_length * 0.5))
		{
			currentIndex = 0;
			currentEntry = *this->_firstEntry;
		}
		else
		{
			currentIndex = this->_length;
			currentEntry = *this->_lastEntry;
		}

		while (currentIndex != index)
		{
			if (currentIndex < index)
			{
				currentEntry = *currentEntry->nextEntry;
				currentIndex++;
			}
			else if (currentIndex > index)
			{
				currentEntry = *currentEntry->previousEntry;
				currentIndex--;
			}
		}

		if (currentIndex == index)
		{
			return *currentEntry;
		}
		else
		{
			return nullptr;
		}
	};

	template <typename Type>
	int MultiList<Type>::GetIndex(Type* value)
	{
		//From Start
		int currentIndex = 0;
		ListEntry* currentEntry = *this->_firstEntry;

		while (currentEntry->value != value && currentEntry->nextEntry != nullptr)
		{
			currentEntry = *currentEntry->nextEntry;
			currentIndex++;
		}

		if (currentEntry->value != value)
		{
			return currentIndex;
		}
		else
		{
			//invalid
			return returnErrorValue;
		}
	};

	template <typename Type>
	bool MultiList<Type>::Contains(Type* value)
	{
		return (this->GetIndex(*value) != returnErrorValue);
	};


	//void Replace(Type*);//Update
	template <typename Type>
	void MultiList<Type>::Swap(int firstIndex, int secondIndex)
	{
		if (((firstIndex < 0 || firstIndex > this->_length) || (secondIndex < 0 || secondIndex > this->_length)) || firstIndex == secondIndex)
		{
			return returnErrorValue;
		}

		//From Start
		int currentIndex = 0;
		ListEntry<Type>* currentEntry = *this->_firstEntry;

		ListEntry* firstEntry = nullptr;
		ListEntry* secondEntry = nullptr;

		while ((firstEntry == nullptr || secondEntry == nullptr) && currentEntry->nextEntry != nullptr)
		{
			if (currentIndex == firstIndex)
			{
				firstEntry = currentEntry;
			}
			else if (currentIndex == secondIndex)
			{
				secondEntry = currentEntry;
			}

			currentEntry = *currentEntry->nextEntry;
			currentIndex++;
		}

		if (firstEntry != nullptr && secondEntry != nullptr)
		{
			Type* tempValue = *firstEntry->value;
			firstEntry->value = *secondEntry->value;
			secondEntry->value = *tempValue;
		}

		return 0;
	};

	//void Sort();//Type Dependant? Check Templates specialization
	//Sort Varients?
	
	//void Remove(Type*);//Create
	//void Remove(int);
	//Pop?

	//Current Related
	//To Start
	//To Next
	//Get Current
	//At End?

	//Comparison Operators?
};

