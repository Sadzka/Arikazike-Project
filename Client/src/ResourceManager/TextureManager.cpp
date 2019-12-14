#include "ResourcesManager/TextureManager.hpp"

TextureManager::~TextureManager() { textures.clear(); }

const sf::Texture& TextureManager::getResource( const std::string& filename )
{
    for( std::map<std::string, sf::Texture>::const_iterator it =  textures.begin(); it !=  textures.end(); it++)
    {
        if( filename == it->first )
        {
            return it->second;
        }
    }

    // The image doesen't exists. Create it and save it.
    sf::Texture texture;
    if( texture.loadFromFile( filename ) )
    {
        textures[filename] = texture;
        return textures[filename];
    }

    #ifdef __DEBUG
    std::cout << "GAME_ERROR: Texture was not found. It is filled with an empty image.\n";
    #endif // __DEBUG

    textures[filename] = texture;
    return textures[filename];
}

bool TextureManager::loadResource( const std::string & filename )
{
    // The image exist
    std::cout << "DEBUG_MESSAGE: loading texture: " << filename << std::endl;
    for( auto it = textures.begin(); it !=  textures.end(); it++)
    {
        if( filename == it->first )
        {
            #ifdef __DEBUG
            std::cout << "DEBUG_MESSAGE: loading already loaded texture! \n";
            #endif // __DEBUG
            return true;
        }
    }

    // The image doesen't exists. Create it and save it.
    sf::Texture texture;
    if( texture.loadFromFile( filename ) )
    {
        textures[filename] = texture;
        //std::cout << "DEBUG_MESSAGE: loading texture: " + filename+ "\n";
        return true;
    }

    //std::cout << "DEBUG_MESSAGE: TextureManager::loadTexture() Texture Exists : " + filename+ "\n";
    return false;
}

void TextureManager::deleteResource( const std::string& filename )
{
    std::map<std::string, sf::Texture>::iterator it = textures.find( filename );
    if( it != textures.end() )
    {
        textures.erase( it );
        return;
    }
}
