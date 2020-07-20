#include "Score.h"

bool CScore::isScoreHeigher(const CScore & score, PlayerStrategy eStrategy) const
{
    switch(eStrategy)
    {
    case PS_P1MIN:
        return (score.m_uPlayersScore.vPlayerScore[0] < m_uPlayersScore.vPlayerScore[0]);
    case PS_P1MAX:
        return (score.m_uPlayersScore.vPlayerScore[0] > m_uPlayersScore.vPlayerScore[0]);
    case PS_P2MIN:
        return (score.m_uPlayersScore.vPlayerScore[1] < m_uPlayersScore.vPlayerScore[1]);
    case PS_P2MAX:
        return (score.m_uPlayersScore.vPlayerScore[1] > m_uPlayersScore.vPlayerScore[1]);
    case PS_P3MIN:
        return (score.m_uPlayersScore.vPlayerScore[2] < m_uPlayersScore.vPlayerScore[2]);
    case PS_P3MAX:
        return (score.m_uPlayersScore.vPlayerScore[2] > m_uPlayersScore.vPlayerScore[2]);
    }

    throw "CStrategy::isScoreHeigher() Unknown strategy";    
}

