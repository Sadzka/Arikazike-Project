#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <map>
#include <iostream>

class  TextureManager
{
    std::map< std::string, sf::Texture > textures;

public:
    ~TextureManager();
    const sf::Texture &	getResource( const std::string & filename );

    bool loadResource(const std::string & filename);
    void deleteResource( const std::string& filename );
};

