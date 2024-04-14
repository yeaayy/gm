#include "game/storage.hpp"

#include <string.h>

#include "list.tcc"
#include "log.h"

Storage::Storage(int carryLimit)
{
	this->carryLimit = carryLimit;
	int count = (int) ResourceType::COUNT;
	this->amount = new int[count];
	this->capacity = new int[count];
	memset(amount, 0, count * sizeof(*amount));
	memset(capacity, 0, count * sizeof(*capacity));
}

Storage::Storage(Storage *src)
{
	this->carryLimit = src->carryLimit;
	int count = (int) ResourceType::COUNT;
	this->amount = new int[count];
	this->capacity = new int[count];
	memcpy(amount, src->amount, count * sizeof(*amount));
	memcpy(capacity, src->amount, count * sizeof(*capacity));
}

Storage::~Storage()
{
	delete[] amount;
	delete[] capacity;
}

bool Storage::transferTo(Storage *dst)
{
	for(int i=0; i<(int) ResourceType::COUNT; i++) {
		ResourceType type = (ResourceType) i;
		if(this->getAmount(type) > dst->getSpace(type)) {
			return false;
		}
	}
	for(int i=0; i<(int) ResourceType::COUNT; i++) {
		dst->amount[i] += this->amount[i];
		this->amount[i] = 0;
	}
	dispatchOnStorageChangeListener();
	dst->dispatchOnStorageChangeListener();
	return true;
}

bool Storage::transferToVia(Storage *carrier, const Storage *dst)
{
	ResourceType types[carrier->carryLimit];
	int amount[carrier->carryLimit];
	int carried = 0;
	for(int i=0; i<(int) ResourceType::COUNT && carried < carrier->carryLimit; i++) {
		ResourceType type = (ResourceType) i;
		int source = getAmount(type);
		if(source > 0) {
			int carrierLimit = carrier->getSpace(type);
			int storageLimit = dst->getSpace(type);
			int carriedAmount = source;
			if(carrierLimit < carriedAmount) carriedAmount = carrierLimit;
			if(storageLimit < carriedAmount) carriedAmount = storageLimit;
			if(carriedAmount > 0) {
				types[carried] = type;
				amount[carried] = carriedAmount;
				carried++;
			}
		}
	}
	if(carried == 0) {
		return false;
	}
	for(int i=0; i<carried; i++) {
		carrier->amount[(int) types[i]] += amount[i];
		this->amount[(int) types[i]] -= amount[i];
	}
	dispatchOnStorageChangeListener();
	carrier->dispatchOnStorageChangeListener();
	return true;
}

bool Storage::canTransferToVia(const Storage *carrier, const Storage *dst)
const {
	int carried = 0;
	for(int i=0; i<(int) ResourceType::COUNT && carried < carrier->carryLimit; i++) {
		ResourceType type = (ResourceType) i;
		int source = getAmount(type);
		if(source > 0) {
			int carrierLimit = carrier->getSpace(type);
			int storageLimit = dst->getSpace(type);
			int carriedAmount = source;
			if(carrierLimit < carriedAmount) carriedAmount = carrierLimit;
			if(storageLimit < carriedAmount) carriedAmount = storageLimit;
			if(carriedAmount > 0) {
				return true;
			}
		}
	}
	return false;
}

int Storage::getAmount(ResourceType type)
const {
	return amount[(int) type];
}

bool Storage::addAmount(ResourceType type, int amount)
{
	int result = getAmount(type) + amount;
	if(result < 0 || result > getCapacity(type)) {
		return false;
	}
	this->amount[(int) type] = result;
	dispatchOnStorageChangeListener();
	return true;
}

int Storage::getCapacity(ResourceType type)
const {
	return capacity[(int) type];
}

bool Storage::setCapacity(ResourceType type, int cap)
{
	if(amount[(int) type] > cap){
		return false;
	}
	capacity[(int) type] = cap;
	dispatchOnStorageChangeListener();
	return true;
}

int Storage::getSpace(ResourceType type)
const {
	return getCapacity(type) - getAmount(type);
}

bool Storage::isEmpty()
const {
	for(int i=0; i<(int) ResourceType::COUNT; i++) {
		if(getAmount((ResourceType) i) > 0) {
			return false;
		}
	}
	return true;
}


bool Storage::isEmpty(ResourceType type)
const {
	return getAmount(type) > 0;
}

bool Storage::isFull(ResourceType type)
const {
	return getAmount(type) == getCapacity(type);
}

ResourceType Storage::getFirstNonEmpty()
const {
	for(int i=0; i<(int) ResourceType::COUNT; i++) {
		if(getAmount((ResourceType) i) > 0) {
			return (ResourceType) i;
		}
	}
	return ResourceType::UNKNOWN;
}

void Storage::dispatchOnStorageChangeListener()
{
	for(int i=0; i<onStorageChangeListeners.size(); i++) {
		onStorageChangeListeners[i]->onStorageChange(this);
	}
}

void Storage::addOnStorageChangeListener(OnStorageChangeListener *listener)
{
	onStorageChangeListeners.push(listener);
}

bool Storage::removeOnStorageChangeListener(OnStorageChangeListener *listener)
{
	return onStorageChangeListeners.removeItem(listener);
}
