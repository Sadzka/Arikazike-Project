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
	sf::Uint32              m_collisionsNumber;
	sf::Uint32            * m_noCollision;
	//UsedTextures            m_usedTextures; /// idk \todo
	sf::Int16            ** m_map;
	Entity                * m_player;

	//unsigned int ConvertCoords(unsigned int l_x, unsigned int l_y); // Method for converting 2D coordinates to 1D ints.

	void loadTiles(const std::string & path);
	void purgeMap();
	void purgeTileSet();

public:
	Map(Shared* shared);
	~Map();

	const sf::Vector2u& getmapsize()const;
	void setPlayer(Entity * player);

	void loadMap(const std::string& path);

	void update(float dT);
	void draw();
    void setCurrentState(State * state);
    bool isCollision( const int &dx, const int &dy );
};
