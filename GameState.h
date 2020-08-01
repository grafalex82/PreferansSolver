#ifndef GAME_STATE_H
#define GAME_STATE_H

/**
 * @file
 * @brief The game state declaration
 */

#include <vector>
#include <iostream>

#include "Player.h"
#include "Score.h"
#include "CardPack.h"
#include "Path.h"

class CVisitedStateCache;

/**
 * @brief The Game State
 *
 * This class represents the game state. This is a data holder class that contains information
 * about player's and their cards, current score, etc. Additionally it implements game logic,
 * as well as calculating optimal turn.
 */
class CGameState
{
public:
    /**
     * @brief Create new game state
     *
     * This constructor allows creating new game state with specified players.
     *
     * It will initialize internal data as follows:
     * - Players will be initialized as specified
     * - trump suit is unknown
     * - active player is player 0
     * - cards on table (cards in current trick) - empty
     * - score - zero for all players
     * .
     *
     * @param p1 - player 1 data
     * @param p2 - player 2 data
     * @param p3 - player 3 data
     */
    CGameState(const CPlayer & p1, const CPlayer & p2, const CPlayer & p3);
    
    /**
     * @brief The game copy constructor
     *
     * This copy constructor will copy the content of specified game.
     *
     * @param rGame - game to copy
     */
    CGameState(const CGameState & rGame);
    
    /**
     * @brief The game destructor
     *
     * This destructor will release internal data, such as allocated player objects
     */
    ~CGameState();

///@name Operators
//@{
public:
    /// Serialize operator
    friend std::ostream& operator<<(std::ostream& out, const CGameState & game);

    /**
     * @brief Less-than operator
     *
     * This operator is intended for comparing two game states.
     *
     * @note This operator is needed to handle the game states in visited states cache.
     * Actually it is not a good idea and most time is wasted in this operator, but it
     * allows to decrease number of processing states significantly.
     *
     * @param rGame - game state to compare with current game state
     *
     * @return \a true if this game state less than specified, otherwise \a false
     */
    bool operator<(const CGameState & rGame) const;

private:    
    /// The blocked assignment operator
    CGameState& operator=(const CGameState & rGame) = delete;
//@}

///@name Attributes operations
//@{
public:
    /**
     * @brief Set the initial score
     *
     * This method is intended for setting the initial game score. For instance it can be used for games
     * that have to be calculated not from beginning.
     *
     * @param score - score to set
     */
    inline void setScore(const CScore & score)
    {
        m_score = score;
    }

    /**
     * @brief Set visited states cache to use
     *
     * This method sets active visitated states cache object, or resets to nullptr.
     * Note, that game state object does not own the cache, just stores a pointer.
     *
     * @param pCache    - pointer to a visited states cache or nullptr if not used
     */
    inline void setVisitedStatesCache(CVisitedStateCache * pCache)
    {
        m_pCache = pCache;
    }

    /**
     * @brief Set active player
     *
     * This method is intended for specifying the active player. For instance it can be used for games
     * that have to be calculated not from beginning
     *
     * @throw "const char *" if player index is out of bounds
     *
     * @param p - the player index
     */    
    inline void setActivePlayer(unsigned int p)
    {
        if(p >= MAX_PLAYERS)
            throw "CGameState::setActivePlayer(): player index is out of bounds";
            
        m_iActivePlayer = p;
    }

    /**
     * @brief Get active player index
     *
     * @return Active player's index
     */
    inline unsigned int getActivePlayer() const
    {
        return m_iActivePlayer;
    }
    
    /**
     * @brief Get trum suit
     *
     * This method is intended for retrieving the trump suit of this game
     *
     * @return the trump suit
     */
    inline CardSuit getTrumpSuit() const
    {
        return m_trumpSuit;
    }

    /**
     * @brief Set the trump suit
     *
     * This method is intended for setting the trump suit for the game.
     *
     * @param suit - the new game trump suit
     */
    inline void setTrumpSuit(CardSuit suit)
    {
        m_trumpSuit = suit;
    }

    /**
     * @brief Get the base suit of current trick
     *
     * This method is intended for retrieving the suit, that is base for the current trick.
     * This means the suit of the first card in trick.
     *
     * @return current suit
     */
    inline CardSuit getCurrentSuit() const
    {
        return m_currentSuit;
    }

    /**
     * @brief Set the base suit of current trick
     *
     * This method may be needed to explicitely set a trick suit, for example in case
     * of pass game
     *
     * @param suit  - suit to set for current trick
     */
    inline void getCurrentSuit(CardSuit suit)
    {
        m_currentSuit = suit;
    }

    /**
     * @brief Retrieve the list of all cards in game.
     *
     * This method creates a list of cards in the game as a sum of players' cards
     *
     * @return card pack of card, that present in game,
     */
    CCardPack getCardsLeft();
//@}

///@name Turns related
//@{    
public:
    
    /**
     * @brief Make the turn
     *
     * This method performs active player's turn with specified card. This method actually
     * implements Preferans logic.
     *
     * Algorithm description:
     * - Remove card from player's card pack
     * - Put card on the table
     * - If this a first card in trick - current suit is set to suit of the played card
     * - pass the turn to the new active player
     * - If this is last card in trick
     *   - Calculate the trick winner
     *   - increase score of winner player
     *   - Prepare for new trick:
     *     - Clear the list of cards on the table
     *     - Reset current suit
     *     .
     *   .
     * .
     *
     * @param card - card to play
     */
    void makeTurn(Card card);

    CPath playGameRecursive();

protected:
    /**
     * @brief Prepare for the new trick
     *
     * This is a helper method that performs preparation for the new trick round when
     * working in recursive mode:
     * - Re-calculate cards left list (need to do this before trick begins,
     *   and this list remains valid until the end of the trick)
     * - Reset current suit
     * .
     */
    void setUpNewTrick();

    /**
     * @brief Prepare a list of valid turns
     *
     * When working in a recursive traversing mode this method is responsible for
     * calculating the list of valid turns for the active player. This method also filters
     * out equial turns in order to optimize processing.
     */
    CCardPack getActivePlayerValidTurns();
//@}
    
///@name Cards Left related functions
//@{
protected:
    /**
     * @brief Set up a new Cards Left object for a trick
     *
     * This is a helper method to create cards left object. It is supposed there
     * will be just one cards left object for all states related to the trick. The
     * root state of the trick will own cards left object, while all others will just
     * have a pointer.
     */
    void setUpCardsLeft();

    /**
     * @brief Release cards left object
     *
     * This is a helper method to release cards left object (if owned) and
     * reset the pointer to a default state
     */
    void releaseCardsLeft();
//@}

protected:
    /// List of players in game
    CPlayer * m_aPlayers[MAX_PLAYERS];
    /// Player, that will do next turn
    unsigned int m_iActivePlayer;

    /// Trump suit in game
    CardSuit m_trumpSuit;
    /// Current suit (undefined if there is no cards in trick yet)
    CardSuit m_currentSuit;
    /// List of cards present on the table (current trick)
    Card m_aCardsOnTable[MAX_PLAYERS];
    /// Number of cards on the table (current trick)
    unsigned int m_iCardsOnTableCount;

    /// Current game score
    CScore m_score;

    /// List of cards currently present in game
    CCardPack * m_pCardsLeft;
    /// Flag indicates that this current object owns m_pCardsLeft
    /// (otherwise it is just a reference to other's state card pack)
    bool m_bOwnsCardsLeft;

    /// Visited states cache or nullptr if not used. Game state object does not own the cache.
    CVisitedStateCache * m_pCache;
};

#endif //GAME_STATE_H
