#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

template<typename T>
class LinkedList
{
public:
	class Item;
	class Item
	{
	  public:
		T value;
		Item *prev = nullptr;
		Item *next = nullptr;
	};

	explicit LinkedList();
	~LinkedList();
	bool isEmpty() const;
	int size() const;
	void push_back(T);
	void push_front(T);
	T remove_back();
	T remove_front();
	bool remove_item(T);
	T first() const;
	T last() const;
	Item *first_item() const;
	Item *last_item() const;
	void ensureCapacity(int capacity);

private:
	int msize;
	Item *mfirst;
	Item *mlast;
};

#endif /* LINKED_LIST_HPP */