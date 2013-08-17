#include "../include/Playground.h"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>


Playground::Playground() : Actor()
{

}


Playground::Playground(  float x, float y, std::string url ) : Actor( x, y, url )
{
    //ctor
}

Playground::~Playground()
{

//    this->points = 0.f;
    //dtor
}

void Playground::playMusic()
{
    if(music.openFromFile("img/sample.ogg"))
    {
        music.play();
    }
    music.setLoop( true );
}

void Playground::pauseMusic()
{
    if( this->music.getStatus() == sf::Music::Playing )
    {
        this->music.pause();
    }
    else if( this->music.getStatus() == sf::Music::Paused )
    {
        this->music.play();
    }
}

void Playground::risePoints()
{
    this->points += 2;
}

int Playground::getPoints()
{
    return this->points;
}

void Playground::resetPoints()
{
    this->points = 0.f;
}


float Playground::points = 0.f;


void Playground::move( float appear )
{
    if( x <= -getDimensions().x + 15.f )
    {
        x = appear;
    }
    else
    {
        sprite.setPosition( x -= v, y );
    }
}

void Playground::setFont()
{
    if( font.loadFromFile( "img/angrybirds.ttf" ) );
    this->text.setFont( font );
    this->text.setCharacterSize( 25 );
    this->text.setColor( sf::Color::Red );
}


void Playground::setScore()
{
    std::fstream f;

    getScore();

    stringVec.push_back( text.getString() );

    sortVec( stringVec );

    for( int i = stringVec.size(); i > 10; i-- )    // kasowanie najgorszego wyniku
    {
        stringVec.pop_back();
    }

    f.open( "score.txt", std::fstream::out | std::fstream::trunc );
    for( unsigned i = 0; i < stringVec.size(); i++ )
    {
        f << stringVec[ i ] << std::endl;
    } // for
    f.close();
}

void Playground::getScore()
{
    std::fstream in( "score.txt", std::fstream::in );

    if( in.good() )
    {
        std::string line;
        while( std::getline( in, line ) )
        {
            if( line != "" )
            {
                stringVec.push_back( line );
            }
        } // while
    } // if

    in.close();
}

std::string Playground::getScore( int* const i )
{
    if( *i >= stringVec.size() )
    {
        std::string s = "----------------";
        return s;
    }
    else
    {
        return stringVec[ *i ];
    }
}

void Playground::sortVec( std::vector<std::string>& stringVec )
{

    for( int i = stringVec.size()-1; i > 0; i-- )
    {
        for( int j = 0; j < i; j++ )
        {
            int a = getNumber( stringVec[ j ] );
            int b = getNumber( stringVec[ j+1 ] );
            if( a < b )
            {
                std::string s = stringVec[ j ];
                stringVec[ j ] = stringVec[ j+1 ];
                stringVec[ j+1 ] = s;
            } // if
        } // for
    } // for
}


std::string Playground::int2str( int i )
{
    std::stringstream ss;
    std::string tmp;
    ss << i;
    ss >> tmp;
    return tmp;
}

int Playground::str2int( std::string str )
{
    std::stringstream ss;
    int tmp;
    ss << str;
    ss >> tmp;
    return tmp;
}

int Playground::getNumber( std::string line )
{
    std::string piece = line;
    std::string stringNumber;
    int number;
    for( int i = 0; i < piece.length(); i++ )
    {
        if( piece[ i ] >= 48 && piece[ i ] <= 57 )
        {
            stringNumber = piece.substr( i, piece.length()-1 );
            number = str2int( stringNumber );
            break;
        } // if
    } // for
    return number;
}
