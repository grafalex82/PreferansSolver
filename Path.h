#ifndef PATH_H
#define PATH_H

/**
 * @file
 * @brief The game path declaration
 */

#include <vector>

#include "CardDefs.h"
#include "Score.h"

/**
 * @brief Path
 *
 * This class an utility class that holds a path to a specific state, as well as resulting state.
 * This class is also indended for calculation the optimal path according to player's strategy,
 * and also it represents a result of game subtree solve.
 *
 * The class basically consists of 2 main items:
 * - path (sequence of turns) that leads from the root of subtree, to a leaf state
 * - a score in that leaf state.
 * .
 *
 * While Path object can store any sequence of turns, this class helps searching a single optimal
 * path of the game.
 */
class CPath
{
public:
    /**
     * @brief Create a leaf (trivial) path object
     *
     * This constructor creates a path object with empty path, but stores the resulting score.
     *
     * @param score - score value for the leaf of the states tree
     */
    CPath(const CScore & score);

    /**
     * @brief Create a intermediate path object
     *
     * This constructor creates a path object supposed to be a intermediate point
     * in the resulting tree. The object is initialized with a player's strategy, so that
     * the class will participate in optimal turn calculation. This is supposed to work
     * in conjuction with \a addSubPath() method.
     *
     * @param strategy - current player's strategy
     */
    CPath(PlayerStrategy strategy);

    /**
     * @brief Return an optimal path calculated by the object
     *
     * Path object is intended for processing multiple input sub-path objects and selecting
     * the one that satisfy player's strategy criteria.
     *
     * @return resulting (optimal) path score
     */
    CScore getOptimalScore() const
    {
        return m_score;
    }

    /**
     * @brief Return an optimal path string
     *
     * This method prints the calculated path to a string
     *
     * @return resulting (optimal) path string
     */
    std::string getOptimalPath() const;

    /**
     * @brief Process a possible subpath
     *
     * This method processes a given subpath, and according to its score and player's
     * strategy selects the optimal one
     *
     * @param card      - card to play
     * @param subpath   - a subpath associated with played card
     */
    void addSubPath(Card card, const CPath & subpath);

protected:
    /**
     * @brief Save a given subpath as optimal one
     *
     * This is a helper method that just stores given subpath and its score as optimal
     * solution. This method is also responsible for combining subpath with a card resulting
     * little longer path.
     *
     * @param card      - card to play
     * @param subpath   - a subpath associated with played card
     */
    void storeOptimalPath(Card card, const CPath & subpath);

protected:
    /// An optimal score for this path
    CScore m_score;

    /// Optimal path that leads to the score (it is stored reversed)
    std::vector<Card> m_path;

    /// A Player's strategy
    PlayerStrategy m_strategy;

};

#endif // PATH_H
