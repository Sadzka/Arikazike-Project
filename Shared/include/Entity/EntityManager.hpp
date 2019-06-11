#pragma once
#include <unordered_map>
#include <functional>
#include "../../Shared/include/Entity/Entity.hpp"

using EntityContainer = std::unordered_map< unsigned int, Entity*>;

class EntityManager
{
protected:

	#ifdef __CLIENT
	sf::Int64               m_clientTime;
	#endif // __CLIENT

	EntityContainer         m_entities;

	unsigned int            m_maxEntities;

	std::vector<sf::Uint32> m_entitiesToRemove;

	void processRemovals();
	void entityCollisionCheck();

public:
	EntityManager();
	~EntityManager();

	Entity * add(const EntityType& type, const std::string& name = "");
	Entity * find(const unsigned int & id);
	Entity * find(const std::string & name);
	void remove(const unsigned int & id);

	void update(float & dT);

	void purge();
	std::unordered_map< unsigned int, Entity*> & getEntities();
	#ifdef __CLIENT
	sf::Int64 getClientTime();
	#endif // __CLIENT
    sf::Uint32 getSize();
};
