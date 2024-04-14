#include "list.hpp"

class TerrainPattern {
  public:
	TerrainPattern();
	TerrainPattern(int width, int height,
				   const char *textureName,
				   float shiftX, float shiftY, float shiftZ,
				   TerrainType *type,
				   int *elevation);
	int width, height;
	const char *textureName;
	float shiftX, shiftY, shiftZ;
	TerrainType *type;
	int *elevation;
	bool isMatching(WorldData *world, int x, int y);
	RenderableSprite *get(WorldData *world, int x, int y);
};

namespace {
List<TerrainPattern> patterns;
}

TerrainPattern::TerrainPattern()
{}

TerrainPattern::TerrainPattern(int width, int height,
							   const char *textureName,
							   float shiftX, float shiftY, float shiftZ,
							   TerrainType *type,
							   int *elevation)
{
	this->width = width;
	this->height = height;
	this->textureName = textureName;
	this->shiftX = shiftX;
	this->shiftY = shiftY;
	this->shiftZ = shiftZ;
	this->type = type;
	this->elevation = elevation;
}

void initPattern()
{
	#define G TERRAIN_TYPE_GROUND
	#define W TERRAIN_TYPE_WATER
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_0_00", 2, 2, 0, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_w_0_1", 1, -1, 0, new TerrainType[9] {W,W,W,W,W,W,W,W,W}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	// water corner
	patterns.push(TerrainPattern(3, 3, "tile_2x2_ggggwwgww_0_1", 2, 2, 0, new TerrainType[9] {G,G,G,G,W,W,G,W,W}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_gggwwgwwg_0_1", 2, 2, 0, new TerrainType[9] {G,W,W,G,W,W,G,G,G}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_gwwgwwggg_0_1", 2, 2, 0, new TerrainType[9] {G,G,G,W,W,G,W,W,G}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_wwgwwgggg_0_1", 2, 2, 0, new TerrainType[9] {W,W,G,W,W,G,G,G,G}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	// water edge
	patterns.push(TerrainPattern(3, 3, "tile_2x2_gggwwwwww_0_1_00", 2, 2, 0, new TerrainType[9] {G,W,W,G,W,W,G,W,W}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_wwwwwwggg_0_1_00", 2, 2, 0, new TerrainType[9] {W,W,G,W,W,G,W,W,G}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_gwwgwwgww_0_1_00", 2, 2, 0, new TerrainType[9] {G,G,G,W,W,W,W,W,W}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_wwgwwgwwg_0_1_00", 2, 2, 0, new TerrainType[9] {W,W,W,W,W,W,G,G,G}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	// water with ground corner
	patterns.push(TerrainPattern(3, 3, "tile_2x2_gwwwwwwww_0_1", 2, 2, 0, new TerrainType[9] {G,W,W,W,W,W,W,W,W}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_wwwwwwwwg_0_1", 2, 2, 0, new TerrainType[9] {W,W,W,W,W,W,W,W,G}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_wwgwwwwww_0_1", 2, 2, 0, new TerrainType[9] {W,W,W,W,W,W,G,W,W}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_wwwwwwgww_0_1", 2, 2, 0, new TerrainType[9] {W,W,G,W,W,W,W,W,W}, new int[9] {0,0,0, 0,0,0, 0,0,0}));
	// Corner hill
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_000220120", 2, 2, 0, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {0,1,2, 0,1,1, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_120220000", 2, 2, -2, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {2,1,0, 1,1,0, 0,0,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_021022000", 2, 2, 0, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {0,0,0, 1,1,0, 2,1,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_000022021", 2, 2, 0, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {0,0,0, 0,1,1, 0,1,2}));
	// Edge hill
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_000222111_00", 2, 2, 0, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {0,1,2, 0,1,2, 0,1,2}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_111222000_00", 2, 2, -2, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {2,1,0, 2,1,0, 2,1,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_021021021_00", 2, 2, 0, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {0,0,0, 1,1,1, 2,2,2}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_120120120_00", 2, 2, -2, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {2,2,2, 1,1,1, 0,0,0}));
	// Corner hill 2
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_111122120", 2, 2, -2, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {2,2,2, 2,1,1, 2,1,0}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_021221111", 2, 2, 0, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {0,1,2, 1,1,2, 2,2,2}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_120122111", 2, 2, -2, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {2,2,2, 1,1,2, 0,1,2}));
	patterns.push(TerrainPattern(3, 3, "tile_2x2_g_0_111221021", 2, 2, -2, new TerrainType[9] {G,G,G,G,G,G,G,G,G}, new int[9] {2,1,0, 2,1,1, 2,2,2}));
	#undef G
	#undef W
}

bool TerrainPattern::isMatching(WorldData *world, int sx, int sy)
{
	int minElevation = __INT32_MAX__;
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			int elevation = world->getElevation(sx + x, sy + y);
			if(elevation < minElevation) {
				minElevation = elevation;
			}
		}
	}
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			int index = y * width + x;
			if(world->getTerrain(sx + x, sy + y) != type[index])
				return false;
			int elevation = world->getElevation(sx + x, sy + y) - minElevation;
			if(elevation != this->elevation[index]) {
				return false;
			}
		}
	}
	return true;
}

RenderableSprite *TerrainPattern::get(WorldData *world, int x, int y)
{
	return new RenderableSprite(atlas[textureName]->copy(), x + shiftX, y + shiftY, (world->getElevation(x, y) + shiftZ) * 0.5f);
}

void getMatchingRule(WorldData *world, int x, int y)
{
	for(int i=0; i<patterns.size(); i++) {
		if(patterns[i].isMatching(world, x, y)) {
			sprites.push(patterns[i].get(world, x, y));
			break;
		}
	}
}
