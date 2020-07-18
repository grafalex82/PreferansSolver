#include <iostream>
#include <time.h>

#include "CardPack.h"
#include "CardDefs.h"
#include "Game.h"
#include "Player.h"

const Card prefCards[] = {
MAKE_CARD(CS_SPIDES, CV_7),
MAKE_CARD(CS_SPIDES, CV_8),
MAKE_CARD(CS_SPIDES, CV_9),
MAKE_CARD(CS_SPIDES, CV_10),
MAKE_CARD(CS_SPIDES, CV_JACK),
MAKE_CARD(CS_SPIDES, CV_QUEEN),
MAKE_CARD(CS_SPIDES, CV_KING),
MAKE_CARD(CS_SPIDES, CV_ACE),

MAKE_CARD(CS_CLUBS, CV_7),
MAKE_CARD(CS_CLUBS, CV_8),
MAKE_CARD(CS_CLUBS, CV_9),
MAKE_CARD(CS_CLUBS, CV_10),
MAKE_CARD(CS_CLUBS, CV_JACK),
MAKE_CARD(CS_CLUBS, CV_QUEEN),
MAKE_CARD(CS_CLUBS, CV_KING),
MAKE_CARD(CS_CLUBS, CV_ACE),

MAKE_CARD(CS_DIAMONDS, CV_7),
MAKE_CARD(CS_DIAMONDS, CV_8),
MAKE_CARD(CS_DIAMONDS, CV_9),
MAKE_CARD(CS_DIAMONDS, CV_10),
MAKE_CARD(CS_DIAMONDS, CV_JACK),
MAKE_CARD(CS_DIAMONDS, CV_QUEEN),
MAKE_CARD(CS_DIAMONDS, CV_KING),
MAKE_CARD(CS_DIAMONDS, CV_ACE),

MAKE_CARD(CS_HEARTS, CV_7),
MAKE_CARD(CS_HEARTS, CV_8),
MAKE_CARD(CS_HEARTS, CV_9),
MAKE_CARD(CS_HEARTS, CV_10),
MAKE_CARD(CS_HEARTS, CV_JACK),
MAKE_CARD(CS_HEARTS, CV_QUEEN),
MAKE_CARD(CS_HEARTS, CV_KING),
MAKE_CARD(CS_HEARTS, CV_ACE)
};

const Card player1Cards[] = {
MAKE_CARD(CS_SPIDES, CV_JACK),
MAKE_CARD(CS_SPIDES, CV_QUEEN),

MAKE_CARD(CS_CLUBS, CV_7),
MAKE_CARD(CS_CLUBS, CV_9),

MAKE_CARD(CS_DIAMONDS, CV_10),
MAKE_CARD(CS_DIAMONDS, CV_JACK),
MAKE_CARD(CS_DIAMONDS, CV_QUEEN),
MAKE_CARD(CS_DIAMONDS, CV_KING),

MAKE_CARD(CS_HEARTS, CV_7),
MAKE_CARD(CS_HEARTS, CV_JACK)
};

const Card player2Cards[] = {
MAKE_CARD(CS_SPIDES, CV_7),
MAKE_CARD(CS_SPIDES, CV_8),
MAKE_CARD(CS_SPIDES, CV_9),
MAKE_CARD(CS_SPIDES, CV_10),

MAKE_CARD(CS_CLUBS, CV_8),

MAKE_CARD(CS_DIAMONDS, CV_7),
MAKE_CARD(CS_DIAMONDS, CV_8),
MAKE_CARD(CS_DIAMONDS, CV_9),

MAKE_CARD(CS_HEARTS, CV_8),
MAKE_CARD(CS_HEARTS, CV_9)
};

const Card player3Cards[] = {
MAKE_CARD(CS_SPIDES, CV_KING),
MAKE_CARD(CS_SPIDES, CV_ACE),

MAKE_CARD(CS_CLUBS, CV_10),
MAKE_CARD(CS_CLUBS, CV_JACK),
MAKE_CARD(CS_CLUBS, CV_QUEEN),

MAKE_CARD(CS_DIAMONDS, CV_ACE),

MAKE_CARD(CS_HEARTS, CV_10),
MAKE_CARD(CS_HEARTS, CV_QUEEN),
MAKE_CARD(CS_HEARTS, CV_KING),
MAKE_CARD(CS_HEARTS, CV_ACE)
};

extern unsigned int giHits;
extern unsigned int giTotal;

int main()
{
    giHits = 0;
    giTotal = 0;

    clock_t tStart = clock();

    CGame game(CPlayer(CCardPack(player1Cards, 10), PS_P2MAX),
		CPlayer(CCardPack(player2Cards, 10), PS_P2MIN),
		CPlayer(CCardPack(player3Cards, 10), PS_P2MAX));

//    game.setTrumpSuit(CS_SPIDES);
//    game.setActivePlayer(2);

//    game.getCardsLeft().removeCard(MAKE_CARD(CS_CLUBS, CV_KING));
//    game.getCardsLeft().removeCard(MAKE_CARD(CS_CLUBS, CV_ACE));

    std::cout << game << std::endl;
    
    CScore initialScore;
    initialScore.setPlayerScore(0, 0);
    game.setScore(initialScore);

    //Some part of game...
/*    game.makeTurn(MAKE_CARD(CS_DIAMONDS, CV_10));
    game.makeTurn(MAKE_CARD(CS_DIAMONDS, CV_9));
    game.makeTurn(MAKE_CARD(CS_DIAMONDS, CV_ACE));

    game.makeTurn(MAKE_CARD(CS_HEARTS, CV_10));
    game.makeTurn(MAKE_CARD(CS_HEARTS, CV_JACK));
    game.makeTurn(MAKE_CARD(CS_HEARTS, CV_9));

    game.makeTurn(MAKE_CARD(CS_DIAMONDS, CV_KING));
    game.makeTurn(MAKE_CARD(CS_DIAMONDS, CV_8));
    game.makeTurn(MAKE_CARD(CS_SPIDES, CV_ACE));

    game.makeTurn(MAKE_CARD(CS_DIAMONDS, CV_QUEEN));
    game.makeTurn(MAKE_CARD(CS_DIAMONDS, CV_7));
    game.makeTurn(MAKE_CARD(CS_SPIDES, CV_KING));

    game.makeTurn(MAKE_CARD(CS_DIAMONDS, CV_JACK));
//////// Interesting moment
    game.makeTurn(MAKE_CARD(CS_SPIDES, CV_10));
    game.makeTurn(MAKE_CARD(CS_CLUBS, CV_QUEEN));

    game.makeTurn(MAKE_CARD(CS_SPIDES, CV_JACK));
    game.makeTurn(MAKE_CARD(CS_SPIDES, CV_9));
    game.makeTurn(MAKE_CARD(CS_CLUBS, CV_JACK));

    game.makeTurn(MAKE_CARD(CS_SPIDES, CV_QUEEN));
    game.makeTurn(MAKE_CARD(CS_SPIDES, CV_8));
    game.makeTurn(MAKE_CARD(CS_CLUBS, CV_10));

    game.makeTurn(MAKE_CARD(CS_CLUBS, CV_7));
    game.makeTurn(MAKE_CARD(CS_CLUBS, CV_8));
    game.makeTurn(MAKE_CARD(CS_HEARTS, CV_ACE));

    game.makeTurn(MAKE_CARD(CS_HEARTS, CV_8));
*/
    CScore score;
    std::cout << "Optiman turn: " << game.getOptimalTurn(score);
    std::cout << " Score: " << score << std::endl;
    
    std::cout << "Total states: " << giTotal<< " (Hits " << giHits << ")" << std::endl;

    clock_t tStop = clock();
    
    std::cout << "Answer given in " << (double)(tStop - tStart)/CLOCKS_PER_SEC << " seconds" << std::endl;
    
    return 0;
}
