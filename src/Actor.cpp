#include "../include/Actor.h"
#include <string>
#include <cmath>


Actor::Actor()
{
    this->x = 0.0;
    this->y = 0.0;
}

Actor::Actor( float x, float y, std::string url )
{
    this->x = x;
    this->y = y;
    setSprite( url );
}

Actor::~Actor()
{
    //dtor
}

void Actor::setPosition( float x, float y )
{
    this->x = x;
    this->y = y;
}

float Actor::getYPosition()
{
   return this->y;
}

float Actor::getXPosition()
{
    return this->x;
}

void Actor::setSprite( std::string url )
{
    if( texture.loadFromFile( url ) )
    {
        sprite.setTexture( texture );
        sprite.setPosition( this->x, this->y );
    } // if
}

void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw( sprite, states );
}

sf::Vector2f Actor::getMiddle()
{
    float vx = this->x + getDimensions().x/2;
    float vy = this->y + getDimensions().y/2;

    return sf::Vector2f( vx, vy );
}

void Actor::setDimensions( float width, float height )
{
    this->width = width;
    this->height = height;
}

sf::Vector2f Actor::getDimensions()
{
    return sf::Vector2f( this->width, this->height );
}

void Actor::setV( float v )
{
    this->v = v;
}

float Actor::getV()
{
    return this->v;
}

sf::Vector2f Actor::getOrigin()
{
    return this->sprite.getOrigin();
}
