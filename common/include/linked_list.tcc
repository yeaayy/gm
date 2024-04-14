#ifndef LINKED_LIST_TCC
#define LINKED_LIST_TCC

#include "linked_list.hpp"

template<typename T>
LinkedList<T>::LinkedList()
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
LinkedList<T>::~LinkedList()
{
	Item *next = mfirst;
	do {
		Item *tmp = next->next;
		delete next;
		next = tmp;
	} while(next != nullptr);
}

template<typename T>
T LinkedList<T>::first() const
{
	return mfirst->next->value;
}

template<typename T>
T LinkedList<T>::last() const
{
	return mlast->prev->value;
}

template<typename T>
class LinkedList<T>::Item *LinkedList<T>::first_item() const
{
	return mfirst->next;
}

template<typename T>
class LinkedList<T>::Item *LinkedList<T>::last_item() const
{
	return mlast->prev;
}

template<typename T>
int LinkedList<T>::size() const
{
	return msize;
}

template<typename T>
bool LinkedList<T>::isEmpty() const
{
	return msize == 0;
}

template<typename T>
void LinkedList<T>::push_back(T item)
{
	Item *a = mlast->prev;
	Item *b = new Item();
	Item *c = mlast;
	b->value = item;
	a->next = b;
	b->next = c;
	c->prev = b;
	b->prev = a;
	++msize;
}

template<typename T>
void LinkedList<T>::push_front(T item)
{
	Item *a = mfirst;
	Item *b = new Item();
	Item *c = mfirst->next;
	b->value = item;
	a->next = b;
	b->next = c;
	c->prev = b;
	b->prev = a;
	++msize;
}

template<typename T>
T LinkedList<T>::remove_front()
{
	Item *a = mfirst;
	Item *b = mfirst->next;
	Item *c = b->next;
	a->next = c;
	c->prev = a;
	T removed = b->value;
	delete b;
	--msize;
	return removed;
}

template<typename T>
T LinkedList<T>::remove_back()
{
	Item *c = mlast;
	Item *b = c->prev;
	Item *a = b->prev;
	a->next = c;
	c->prev = a;
	T removed = b->value;
	delete b;
	--msize;
	return removed;
}

template<typename T>
bool LinkedList<T>::remove_item(T item)
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

#endif /* LINKED_LIST_TCC */