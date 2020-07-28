#include <sstream>

#include <catch2/catch.hpp>
#include "CardDefs.h"
#include "CardPack.h"
#include "Score.h"
#include "Player.h"
#include "GameState.h"

template<class T>
std::string getObjStr(T obj)
{
    std::stringstream str;
    str << obj;
    return str.str();
}

TEST_CASE( "Card basic functions", "Cards" )
{
    // getSuitSymb()
    REQUIRE(getSuitSymb(CS_SPIDES) == '^');
    REQUIRE(getSuitSymb(CS_CLUBS) == '+');
    REQUIRE(getSuitSymb(CS_DIAMONDS) == '$');
    REQUIRE(getSuitSymb(CS_HEARTS) == '@');

    // parseSuitSymb()
    REQUIRE(parseSuitSymb('^') == CS_SPIDES);
    REQUIRE(parseSuitSymb('+') == CS_CLUBS);
    REQUIRE(parseSuitSymb('$') == CS_DIAMONDS);
    REQUIRE(parseSuitSymb('@') == CS_HEARTS);

    // getCardValueSymb()
    REQUIRE(getCardValueSymb(CV_2) == '2');
    REQUIRE(getCardValueSymb(CV_10) == '1');
    REQUIRE(getCardValueSymb(CV_JACK) == 'J');
    REQUIRE(getCardValueSymb(CV_QUEEN) == 'Q');
    REQUIRE(getCardValueSymb(CV_KING) == 'K');
    REQUIRE(getCardValueSymb(CV_ACE) == 'A');

    // parseCardValueSymb()
    REQUIRE(parseCardValueSymb('2') == CV_2);
    REQUIRE(parseCardValueSymb('1') == CV_10);
    REQUIRE(parseCardValueSymb('J') == CV_JACK);
    REQUIRE(parseCardValueSymb('Q') == CV_QUEEN);
    REQUIRE(parseCardValueSymb('K') == CV_KING);
    REQUIRE(parseCardValueSymb('A') == CV_ACE);

    // parseCard()
    REQUIRE(parseCard("K+") == MAKE_CARD(CS_CLUBS, CV_KING));
    REQUIRE(parseCard("Q@") == MAKE_CARD(CS_HEARTS, CV_QUEEN));

    // Old style MAKE_CARD macro
    Card card = MAKE_CARD(CS_CLUBS, CV_KING);
    REQUIRE(card == 0x1B); // Card internal representation
    REQUIRE(getSuit(card) == CS_CLUBS); // getSuit()
    REQUIRE(getSuitSymb(getSuit(card)) == '+'); //getSuitSymb()
    REQUIRE(getCardValue(card) == CV_KING); // getCardValue()
    REQUIRE(getCardValueSymb(getCardValue(card)) == 'K'); // getCardValueSymb()

    // Card printing
    REQUIRE(getObjStr(card) == "K+");
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

    // Calculate tricks - all play same suit
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_CLUBS, CV_JACK),
                            MAKE_CARD(CS_CLUBS, CV_KING),
                            MAKE_CARD(CS_CLUBS, CV_8),
                            CS_UNKNOWN) == 1);
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_CLUBS, CV_8),
                            MAKE_CARD(CS_CLUBS, CV_QUEEN),
                            MAKE_CARD(CS_CLUBS, CV_ACE),
                            CS_UNKNOWN) == 2);
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_CLUBS, CV_10),
                            MAKE_CARD(CS_CLUBS, CV_9),
                            MAKE_CARD(CS_CLUBS, CV_8),
                            CS_UNKNOWN) == 0);

    // Calculate tricks - all play different suits
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_SPIDES, CV_JACK),
                            MAKE_CARD(CS_CLUBS, CV_KING),
                            MAKE_CARD(CS_HEARTS, CV_8),
                            CS_UNKNOWN) == 0);
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_HEARTS, CV_JACK),
                            MAKE_CARD(CS_DIAMONDS, CV_KING),
                            MAKE_CARD(CS_SPIDES, CV_8),
                            CS_UNKNOWN) == 0);
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_HEARTS, CV_JACK),
                            MAKE_CARD(CS_DIAMONDS, CV_KING),
                            MAKE_CARD(CS_SPIDES, CV_8),
                            CS_CLUBS) == 0);

    // Calculate tricks - someone plays tump
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_SPIDES, CV_JACK),
                            MAKE_CARD(CS_CLUBS, CV_KING),
                            MAKE_CARD(CS_HEARTS, CV_8),
                            CS_HEARTS) == 2);
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_SPIDES, CV_JACK),
                            MAKE_CARD(CS_CLUBS, CV_KING),
                            MAKE_CARD(CS_HEARTS, CV_8),
                            CS_SPIDES) == 0);
    REQUIRE(calcTrickWinner(MAKE_CARD(CS_SPIDES, CV_JACK),
                            MAKE_CARD(CS_CLUBS, CV_KING),
                            MAKE_CARD(CS_HEARTS, CV_8),
                            CS_CLUBS) == 1);
}

TEST_CASE( "Card Pack creation and basic operations", "Card Pack")
{
    // Check card pack creation (Note, cards are shuffled)
    CCardPack pack("9^ 7^ K^ 1^ Q^ J^ 8^ A^");
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

    // Check hasCard()
    REQUIRE(pack.hasCard(MAKE_CARD(CS_SPIDES, CV_7)) == true);
    REQUIRE(pack.hasCard(MAKE_CARD(CS_CLUBS, CV_7)) == false);

    // Check hasSuit()
    REQUIRE(pack.hasSuit(CS_SPIDES) == true);
    REQUIRE(pack.hasSuit(CS_CLUBS) == false);
    REQUIRE(pack.hasSuit(CS_UNKNOWN) == false);
}

TEST_CASE( "Card Pack additions/removal", "Card Pack")
{
    // Make 2 non-intersecting card packs
    CCardPack pack1("7^ 8^ Q^ K^");
    CCardPack pack2("9^ 1^ J^ A^");

    // Add card packs and check the result has all the cards, and it is sorted
    CCardPack pack = pack1 + pack2;
    REQUIRE(pack.getPackStr() == " 7^ 8^ 9^ 1^ J^ Q^ K^ A^");

    // Remove cards
    pack.removeCard(MAKE_CARD(CS_SPIDES, CV_9));
    REQUIRE(pack.getPackStr() == " 7^ 8^ 1^ J^ Q^ K^ A^");
    pack.removeCard(MAKE_CARD(CS_SPIDES, CV_KING));
    REQUIRE(pack.getPackStr() == " 7^ 8^ 1^ J^ Q^ A^");
}

TEST_CASE( "Card Pack - filtering", "Card Pack")
{
    // Check card pack creation
    CCardPack pack("7^ 8^ 9+ 1+ J$ Q$ K@ A@");

    // Only clubs remain
    REQUIRE(pack.getSubset(CS_CLUBS).getPackStr() == " 9+ 1+");

    // Only spides remain
    REQUIRE(pack.getSubset(CS_SPIDES).getPackStr() == " 7^ 8^");

    // Unknown suit leads to an empty subset
    REQUIRE(pack.getSubset(CS_UNKNOWN).getPackStr() == "");
}


TEST_CASE( "Card Pack - card equivalence", "Card Pack")
{
    CCardPack pack("7^ 8^ 9^ J^ Q^ K^ A^ A+");

    SECTION("Check areCardsEquivalent()")
    {
        // Cards located next to each other are equivalent regardless of their value
        REQUIRE(pack.areCardsEquivalent(MAKE_CARD(CS_SPIDES, CV_7), MAKE_CARD(CS_SPIDES, CV_8)));
        REQUIRE(pack.areCardsEquivalent(MAKE_CARD(CS_SPIDES, CV_9), MAKE_CARD(CS_SPIDES, CV_JACK)));

        // Cards located separately cannot be equivalent (TODO: unless they belong to the same player)
        REQUIRE(pack.areCardsEquivalent(MAKE_CARD(CS_SPIDES, CV_7), MAKE_CARD(CS_SPIDES, CV_QUEEN)) == false);

        // Cards of different suits can't be equivalent
        REQUIRE(pack.areCardsEquivalent(MAKE_CARD(CS_SPIDES, CV_ACE), MAKE_CARD(CS_CLUBS, CV_ACE)) == false);
    }

    SECTION("Check filterOutEquivalentTurns - a single card")
    {
        CCardPack pack2("9^");

        pack2.filterOutEquivalentCards(pack);
        REQUIRE(pack2.getPackStr() == " 9^");
    }

    SECTION("Check filterOutEquivalentTurns - another single card")
    {
        CCardPack pack2("A+");

        pack2.filterOutEquivalentCards(pack);
        REQUIRE(pack2.getPackStr() == " A+");
    }

    SECTION("Check filterOutEquivalentTurns - a few consequent cards")
    {
        CCardPack pack2("7^ 8^ 9^");

        pack2.filterOutEquivalentCards(pack);
        REQUIRE(pack2.getPackStr() == " 9^");
    }

    SECTION("Check filterOutEquivalentTurns - a few groups of consequent cards")
    {
        CCardPack pack2("7^ 8^ 9^    K^ A^");

        pack2.filterOutEquivalentCards(pack);
        REQUIRE(pack2.getPackStr() == " 9^ A^");
    }

    SECTION("Check filterOutEquivalentTurns - more groups of consequent cards")
    {
        CCardPack pack2("7^ 8^   J^ Q^   A^   A+");

        pack2.filterOutEquivalentCards(pack);
        REQUIRE(pack2.getPackStr() == " 8^ Q^ A^ A+");
    }

    SECTION("Check filterOutEquivalentTurns - consecutive group of different suits")
    {
        CCardPack pack2("K^ A^   A+");

        pack2.filterOutEquivalentCards(pack);
        REQUIRE(pack2.getPackStr() == " A^ A+");
    }

    SECTION("Check filterOutEquivalentTurns - packs are the same")
    {
        CCardPack pack2("7^ 8^ 9^ J^ Q^ K^ A^ A+");

        pack2.filterOutEquivalentCards(pack);
        REQUIRE(pack2.getPackStr() == " A^ A+");
    }
}

TEST_CASE("Check main CScore operations", "Score")
{
    // Create and fill score object
    CScore score;
    score.setPlayerScore(0, 3);
    score.setPlayerScore(1, 2);
    score.setPlayerScore(2, 1);

    // Check created values
    REQUIRE(score.getPlayerScore(0) == 3);
    REQUIRE(score.getPlayerScore(1) == 2);
    REQUIRE(score.getPlayerScore(2) == 1);

    // Check score printing
    REQUIRE(getObjStr(score) == "(3, 2, 1)");

    // Copy score object
    CScore score2 = score;
    REQUIRE(getObjStr(score) == getObjStr(score2));

    // Check operator< for the same score
    REQUIRE((score < score2) == false);
    REQUIRE((score2 < score) == false);

    // Inc player score
    score.incPlayerScore(0);
    REQUIRE(score.getPlayerScore(0) == 4);
    REQUIRE(getObjStr(score) == "(4, 2, 1)");

    // Check operator< for different scores (using lexicographical order)
    REQUIRE((score < score2) == false);
    REQUIRE((score2 < score) == true);

}

TEST_CASE("CScore player strategies", "Score")
{
    CScore score1;
    score1.setPlayerScore(0, 3);
    score1.setPlayerScore(1, 2);
    score1.setPlayerScore(2, 1);

    CScore score2;
    score2.setPlayerScore(0, 2);
    score2.setPlayerScore(1, 3);
    score2.setPlayerScore(2, 4);

    REQUIRE(score1.isScoreHeigher(score2, PS_P1MAX) == false);
    REQUIRE(score1.isScoreHeigher(score2, PS_P1MIN) == true);
    REQUIRE(score1.isScoreHeigher(score2, PS_P2MAX) == true);
    REQUIRE(score1.isScoreHeigher(score2, PS_P2MIN) == false);
    REQUIRE(score1.isScoreHeigher(score2, PS_P3MAX) == true);
    REQUIRE(score1.isScoreHeigher(score2, PS_P3MIN) == false);
}

TEST_CASE("Player's valid turns", "Player")
{
    CCardPack pack("7^ 8^ 9+ 1+ J$ Q$");
    CPlayer player(pack, PS_P1MAX);

    SECTION("No trick suit specified (all turns are valid)")
    {
        CCardPack pack2 = player.getListOfValidTurns(CS_UNKNOWN, CS_DIAMONDS);
        REQUIRE(pack2.getPackStr() == " 7^ 8^ 9+ 1+ J$ Q$");
    }

    SECTION("Play spides")
    {
        CCardPack pack2 = player.getListOfValidTurns(CS_SPIDES, CS_DIAMONDS);
        REQUIRE(pack2.getPackStr() == " 7^ 8^");
    }

    SECTION("Play hearts (but we do not have ones)")
    {
        CCardPack pack2 = player.getListOfValidTurns(CS_HEARTS, CS_DIAMONDS);
        REQUIRE(pack2.getPackStr() == " J$ Q$");
    }

    SECTION("Play hearts (but we have neither hearts, nor trumps)")
    {
        CCardPack pack2 = player.getListOfValidTurns(CS_HEARTS, CS_UNKNOWN);
        REQUIRE(pack2.getPackStr() == " 7^ 8^ 9+ 1+ J$ Q$");
    }
}

TEST_CASE("Game State ", "Game State")
{
    // Create 3 players
    CCardPack pack1("7^ 9+ Q$");
    CPlayer player1(pack1, PS_P2MIN);

    CCardPack pack2("9^ 7+ K$");
    CPlayer player2(pack2, PS_P2MAX);

    CCardPack pack3("K^ J+ 7$");
    CPlayer player3(pack3, PS_P2MIN);

    // Create a game state
    CGameState state(player1, player2, player3);

    SECTION("Calculate all cards in the game")
    {
        // Make sure resulting cards pack is sorted
        CCardPack allCards = state.getCardsLeft();
        REQUIRE(getObjStr(allCards) == " 7^ 9^ K^ 7+ 9+ J+ 7$ Q$ K$");
    }
}
