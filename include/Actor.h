#ifndef ACTOR_H
#define ACTOR_H
#include <SFML/Graphics.hpp>

class Actor : public sf::Drawable
{
    public:
        Actor();
        Actor( float x, float y, std::string url );
        virtual ~Actor();
        void setPosition( float x, float y );
        float getXPosition();
        float getYPosition();
        void setSprite( std::string url );
        sf::Vector2f getMiddle();
        void setDimensions( float width, float height );
        sf::Vector2f getDimensions();
        float getV();
        void setV( float v );
        sf::Vector2f getOrigin();

    protected:
        sf::Texture texture;
        sf::Sprite sprite;
        float x, y;
        float width;
        float height;
        float v;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
};

#endif // ACTOR_H
