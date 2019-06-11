#pragma once
#include <unordered_map>
#include <SFML/Config.hpp>
#include <fstream>
#include <sstream>
#include "Map/Tile.hpp"
#include "States/State.hpp"
#include "../../../shared/include/Entity/Entity.hpp"

class Shared;

enum Tiles
{
    size = 64,
    width = 448,
    height = 192
};

using Tileset = std::vector<Tile*>;
using UsedTextures = std::vector < std::string >;
class Map
{
	Tileset                 m_tileMap;
	sf::Vector2u            m_mapsize;
	State                 * m_currentState;
	Shared                * m_shared;
	sf::Uint32              m_tileId;
	//UsedTextures            m_usedTextures; /// idk \todo
	sf::Int16            ** m_map;
	//Entity                * m_entity;

	//unsigned int ConvertCoords(unsigned int l_x, unsigned int l_y); // Method for converting 2D coordinates to 1D ints.

	void loadTiles(const std::string & path);
	void purgeMap();
	void purgeTileSet();

public:
	Map(Shared* shared);
	~Map();

	const sf::Vector2u& getmapsize()const;

	void loadMap(const std::string& l_path);

	void update(float l_dT);
	void draw();
    void setCurrentState(State * state);
};
