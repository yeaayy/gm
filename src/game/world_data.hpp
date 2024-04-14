#ifndef GAME_WORLD_DATA_HPP
#define GAME_WORLD_DATA_HPP

#include "list.hpp"
#include "path_finder.hpp"

class GameObject;
class Townie;

typedef enum TerrainType {
	TERRAIN_TYPE_GROUND = 0,
	TERRAIN_TYPE_CLIFF,
	TERRAIN_TYPE_EDGE,
	TERRAIN_TYPE_WATER,
	TERRAIN_TYPE_FLOOD,
	TERRAIN_TYPE_MAX
} TerrainType;

typedef enum TerrainFlag {
	TERRAIN_FLAG_NONE = 0,
	TERRAIN_FLAG_FLAT,
	TERRAIN_FLAG_OBJECT,
	TERRAIN_FLAG_NON_FLAT,
	TERRAIN_FLAG_WATER,
	TERRAIN_FLAG_SHORE,
	TERRAIN_FLAG_HILL,
	TERRAIN_FLAG_CLIFT,
	TERRAIN_FLAG_MAX = 15,
	TERRAIN_FLAG_DIR = 48,
	TERRAIN_FLAG_DIR_UL =  0,
	TERRAIN_FLAG_DIR_UR = 16,
	TERRAIN_FLAG_DIR_DL = 32,
	TERRAIN_FLAG_DIR_DR = 48,
} TerrainFlag;

class WorldData : public pfa::World
{
	friend class GameObject;
public:
	WorldData();
	virtual ~WorldData();
	void init(int width, int height);
	bool load(const char *path);
	bool saveAs(const char *path);
	bool save();
	TerrainType getTerrain(int x, int y);
	void setTerrain(int x, int y, TerrainType type);
	int getElevation(int x, int y) const override;
	void setElevation(int x, int y, int elevation);
	void addElevation(int x, int y, int elevation);
	float getTerrainCost(int x, int y) const override;
	TerrainFlag getFlag(int x, int y) const;
	GameObject *getObjectAt(int x, int y);
	List<GameObject*> &getObjects();
	List<Townie*> &getTownies();
	void put(GameObject *);
	void remove(GameObject *);
	void put(Townie *);
	void remove(Townie *);
	int getTerrainWidth() const;
	int getTerrainHeight() const;
	int getWidth() const override;
	int getHeight() const override;
	pfa::PathFinder *getPathFinder();

	class GameObjectListener
	{
	public:
		virtual void onGameObjectAdded(GameObject*) = 0;
		virtual void onGameObjectRemoved(GameObject*) = 0;
	};
	void addGameObjectListener(GameObjectListener *);
	void removeGameObjectListener(GameObjectListener *);
	class TownieListener
	{
	public:
		virtual void onTownieAdded(Townie*) = 0;
		virtual void onTownieRemoved(Townie*) = 0;
	};
	void addTownieListener(TownieListener *);
	void removeTownieListener(TownieListener *);

private:
	bool isOutOfRange(int x, int y) const;
	bool isOutOfRange2(int x, int y) const;

	GameObject **objectGrid;
	TerrainType *terrain;
	TerrainFlag *flags;
	int *elevation;
	int width, height;
	List<GameObject*> objects;
	List<Townie*> townies;
	List<GameObjectListener*> objectsListeners;
	List<TownieListener*> towniesListeners;
	pfa::PathFinder *pfa;
	char *path;

	void fillFlag();
	void updateFlag(int x, int y);
};

#endif /* GAME_WORLD_DATA_HPP */