#pragma once
#include "../../Shared/include/Entity/DestructibleTypes.hpp"
#include "Entity/EntityManager.hpp"
#include "Entity/EntitySnapshot.hpp"
#include "Entity/Entity.hpp"
#include "Shared.hpp"

class Shared;

class C_EntityManager : public EntityManager
{
	Shared * m_shared;
	unsigned int itemId;
public:

	C_EntityManager(Shared * shared);

	void draw();
	Shared * getShared();
    Entity * add(const EntityType& type, const std::string& name, const sf::Uint32 & id);
    Entity * add(Entity * entity);
    void updateEntity(EntitySnapshot & snapshot, const int & id);


	void add(const DestructibleType &type, sf::Vector2f pos, const sf::Uint32& id);
	void remove(const sf::Uint32& id);

	void addItem(const DestructibleType &type, sf::Vector2f pos);
};
