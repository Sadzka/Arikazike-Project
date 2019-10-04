#pragma once
#include "Entity/EntityManager.hpp"
#include "Entity/EntitySnapshot.hpp"
#include "Entity/Entity.hpp"
#include "Shared.hpp"

class Shared;

class C_EntityManager : public EntityManager
{
	Shared * m_shared;
public:

	C_EntityManager(Shared * shared);

	void draw();
	Shared * getShared();
    Entity * add(const EntityType& type, const std::string& name, const sf::Uint32 & id);
    Entity * add(Entity * entity);
    void updateEntity(EntitySnapshot & snapshot, const int & id);
};
