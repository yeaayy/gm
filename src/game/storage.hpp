#ifndef GAME_STORAGE_HPP
#define GAME_STORAGE_HPP

#include "list.hpp"

enum class ResourceType {
	WOOD,
	STONE,
	WATER,
	COUNT,
	UNKNOWN
};

class Storage {
  public:
	class OnStorageChangeListener
	{
		public: virtual void onStorageChange(Storage *storage) = 0;
	};

	Storage(int carryLimit);
	Storage(Storage *src);
	~Storage();
	bool transferTo(Storage *dst);
	bool transferToVia(Storage *carrier, const Storage *dst);
	bool canTransferToVia(const Storage *carrier, const Storage *dst) const;
	int transferTo(ResourceType, int amount, Storage *dst);
	int getAmount(ResourceType) const;
	bool addAmount(ResourceType, int);
	int getCapacity(ResourceType) const;
	int getSpace(ResourceType) const;
	bool setCapacity(ResourceType, int);
	bool isEmpty() const;
	bool isEmpty(ResourceType) const;
	bool isFull(ResourceType) const;
	ResourceType getFirstNonEmpty() const;
	void dispatchOnStorageChangeListener();
	void addOnStorageChangeListener(OnStorageChangeListener *listener);
	bool removeOnStorageChangeListener(OnStorageChangeListener *listener);

  private:
	int carryLimit;
	int *amount;
	int *capacity;
	List<OnStorageChangeListener*> onStorageChangeListeners;
};

#endif /* GAME_STORAGE_HPP */