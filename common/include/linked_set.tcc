#ifndef LINKED_SET_TCC
#define LINKED_SET_TCC

#include "linked_set.hpp"

template<typename T>
LinkedSet<T>::LinkedSet()
{
	msize = 0;
	mfirst = new Item();
	mlast = new Item();
	mfirst->next = mlast;
	mfirst->prev = nullptr;
	mlast->next = nullptr;
	mlast->prev = mfirst;
}

template<typename T>
LinkedSet<T>::~LinkedSet()
{
	Item *next = mfirst;
	do {
		Item *tmp = next->next;
		delete next;
		next = tmp;
	} while(next != nullptr);
}

template<typename T>
LinkedSet<T>::Item *LinkedSet<T>::first() const
{
	return mfirst->next;
}

template<typename T>
LinkedSet<T>::Item *LinkedSet<T>::last() const
{
	return mlast;
}

template<typename T>
int LinkedSet<T>::size() const
{
	return msize;
}

template<typename T>
bool LinkedSet<T>::isEmpty() const
{
	return msize == 0;
}

template<typename T>
bool LinkedSet<T>::contain(T item)
{
    Item *curr = mfirst->next;
    while(curr != mlast) {
        if(item == curr->value)
            return true;
        curr = curr->next;
    }
    return false;
}

template<typename T>
bool LinkedSet<T>::put(T item)
{
    if(contain(item)) {
        return false;
    }
	Item *a = mlast->prev;
	Item *b = new Item();
	Item *c = mlast;
	b->value = item;
	a->next = b;
	b->next = c;
	c->prev = b;
	b->prev = a;
	++msize;
    return true;
}

template<typename T>
bool LinkedSet<T>::remove(T item)
{
	Item *curr = mfirst->next;
	while(curr != mlast) {
		if(curr->value == item) {
			Item *prev = curr->prev;
			Item *next = curr->next;
			prev->next = next;
			next->prev = prev;
			delete curr;
			--msize;
			return true;
		}
		curr = curr->next;
	}
	return false;
}

#endif /* LINKED_SET_TCC */