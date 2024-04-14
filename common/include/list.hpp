#ifndef LIST_HPP
#define LIST_HPP

template<typename T>
class List
{
public:
	explicit List(int initialCapacity = 5, int capacityIncrement = 5);
	~List();
	T get(int index) const;
	T operator[] (int index) const;
	T& operator[] (int index);
	int size() const;
	void ensureCapacity(int capacity);
	int push(T item);
	void insert(int index, T item);
	int indexOf(T item) const;
	bool removeItem(T item);
	T remove(int index);
	void clear();
	T *values() const;
	void sort(int(*comparator)(const T*, const T*));

private:
	int msize;
	int mcapacity;
	int mcapacityIncrement;
	T *mlist;
};

#endif /* LIST_HPP */