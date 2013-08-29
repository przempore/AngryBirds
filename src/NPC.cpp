#include "../include/NPC.h"

NPC::NPC() : Actor()
{
    this->x = 0;
    this->y = 0;
}

NPC::NPC( float x, float y, std::string url ) : Actor( x, y, url )
{

}

NPC::~NPC()
{
    //dtor
}

void NPC::move()
{
    if( x < -getDimensions().x )
    {
        x = 800;
    }
    else
    {
        sprite.setPosition( x -= v, y );

    }
}


void NPC::setSprite( std::string url )
{
    if( texture.loadFromFile( url ) )
    {
        sprite.setTexture( texture );
    } // if
}
