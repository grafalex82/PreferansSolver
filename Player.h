#ifndef PLAYER_H
#define PLAYER_H

/**
 * @file
 * @brief Player declaration
 */

#include <iostream>

#include "CardPack.h"
#include "Score.h"

//Forward declaration
class CGameState;

/**
 * @brief The player class
 *
 * This class represents the single player in the game.
 * It encapsulates all functionality connected with the player: player cards, player turns, etc.
 */
class CPlayer
{
public:
    /**
     * @brief Create the new player
     *
     * This cnstructor will create a new player object with specified cards and strategy.
     *
     * @param cardPack  - cards, that player will start with
     * @param eStrategy - player's strategy
     */
    CPlayer(const CCardPack & cardPack, PlayerStrategy eStrategy);

    /**
     * @brief Player object copy constructor
     *
     * This constructor will copy the content of specified player object.
     *
     * @param rPlayer - player to copy
     */
    CPlayer(const CPlayer & rPlayer);

    /**
     * @brief Player object destructor
     *
     * This destructor will release object internal data
     */
    ~CPlayer();

///@name Operators
//@{
public:    
    /**
     * @brief Assignment operator
     *
     * This operator will copy the content of specified player object to current object.
     *
     * @param rPlayer - player object to copy
     */
    CPlayer& operator=(const CPlayer & rPlayer);

    /**
     * @brief Less-than operator
     *
     * This operator will compare current player with specified.
     *
     * @param rPlayer - player to compare with
     *
     * @return \a true if current player is less than specified, \a false otherwise
     */
    inline bool operator<(const CPlayer & rPlayer) const
    {
        if(m_cardPack < rPlayer.m_cardPack)
            return true;
        if(rPlayer.m_cardPack < m_cardPack)
            return false;

        if(m_eStrategy < rPlayer.m_eStrategy)
            return true;
//        if(rPlayer.m_eStrategy < m_eStrategy)
//            return false;
	
        return false;
    }
    
    /// Serialization operator. Serialize the player state to the specified stream as a string.
    friend std::ostream& operator<< (std::ostream& out, const CPlayer & player);    
//@}

///@name Operatrions
//@{
public:
    /**
     * @brief Return a list of valid turns for a particular game situation
     *
     * This method is intended for calculating a list of valid turns for a particular
     * game situation, taking into account current suit (if defined), and trump (if specified)
     *
     * Method will filter player's card pack according to the following criteria:
     * - If no trick suit specified the player is allowed to play any card
     * - If trick suit is specified - result will include same suit only
     * - If trick suit is specified, but the player does not have this suit - the algorithm
     *   will return trumps
     * - If there is are cards of the needed suit, and there are no trumps, resulting list
     *   will include all the other cards
     *   .
     *
     * @param suit  - the suit of the current trick. It can be \a CS_UNKNOWN if the suit is
     *                not defined yet (e.g. the player is on the first hand)
     * @param trump - the trump suit of the current game
     *
     * @todo the algorithm may apply removing equivalent cards here
     *
     * @return filtered card pack that has only valid turns
     */
    CCardPack getListOfValidTurns(CardSuit suit, CardSuit trump = CS_UNKNOWN);

    /**
     * @brief Remove card from player's cards
     *
     * This method is intended for removing the specified card from card pack handled by player.
     *
     * @param card - card to remove
     */
    inline void removeCard(Card card)
    {
        m_cardPack.removeCard(card);
    }
    
    /**
     * @brief Check if player has cards
     *
     * This method is intended for checking if player has at least one card
     *
     * @return \a true if player has at least one card, \a false otherwise
     */
    inline bool hasCards() const
    {
        return (m_cardPack.getCardsCount() > 0);
    }

    /**
     * @brief Retrieve the player's cards
     *
     * This method is intended for retrieving the cards of this player
     *
     * @return Cards of this player
     */
    inline CCardPack getCards() const
    {
        return m_cardPack;
    }

    /**
     * @brief Retrieve the player's strategy
     *
     * @return player strategy
     */
    inline PlayerStrategy getPlayerStrategy() const
    {
        return m_eStrategy;
    }
//@}
        
protected:
    /// Player's cards
    CCardPack m_cardPack;
    /// Player's strategy
    PlayerStrategy m_eStrategy;
};

#endif //PLAYER_H
