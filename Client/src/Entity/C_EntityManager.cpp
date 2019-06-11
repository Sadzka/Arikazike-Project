#include "Entity/C_EntityManager.hpp"

C_EntityManager::C_EntityManager(Shared * shared) : m_shared(shared) { }


void C_EntityManager::draw()
{
	for(auto &itr : m_entities)
    {
		if (!m_shared->m_wind->getViewSpace().contains(itr.second->getPosition() )){ continue; }
            itr.second->draw( m_shared->m_wind->getRenderWindow() );
	}
}

Shared * C_EntityManager::getShared(){ return m_shared; }

Entity * C_EntityManager::add(const EntityType& type, const std::string& name, const sf::Uint32 & id)
{
	Entity * entity = new Entity(this);

	auto x = m_entities.find(id);
	if(x != m_entities.end())
    {
        #ifdef __DEBUG
        cout << "adding character with ID collision! \n";
        #endif // __DEBUG
        return nullptr;
    }

	entity->m_id = id;
	if (name != "")
    {
        entity->m_name = name;
    }

	m_entities.emplace(entity->getId(), entity);

	/// \todo
	//return find(m_idCounter-1);
}

Entity * C_EntityManager::add(Entity * entity)
{
	auto x = m_entities.find( entity->getId() );
	if(x != m_entities.end())
    {
        #ifdef __DEBUG
        cout << "adding character with ID collision! \n";
        #endif // __DEBUG
        delete entity;
        return nullptr;
    }
	m_entities.emplace(entity->getId(), entity);

	for( auto x : m_entities) cout << x.second->getName() << " " << x.second->getId() << " " << x.second->getPosition().x << " " << x.second->getPosition().y << endl;
	/// \todo
	//return find(m_idCounter-1);
}

void C_EntityManager::updateEntity(sf::Packet & packet, const int & id)
{
    EntitySnapshot snapshot;
    packet >> snapshot;

    Entity * entity = find(id);
    if( entity == nullptr)
    {
        m_shared->m_client->addUnknownEntity(id);
        return;
    }
    entity->setLastUpdate(m_clientTime);
    entity->applySnapshot(snapshot);
}
