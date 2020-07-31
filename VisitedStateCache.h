#ifndef VISITEDSTATECACHE_H
#define VISITEDSTATECACHE_H

/**
 * @file
 * @brief The Visited states cache declaration
 */

#include <map>
#include <sstream>

#include "GameState.h"
#include "Path.h"

/**
 * @brief Visited States Cache
 *
 * While traversing game states tree some of the states repeat. No need to process them
 * multiple times. This class is intended to store (cache) visited states and retrieve processing
 * result quickly when needed.
 *
 * Technically it is implemented using a map between state and resulting path.
 */
class CVisitedStateCache
{
public:

    /**
     * @brief Create an empty cache object
     *
     * Creates an empty cache object. States counters are also zeroed.
     */
    CVisitedStateCache()
    {
        m_iCacheHits = 0;
    }

    /**
     * @brief Add a new state to the cache
     *
     * This method stores a given state and associate a given result with this state
     * so that it can be retrieved later.
     *
     * @param state - state to store
     * @param path  - an optimal path associated with this state
     */
    void addVisitedState(const CGameState & state, const CPath & path)
    {
        m_cache.emplace(state, path);
    }

    /**
     * @brief Retrieve a state from the cache
     *
     * This method searches the given state in the cache and returns the path associated
     * with this state, so that no need to process the state once more. If no state found
     * in the card, an invalid path is returned.
     *
     * This method also increment hit counter if the state is found. No need to track cache
     * misses as any cache miss will eventually get back as a new state.
     *
     * @param state - state to search
     *
     * @return an optimal path associated with this state, or invalid path object if no state found
     */
    CPath getVisitedState(const CGameState & state) const
    {
        MapGameToPathCIt it = m_cache.find(state);
        if(it != m_cache.end())
        {
            m_iCacheHits++;
            return it->second;
        }

        return CPath();
    }

    /**
     * @brief Get hit count stats
     *
     * @return Current hit counter value
     */
    size_t getHitsCount() const
    {
        return m_iCacheHits;
    }

    /**
     * @brief Get cache size
     *
     * @return Current cache size
     */
    size_t getCacheSize() const
    {
        return m_cache.size();
    }

protected:
    /// Handy typedef for the cache storage type
    typedef std::map<CGameState, CPath> MapGameToPath;
    /// Handy typedef for the cache iterator type
    typedef MapGameToPath::const_iterator MapGameToPathCIt;

    /// The storage of visited states and their solve paths
    MapGameToPath m_cache;

    /// Number of cache hits
    mutable size_t m_iCacheHits;
};

#endif // VISITEDSTATECACHE_H
