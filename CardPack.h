#ifndef CARD_PACK_H
#define CARD_PACK_H

/**
 * @file
 * @brief Cards Pack class definition
 */

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include "CardDefs.h"

/// Maximum number of cards, that can be handled in one pack. For Marraige is 4 suits of 8 cards.
const unsigned int MAX_CARDS = 4*8;

/**
 * @brief The Cards Pack class
 *
 * This class represents the pack of cards. It handles the array of cards and provides a few
 * handy functions.
 *
 * This class allows to handle the cards pack, so it can be only initialized with an array of cards.
 * The only operation is allowed to modify the card pack is remove specified card from a pack. Thus
 * there is no public empty constructor for this class.
 *
 * Class handles the cards array as simple array of fixed size to provide maximum performace.
 *
 * @note It is assumed, that handled array of cards is always sorted.
 * @note The class can handle unknown cards. They are treated as playholders for real cards
 *       that may be calculated later
 */
class CCardPack
{

private:
    /**
     * @brief Empty Cards Pack constructor
     *
     * This constructor will create an empty cards pack
     */
    CCardPack();

public:    
    /**
     * @brief Cards Pack constructor (from string)
     *
     * This constructor will parse given string and create cards pack with parsed cards
     * Constructor provides cards sorting to provide maximum performance
     *
     * @param cards     - string to parse
     */
    CCardPack(const char * cards);

    /**
     * @brief Cards Pack copy constructor
     *
     * This copy constructor will copy content of specified Card Pack
     *
     * @param rPack - the source cards pack
     */
    CCardPack(const CCardPack & rPack);
    /**
     * @brief Cards Pack destructor
     *
     * This destructor will free handled array.
     */
    ~CCardPack();

///@name Operators
//@{
public:
    /**
     * @brief Assignment operator
     *
     * This operator will copy the content of specified card pack.
     *
     * @param rPack - cards pack to copy
     *
     * @return the copied cards pack
     */    
    CCardPack & operator=(const CCardPack & rPack);
    
    /**
     * @brief less than operator
     *
     * This operator will compare two card packs. 
     *
     * @note Implemented algorithm is not a lexicographical order. It only provides two axioms:
     * a=a -> false; if a<b=true, then b<a = false.
     *
     * @param rPack - another pack to compare with.
     *
     * @return \a true if current pack is less than specified, \a false otherwise
     */
    inline bool operator<(const CCardPack & rPack) const
    {
        if(m_iCardsCount < rPack.m_iCardsCount)
            return true;
        if(rPack.m_iCardsCount < m_iCardsCount)
            return false;
    
        for(unsigned int i=0; i<m_iCardsCount; i++)
        {
            if(m_aCards[i] < rPack.m_aCards[i])
                return true;
            if(rPack.m_aCards[i] < m_aCards[i])
                return false;
        }
        
        return false;
    }

    /**
     * @brief equivalence operator
     *
     * This operator will compare two card packs and return true if they are equal
     *
     * @param rPack - another pack to compare with.
     *
     * @return \a true if current pack is equal, \a false otherwise
     */
    inline bool operator==(const CCardPack & rPack) const
    {
        return m_iCardsCount == rPack.m_iCardsCount &&
                memcmp(m_aCards, rPack.m_aCards, m_iCardsCount) == 0;
    }

    /// Serialization operator. It stores the cards pack as a string (each card separated by space) to the stream.
    friend inline std::ostream& operator<< (std::ostream& out, const CCardPack & cardPack)
    {
        out << cardPack.getPackStr();
        return out;
    }
    
    /**
     * @brief Addition operator
     *
     * This operator is intended for combination of two card packs (union).
     *
     * Algorithm assumes, that both card pack are sorted, resulting card pack will be also sorted.
     *
     * @throw "const char *" exception if cards from both packs cannot fit in one pack, or if one card is found
     * in two card packs (thus, input card packs shall have different cards to combine)
     *
     * @param rPack - card pack to be added to current card pack
     *
     * @return The combined card pack
     */
    CCardPack operator +(const CCardPack &rPack);

    /**
     * @brief Subtraction operator
     *
     * This operator is intended for removing elements that are present in the reference pack.
     *
     * Algorithm assumes, that both card pack are sorted, resulting card pack will be also sorted.
     *
     * @param rPack - card pack to be subtracted from current card pack
     *
     * @return The pack containing only those elements, that were not present in the reference card pack
     */
    CCardPack operator -(const CCardPack &rPack);
//@}

///@name Data retrieving operators
//@{
public:
    /**
     * @brief Retrive string representation of card pack
     *
     * This method is intended for retrieving of the string representation
     * of the card pack
     *
     * @return string, containing the cards pack
     */
    std::string getPackStr() const;

    /**
     * @brief Retrieve cards count
     *
     * This method is intended for retrieving the count of cards handled in this cards pack
     *
     * @return number of cards
     */
    inline unsigned int getCardsCount() const
    {
        return m_iCardsCount;
    }
    
    /**
     * @brief Retrieve the card by index
     *
     * This method is intended for retrieving the card by specified index
     *
     * @note Index bounds are not checked
     *
     * @param idx   - index of the card to be retrieved
     *
     * @return The card value
     */
    inline Card getCard(unsigned int idx) const
    {
        return m_aCards[idx];
    }

    /**
     * @brief Check whether the given card is in the pack
     *
     * @param card   - card to check
     *
     * @return \a true if the card is in the list, \a false otherwise
     */
    inline bool hasCard(Card card) const
    {
        for(unsigned int i=0; i<m_iCardsCount; i++)
        {
            if(m_aCards[i] == card)
                return true;
        }
        return false;
    }

    /**
     * @brief Check whether the card pack has any cards of specified card suit
     *
     * @param suit   - suit to search
     *
     * @return \a true if there are cards of specified suit, \a false otherwise
     */
    inline bool hasSuit(CardSuit suit) const
    {
        // We do not have cards with unknown suits, for sure
        if(suit == CS_UNKNOWN)
            return false;

        // Just search for the requested suit
        for(unsigned int i=0; i<m_iCardsCount; i++)
        {
            if(getSuit(m_aCards[i]) == suit)
                return true;
        }
        return false;
    }

    /**
     * @brief Check whether the card pack has any unknown cards
     *
     * @return \a true if there are unknown cards in the pack, \a false otherwise
     */
    inline bool hasUnknownCards() const
    {
        // Just search for unknown cards
        for(unsigned int i=0; i<m_iCardsCount; i++)
        {
            if(m_aCards[i] == UNKNOWN_CARD)
                return true;
        }
        return false;
    }

    /**
     * @brief compare two cards and check their equivalence
     *
     * This method allows to compare two cards in terms of equivalence. Equivalent cards are cards of the same suit
     * and there is no other card, that heigher than left card and less than right. In terms of sorted cards array
     * This cards will be located in two neighbor cells.
     *
     * It is assumed, that cards array is sorted and specified left card is less than right.
     *
     * @param left  - left compared card
     * @param right - right compared card
     *
     * @return \a true if cards are equivalent, \a false otherwise
     */
    bool areCardsEquivalent(Card left, Card right) const;
//@}

///@name Operations
//@{
public:
    /**
     * @brief random split the deck
     *
     * This method randomly splits the pack and extract specified number of cards to a new deck.
     * Extracted cards are removed from current deck
     *
     * @note both decks get sorted
     *
     * @param size  - number of cards to extract
     *
     * @return a new pack containing extracted cards
     */
    CCardPack extractRandomCards(unsigned int size);

    /**
     * @brief Remove card
     *
     * This method is intended for removing the specified card from the current cards pack.
     *
     * @note If specified card does not contained in current cards pack, cards pack will not be changed.
     *
     * @param card - card to be removed
     */
    void removeCard(Card card);

    /**
     * @brief return a subset of cards that match the given suit
     *
     * This method is widely used when calculating a list of valid player turns
     *
     * @param suit - the suit to include into a resulting subset
     */
    CCardPack getSubset(CardSuit suit) const;

    /**
     * @brief filter out equivalent cards
     *
     * This method filters out cards that have the same value in the given reference
     * card pack. Cards are equivalent if they are located contiguously in the reference
     * pack.
     *
     * @param suit - the suit to include into a resulting subset
     */
    void filterOutEquivalentCards(const CCardPack & ref);

//@}

protected:
    /// The cards array
    Card m_aCards[MAX_CARDS];
    /// Current number of cards in handled array
    unsigned int m_iCardsCount;
};

#endif // CARD_PACK_H
