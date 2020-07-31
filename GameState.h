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
 * about player's and their cards, current score, etc.
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
     * - active player is player 1
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
     * @note This operator is needed to handle the game states in visited states set. Actually it is not a 
     * good idea and most time is wasted in this operator, but it allows to decrease number of states to visit.
     *
     * @param rGame - game state to compare with current game state
     *
     * @return \a true if this game state less than specified, otherwise \a false
     */
    bool operator<(const CGameState & rGame) const;

private:    
    /// The blocked assignment operator
    CGameState& operator=(const CGameState & rGame);
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
     * @brief Get optimal turn for active player
     *
     * This method is intended for checking what turn of active player is optimal.
     * Actually this method will redirect execution to the active player's CPlayer::getOptimalTurn() method.
     *
     * @param score - reference to a score object which will be filled with score, that is possible is all players
     * will make optimal turns.
     *
     * @return the card with optimal turn (for active player)
     */
    Card getOptimalTurn(CScore & score);
    
    /**
     * @brief Guess the turn
     *
     * This method is intended for guessing the rest of game, if current player plays the specified card.
     * As result the score will be returned - it indicates final score, if all players will play with optimal strategy.
     * 
     * Algorithm description:
     * - Play the specified card (using the CGameState::makeTurn() method). Game state will be changed:
     *   - new active player
     *   - trick handled
     *   - score updated
     * - Check if new game state is already visited. If visited - return the saved score value
     * - Retrieve optimal score for the rest of game
     * - Save the state in the list of visited states.
     * .
     *
     * @param card - card to guess
     *
     * @return score of the game, after active player plays specified card and all players begin to play
     * with optimal strategy.
     */
    CScore guessTurn(Card card);
    
    /**
     * @brief Make the turn
     *
     * This method performs active player turn with specified card. This method put the specified card 
     * on the table. If trick is over, method will update game score.
     *
     * Algorithm description:
     * - Remove card from player's card
     * - Put card on the table
     * - new active player is next player
     * - If this a first card in trick
     *   - Current suit is a suit of this card
     *   .
     * - If this is last card in trick
     *   - Calculate the trick winner
     *   - increase score of winner player
     *   - Prepare for new trick:
     *     - Remove cards of the trick from list of all cards in game
     *     - Clear the list of cards on the table
     *     - Reset current suit
     *     - Active player is a player, that wins the trick
     *     .
     *   .
     * .
     *
     * @param card - card to play
     */
    void makeTurn(Card card);

    void setUpNewTrick();
    CCardPack getActivePlayerValidTurns();
    CPath playGameRecursive();
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
