#ifndef __MY_ARRAY_INL__
#define __MY_ARRAY_INL__

#include "MyArray.h"

template <class T>
MyArray<T>::MyArray(size_t size)
{
	this->_array = new elem[size]{ elem() };
	this->arraySize = size;

	//ÿһ������Ԫ�صĸ���
	this->arrayGrouopElementsNumber = static_cast<int>(sqrt(size));
	//�������
	this->arrayGroupSize = size / arrayGrouopElementsNumber;
	//������������һ��,ʣ��Ԫ�ص�����
	int lastNumber = size - arrayGroupSize * arrayGrouopElementsNumber;
	//��ʣ��
	if (lastNumber != 0)
	{
		//Ҫ�����һ��
		++arrayGroupSize;
	}
	this->arrayGroup = new array_group[arrayGroupSize];
	for (size_t x = 0; x < arrayGroupSize; ++x)
	{
		if (lastNumber != 0 && x == arrayGroupSize - 1)
		{
			//Ҫ�������һ�鲻��Ԫ�صķ���
			arrayGroup[x].maxSize = lastNumber;
			arrayGroup[x].first = x * arrayGrouopElementsNumber;
			arrayGroup[x].last = (x + 1) * arrayGrouopElementsNumber - 1;
		}
		else {
			arrayGroup[x].maxSize = arrayGrouopElementsNumber;
			arrayGroup[x].first = x * arrayGrouopElementsNumber;
			arrayGroup[x].last = (x + 1) * arrayGrouopElementsNumber - 1;
		}
	}
}

template <class T>
MyArray<T>::~MyArray() {
	delete[] this->arrayGroup;
	delete[] this->_array;
}

template <class T>
const typename MyArray<T>::array_at MyArray<T>::insert(T& elem)
{
	for (size_t x = 0; x < arrayGroupSize; ++x)
	{
		auto nowGroup = arrayGroup[x];
		//�����ǰ��û����
		if (nowGroup.size != nowGroup.maxSize)
		{
			//Ѱ�����ڵĿ���λ��
			for (int y = nowGroup.first; y <= nowGroup.last; ++y)
			{
				//���ֿ���
				auto nowElem = _array[y];
				if (nowElem.isFree)
				{
					nowElem.isFree = false;
					nowElem.elem = elem;

					//��ĳ�Ա����+1
					++elementsNumber;
					++nowGroup.size;

					return array_at(y, x);
				}
			}
		}
	}
	array_at at(-1, -1);
	at.badStatus = true;
	return at;
}

template <class T>
void MyArray<T>::remove(const array_at& _where)
{
	if (!_where.badStatus) 
	{
		this->_array[_where._array_at].isFree = true;
		--elementsNumber;
		this->arrayGroup[_where._group_at].size -= 1;
	}
}

template <class T>
std::vector<T> MyArray<T>::getElements() const
{
	if (elementsNumber != 0)
	{
		std::vector<T> temp;
		temp.reserve(elementsNumber);
		for (size_t x = 0; x < arraySize; ++x)
		{
			if (!_array[x].isFree)
			{
				temp.push_back(_array[x].elem);
			}
		}
		return temp;
	}
	return std::vector<T>();
}

template <class T>
inline size_t MyArray<T>::getElementsCount() const
{
	return this->elementsNumber;
}


#endif
