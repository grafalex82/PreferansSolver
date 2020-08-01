#include <sstream>

#include <catch2/catch.hpp>
#include "CardDefs.h"
#include "CardPack.h"
#include "Score.h"
#include "Player.h"
#include "GameState.h"
#include "Path.h"
#include "VisitedStateCache.h"

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
    REQUIRE(getSuitSymb(CS_UNKNOWN) == '?');

    // parseSuitSymb()
    REQUIRE(parseSuitSymb('^') == CS_SPIDES);
    REQUIRE(parseSuitSymb('+') == CS_CLUBS);
    REQUIRE(parseSuitSymb('$') == CS_DIAMONDS);
    REQUIRE(parseSuitSymb('@') == CS_HEARTS);
    REQUIRE(parseSuitSymb('?') == CS_UNKNOWN);

    // getCardValueSymb()
    REQUIRE(getCardValueSymb(CV_2) == '2');
    REQUIRE(getCardValueSymb(CV_10) == '1');
    REQUIRE(getCardValueSymb(CV_JACK) == 'J');
    REQUIRE(getCardValueSymb(CV_QUEEN) == 'Q');
    REQUIRE(getCardValueSymb(CV_KING) == 'K');
    REQUIRE(getCardValueSymb(CV_ACE) == 'A');
    REQUIRE(getCardValueSymb(CV_UNKNOWN) == '?');

    // parseCardValueSymb()
    REQUIRE(parseCardValueSymb('2') == CV_2);
    REQUIRE(parseCardValueSymb('1') == CV_10);
    REQUIRE(parseCardValueSymb('J') == CV_JACK);
    REQUIRE(parseCardValueSymb('Q') == CV_QUEEN);
    REQUIRE(parseCardValueSymb('K') == CV_KING);
    REQUIRE(parseCardValueSymb('A') == CV_ACE);
    REQUIRE(parseCardValueSymb('?') == CV_UNKNOWN);

    // parseCard()
    REQUIRE(parseCard("K+") == MAKE_CARD(CS_CLUBS, CV_KING));
    REQUIRE(parseCard("Q@") == MAKE_CARD(CS_HEARTS, CV_QUEEN));
    REQUIRE(parseCard("Q?") == UNKNOWN_CARD);
    REQUIRE(parseCard("??") == UNKNOWN_CARD);
    REQUIRE(parseCard("??") == UNKNOWN_CARD);

    // Old style MAKE_CARD macro
    Card card = MAKE_CARD(CS_CLUBS, CV_KING);
    REQUIRE(card == 0x1B); // Card internal representation
    REQUIRE(getSuit(card) == CS_CLUBS); // getSuit()
    REQUIRE(getSuitSymb(getSuit(card)) == '+'); //getSuitSymb()
    REQUIRE(getCardValue(card) == CV_KING); // getCardValue()
    REQUIRE(getCardValueSymb(getCardValue(card)) == 'K'); // getCardValueSymb()

    // Card printing
    REQUIRE(getObjStr(card) == "K+");
    REQUIRE(getObjStr(UNKNOWN_CARD) == "??");
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
    REQUIRE(pack.hasUnknownCards() == false);

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

    // Check a pack with unknown cards
    CCardPack pack3("9^ ?? K^ ?? Q^ ??");
    REQUIRE(pack3.hasUnknownCards() == true);
    REQUIRE(pack3.getPackStr() == " 9^ Q^ K^ ?? ?? ??");
}

TEST_CASE( "Card Pack additions/removal", "Card Pack")
{
    // Make 2 non-intersecting card packs
    CCardPack pack1("7^ 8^ Q^ K^ ??");
    CCardPack pack2("9^ 1^ J^ A^ ??");

    // Add card packs and check the result has all the cards, and it is sorted
    CCardPack pack = pack1 + pack2;
    REQUIRE(pack.getPackStr() == " 7^ 8^ 9^ 1^ J^ Q^ K^ A^ ?? ??");

    // Remove cards
    pack.removeCard(MAKE_CARD(CS_SPIDES, CV_9));
    REQUIRE(pack.getPackStr() == " 7^ 8^ 1^ J^ Q^ K^ A^ ?? ??");
    pack.removeCard(MAKE_CARD(CS_SPIDES, CV_KING));
    REQUIRE(pack.getPackStr() == " 7^ 8^ 1^ J^ Q^ A^ ?? ??");
    pack.removeCard(UNKNOWN_CARD);
    REQUIRE(pack.getPackStr() == " 7^ 8^ 1^ J^ Q^ A^ ??");
    pack.removeCard(UNKNOWN_CARD);
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
    CScore score(3, 2, 1);

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
    CPlayer player("7^ 8^ 9+ 1+ J$ Q$", PS_P1MAX);
    REQUIRE(player.getPlayerStrategy() == PS_P1MAX);

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

TEST_CASE("Game State", "Game State")
{
    // Create 3 players
    CPlayer player1("7^ 9+ Q$", PS_P2MIN);
    CPlayer player2("9^ 7+ K$", PS_P2MAX);
    CPlayer player3("K^ J+ 7$", PS_P2MIN);

    // Create a game state
    CGameState state(player1, player2, player3);

    SECTION("Calculate all cards in the game")
    {
        // Make sure resulting cards pack is sorted
        CCardPack allCards = state.getCardsLeft();
        REQUIRE(getObjStr(allCards) == " 7^ 9^ K^ 7+ 9+ J+ 7$ Q$ K$");
    }
}

TEST_CASE("Game path functions", "Game Path")
{
    CPath invalidPath;
    REQUIRE(invalidPath.isValid() == false);

    // create a leaf path object (one that does not really have a path, but have a score)
    CPath path1(CScore(1, 2, 3));
    REQUIRE(getObjStr(path1.getOptimalScore()) == "(1, 2, 3)");
    REQUIRE(path1.getOptimalPath() == "");
    REQUIRE(path1.isValid() == true);

    // Create a few more leaf objects
    CPath path2(CScore(3, 2, 1));
    CPath path3(CScore(1, 3, 2));

    // Create a parent path object
    CPath parentPath(PS_P1MAX);
    REQUIRE(getObjStr(parentPath.getOptimalScore()) == "(0, 0, 0)"); // Default score
    REQUIRE(parentPath.isValid() == false);

    // Add a few subpaths to the parent path and check the processing
    parentPath.addSubPath(MAKE_CARD(CS_SPIDES, CV_KING), path1);
    parentPath.addSubPath(MAKE_CARD(CS_CLUBS, CV_QUEEN), path2);
    parentPath.addSubPath(MAKE_CARD(CS_HEARTS, CV_JACK), path3);

    // path2 should win according to P1MAX strategy
    REQUIRE(getObjStr(parentPath.getOptimalScore()) == "(3, 2, 1)");
    REQUIRE(parentPath.getOptimalPath() == " Q+");
    REQUIRE(parentPath.isValid() == true);

    // Prepare a bigger path object
    CPath megaPath(PS_P2MIN);
    megaPath.addSubPath(MAKE_CARD(CS_DIAMONDS, CV_9), CPath(CScore(4, 3, 2)));
    megaPath.addSubPath(MAKE_CARD(CS_SPIDES, CV_7), parentPath);
    REQUIRE(getObjStr(megaPath.getOptimalScore()) == "(3, 2, 1)");
    REQUIRE(megaPath.getOptimalPath() == " 7^ Q+");
    REQUIRE(megaPath.isValid() == true);
}

TEST_CASE("Visited State Cache", "Visited State Cache")
{
    // Create an empty cache
    CVisitedStateCache cache;
    REQUIRE(cache.getHitsCount() == 0);
    REQUIRE(cache.getCacheSize() == 0);

    // Create a few game state objects
    CPlayer player1("7^ 9+ Q$", PS_P2MIN);
    CPlayer player2("9^ 7+ K$", PS_P2MAX);
    CPlayer player3("K^ J+ 7$", PS_P2MIN);
    CGameState state(player1, player2, player3);
    state.setScore(CScore(1, 2, 3));
    CGameState anotherState(player3, player2, player1);
    state.setScore(CScore(3, 2, 1));

    // Create a path
    CPath path(CScore(1, 2, 3));
    CPath anotherPath(CScore(3, 2, 1));

    // Add the state to the cache
    cache.addVisitedState(state, path);
    REQUIRE(cache.getCacheSize() == 1);

    // Check the state is there
    CPath retpath1 = cache.getVisitedState(state);
    REQUIRE(retpath1.isValid() == true);
    REQUIRE(getObjStr(retpath1.getOptimalScore()) == "(1, 2, 3)");
    REQUIRE(cache.getHitsCount() == 1);

    // Check if other state is not yet there
    CPath retpath2 = cache.getVisitedState(anotherState);
    REQUIRE(retpath2.isValid() == false);
    REQUIRE(cache.getCacheSize() == 1);

    // Add another state
    cache.addVisitedState(anotherState, anotherPath);
    REQUIRE(cache.getCacheSize() == 2);

    // Check if the new state is now there
    CPath retpath3 = cache.getVisitedState(anotherState);
    REQUIRE(retpath3.isValid() == true);
    REQUIRE(cache.getCacheSize() == 2);
    REQUIRE(cache.getHitsCount() == 2);
}
