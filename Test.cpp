#define CATCH_CONFIG_MAIN

#include <sstream>

#include <catch2/catch.hpp>
#include "CardDefs.h"

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


#if 0

int Factorial( int number ) {
   return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
// return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

//TEST_CASE( "Factorial of 0 is 1 (fail)", "[single-file]" ) {
//    REQUIRE( Factorial(0) == 1 );
//}

TEST_CASE( "Factorials of 1 and higher are computed (pass)", "[single-file]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}


std::string one() {
    return "1";
}

TEST_CASE( "Assert that something is true (pass)", "[require]" ) {
    REQUIRE( one() == "1" );
}

TEST_CASE( "Assert that something is true (fail)", "[require]" ) {
    REQUIRE( one() == "x" );
}

TEST_CASE( "Assert that something is true (continue after failure)", "[check]" ) {
    WARN( "CHECK continues after failure:" );

    CHECK(   one() == "x" );
    REQUIRE( one() == "1" );
}

TEST_CASE( "vectors can be sized and resized", "[vector]" ) {

    std::vector<int> v( 5 );

    REQUIRE( v.size() == 5 );
    REQUIRE( v.capacity() >= 5 );

    SECTION( "resizing bigger changes size and capacity" ) {
        v.resize( 10 );

        REQUIRE( v.size() == 10 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "resizing smaller changes size but not capacity" ) {
        v.resize( 0 );

        REQUIRE( v.size() == 0 );
        REQUIRE( v.capacity() >= 5 );
    }
    SECTION( "reserving bigger changes capacity but not size" ) {
        v.reserve( 10 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "reserving smaller does not change size or capacity" ) {
        v.reserve( 0 );

        REQUIRE( v.size() == 6 );
        REQUIRE( v.capacity() >= 5 );
    }
}

#endif
