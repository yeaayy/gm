#ifndef GAME_CHUNK_HPP
#define GAME_CHUNK_HPP



class WorldChunk {
	friend class GameView;
  public:
	WorldChunk(int x, int y);

  private:
	int x, y;
};

#endif /* GAME_CHUNK_HPP */