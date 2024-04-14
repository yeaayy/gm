#ifndef SET_HPP
#define SET_HPP

template<typename T>
class LinkedSet
{
public:
	class Item
	{
	public:
		T value;
		Item *prev = nullptr;
		Item *next = nullptr;
	};

	explicit LinkedSet();
	~LinkedSet();
	bool isEmpty() const;
	int size() const;
	bool put(T);
	bool contain(T);
	bool remove(T);
	Item *first() const;
	Item *last() const;

private:
	int msize;
	Item *mfirst;
	Item *mlast;
};

#endif /* SET_HPP */