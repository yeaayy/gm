#ifndef MAP_HPP
#define MAP_HPP

template<typename T>
struct DefaultComparator {
	int operator() (T a, T b);
};

template<typename T>
struct Comparator {
	int operator() (T a, T b);
};

template<typename K, typename V, typename C = struct DefaultComparator<K> >
class Map
{
public:
	class pair
	{
	public:
		K key;
		V value;
		pair(K key, V value);
	};

	class iterator
	{
	public:
		bool operator!= (iterator& other);
		pair operator *();
		void operator++ ();
		iterator(int i, K *keys, V *values);

	private:
		int index;
		K *_keys;
		V *_values;
	};

	explicit Map(int initialCapacity = 5, int capacityIncrement = 5);
	V value(int index) const;
	K key(int index) const;
	V set(int index, V value);
	int indexOf(K key) const;
	int count(K key) const;
	int size() const;
	void ensureCapacity(int capacity);
	int add(K key, V value);
	void remove(int index);
	void clear();
	V operator[] (K key) const;

	iterator begin() const;
	iterator end() const;

private:
	int _size;
	int _capacity;
	int _capacityIncrement;
	K *_keys;
	V *_values;
};

#endif /* MAP_HPP */
