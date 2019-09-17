#include "../../Shared/include/Entity/EntityManager.hpp"

EntityManager::EntityManager()
{
    #ifdef __CLIENT
    m_clientTime = 0;
    #endif // __CLIENT
}

EntityManager::~EntityManager(){ purge(); }

Entity * EntityManager::add(const EntityType& type, const std::string& name)
{
    /// \todo
    /// bugged, unknown character id
#ifdef __CLIENT
	Entity * entity = new Entity( (C_EntityManager*)this);
#else
	Entity * entity = new Entity();
#endif // __CLIENT

	if (name != "")
    {
        entity->m_name = name;
    }

	m_entities.emplace(entity->getId(), entity);

	/// \todo
// TODO (Sadza#1#): add exception
        throw 0;
	//return find(m_idCounter-1);
}

Entity * EntityManager::find(const std::string & name)
{
	for(auto &itr : m_entities)
    {
		if(itr.second->getName() == name)
			return itr.second;
	}
	return nullptr;
}

Entity * EntityManager::find(const unsigned int & id)
{
    for(auto &itr : m_entities)
    {
		if(itr.second->getId() == id)
			return itr.second;
	}
    return nullptr;
}

void EntityManager::remove(const unsigned int & id)
{
	m_entitiesToRemove.emplace_back(id);
}

void EntityManager::update(float & dT)
{
	for(auto &itr : m_entities)
    {
		itr.second->update(dT);
    }

	#ifdef __CLIENT
	m_clientTime += dT*1000;
	if(m_clientTime < 0)
        m_clientTime -= LONG_MAX;

    for (auto itr = m_entities.begin(); itr != m_entities.end(); itr++ )
    {
		if( abs(m_clientTime - itr->second->getLastUpdate()) > 3000 )
        {
            itr->second->getSpriteSheet().setAlpha( 255 - (abs(m_clientTime - itr->second->getLastUpdate()) - 3000)/((double)2000)*255 );

            if( abs(m_clientTime - itr->second->getLastUpdate() ) > 5000 )
               m_entities.erase(itr);
        }
        else itr->second->getSpriteSheet().setAlpha( 255 );
    }
	#endif // __CLIENT

	entityCollisionCheck();
	processRemovals();
}

void EntityManager::purge()
{
	for (auto &itr : m_entities)
		delete itr.second;

	m_entities.clear();
}

std::unordered_map< unsigned int, Entity*> & EntityManager::getEntities() { return m_entities; }

void EntityManager::processRemovals()
{
	while(m_entitiesToRemove.begin() != m_entitiesToRemove.end())
    {
		unsigned int id = m_entitiesToRemove.back();
		auto itr = m_entities.find(id);
		if(itr != m_entities.end())
		{
		    #ifdef __DEBUG
			std::cout << "Discarding entity: " << itr->second->getId() << std::endl;
		    #endif // __DEBUG
			delete itr->second;
			m_entities.erase(itr);
		}
		m_entitiesToRemove.pop_back();
	}
}

void EntityManager::entityCollisionCheck()
{
    /// \todo
}

#ifdef __CLIENT
sf::Int64 EntityManager::getClientTime() { return m_clientTime; }
#endif // __CLIENT

sf::Uint32 EntityManager::getSize() { return m_entities.size(); }


