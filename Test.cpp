#include <sstream>

#include <catch2/catch.hpp>
#include "CardDefs.h"
#include "CardPack.h"

TEST_CASE( "Card functions", "Cards" )
{
    Card card = MAKE_CARD(CS_CLUBS, CV_KING);
    REQUIRE(card == 0x1B); // Card internal representation
    REQUIRE(getSuit(card) == CS_CLUBS); // getSuit()
    REQUIRE(getSuitSymb(getSuit(card)) == '+'); //getSuitSymb()
    REQUIRE(getCardValue(card) == CV_KING); // getCardValue()
    REQUIRE(getCardValueSymb(getCardValue(card)) == 'K'); // getCardValueSymb()

    std::stringstream str;
    str << card;
    REQUIRE(str.str() == "K+");
}

TEST_CASE( "Card comparison", "Cards" )
{
    // Compare the same card
    REQUIRE(isCardHeigher(MAKE_CARD(CS_CLUBS, CV_KING), MAKE_CARD(CS_CLUBS, CV_KING)) == false);

    // Compare different cards of the same suit
    REQUIRE(isCardHeigher(MAKE_CARD(CS_CLUBS, CV_KING), MAKE_CARD(CS_CLUBS, CV_JACK)) == true);
    REQUIRE(isCardHeigher(MAKE_CARD(CS_CLUBS, CV_JACK), MAKE_CARD(CS_CLUBS, CV_JACK)) == false);
    REQUIRE(isCardHeigher(MAKE_CARD(CS_CLUBS, CV_KING), MAKE_CARD(CS_CLUBS, CV_ACE)) == false);

    // Compare cards of different suits
    REQUIRE(isCardHeigher(MAKE_CARD(CS_CLUBS, CV_KING), MAKE_CARD(CS_HEARTS, CV_JACK)) == false);
    REQUIRE(isCardHeigher(MAKE_CARD(CS_CLUBS, CV_JACK), MAKE_CARD(CS_HEARTS, CV_KING)) == false);

    // Compare different cards when trump is defined
    REQUIRE(isCardHeigher(MAKE_CARD(CS_CLUBS, CV_KING), MAKE_CARD(CS_HEARTS, CV_JACK), CS_HEARTS) == false);
    REQUIRE(isCardHeigher(MAKE_CARD(CS_CLUBS, CV_JACK), MAKE_CARD(CS_HEARTS, CV_KING), CS_CLUBS) == true);
}

TEST_CASE( "Card Pack creation", "Card Pack")
{
    const Card prefCards[] =
    {
        MAKE_CARD(CS_SPIDES, CV_7),
        MAKE_CARD(CS_SPIDES, CV_8),
        MAKE_CARD(CS_SPIDES, CV_9),
        MAKE_CARD(CS_SPIDES, CV_10),
        MAKE_CARD(CS_SPIDES, CV_JACK),
        MAKE_CARD(CS_SPIDES, CV_QUEEN),
        MAKE_CARD(CS_SPIDES, CV_KING),
        MAKE_CARD(CS_SPIDES, CV_ACE)
    };

    // Check card pack creation
    CCardPack pack(prefCards, sizeof(prefCards) / sizeof(Card));
    REQUIRE(pack.getCardsCount() == 8);

    // Check card pack copying
    CCardPack pack2 = pack;
    REQUIRE(pack2.getCardsCount() == 8);

    // Check card pack printing
    REQUIRE(pack2.getPackStr() == " 7^ 8^ 9^ 1^ J^ Q^ K^ A^");

    // Check that copied packs are equivalent
    REQUIRE(pack == pack2);

    // Check that cards can be retrieved
    REQUIRE(pack.getCard(0) == MAKE_CARD(CS_SPIDES, CV_7));
    REQUIRE(pack.getCard(2) == MAKE_CARD(CS_SPIDES, CV_9));
    REQUIRE(pack.getCard(6) == MAKE_CARD(CS_SPIDES, CV_KING));
}

TEST_CASE( "Card Pack additions", "Card Pack")
{
    // Make 2 non-intersecting card packs
    const Card pack1Cards[] =
    {
        MAKE_CARD(CS_SPIDES, CV_7),
        MAKE_CARD(CS_SPIDES, CV_8),
        MAKE_CARD(CS_SPIDES, CV_QUEEN),
        MAKE_CARD(CS_SPIDES, CV_KING)
    };
    CCardPack pack1(pack1Cards, sizeof(pack1Cards) / sizeof(Card));

    const Card pack2Cards[] =
    {
        MAKE_CARD(CS_SPIDES, CV_9),
        MAKE_CARD(CS_SPIDES, CV_10),
        MAKE_CARD(CS_SPIDES, CV_JACK),
        MAKE_CARD(CS_SPIDES, CV_ACE)
    };
    CCardPack pack2(pack2Cards, sizeof(pack2Cards) / sizeof(Card));

    // Add card packs and check the result has all the cards, and it is sorted
    CCardPack pack = pack1 + pack2;
    REQUIRE(pack.getPackStr() == " 7^ 8^ 9^ 1^ J^ Q^ K^ A^");
}
