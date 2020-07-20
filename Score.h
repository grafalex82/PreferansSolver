#ifndef _SCORE_H_
#define _SCORE_H_

/**
 * @file
 * @brief Players strategy and game score declaration
 */

#include <iostream>

/// The player strategy
enum PlayerStrategy
{
    /// Strategy of the player is to minimize number of tricks of player 1
    PS_P1MIN,
    /// Strategy of the player is to maximize number of tricks of player 1
    PS_P1MAX,
    /// Strategy of the player is to minimize number of tricks of player 2
    PS_P2MIN,
    /// Strategy of the player is to maximize number of tricks of player 2
    PS_P2MAX,
    /// Strategy of the player is to minimize number of tricks of player 3
    PS_P3MIN,
    /// Strategy of the player is to maximize number of tricks of player 3
    PS_P3MAX
};

/// Number of players in the game
const unsigned int MAX_PLAYERS = 3;

/**
 * @brief Game score class
 *
 * This class represents the game score. This class have possibility to compare the scores
 * according to specified player strategy.
 *
 * To provide maximum performace, players' score is defined as union of array and integer:
 * if method does not need to access to single player score, it uses score value as single value.
 */
class CScore
{
public:
    /**
     * @brief Game score constructor
     *
     * This constructor fills score for all players with zero
     */
    CScore()
    {
        m_uPlayersScore.lPlayersScore = 0;
    }
    
    /**
     * @brief Game score copy constructor
     *
     * This constructor will copy specified score
     *
     * @param rScore - score to copy
     */
    CScore(const CScore & rScore)
    {
        m_uPlayersScore.lPlayersScore = rScore.m_uPlayersScore.lPlayersScore;
    }
    
    /**
     * @brief Game score destructor
     *
     * This destructor will release internal data
     */
    ~CScore()
    {
    }
    
///@name Operators
//@{
public:
    /**
     * @brief Assignment operator
     *
     * This operator is intended for copying of specified game score to this object
     *
     * @param rScore - score to copy
     * 
     * @return reference to this object
     */
    inline CScore& operator=(const CScore& rScore)
    {
        if(this != &rScore)
        {
            m_uPlayersScore.lPlayersScore = rScore.m_uPlayersScore.lPlayersScore;
        }
    
        return *this;
    }

    /**
     * @brief Less than operator
     *
     * This operator will compare the two game scores in lexicographical order.
     * 
     * @param rScore - object to compare with
     *
     * @return \a true if this object less than scpecified.
     */
    inline bool operator<(const CScore & rScore) const
    {
        return (m_uPlayersScore.lPlayersScore < rScore.m_uPlayersScore.lPlayersScore);
    }

    /**
     * @brief serialization operator
     *
     * This operator will serialize the current score to a stream.
     */
    friend std::ostream& operator<< (std::ostream& out, const CScore & score)
    {
        out << '(' << static_cast<unsigned int>(score.m_uPlayersScore.vPlayerScore[0]) <<
          ", " << static_cast<unsigned int>(score.m_uPlayersScore.vPlayerScore[1]) <<
          ", " << static_cast<unsigned int>(score.m_uPlayersScore.vPlayerScore[2]) << ')';
        return out;
    }
//@}

///@name Operations
//@{
public:
    /**
     * @brief Specify the player score
     *
     * This method is intended for specifying the new score of the specified player
     *
     * @note To increase performance, this method does not check the player number.
     *
     * @param player    - the player to change score
     * @param score     - new player of specified score
     */
    inline void setPlayerScore(unsigned int player, unsigned char score)
    {
        m_uPlayersScore.vPlayerScore[player] = score;
    }
    
    /**
     * @brief Retrieve the player score
     *
     * This method is intended for retrieving the player score
     *
     * @note To increase performance, this method does not check the player number.
     *
     * @param player    - the player to get score
     *
     * @return The player score.
     */
    inline unsigned char getPlayerScore(unsigned int player) const
    {
        return m_uPlayersScore.vPlayerScore[player];
    }
    
    /**
     * @brief Increment score
     *
     * This method is intended for incrementing the score of specified player.
     *
     * @note To increase performance, this method does not check the player number.
     *
     * @param player    - the player number
     */
    inline void incPlayerScore(unsigned int player)
    {
        m_uPlayersScore.vPlayerScore[player]++;
    }

    /**
     * @brief check the score according to strategy
     *
     * This method is intended for comparison current score and specified. Method allows to check
     * if specified score is better than current according to specified strategy.
     *
     * @param score     - score to check
     * @param eStrategy - the player strategy. Scores will be compared according to this strategy
     *
     * @return \a true if specified score is better than current according to specified strategy, \a false otherwise.
     */
    bool isScoreHeigher(const CScore & score, PlayerStrategy eStrategy) const;

protected:
    union
    {
        /// Players score array
        unsigned char vPlayerScore[MAX_PLAYERS];
        /// Players score, presented as single integer value
        unsigned long lPlayersScore;
    } m_uPlayersScore;
};

#endif //_SCORE_H_
