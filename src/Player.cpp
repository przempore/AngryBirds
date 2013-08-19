#include "../include/Player.h"

Player::Player() : Actor()
{

}

Player::Player( float x, float y, std::string url ) : Actor( x, y, url )
{

}

Player::~Player()
{
    this->HPpoints = 100.f;
    //dtor
}

void Player::moveUp()
{
    if( y > 25 && y <= 297 )
    {
        sprite.setPosition( this->x, this->y -= this->v );
    }
}

void Player::moveDown()
{

    if( this->y > 0 && this->y <= 297 )
    {
        sprite.setPosition( this->x, this->y += this->v );
    }

}

int Player::getHPpoints()
{
    return this->HPpoints;
}

void Player::reduceHPpoints()
{
    this->HPpoints -= 5.f;
}

float Player::HPpoints = 100;

void Player::heal()
{
    this->HPpoints += 25;
    if( HPpoints >= 100 )
    {
        this->HPpoints = 100;
    }
}
