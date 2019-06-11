#include "Database/MySQL.hpp"
#include "../../include/Server.hpp"

using std::cout;
using std::endl;

void MySQL::HTMLEntities(std::string & str)
{
    for (int j = 0; j < to_replaceSize; j++ )
    {
        for (int i = 0; i < str.length(); i++ )
        {
            if(str[i] == to_replace[j])
                str[i] = 'x';
        }
    }
}

MySQL::MySQL(Server * server) : m_server(server)
{

}

MySQL::~MySQL()
{
    mysql_close(connection);
}

bool MySQL::connect()
{
    connection = mysql_init(0);

    connection = mysql_real_connect(connection, "localhost", "root", "", "arikazike", 0, NULL, 0);
    return connection;
}

int MySQL::_Connect(std::string & username, std::string & password)
{
    if(username.length() < 3 || password.length() < 3) return -1;
    HTMLEntities(username);
    HTMLEntities(password);

    ask = "SELECT salt, password, ID FROM users WHERE username='" + username + "'";
    bool failed = mysql_query(connection, ask.c_str());
    if(!failed)
    {
        MYSQL_RES * result = mysql_store_result(connection);
        if(mysql_num_rows(result) <= 0)
            return -1;
        row = mysql_fetch_row(result);
        //row[0] -> hash
        //row[1] -> password
        //row[2] -> ID

        if( sha512(row[0] + password) == row[1])
        {
            int id = atoi(row[2]);
            mysql_free_result(result);
            return id;
        }
        mysql_free_result(result);
    }
    return -1;
}

bool MySQL::_GetCharacterList(const unsigned int & id, sf::Packet & packet)
{
    ask = "SELECT name, CharacterID FROM `characters` WHERE ID='" + std::to_string(id) + "'";
    bool failed = mysql_query(connection, ask.c_str());
    if(!failed)
    {
        MYSQL_RES * result = mysql_store_result(connection);
        int much = mysql_num_rows(result);
        packet<<much;

        std::cout<<"Ilosc postaci: "<< much <<"\n";

        if(much > 0)
        {
            for (int i = 0; i < much; i++ )
            {
                row = mysql_fetch_row(result);
                packet<<row[0];
                sf::Uint32 cid = atoi(row[1]);
                packet<<cid;
            }
            mysql_free_result(result);
            return true;
        }
    }
    return false;
}

void MySQL::_GetCharacter(ClientInfo * client, sf::Uint32 characterID, sf::Packet & packet)
{

    ask =   "SELECT name, x, y, health, mana, location, race FROM `characters`"
            "WHERE ID='" + std::to_string(client->m_clientID) + "' "
            "AND CharacterID='" + std::to_string(characterID) + "'";

    bool failed = mysql_query(connection, ask.c_str());
    if(failed)
        return;


    MYSQL_RES * result = mysql_store_result(connection);
    if(mysql_num_rows(result) <= 0)
    {
        mysql_free_result(result);
        return;
    }
    row = mysql_fetch_row(result);
    /*
    row[0] = name
    row[1] = x
    row[2] = y
    row[3] = health
    row[4] = mana
    row[5] = location
    row[6] = race
    */
    Entity * entity = new Entity;
    entity->addComponent<CHealth>();
    entity->addComponent<CMana>();

    entity->setId(characterID);
    entity->setType(EntityType::Player);
    entity->setName( row[0] );
    entity->setPosition( atoi(row[1]), atoi(row[2]));
    entity->getComponent<CHealth>()->setHealth( atoi(row[3]) );
    entity->getComponent<CMana>()->setMana( atoi(row[4]) );
    entity->setLocation( Location( atoi(row[5]) ) );
    entity->setRace( EntityRace( atoi(row[6]) ) );

    packet << *entity;

    client->m_character = entity;
    mysql_free_result(result);
}
