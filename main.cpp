#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "include/Playground.h"
#include "include/Player.h"
#include "include/NPC.h"

void game( sf::RenderWindow&, Playground& textPoints );
bool menu( sf::RenderWindow& );
bool endGameMenu( sf::RenderWindow& oknoAplikacji, Playground& textPoints );
bool collision( Actor& player, NPC& npc );
void score( Playground& textPoints, bool& repeatScore );
bool enterScore( sf::RenderWindow& scoreWindow, Playground& textPoints );

int main()
{
    bool isAgain = true ;
    do
    {
        Playground textPoints;
        textPoints.text.setString( "" );
        sf::RenderWindow oknoAplikacji( sf::VideoMode( 800, 600, 32 ), "Latajacy ptak" );
        bool play = false;
        while( !play )
        {
            play = menu( oknoAplikacji );
        }
        game( oknoAplikacji, textPoints );
        isAgain = endGameMenu( oknoAplikacji, textPoints );
        textPoints.resetPoints();
    }while( isAgain );

    return 0;
}

void game( sf::RenderWindow& oknoAplikacji, Playground& textPoints )
{

    // game background
    Playground sky( 0.f, 0.f, "img/tlo.png" );
    sky.setDimensions( 800, 357 );
    const float skyV = 10.f;
    sky.setV( skyV );
    sky.playMusic();        // play music
    Playground sky2( sky.getDimensions().x, 0.f, "img/tlo.png" );
    sky2.setDimensions( sky.getDimensions().x, sky.getDimensions().y );
    sky2.setV( skyV );

    Playground ground( 0.0f, sky.getDimensions().y, "img/kafelek.png" );
    ground.setDimensions( 465.f, 255.f );
    const float groundV = 15.f;
    ground.setV( groundV );
    Playground ground2( ground.getDimensions().x, sky.getDimensions().y, "img/kafelek.png" );
    ground2.setDimensions( 465.f, 255.f );
    ground2.setV( groundV );
    Playground ground3( ground.getDimensions().x + ground2.getDimensions().x, sky.getDimensions().y, "img/kafelek.png" );
    ground3.setDimensions( 465.f, 255.f );
    ground3.setV( groundV );


    // player
    float playerX = 60.0f;
    float playerY = 297.0f;
    const float playerV = 15.f;      // predkosc gracza
    Player player( playerX, playerY, "img/gracz.png" );
    player.setDimensions( 45.f, 60.f );
    player.setV( playerV );

    // pasek zycia
    float hpLineX = 300.f;
    float hpLineY  = 20.f;
    sf::RectangleShape hpLine( sf::Vector2f( hpLineX, hpLineY ) );
    hpLine.setPosition( 115.f, 570.f );
    hpLine.setFillColor( sf::Color::Red );

    // player hp points
    Playground textHP;
    textHP.text.setString( "HP: " + textHP.int2str( player.getHPpoints() ) );
    textHP.setFont();
    textHP.text.setPosition( 30.f, 565.f );

    // Points
    textPoints.text.setString( textPoints.int2str( textPoints.getPoints() ) );
    textPoints.setFont();
    textPoints.text.setPosition( sf::Vector2f( 650.f, 30.f ) );

    // NPC
    std::string NPCurl = "img/swinia.png";
    const int NPCnum = 5 ;           // liczba NPCow
    float NPCv = 25.f;            // predkosc NPCow
    NPC npcTab[ NPCnum ];               // tablica NPC

    srand( static_cast<unsigned>( time( NULL ) ) );
    for( int i = 0; i < NPCnum; i++ )
    {
        int r = rand() % 297;
        int r2 = 800 + rand() % 701;
        npcTab[ i ].setDimensions( 60, 60 );
        npcTab[ i ].setSprite( NPCurl );
        npcTab[ i ].setPosition( r2, r );
        npcTab[ i ].setV( NPCv );
    } // for


    // NPC King
    NPC king( 2800, 223, "img/swinia3.png" );
    king.setDimensions( 116, 134 );
    king.setV( 50.f );

    // heal
    NPC heal( 800 + rand() % 600, rand() % 304, "img/heart.png" );
    heal.setDimensions( 60.f, 53.f );
    heal.setV( 47.f );

    NPC fast( 800 + rand() % 600, rand() % 304, "img/fast.png" );
    fast.setDimensions( 101.f, 41.f );
    fast.setV( 25.f );

    NPC shield( 1500 + rand() % 600, rand() % 275, "img/GoldenShield.png" );
    shield.setDimensions( 70.f, 82.f );
    shield.setV( 47.f );

    NPC slow( 800 + rand() % 600, rand() % 304, "img/slow.jpg" );
    slow.setDimensions( 59.f, 59.f );
    slow.setV( 47.f );


    bool pauseGame = false;      // PAUSE GAME
    bool isShield = false;
    int shieldPkt = 0;
    // glowna petla programu
    while( oknoAplikacji.isOpen() )
    {
        sf::Clock clock;

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::M ) )          // mute sound
        {
            sky.pauseMusic();
        } // if

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )         // wychodzenie z gry
        {
            while( player.getHPpoints() != 0 )
            {
                player.reduceHPpoints();
            }
        }// if

        while( clock.getElapsedTime().asMilliseconds() < 70 );        // spowolnienie gry timerem.

        // petla menu
        sf::Event event;
        while( oknoAplikacji.pollEvent( event ) )
        {
            if( event.type == sf::Event::Closed )       // zamykanie okna
            {
                oknoAplikacji.close();
            }
        } // while

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
        {
            oknoAplikacji.close();
        }
        // poruszanie sie gracza
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) || sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
        {
            player.moveUp();
        }
        else if( player.getYPosition() < playerY )
        {
            player.moveDown();
        }

        if( collision( player, heal ) )
        {
            player.heal();
            hpLine.setSize( sf::Vector2f( player.getHPpoints() * 3, hpLineY ) );
        }

        if( collision( player, fast ) )
        {
            for( int i = 0; i < NPCnum; i++ )
            {
                npcTab[ i ].setV( npcTab[ i ].getV() + 7.f );
            }
        }

        if( collision( player, slow ) )
        {
            for( int i = 0; i < NPCnum; i++ )
            {
                float tmpV = npcTab[ i ].getV() - 5.f;
                if( tmpV > NPCv - 10.f )
                {
                    npcTab[ i ].setV( tmpV );
                }
            }
        }

        if( collision( player, shield ) )
        {
            isShield = true;
            shieldPkt = textPoints.getPoints();
        }

        if( isShield )
        {
            player.setSprite( "img/graczShield.png" );
        }
        else
        {
            player.setSprite( "img/gracz.png" );
        }

        if( textPoints.getPoints() >= shieldPkt + 75 )
        {
            isShield = false;
        }

        // NPC
        for( int i = 0; i < NPCnum; i++ )   // kolizja
        {
            if( collision( player, npcTab[ i ] ) || collision( player, king ) )
            {

                if( !isShield )
                {
                    player.setSprite( "img/skwaszone.png" );
                    player.reduceHPpoints();
                }
                hpLine.setSize( sf::Vector2f( player.getHPpoints() * 3, hpLineY ) );
            }
        } // for

        if( king.getXPosition() <= -king.getDimensions().x )
        {
            int x = 1600 + rand() % 2800;
            king.setPosition( x, king.getYPosition() );
        } // if

        if( heal.getXPosition() <= -heal.getDimensions().x )
        {
            int x = 35500 + rand() % 20500;
            int y = rand() % (int)( sky.getDimensions().y - heal.getDimensions().y );
            heal.setPosition( x, y );
        } // if

        if( fast.getXPosition() <= -fast.getDimensions().x )
        {
            int x = 10500 + rand() % 5500;
            int y = rand() % (int)( sky.getDimensions().y - fast.getDimensions().y );
            fast.setPosition( x, y );
        }

        if( slow.getXPosition() <= -slow.getDimensions().x )
        {
            int x = 22500 + rand() % 15500;
            int y = rand() % (int)( sky.getDimensions().y - slow.getDimensions().y );
            slow.setPosition( x, y );
        }

        if( shield.getXPosition() <= -shield.getDimensions().x )
        {
            int x = 22500 + rand() % 15500;
            int y = rand() % (int)( sky.getDimensions().y - shield.getDimensions().y );
            shield.setPosition( x, y );
        }

        if( player.getHPpoints() <= 0.0 )
        {
            break;
        }
        slow.move();
        fast.move();
        heal.move();
        king.move();
        shield.move();
        sky.move( sky.getDimensions().x );
        sky2.move( sky.getDimensions().x );
        ground.move( ground.getDimensions().x + ground2.getDimensions().x );
        ground2.move( ground.getDimensions().x + ground2.getDimensions().x );
        ground3.move( ground.getDimensions().x + ( ground2.getDimensions().x ) );

        for( int i = 0; i < NPCnum; i++ )
        {
            npcTab[ i ].move();
            if( npcTab[ i ].getXPosition() <= -npcTab[ i ].getDimensions().x )
            {
                int x = 800 + rand() % 600;
                npcTab[ i ].setPosition( x, 0 );
            } // if
            if( npcTab[ i ].getXPosition() > 800 && npcTab[ i ].getXPosition() < 900 )
            {
                int y = 0;
                if( player.getYPosition() - 50 >= 0 )
                {
                    y = player.getYPosition() - 50 + ( rand() % 115 );
                }
                if( y > sky.getDimensions().y - npcTab[ i ].getDimensions().y )
                {
                    y = sky.getDimensions().y - npcTab[ i ].getDimensions().y;
                }
                npcTab[ i ].setPosition( npcTab[ i ].getXPosition(), y );
            } // if
        } // for

        // przyspieszanie NPC
        float division = textPoints.getPoints() / 5.f;
        for( int i = 0; i < NPCnum; i++ )
        {
            if( division > NPCv )
            {
                npcTab[ i ].setV(  npcTab[ i ].getV() + 0.01f );
            }
        } // for


        oknoAplikacji.draw( sky );
        oknoAplikacji.draw( sky2 );
        oknoAplikacji.draw( ground );
        oknoAplikacji.draw( ground2 );
        oknoAplikacji.draw( ground3 );
        oknoAplikacji.draw( king );
        oknoAplikacji.draw( heal );
        oknoAplikacji.draw( fast );
        oknoAplikacji.draw( slow );
        oknoAplikacji.draw( shield );

        for( int i = 0; i < NPCnum; i++ )
        {
            oknoAplikacji.draw( npcTab[ i ] );
        }

        oknoAplikacji.draw( player );
        clock.restart();
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::P ) )           // PAUSE GAME
        {
            pauseGame = true;
            Playground beginText;
            beginText.setFont();
            beginText.text.setString( "Paused\n(B)egin" );
            beginText.text.setPosition( 360.f, 280.f );
            oknoAplikacji.draw( beginText.text );
            oknoAplikacji.display();
        }// if

        while( pauseGame )
        {
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::B ) )
            {
                pauseGame = false;
            } // if
        } // while

        Playground muteText;
        muteText.setPosition( 0.f, 0.f );
        muteText.setFont();
        muteText.text.setString( "(M)ute  (P)ause   (Q)uit" );
        oknoAplikacji.draw( muteText.text );

        textPoints.risePoints();
        textPoints.text.setString(  "Score: " + textPoints.int2str( textPoints.getPoints() ) );
        oknoAplikacji.draw( textPoints.text );
        oknoAplikacji.draw( hpLine );
        textHP.text.setString( "HP: " + textPoints.int2str( player.getHPpoints() ) );
        oknoAplikacji.draw( textHP.text );
        oknoAplikacji.display();
        oknoAplikacji.clear();
    } // while petla gry
}

bool menu( sf::RenderWindow& oknoAplikacji )
{
    oknoAplikacji.clear();

    Playground menuBack( 0.0f, 0.0f, "img/menu.png");
    Playground startB( 150.0f, 250.0f, "img/start.png" );

    sf::Event event;
    while( oknoAplikacji.pollEvent( event ) )
    {
        if( event.type == sf::Event::Closed )
        {
            oknoAplikacji.close();
            return true;
        }
    }
    bool isClicked = sf::Mouse::isButtonPressed( sf::Mouse::Left );
    bool isMarked = startB.getXPosition() <= sf::Mouse::getPosition( oknoAplikacji ).x &&
                    startB.getXPosition() + 263.0 >= sf::Mouse::getPosition( oknoAplikacji ).x &&
                    startB.getYPosition() <= sf::Mouse::getPosition( oknoAplikacji ).y &&
                    startB.getYPosition() + 205.0 >= sf::Mouse::getPosition( oknoAplikacji ).y;

    if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
    {
        oknoAplikacji.close();
        return true;
    }
    if( isMarked )
    {
        oknoAplikacji.draw( menuBack );
        oknoAplikacji.draw( startB );
    }
    else
    {
        oknoAplikacji.draw( menuBack );
    }

    if( isMarked && isClicked )     // start click
    {
        return true;
    }



    Playground exitB( 550.0, 150.0, "img/exit.png" );
    exitB.setDimensions( 155.f, 100.f );

    bool exitIsMarked = exitB.getXPosition() <= sf::Mouse::getPosition( oknoAplikacji ).x &&
                        exitB.getXPosition() + exitB.getDimensions().x >= sf::Mouse::getPosition( oknoAplikacji ).x &&
                        exitB.getYPosition() <= sf::Mouse::getPosition( oknoAplikacji ).y &&
                        exitB.getYPosition() + exitB.getDimensions().y >= sf::Mouse::getPosition( oknoAplikacji ).y;
    if( exitIsMarked )
    {
        oknoAplikacji.draw( menuBack );
        oknoAplikacji.draw( exitB );
        if( isClicked )             // exit click
        {
            oknoAplikacji.close();
            return true;
        }
    }

    oknoAplikacji.display();

    return false;
}

bool endGameMenu( sf::RenderWindow& oknoAplikacji, Playground& textPoints )
{

    bool repeatScore = true;

    if( oknoAplikacji.isOpen() )
    {
        score( textPoints, repeatScore );
    }

    while( oknoAplikacji.isOpen() )
    {
        oknoAplikacji.clear();
        Playground background( 15.0, 42.0, "img/boom.png" );
        background.setDimensions( 328.0, 258.0 );

        sf::Event event;
        while( oknoAplikacji.pollEvent( event ) )
        {
            if( event.type ==  sf::Event::Closed )
            {
                oknoAplikacji.close();
                return false;
            }
        } // while

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
        {
            oknoAplikacji.close();
        }

        Playground scoreButton( background.getDimensions().x + 150.0 , 42.0, "img/scoreBut.png" );
        scoreButton.setDimensions( 212.0, 134.0 );

        bool isClicked = sf::Mouse::isButtonPressed( sf::Mouse::Left );
        bool isMarked = scoreButton.getXPosition() <= sf::Mouse::getPosition( oknoAplikacji ).x &&
                        scoreButton.getXPosition() + scoreButton.getDimensions().x >= sf::Mouse::getPosition( oknoAplikacji ).x &&
                        scoreButton.getYPosition() <= sf::Mouse::getPosition( oknoAplikacji ).y &&
                        scoreButton.getYPosition() + scoreButton.getDimensions().y >= sf::Mouse::getPosition( oknoAplikacji ).y;

        if( isMarked )
        {
            scoreButton.setSprite( "img/scoreBut2.png" );
            if( isClicked )
            {
                score( textPoints, repeatScore );
            }
        }


        Playground exit( 35.0, background.getDimensions().y + 100.0, "img/exit.png" );
        exit.setDimensions( 155.0, 100.0 );

        isMarked = exit.getXPosition() <= sf::Mouse::getPosition( oknoAplikacji ).x &&
                        exit.getXPosition() + exit.getDimensions().x >= sf::Mouse::getPosition( oknoAplikacji ).x &&
                        exit.getYPosition() <= sf::Mouse::getPosition( oknoAplikacji ).y &&
                        exit.getYPosition() + exit.getDimensions().y >= sf::Mouse::getPosition( oknoAplikacji ).y;

        if( isMarked )
        {
            exit.setSprite( "img/exitBIGGER.png" );
            if( isClicked && isMarked )
            {
                oknoAplikacji.close();
                return false;
            }
        }

        Playground playAgain( 350.0, 300.0, "img/repeat.png" );                 // repeat button


        isMarked = playAgain.getXPosition() <= sf::Mouse::getPosition( oknoAplikacji ).x &&
                    playAgain.getXPosition() + 299.0 >= sf::Mouse::getPosition( oknoAplikacji ).x &&
                    playAgain.getYPosition() <= sf::Mouse::getPosition( oknoAplikacji ).y &&
                    playAgain.getYPosition() + 213.0 >= sf::Mouse::getPosition( oknoAplikacji ).y;

        if( isMarked )
        {
            playAgain.setSprite( "img/repeat2.png" );
            if( isClicked )                             // click repeat button
            {
                oknoAplikacji.close();
                return true;
            }
        }

        oknoAplikacji.draw( background );
        textPoints.setPosition( 380, 25 );
        oknoAplikacji.draw( textPoints.text );
        oknoAplikacji.draw( scoreButton );
        oknoAplikacji.draw( playAgain );              // repeat button
        oknoAplikacji.draw( exit );
        oknoAplikacji.display();
    } // while
}

bool collision( Actor& player, NPC& npc )
{

    float lx = abs( player.getMiddle().x - npc.getMiddle().x );
    float ly = abs( player.getMiddle().y - npc.getMiddle().y );

    if( ( player.getDimensions().x  - 10)/2 + ( npc.getDimensions().x - 10 )/2 > lx  && ( player.getDimensions().y - 10 )/2 + ( npc.getDimensions().y - 10 )/2 > ly )
    {
        return true;
    }

    return false;

}

void score( Playground& textPoints, bool& repeatScore )
{
    sf::RenderWindow scoreWindow( sf::VideoMode( 900, 540 ), "Scores:" );
    Playground background( 0, 0, "img/scoreBackground.jpg" );
    background.setDimensions( 900, 540 );
    textPoints.text.setPosition( 10, 10 );
//    textPoints.text.setColor( sf::Color::Green );

    bool isPlay = false;
    while( scoreWindow.isOpen() )
    {
        scoreWindow.clear();

        sf::Event event;
        while( scoreWindow.pollEvent( event ) )
        {
            if( event.type == sf::Event::Closed )
            {
                scoreWindow.close();
            }
        } // while
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
        {
            scoreWindow.close();
        }

        if( !isPlay && repeatScore )
        {
            isPlay = enterScore( scoreWindow, textPoints );
            repeatScore = false;
            textPoints.setScore();
        } // if
        const int numScores = 10;
        scoreWindow.draw( background );
        Playground scores[ numScores ];
        for( int i = 0; i < numScores; i++ )
        {
            scores[ i ].setFont();
            scores[ i ].text.setPosition( 15, i * 40 + 50 );
            scores[ i ].text.setString( textPoints.int2str( i+1 ) + ". " + textPoints.getScore( &i ) );
            scores[ i ].text.setColor( sf::Color::Black );
            scoreWindow.draw( scores[ i ].text );
        } // for

        scoreWindow.draw( textPoints.text );
        scoreWindow.display();
    }//while isOpen()
}


bool enterScore( sf::RenderWindow& scoreWindow, Playground& textPoints )
{
    scoreWindow.clear();

    Playground background( 0.f, 0.f, "img/scoreBackground.jpg" );

    std::string before = textPoints.text.getString();
    for( int i = 0; i < before.length(); i++ )          // zwraca same liczby w score
    {
        if( before[ i ]  == ' ' )
        {
            before = before.substr( i, before.length()-1 );
            break;
        }
    } // for
    textPoints.text.setColor( sf::Color::Black );
    textPoints.text.setString( "Enter Your name: " );
    std::string name = "";
    while( scoreWindow.isOpen() )
    {
        sf::Event event;
        while( scoreWindow.pollEvent( event ) )
        {
            switch( event.type )
            {
            case sf::Event::Closed:
                textPoints.text.setString( "Unknown: " + before );
                scoreWindow.close();
                break;
            case sf::Event::TextEntered:

                if( event.text.unicode >= 33 && event.text.unicode <= 126 )
                {
                    name += (char)event.text.unicode;
                    textPoints.text.setString( name );
                    scoreWindow.draw( textPoints );
                } // if
                else if( event.text.unicode == 8 )
                {
                    name = name.substr( 0, name.length() - 1 );
                    textPoints.text.setString( name );
                    scoreWindow.draw( textPoints );

                } // else if
                else if( event.text.unicode == 20 )
                {
                    name += " ";
                    textPoints.text.setString( name );
                    scoreWindow.draw( textPoints );
                } //else if
                break;
            } // switch
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Return ) )
            {
                if( name != "" )
                {
                    std::string s = textPoints.text.getString() + " " + before;
                    textPoints.text.setString( s );
                }
                else
                {
                    textPoints.text.setString( "" );
                }
                return true;
            } // if
        } // while
        scoreWindow.draw( background );
        scoreWindow.draw( textPoints.text );
        scoreWindow.display();
    } // while

    return false;
}

