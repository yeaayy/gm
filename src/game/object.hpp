#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

class WorldData;

class GameObject
{
	friend class WorldData;
public:
	GameObject(int type, int x, int y, int width, int height);
	virtual ~GameObject();
	int getType() const;
	int getX() const;
	int getY() const;
	int getWidth() const;
	int getHeight() const;

protected:
	virtual void onPlaced(WorldData *dst);
	virtual void onRemoved(WorldData *dst);

 private:
	int type;
	int x, y;
	int width, height;
};

#endif /* GAME_OBJECT_HPP */
