#include <iostream>
#include <time.h>

#include "CardPack.h"
#include "CardDefs.h"
#include "GameState.h"
#include "Player.h"

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

int main()
{
    clock_t tStart = clock();

    CGameState game(CPlayer(CCardPack(player1Cards, 10), PS_P2MAX),
                    CPlayer(CCardPack(player2Cards, 10), PS_P2MIN),
                    CPlayer(CCardPack(player3Cards, 10), PS_P2MAX));

    game.setTrumpSuit(CS_SPIDES);

    std::cout << game << std::endl;

    CScore initialScore;
    game.setScore(initialScore);
    game.playGameRecursive();
    clock_t tStop = clock();

    std::cout << "Whole tree traversed in " << (double)(tStop - tStart)/CLOCKS_PER_SEC << " seconds" << std::endl;

    return 0;
}
