#ifndef CARD_DEFS_H
#define CARD_DEFS_H

/**
 * @file
 * @brief Card related definitions
 */

#include <iostream>

/// Typedef for single card
typedef unsigned char Card;

///@name Suit related
//@{

/// Card suits enumerator
enum CardSuit
{
    /// Value for Spides suit
    CS_SPIDES = 0x00,
    /// Value for Clubs suit
    CS_CLUBS = 0x10,
    /// Value for Diamonds suit
    CS_DIAMONDS = 0x20,
    /// Value for Hearts suit
    CS_HEARTS = 0x30,
    /// Unknown suit
    CS_UNKNOWN = 0xf0
};

/// Mask for suit
const Card CARD_SUIT_MASK = 0xf0;

/**
 * @brief retrieve suit value
 *
 * This function will retrieve the suit value of the specified card
 *
 * @param card - card to retrieve value from
 *
 * @return suit of the specified card
 */
inline CardSuit getSuit(Card card)
{
    return static_cast<CardSuit>(card & CARD_SUIT_MASK);
}

/// Symbol for Spides suit
const char SUIT_SYMB_SPIDES = '^';
/// Symbol for Clubs suit
const char SUIT_SYMB_CLUBS = '+';
/// Symbol for Diamonds suit
const char SUIT_SYMB_DIAMONDS = '$';
/// Symbol for Hearts suit
const char SUIT_SYMB_HEARTS = '@';

/**
 * @brief Retrieve suit symbol
 *
 * This function is intended for retrieving the suit symbol for specified suit. 
 * It is used for card printing.
 *
 * @throw "const char *" exception, indicates cause of error
 *
 * @param suit - requested suit
 *
 * @return the suit symbol
 */
inline char getSuitSymb(CardSuit suit)
{
    switch(suit)
    {
    case CS_SPIDES:
        return SUIT_SYMB_SPIDES;
    case CS_CLUBS:
        return SUIT_SYMB_CLUBS;
    case CS_DIAMONDS:
        return SUIT_SYMB_DIAMONDS;
    case CS_HEARTS:
        return SUIT_SYMB_HEARTS;
    default:
        break;
    }
    
    throw "getSuitSymb(): Unrecognized suit";
}
//@}

/// Card values enumerator
enum CardValue
{
    /// Value for 2 card
    CV_2 = 0,
    /// Value for 3 card
    CV_3,
    /// Value for 4 card
    CV_4,
    /// Value for 5 card
    CV_5,
    /// Value for 6 card
    CV_6,
    /// Value for 7 card
    CV_7,
    /// Value for 8 card
    CV_8,
    /// Value for 9 card
    CV_9,
    /// Value for 10 card
    CV_10,
    /// Value for Jack card
    CV_JACK,
    /// Value for Queen card
    CV_QUEEN,
    /// Value for King card
    CV_KING,
    /// Value for Ace card
    CV_ACE,
    /// Unknown card value
    CV_UNKNOWN = 0x0f
};

/// The card value mask
const unsigned char CARD_VALUE_MASK = 0x0f;

/**
 * @brief Retrieve the card value
 *
 * This function is intended for retrieving the card value of specified card
 *
 * @param card - card to retrieve value from
 *
 * @return the card value
 */
inline CardValue getCardValue(Card card)
{
    return static_cast<CardValue>(card & 0x0f);
}

/// Symbols for card values
const char cardValueChars[] = {'2', '3', '4', '5', '6', '7', '8', '9', '1', 'J', 'Q', 'K', 'A'};

/**
 * @brief Retrieve the card value symbol
 *
 * This function is intended for retrieving the symbol, that represents the specified card value
 *
 * @throw "const char *" exception, indicating the cause of error
 *
 * @param v - value of the card, that symbol should be retrieved from
 *
 * @return The symbol, that represents the card value
 */
inline char getCardValueSymb(CardValue v)
{
    if(v <= CV_ACE)
        return cardValueChars[static_cast<size_t>(v)];
    
    throw "getCardValueSymb(): unknown card value";
}

/// Macro for making the whole card value from card suit and card value
#define MAKE_CARD(suit, value) static_cast<Card>((suit&0xf0) | (value & 0x0f))

/**
 * @brief Card serialization operator
 *
 * This operator is intended for serialization of the specified card to string stream.
 *
 * @param out   - stream, that card will be serialized to
 * @param card  - card to be serialized
 *
 * @return stream with serialized card
 */
inline std::ostream& operator<< (std::ostream & out, const Card& card)
{
    out << getCardValueSymb(getCardValue(card)) << getSuitSymb(getSuit(card));
    return out;
}

/**
 * @brief Compare two cards
 *
 * This function is intended for comparing two cards according to specified trump suit.
 * At is assumed, that reference card has been put first and function will compare specified card
 * with reference cards.
 *
 * For example if reference card is 8 spides, no trump suit is defined, so all cards from 9 to ace of spides
 * will be assumed as heigher, all others will be lower.
 *
 * @param card  - card to be checked
 * @param ref   - reference card
 * @param trump - the trump suit, or CS_UNKNOWN if no trump suit defined
 *
 * @return \a true if card is heigher than reference card.
 */
inline bool isCardHeigher(Card card, Card ref, CardSuit trump = CS_UNKNOWN)
{
    if(trump != CS_UNKNOWN)
    {
        if(getSuit(card) == trump)
	    {
            // Both cards are trumps
            if(getSuit(ref) == trump)
		        return (getCardValue(card) > getCardValue(ref));
	        
            // Our card is a trump, but reference is not
            return true;
	    }
	    else
	    {
            // Our card is not a trump, while reference is
	        if(getSuit(ref) == trump)
		        return false;
	    }
    }

    // Both cards are not trumps, we can compare them only if they have the same suit
    if(getSuit(card) == getSuit(ref))
	    return (getCardValue(card) > getCardValue(ref));
    
    return false;
}

#endif //CARD_DEFS_H

