#include "../../Shared/include/Entity/Destructible.hpp"
#include "../../Shared/include/Entity/Item.hpp"
#include "Entity/C_EntityManager.hpp"

C_EntityManager::C_EntityManager(Shared * shared) : m_shared(shared), itemId(0) { }

void C_EntityManager::draw()
{
    for(auto &itr : m_items)
    {
		if (!m_shared->m_wind->getViewSpace().contains(itr.second->getPosition() )){ continue; }
            itr.second->draw( m_shared->m_wind->getRenderWindow() );
	}

	for(auto &itr : m_entities)
    {
		if (!m_shared->m_wind->getViewSpace().contains(itr.second->getPosition() )){ continue; }
            itr.second->draw( m_shared->m_wind->getRenderWindow() );
	}

    for(auto &itr : m_destructible)
    {
		if (!m_shared->m_wind->getViewSpace().contains(itr.second->getPosition() )){ continue; }
            itr.second->draw( m_shared->m_wind->getRenderWindow() );
	}

    for(auto &itr : m_entities)
    {
		for(auto &itrItem : m_items)
        {
            if( itrItem.second->contains(itr.second->getPosition(), 32 ) )
            {
                m_items.erase(itrItem.first);
            }
        }
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
	return nullptr;
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

	for( auto x : m_entities)
        cout << x.second->getName() << " " << x.second->getId() << " " << x.second->getPosition().x << " " << x.second->getPosition().y << endl;

	/// \todo
	//return find(m_idCounter-1);
	return nullptr;
}

void C_EntityManager::updateEntity(EntitySnapshot & snapshot, const int & id)
{
    Entity * entity = find(id);
    if( entity == nullptr)
    {
        m_shared->m_client->addUnknownEntity(id);
        return;
    }
    entity->setLastUpdate(m_clientTime);
    entity->applySnapshot(snapshot);
}

void C_EntityManager::add(const DestructibleType &type, sf::Vector2f pos, const sf::Uint32& id)
{
    if((int)type == -1)
    {
        for (auto &destr : m_destructible)
        {
            if(destr.second->getId() == id)
            {
                m_destructible.erase( id );
                return;
            }
        }

        return;
    }
    Destructible * destr = new Destructible(type, pos, id, m_shared);
    auto x = m_destructible.find(id);
	if(x != m_destructible.end())
        return;

    m_destructible.emplace(destr->getId(), destr);
}

void C_EntityManager::remove(const sf::Uint32& id)
{
    auto x = m_destructible.find(id);
	if(x == m_destructible.end())
        return;

    m_destructible.erase(x);
}

void C_EntityManager::addItem(const DestructibleType &type, sf::Vector2f pos)
{
    Item * item = new Item((int)type, pos, m_shared);
    m_items.emplace(itemId++, item);
}
