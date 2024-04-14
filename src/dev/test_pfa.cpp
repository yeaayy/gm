
#include <stdio.h>

#include "log.h"
#include "pfa_simple.hpp"
#include "dev/world.hpp"
#include "game/world_data.hpp"

void dump(const pfa::Point *path)
{
	const int directionMapping[9] = {
		-1, 5, 1, 7, 3, 4, 2, 6, 0,
	};
	int minX = 0x7fffffff, maxX = -0x80000000;
	int minY = 0x7fffffff, maxY = -0x80000000;
	auto curr = path;
	do {
		int x = curr->x;
		int y = curr->y;
		if(x > maxX) maxX = x;
		if(x < minX) minX = x;
		if(y > maxY) maxY = y;
		if(y < minY) minY = y;
	} while((curr = curr->prev()) != nullptr);
	int array[maxY - minY + 1][maxX - minX + 1];
	for(int y=minY; y<=maxY; y++) {
		for(int x=minX; x<=maxX; x++) {
			array[y - minY][x - minX] = 0;
		}
	}
	curr = path;
	do {
		array[curr->y - minY][curr->x - minX] = (int) curr->direction;
	} while((curr = curr->prev()) != nullptr);
	for(int y=maxY; y>=minY; y--) {
		for(int x=minX; x<=maxX; x++) {
			printf("%d ", array[y - minY][x - minX]);
		}
		printf("\n");
	}
}

int main() {
	WorldData *world = new WorldData();
	assert_true(world->load("../assets/world/000.data"));

	pfa::SimplePathFinder *spf = new pfa::SimplePathFinder(new dev::World(world));
    auto path = spf->findPath(14, 19, 9, 24);
    dump(path);
    return 0;
}