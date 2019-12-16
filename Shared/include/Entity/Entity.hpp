#pragma once
#include <typeinfo> //Templates
#include <string>
#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>
#include "Entity/Components/Component.hpp"
#include "Entity/Components/Health.hpp"
#include "EntityState.hpp"
#include "EntityType.hpp"
#include "EntitySnapshot.hpp"
#include "EntityRace.hpp"
#include "Object.hpp"
#include "../Locations.hpp"
#include "Directions.hpp"


#ifdef __CLIENT
#include "Spritesheet/Spritesheet.hpp"
#include <SFML/Graphics.hpp>
#endif // __CLIENT

class C_EntityManager;

using Components = std::unordered_map< unsigned int, Component*>;

class Entity : public Object
{
    friend class EntityManager;
    friend class C_EntityManager;
    /// temp
    friend class Client;
    friend class Server;
    ///

    Components      m_components;
    Location        m_location;
    EntityType      m_type;
    EntityState     m_state; // Current entity state.
    EntityRace      m_race;
    sf::Int8        dx;
    sf::Int8        dy;
    bool            attacking;
    sf::Int32       attackingStart;

    //Stats
    sf::Uint16      stamina;
    sf::Uint16      strength;
    sf::Uint16      intelect;
    sf::Uint16      agility;
    sf::Uint16      spirit;

#ifdef __CLIENT
    SpriteSheet m_spritesheet;
    SpriteSheet m_spritesheetWeapon;
    sf::Uint64 m_lastUpdate;
#else
    sf::Vector2f direction;

    sf::Vector2f getDirection() { return direction; }
#endif // __CLIENT
    sf::Int32 m_lasthearthbeat;


    sf::Vector2f m_size; // Size of the collision box.
    sf::Vector2f m_hitbox; // The bounding box for collisions.

    // stats
    float m_speed; // Value of speed

    void updateAABB();

public:

#ifdef __CLIENT
    Entity(C_EntityManager * entitymgr);
#else
    Entity();
#endif // __CLIENT
    ~Entity();



    template <class T>
    T* addComponent()
    {
        //std::cout<<"adding: "<<typeid(T).hash_code() <<" {aka} "<< typeid(T).name() <<std::endl;

        auto comp = m_components.find( typeid(T).hash_code() );
        if(comp == m_components.end())
        {
            Component * comp = new T();
            m_components.insert( {typeid(T).hash_code(), comp} );

            return static_cast<T*>(comp);
        }
// TODO (Sadza#1#): add exception
        throw 0;
    }

    template <class T>
    T* getComponent()
    {
        auto comp = m_components.find( typeid(T).hash_code() );
        if(comp != m_components.end()) {
            return static_cast<T*>(comp->second);
        }
        std::cout << "Trying to get not existed component, returning 0! <CRASH>\n";
        //throw std::string("Not Found Component" ) ;
        return nullptr;
    }


    void setType(const EntityType & type);
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f pos);
    void setPosition(sf::Vector2i pos);
    void setSize(float l_x, float l_y);
    void setState(const EntityState & state);
    void setLocation(const Location & location);
    void setRace(const EntityRace & race);
    void setMoving(const sf::Int8 & x, const sf::Int8 & y);

    void setName(std::string name);
    void setId(unsigned int id);

    const sf::Vector2f & getSize()const;
    std::string getName()const;
    Location getLocation()const;
    EntityState getState()const;
    unsigned int getId()const;
    EntityType getType()const;
    //const sf::Vector2f getPosition()const;
    EntityRace getRace()const;
    float getSpeed()const;
    bool isAttacking()const;
    sf::Int32 getAttackingTime()const;
    sf::Vector2f getDx()const;

    void move(float x, float y);
    void update(const float & dTime);
    void setAttacking(const bool& attacking);
    void setAttackingStart(sf::Int32 hearhbeat);

#ifdef __CLIENT
    void draw(sf::RenderWindow * window);
    SpriteSheet & getSpriteSheet();
    SpriteSheet & getSpriteSheetWeapon();
    void applySnapshot(EntitySnapshot& snapshot);
    void setLastUpdate( const sf::Int64 & time);
    sf::Int64 getLastUpdate()const;
#else
    EntitySnapshot getSnapshot();
    void setLastHearhbeat(const sf::Int32 & time);
    sf::Int32 getLastHearhbeat()const;
    int m_weapon;
    void setWeapon(const int &weapon) { m_weapon = weapon; }
    int getWeapon()const { return m_weapon; }
#endif // __CLIENT

    friend sf::Packet& operator<< (sf::Packet& packet, const Entity& entity);
    friend sf::Packet& operator>> (sf::Packet& packet, Entity& entity);
};
