#ifndef PLAYER_H
#define PLAYER_H

#include "../include/Actor.h"


class Player : public Actor
{
    public:
        Player();
        Player( float x, float y, std::string url );
        Player( float x, float y, std::string url, sf::Vector2f dim );
        virtual ~Player();
        void moveUp();
        void moveDown();
        int getHPpoints();
        void reduceHPpoints();

    protected:
        static float HPpoints;
    private:
};

#endif // PLAYER_H
