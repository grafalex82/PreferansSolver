#include "Path.h"

CPath::CPath(const CScore & score)
    : m_score(score)
    , m_strategy(PS_P1MIN)
{

}

CPath::CPath(PlayerStrategy strategy)
    : m_strategy(strategy)
{

}
