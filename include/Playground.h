#ifndef PLAYGROUND_H
#define PLAYGROUND_H
#include "Actor.h"
#include <SFML/Audio.hpp>

class Playground : public Actor
{
    public:
        Playground();
        Playground( float x, float y, std::string url );
        virtual ~Playground();
        void playMusic();
        void pauseMusic();
        void risePoints();
        int getPoints();
        void resetPoints();
        void move( float appear );
        sf::Text text;
        void setFont();
        void setScore();
        void getScore();
        std::string getScore( int* const i );
        std::string int2str( int i );
        void sortVec( std::vector<std::string>& stringVec );
        int str2int( std::string str );
        int getNumber( std::string line );

    protected:
    private:
        static float points;
        sf::Music music;
        sf::Font font;
        std::vector <std::string> stringVec;
};

#endif // PLAYGROUND_H
