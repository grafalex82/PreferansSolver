#ifndef _GAME_H_
#define _GAME_H_

/**
 * @file
 * @brief The game declaration
 */

#include <vector>
#include <iostream>

#include "Player.h"
#include "Score.h"
#include "CardPack.h"

/**
 * @brief The Game
 *
 * This class represents the game state and responsible for tricks counting, active player regulation, etc.
 */
class CGame
{
public:
    /**
     * @brief Create new game
     *
     * This constructr allows to create new game with specified players.
     *
     * It will initialize internal data as follows:
     * - Players will be initialized as specified
     * - cards presented in game as union of cards of all players
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
    CGame(const CPlayer & p1, const CPlayer & p2, const CPlayer & p3);
    
    /**
     * @brief The game copy constructor
     *
     * This copy constructor will copy the content of specified game.
     *
     * @param rGame - game to copy
     */
    CGame(const CGame & rGame);
    
    /**
     * @brief The game destructor
     *
     * This destructor will release internal data, such as allocated player objects
     */
    ~CGame();

///@name Operators
//@{
public:
    /// Serialize operator
    friend std::ostream& operator<<(std::ostream& out, const CGame & game);

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
    bool operator<(const CGame & rGame) const;

private:    
    /// The blocked assignment operator
    CGame& operator=(const CGame & rGame);
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
            throw "CGame::setActivePlayer(): player index is out of bounds";
            
        m_iActivePlayer = p;
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
     * This method is intended for retrieving the pack of all cards, that are present in game.
     *
     * @return card pack of card, that present in game,
     */
    inline CCardPack & getCardsLeft()
    {
        return m_cardsLeft;
    }
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
     * - Play the specified card (using the CGame::makeTurn() method). Game state will be changed:
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
    CCardPack m_cardsLeft;
};

#endif //_GAME_H_
