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
/// Symbol for Unknown suit
const char SUIT_SYMB_UNKNOWN = '?';

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
    
    return SUIT_SYMB_UNKNOWN;;
}

/**
 * @brief Parse suit symbol
 *
 * This function parses the given symbol and returns suit value
 *
 * @throw "const char *" exception, indicates cause of error
 *
 * @param c - character to parse
 *
 * @return parsed suit
 */
inline CardSuit parseSuitSymb(char c)
{
    switch(c)
    {
    case SUIT_SYMB_SPIDES:
        return CS_SPIDES;
    case SUIT_SYMB_CLUBS:
        return CS_CLUBS;
    case SUIT_SYMB_DIAMONDS:
        return CS_DIAMONDS;
    case SUIT_SYMB_HEARTS:
        return CS_HEARTS;
    case SUIT_SYMB_UNKNOWN:
        return CS_UNKNOWN;
    default:
        break;
    }

    throw "parseSuitSymb(): Unrecognized suit symbol";
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
    CV_UNKNOWN
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
const char cardValueChars[] = {'2', '3', '4', '5', '6', '7', '8', '9', '1', 'J', 'Q', 'K', 'A', '?'};

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
    if(v <= CV_UNKNOWN)
        return cardValueChars[static_cast<size_t>(v)];
    
    throw "getCardValueSymb(): unknown card value";
}

/**
 * @brief Parse the card value symbol
 *
 * This function parses card value symbol and returns card value
 *
 * @throw "const char *" exception, indicating the cause of error
 *
 * @param c - symbol to parse
 *
 * @return resulting card value
 */
inline CardValue parseCardValueSymb(char c)
{
    for(unsigned int i=0; i<sizeof(cardValueChars); i++)
        if(cardValueChars[i] == c)
            return static_cast<CardValue>(i);

    throw "parseCardValueSymb(): unknown card value";
}


/// Macro for making the whole card value from card suit and card value
#define MAKE_CARD(suit, value) static_cast<Card>((suit&0xf0) | (value & 0x0f))

/// A special value representing an unknown card
const Card UNKNOWN_CARD = MAKE_CARD(CS_UNKNOWN, CV_UNKNOWN);

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
 * @brief Parse card
 *
 * This function is intended to parse a sequence of 2 chars representing a card
 *
 * @note if value or suit is unknown whole card is considered as unknown.
 *
 * @param cardStr - card string to parse
 *
 * @return card object, or an unknown card
 */
inline Card parseCard(const char * cardStr)
{
    CardValue cv = parseCardValueSymb(cardStr[0]);
    CardSuit cs = parseSuitSymb(cardStr[1]);

    // If suit of value is unknown - whole card is unknown as well
    if(cv == CV_UNKNOWN || cs == CS_UNKNOWN)
        return UNKNOWN_CARD;

    return MAKE_CARD(cs, cv);
}

/**
 * @brief Dump a card to string
 *
 * @param card - card to dump
 *
 * @return card representation string
 */
inline std::string getCardStr(Card card)
{
    char temp[3] = "  ";

    temp[0] = getCardValueSymb(getCardValue(card));
    temp[1] = getSuitSymb(getSuit(card));
    return temp;
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
 * @note this function does not support unknown cards, only exact ones
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

/**
 * @brief Calculate the trick winner
 *
 * This function accepts 3 cards on the table and calculates the winner, taking into
 * account trump if specified. It is expected that all 3 cards were placed according
 * to Preferans rules.
 *
 * It is expected that card1 was put first, and all other cards are compared to the 1st card.
 * For example, card1 is 7 spides, but other players have neigher spides, no trumps, then
 * 1st player wins.
 *
 * @note this function does not support unknown cards, only exact ones
 *
 * @param card1 - card played by 1st player
 * @param card2 - card played by 2nd player
 * @param card3 - card played by 3rd player
 * @param trump - the trump suit, or CS_UNKNOWN if no trump suit defined
 *
 * @return zero based number of player that wins this trick
 */
inline unsigned int calcTrickWinner(Card card1, Card card2, Card card3, CardSuit trump = CS_UNKNOWN)
{
    unsigned int iWinner = 0;
    Card highestCard = card1;

    // Compare card1 and card2
    if(isCardHeigher(card2, card1, trump))
    {
        iWinner = 1;
        highestCard = card2;
    }

    // And then compare them with card3
    if(isCardHeigher(card3, highestCard, trump))
        iWinner = 2;

    return iWinner;
}

#endif //CARD_DEFS_H

