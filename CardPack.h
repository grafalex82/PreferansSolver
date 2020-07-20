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
 * This class represents the pack of cards. It handles the array of cards.
 *
 * This class allows to handle the cards pack, so it can be only initialized with an array of cards.
 * The only operation is allowed to modify the card pack is remove specified card from a pack. Thus
 * there is no empty constructor for this class.
 *
 * Class handles the cards array as simple array of fixed size to provide maximum performace.
 *
 * @note This class is designed to provide maximum performace, so no STL is used.
 * @note It is assumed, that handled array of cards is always sorted.
 */
class CCardPack
{

public:
    /**
     * @brief Cards Pack constructor
     *
     * This constructor will create cards pack and fills it with values from specified array of cards.
     *
     * Constructor provides cards sorting to provide maximum performance. Due to cards count is not big,
     * the bubble sorting is used :)
     *
     * @param pCards    - array of cards
     * @param iCount    - number of cards in array
     */
    CCardPack(const Card * pCards, unsigned int iCount);
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
//@}

///@name Operations
//@{
public:
    /**
     * @brief Shuffle the current cards pack
     *
     * This method is intended for shuffle the current card pack.
     *
     * @note Shuffling algorithm may be refined
     */
    //void shuffle();
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
    bool compareCards(Card left, Card right) const;
//@}

protected:
    /// The cards array
    Card m_aCards[MAX_CARDS];
    /// Current number of cards in handled array
    unsigned int m_iCardsCount;
};

#endif // CARD_PACK_H
