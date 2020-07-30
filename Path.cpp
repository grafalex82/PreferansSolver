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

std::string CPath::getOptimalPath() const
{
    std::string sRes;
    for (auto it = m_path.rbegin(); it != m_path.rend(); ++it)
    {
        char temp[4] = "   ";

        temp[1] = getCardValueSymb(getCardValue(*it));
        temp[2] = getSuitSymb(getSuit(*it));

        sRes.append(std::string(temp));
    }

    return sRes;
}

void CPath::storeOptimalPath(Card card, const CPath & subpath)
{
    // Optimal score is the resulting score of the subpath
    m_score = subpath.getOptimalScore();

    // Optimal path consists of the subpath with additional card
    m_path = subpath.m_path;
    m_path.push_back(card);
}

void CPath::addSubPath(Card card, const CPath & subpath)
{
    // First subpath would be the optimal one
    if(m_path.size() == 0)
    {
        storeOptimalPath(card, subpath);
    }
    else
    {
        if(m_score.isScoreHeigher(subpath.getOptimalScore(), m_strategy))
            storeOptimalPath(card, subpath);
    }
}
