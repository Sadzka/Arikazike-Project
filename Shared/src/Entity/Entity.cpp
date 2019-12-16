#include "Entity/EntityManager.hpp"

#ifdef __CLIENT

#include "Entity/C_EntityManager.hpp"

Entity::Entity(C_EntityManager * entitymgr) :  m_type(EntityType::Base), m_state(EntityState::Idle),
                 m_spritesheet(entitymgr->getShared()->m_textureManager),
                 m_spritesheetWeapon(entitymgr->getShared()->m_textureManager),
                 m_speed(192), dx(0), dy(0), m_lastUpdate(0), attacking(false) { }
                //, m_spritesheet(entityMgr->getShared()->m_textureManager)
#else
Entity::Entity() : m_type(EntityType::Base), m_state(EntityState::Idle), dx(0), dy(0), m_speed(192),
                 m_lasthearthbeat(0), attacking(false)
                 { }
#endif // __CLIENT

Entity::~Entity() { }

void Entity::update(const float & dTime)
{

#ifdef __CLIENT
    m_spritesheet.update(dTime);
    m_spritesheetWeapon.update(dTime);

    if(attacking)
    {
        m_spritesheet.setAnimation("Attack", true, true);
        m_spritesheetWeapon.setAnimation("Attack", true, true);
        return;
    }

    if(dx == 0 && dy == 0)
    {
        m_spritesheet.setAnimation("Idle", true, true);
        m_spritesheetWeapon.setAnimation("Idle", true, true);
        return;
    }

    m_spritesheet.setAnimation("Walk", true, true);
    m_spritesheetWeapon.setAnimation("Walk", true, true);

    if(dx == 1)
    {
        m_spritesheet.setDirection(Direction::Right);
        m_spritesheetWeapon.setDirection(Direction::Right);
    }
    else if(dx == -1)
        {
        m_spritesheet.setDirection(Direction::Left);
        m_spritesheetWeapon.setDirection(Direction::Left);

    }
    else if(dy == 1)
    {
        m_spritesheet.setDirection(Direction::Down);
        m_spritesheetWeapon.setDirection(Direction::Down);
    }
    else if(dy == -1)
    {
        m_spritesheet.setDirection(Direction::Up);
        m_spritesheetWeapon.setDirection(Direction::Up);
    }
#else
#endif // __CLIENT

    move(dx * dTime * getSpeed(), dy * dTime * getSpeed() );
    //Move(deltaPos.x, deltaPos.y);
    //CheckCollisions();
    //ResolveCollisions();
}

void Entity::updateAABB()
{
#ifdef __CLIENT
    m_spritesheet.setSpritePosition(m_position);
    m_spritesheetWeapon.setSpritePosition(m_position);
#endif // __CLIENT
}

void Entity::move(float x, float y)
{

    m_positionOld = m_position;
    if( x != 0 && y != 0) m_position += sf::Vector2f(x/1.4142, y/1.4142);
    else m_position += sf::Vector2f(x, y);

    #ifdef __SERVER
    if(x != 0)
    {
        direction.x = (x>0) ? (1) : (-1);
        direction.y = 0;
    }
    if(y != 0)
    {
        direction.x = 0;
        direction.y = (y>0) ? (1) : (-1);
    }
    #endif // __SERVER
    updateAABB();
}

void Entity::setType(const EntityType & type) { m_type = type; }

void Entity::setPosition(float x, float y)
{
	m_position = sf::Vector2f(x,y);
	updateAABB();
}

void Entity::setPosition(sf::Vector2f pos)
{
	m_position = pos;
	updateAABB();
}

void Entity::setPosition(sf::Vector2i pos)
{
	m_position = sf::Vector2f(pos);
	updateAABB();
}

void Entity::setSize(float x, float y)
{
	m_size = sf::Vector2f(x, y);
	updateAABB();
}

void Entity::setState(const EntityState& state)
{
	if (m_state == EntityState::Dead){ return; }
	m_state = state;
}
void Entity::setLocation(const Location & location) { m_location = location; }
void Entity::setRace(const EntityRace & race) { m_race = race; }
void Entity::setName(std::string name) { m_name = name; }
void Entity::setId(unsigned int id) { m_id = id; }
void Entity::setMoving(const sf::Int8 & x, const sf::Int8 & y) { dx = x; dy = y; };
void Entity::setAttacking(const bool& attacking) { this->attacking = attacking; }
void Entity::setAttackingStart(sf::Int32 hearhbeat) { this->attackingStart = hearhbeat; }

const sf::Vector2f& Entity::getSize()const { return m_size; }
std::string Entity::getName()const { return m_name; }
Location Entity::getLocation()const { return m_location; }
EntityState Entity::getState()const { return m_state; }
unsigned int Entity::getId()const { return m_id; }
EntityType Entity::getType()const { return m_type; }
EntityRace Entity::getRace()const { return m_race; }
float Entity::getSpeed()const { return m_speed; }
bool Entity::isAttacking()const { return attacking; };
sf::Int32 Entity::getAttackingTime()const { return attackingStart; };
sf::Vector2f Entity::getDx()const { return sf::Vector2f(dx, dy); }

#ifdef __CLIENT
SpriteSheet & Entity::getSpriteSheet() { return m_spritesheet; }
SpriteSheet & Entity::getSpriteSheetWeapon() { return m_spritesheetWeapon; }

void Entity::draw(sf::RenderWindow * window)
{
    m_spritesheet.draw(window);
    m_spritesheetWeapon.draw(window);
}

void Entity::applySnapshot(EntitySnapshot& snapshot)
{
    /// \todo szacowanie pozycji ///
    m_position = snapshot.position;
    ///
    dx = snapshot.dx;
    dy = snapshot.dy;
    m_lasthearthbeat = m_lastUpdate;
}

void Entity::setLastUpdate( const sf::Int64 & time) { m_lastUpdate = time; }

sf::Int64 Entity::getLastUpdate()const { return m_lastUpdate; }

#else
EntitySnapshot Entity::getSnapshot()
{
    EntitySnapshot snapshot;
    snapshot.position = m_position;
    snapshot.dx = dx;
    snapshot.dy = dy;
    return snapshot;
}
void Entity::setLastHearhbeat(const sf::Int32 & time) { m_lasthearthbeat = time; }
sf::Int32 Entity::getLastHearhbeat()const { return m_lasthearthbeat; }
#endif // __CLIENT

sf::Packet& operator << (sf::Packet& packet, const Entity& entity)
{
    return packet << entity.m_name
                  << sf::Uint16(entity.m_location)
                  << entity.m_id
                  << sf::Uint8(entity.m_race)
                  << sf::Uint8(entity.m_type)
                  << entity.m_position.x
                  << entity.m_position.y;
}

sf::Packet& operator >> (sf::Packet& packet, Entity& entity)
{
    sf::Uint16 location;
    sf::Uint8 type;
    sf::Uint8 race;
    packet>> entity.m_name
          >> location
          >> entity.m_id
          >> race
          >> type
          >> entity.m_position.x
          >> entity.m_position.y;

    entity.m_type = EntityType(type);
    entity.m_race = EntityRace(race);
    entity.m_location = Location(location);

    return packet;
}


