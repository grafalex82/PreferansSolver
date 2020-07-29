#ifndef PATH_H
#define PATH_H

/**
 * @file
 * @brief The game path declaration
 */


#include "Score.h"

/**
 * @brief Path
 *
 * This class an utility class that holds a path to a specific state, as well as resulting state.
 * This class is also indended for calculation the optimal path according to player's strategy,
 * and also it represents a result of game subtree solve.
 *
 * The class basically consists of 2 main parts:
 * - path (sequence of turns) that leads from the root of subtree, to a leaf state
 * - a score in that leaf state.
 * .
 *
 * While Path object can store any sequence of turns, this class helps searching a single optimal
 * path of the game.
 *
 * The class can work in 2 different (mutual exclusive modes), each has a set of functions. One
 * represents a leaf states with a trivial subpath. These object has only score value. Other mode
 * is to calculate an optimal path having number of subpath objects as inputs. The reason that 2
 * different modes are handled by the same class is to have a single interface for \a
 * CGameState::playGameRecursive() that may return an object of one of these 2 types.
 */
class CPath
{
public:
    /**
     * @brief Create a leaf (trivial) path object
     *
     * This constructor creates an empty path object, but stores the resulting score.
     *
     * @param score - score value for the leaf of the states tree
     */
    CPath(const CScore & score);


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

protected:
    CScore m_score;
};

#endif // PATH_H
