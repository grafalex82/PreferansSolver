#include "Player.h"
#include "GameState.h"

#include <string>
#include <vector>


CPlayer::CPlayer(const CCardPack & cardPack, PlayerStrategy eStrategy) :
    m_cardPack(cardPack)
{
    m_eStrategy = eStrategy;
}

CPlayer::CPlayer(const CPlayer & rPlayer) :
    m_cardPack(rPlayer.m_cardPack)
{
    m_eStrategy = rPlayer.m_eStrategy;
}

CPlayer::~CPlayer()
{
}
    
CPlayer& CPlayer::operator=(const CPlayer & rPlayer)
{
    if(this != &rPlayer)
    {
        m_cardPack = rPlayer.m_cardPack;
        m_eStrategy = rPlayer.m_eStrategy;
    }
    
    return *this;
}

std::ostream& operator<< (std::ostream& out, const CPlayer & player)
{
    out << "Player cards: " << player.m_cardPack;
    return out;
}

CCardPack CPlayer::getListOfValidTurns(CardSuit suit, CardSuit trump)
{
    // All cards are valid if no suit specified
    if(suit == CS_UNKNOWN)
        return m_cardPack;

    // Search for requested suit first
    if(m_cardPack.hasSuit(suit))
        return m_cardPack.getSubset(suit);

    // Then search for trumps
    if(m_cardPack.hasSuit(trump))
        return m_cardPack.getSubset(trump);

    // Otherwise all the remaining cards are valid
    return m_cardPack;
}

