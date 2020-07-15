#include "Game.h"

#include <map>

typedef std::map<CGame, CScore> MapGameToScore;
typedef MapGameToScore::const_iterator MapGameToScoreCIt;
typedef MapGameToScore::value_type MapGameToScoreVt;

static MapGameToScore gVisitedStates;
unsigned int giHits;
unsigned int giTotal;

CGame::CGame(const CPlayer & p1, const CPlayer & p2, const CPlayer & p3) :
    m_score(),
    m_cardsLeft(p1.getCards() + p2.getCards() + p3.getCards())
{
    m_aPlayers[0] = new CPlayer(p1);
    m_aPlayers[1] = new CPlayer(p2);
    m_aPlayers[2] = new CPlayer(p3);
    
    m_trumpSuit = CS_UNKNOWN;
    m_currentSuit = CS_UNKNOWN;
    m_iActivePlayer = 0;
    
    m_iCardsOnTableCount = 0;    
}

CGame::CGame(const CGame & rGame) :
    m_score(rGame.m_score),
    m_cardsLeft(rGame.m_cardsLeft)
{
    m_aPlayers[0] = new CPlayer(*rGame.m_aPlayers[0]);
    m_aPlayers[1] = new CPlayer(*rGame.m_aPlayers[1]);
    m_aPlayers[2] = new CPlayer(*rGame.m_aPlayers[2]);

    m_trumpSuit = rGame.m_trumpSuit;
    m_currentSuit = rGame.m_currentSuit;
    m_iActivePlayer = rGame.m_iActivePlayer;
    
    m_iCardsOnTableCount = rGame.m_iCardsOnTableCount;
    memcpy(m_aCardsOnTable, rGame.m_aCardsOnTable, 3*sizeof(Card));
}

CGame::~CGame()
{
    delete m_aPlayers[0];
    delete m_aPlayers[1];
    delete m_aPlayers[2];
}

bool CGame::operator<(const CGame & rGame) const
{
    if(m_iCardsOnTableCount < rGame.m_iCardsOnTableCount)
        return true;
    if(rGame.m_iCardsOnTableCount < m_iCardsOnTableCount)
        return false;
        
    for(unsigned int i=0; i<m_iCardsOnTableCount; i++)
    {
        if(m_aCardsOnTable[i] < rGame.m_aCardsOnTable[i])
            return true;
        if(rGame.m_aCardsOnTable[i] < m_aCardsOnTable[i])
            return false;
    }
    
    if(m_cardsLeft < rGame.m_cardsLeft)
        return true;
    if(rGame.m_cardsLeft < m_cardsLeft)
        return false;

    for(unsigned int i=0; i<MAX_PLAYERS; i++)
    {
        if(*(m_aPlayers[i]) < *(rGame.m_aPlayers[i]))
            return true;
        if(*(rGame.m_aPlayers[i]) < *(m_aPlayers[i]))
            return false;
    }

    if(m_iActivePlayer < rGame.m_iActivePlayer)
        return true;
    if(rGame.m_iActivePlayer < m_iActivePlayer)
        return false;

    if(m_score < rGame.m_score)
        return true;
    if(rGame.m_score < m_score)
        return false;
	
    if(m_trumpSuit < rGame.m_trumpSuit)
        return true;
    if(rGame.m_trumpSuit < m_trumpSuit)
        return false;
	
    if(m_currentSuit < rGame.m_currentSuit)
        return true;
//    if(rGame.m_currentSuit < m_currentSuit)
//	return false;

    return false;
}

std::ostream& operator<< (std::ostream& out, const CGame & game)
{
    out << std::endl;
    out << "0: " << *game.m_aPlayers[0] << std::endl;
    out << "1: " << *game.m_aPlayers[1] << std::endl;
    out << "2: " << *game.m_aPlayers[2] << std::endl;
    out << "Active player: " << game.m_iActivePlayer;
    
    return out;
}

Card CGame::getOptimalTurn(CScore & score)
{
    return m_aPlayers[m_iActivePlayer]->getOptimalTurn(this, score, (m_iCardsOnTableCount == 0));
}

CScore CGame::guessTurn(Card card)
{
    makeTurn(card);

    giTotal++;
    MapGameToScoreCIt it = gVisitedStates.find(*this);
    if(it != gVisitedStates.end())
    {
        giHits++;
        return it->second;
    }
    
    CScore ret;
    if(m_aPlayers[m_iActivePlayer]->hasCards())
    {
        getOptimalTurn(ret);
    }
    else
        ret = m_score;

    gVisitedStates.insert(MapGameToScoreVt(*this, ret));

    return ret;
}

void CGame::makeTurn(Card card)
{
    m_aPlayers[m_iActivePlayer]->removeCard(card);
    m_aCardsOnTable[m_iCardsOnTableCount++] = card;
    m_iActivePlayer = (++m_iActivePlayer) % MAX_PLAYERS;
    
    if(m_iCardsOnTableCount == 1)
    {
        m_currentSuit = getSuit(card);
    }
    else
    if(m_iCardsOnTableCount == 3)
    {
        //Calculate the winner
        unsigned int iWinner = 0;
        if(isCardHeigher(m_aCardsOnTable[1], m_aCardsOnTable[0], m_trumpSuit))
            iWinner = 1;
        if(isCardHeigher(m_aCardsOnTable[2], m_aCardsOnTable[iWinner], m_trumpSuit))
            iWinner = 2;

        // Increase the winner's score
        iWinner = (m_iActivePlayer + iWinner) % MAX_PLAYERS;
        m_score.incPlayerScore(iWinner);

        // Prepare for new trick
        for(unsigned int i=0; i<3; i++)
            m_cardsLeft.removeCard(m_aCardsOnTable[i]);
	    
        m_iCardsOnTableCount = 0;
        m_currentSuit = CS_UNKNOWN;
        m_iActivePlayer = iWinner;
    }
}

