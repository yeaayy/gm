#ifndef MAP_TCC
#define MAP_TCC

#include "map.hpp"

#include <malloc.h>

#include "log.h"

template<typename T>
int DefaultComparator<T>::operator() (T a, T b)
{
	return a == b ? 0 : 1;
}

/**************************************************
 *                    Map::pair                   *
 **************************************************/

template<typename K, typename V, typename C>
Map<K, V, C>::pair::pair(K k, V v)
	: key(k), value(v)
{}

/**************************************************
 *                  Map::iterator                 *
 **************************************************/

template<typename K, typename V, typename C>
Map<K, V, C>::iterator::iterator(int i, K *keys, V *values)
	: index(i), _keys(keys), _values(values)
{}

template<typename K, typename V, typename C>
bool Map<K, V, C>::iterator::operator!= (Map<K, V, C>::iterator& other)
{
	return index != other.index;
}

template<typename K, typename V, typename C>
class Map<K, V, C>::pair Map<K, V, C>::iterator::operator* ()
{
	return pair(_keys[index], _values[index]);
}

template<typename K, typename V, typename C>
void Map<K, V, C>::iterator::operator++ ()
{
	index++;
}

/**************************************************
 *                       Map                      *
 **************************************************/

template<typename K, typename V, typename C>
Map<K, V, C>::Map(int initialCapacity, int capacityIncrement)
	:_size(0), _capacity(initialCapacity), _capacityIncrement(capacityIncrement)
{
	_keys = new K[initialCapacity];
	_values = new V[initialCapacity];
}

template<typename K, typename V, typename C>
V Map<K, V, C>::operator[] (K key) const
{
	C cmp;
	for(int i=0; i<_size; i++) {
		if(cmp(key, _keys[i]) == 0) {
			return _values[i];
		}
	}
	throw key;
}

template<typename K, typename V, typename C>
V Map<K, V, C>::value(int index) const
{
	assert_ibetweenie(index, 0, _size);
	return _values[index];
}

template<typename K, typename V, typename C>
K Map<K, V, C>::key(int index) const
{
	assert_ibetweenie(index, 0, _size);
	return _keys[index];
}

template<typename K, typename V, typename C>
V Map<K, V, C>::set(int index, V value)
{
	assert_ibetweenie(index, 0, _size);
	V old = _values[index];
	_values[index] = value;
	return old;
}

template<typename K, typename V, typename C>
int Map<K, V, C>::indexOf(K key) const
{
	C cmp;
	for(int i=0; i<_size; i++) {
		if(cmp(key, _keys[i]) == 0) {
			return i;
		}
	}
	return -1;
}

template<typename K, typename V, typename C>
int Map<K, V, C>::count(K key) const
{
	C cmp;
	int found = 0;
	for(int i=0; i<_size; i++) {
		if(cmp(key, _keys[i]) == 0) {
			found++;
		}
	}
	return found;
}

template<typename K, typename V, typename C>
int Map<K, V, C>::size() const
{
	return _size;
}

template<typename K, typename V, typename C>
void Map<K, V, C>::ensureCapacity(int capacity)
{
	if(capacity <= _capacity) {
		return;
	}
	int newCapacity = (capacity + _capacityIncrement - 1) / _capacityIncrement * _capacityIncrement;
	_keys = (K*) realloc(_keys, newCapacity * sizeof(K));
	_values = (V*) realloc(_values, newCapacity * sizeof(V));
	_capacity = newCapacity;
}

template<typename K, typename V, typename C>
int Map<K, V, C>::add(K key, V value)
{
	ensureCapacity(_size + 1);
	_keys[_size] = key;
	_values[_size] = value;
	return _size++;
}

template<typename K, typename V, typename C>
void Map<K, V, C>::remove(int index)
{
	--_size;
	for(int i=index; i<_size; i++) {
		_keys[i] = _keys[i + 1];
		_values[i] = _values[i + 1];
	}
}

template<typename K, typename V, typename C>
void Map<K, V, C>::clear()
{
	_size = 0;
}

template<typename K, typename V, typename C>
class Map<K, V, C>::iterator Map<K, V, C>::begin() const
{
	return iterator(0, _keys, _values);
}

template<typename K, typename V, typename C>
class Map<K, V, C>::iterator Map<K, V, C>::end() const
{
	return iterator(_size, _keys, _values);
}

#endif /* MAP_TCC */
