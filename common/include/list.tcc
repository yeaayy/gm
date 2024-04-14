#ifndef LIST_TCC
#define LIST_TCC

#include "list.hpp"

#include "log.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

template<typename T>
List<T>::List(int initialCapacity, int capacityIncrement)
  : msize(0), mcapacity(initialCapacity), mcapacityIncrement(capacityIncrement)
{
	mlist = new T[initialCapacity];
}

template<typename T>
List<T>::~List()
{
	delete[] mlist;
}

template<typename T>
T List<T>::get(int index) const
{
	assert_ibetweenie(index, 0, msize);
	return mlist[index];
}

template<typename T>
T List<T>::operator[] (int index) const
{
	assert_ibetweenie(index, 0, msize);
	return mlist[index];
}

template<typename T>
T& List<T>::operator[] (int index)
{
	assert_ibetweenie(index, 0, msize);
	return mlist[index];
}

template<typename T>
int List<T>::size() const
{
	return msize;
}

template<typename T>
void List<T>::ensureCapacity(int capacity)
{
	if(capacity <= mcapacity) {
		return;
	}
	int newCapacity = (capacity + mcapacityIncrement - 1) / mcapacityIncrement * mcapacityIncrement;
	mlist = (T*) realloc(mlist, newCapacity * sizeof(T));
	mcapacity = newCapacity;
}

template<typename T>
int List<T>::push(T item)
{
	ensureCapacity(msize + 1);
	mlist[msize] = item;
	return msize++;
}

template<typename T>
void List<T>::insert(int index, T item)
{
	ensureCapacity(msize + 1);
	memmove(mlist + index + 1, mlist + index, (msize - index) * sizeof(T));
	msize++;
	mlist[index] = item;
}

template<typename T>
int List<T>::indexOf(T item) const
{
	for(int i=0; i<msize; i++) {
		if(item == mlist[i]) {
			return i;
		}
	}
	return -1;
}

template<typename T>
T List<T>::remove(int index)
{
	T removed = mlist[index];
	--msize;
	for(int i=index; i<msize; i++) {
		mlist[i] = mlist[i + 1];
	}
	return removed;
}

template<typename T>
bool List<T>::removeItem(T item)
{
	for(int i=0; i<msize; i++) {
		if(item == mlist[i]) {
			remove(i);
			return true;
		}
	}
	return false;
}


template<typename T>
void List<T>::clear()
{
	msize = 0;
}

template<typename T>
T *List<T>::values() const
{
	return mlist;
}

template<typename T>
void List<T>::sort(int(*comparator)(const T*, const T*))
{
	qsort(mlist, msize, sizeof(T), (int(*)(const void*, const void*)) comparator);
}

#endif /* LIST_TCC */