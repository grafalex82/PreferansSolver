#include "GameState.h"
#include "VisitedStateCache.h"

#include <map>
#include <sstream>

CGameState::CGameState(const CPlayer & p1, const CPlayer & p2, const CPlayer & p3)
    : m_score()
    , m_pCardsLeft(nullptr)
    , m_bOwnsCardsLeft(false)
{
    m_aPlayers[0] = new CPlayer(p1);
    m_aPlayers[1] = new CPlayer(p2);
    m_aPlayers[2] = new CPlayer(p3);
    
    m_trumpSuit = CS_UNKNOWN;
    m_currentSuit = CS_UNKNOWN;
    m_iActivePlayer = 0;
    
    m_iCardsOnTableCount = 0;

    m_pCache = nullptr;
}

CGameState::CGameState(const CGameState & rGame)
    : m_score(rGame.m_score)

    // The copy will have the same reference to cards left pack, but it will not own it
    , m_pCardsLeft(rGame.m_pCardsLeft)
    , m_bOwnsCardsLeft(false)
{
    m_aPlayers[0] = new CPlayer(*rGame.m_aPlayers[0]);
    m_aPlayers[1] = new CPlayer(*rGame.m_aPlayers[1]);
    m_aPlayers[2] = new CPlayer(*rGame.m_aPlayers[2]);

    m_trumpSuit = rGame.m_trumpSuit;
    m_currentSuit = rGame.m_currentSuit;
    m_iActivePlayer = rGame.m_iActivePlayer;
    
    m_iCardsOnTableCount = rGame.m_iCardsOnTableCount;
    memcpy(m_aCardsOnTable, rGame.m_aCardsOnTable, 3*sizeof(Card));

    m_pCache = rGame.m_pCache;
}

CGameState::~CGameState()
{
    delete m_aPlayers[0];
    delete m_aPlayers[1];
    delete m_aPlayers[2];

    releaseCardsLeft();
}

CCardPack CGameState::getCardsLeft()
{
    return m_aPlayers[0]->getCards() +
           m_aPlayers[1]->getCards() +
           m_aPlayers[2]->getCards();
}

void CGameState::setUpCardsLeft()
{
    releaseCardsLeft();
    m_pCardsLeft = new CCardPack(getCardsLeft()); // TODO: optimize object creation
    m_bOwnsCardsLeft = true;
}

void CGameState::releaseCardsLeft()
{
    if(!m_bOwnsCardsLeft)
        return;

    delete m_pCardsLeft;
    m_pCardsLeft = nullptr;
    m_bOwnsCardsLeft = false;
}

bool CGameState::operator<(const CGameState & rGame) const
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
    if(rGame.m_currentSuit < m_currentSuit)
        return false;

    return false;
}

std::ostream& operator<< (std::ostream& out, const CGameState & game)
{
    out << std::endl;
    out << "0: " << *game.m_aPlayers[0] << std::endl;
    out << "1: " << *game.m_aPlayers[1] << std::endl;
    out << "2: " << *game.m_aPlayers[2] << std::endl;
    out << "Active player: " << game.m_iActivePlayer << std::endl;
    out << "Current score: " << game.m_score;
    
    return out;
}

void CGameState::makeTurn(Card card)
{
    // Update players and table information
    m_aPlayers[m_iActivePlayer]->removeCard(card);
    m_aCardsOnTable[m_iCardsOnTableCount++] = card;
    m_iActivePlayer = (++m_iActivePlayer) % MAX_PLAYERS;
    
    // First card on the table defines current trick suit (if not defined explicitely)
    if(m_iCardsOnTableCount == 1 && m_currentSuit != CS_UNKNOWN)
        m_currentSuit = getSuit(card);

    // 3rd card on the table completes the trick
    if(m_iCardsOnTableCount == 3)
    {
        //Calculate the winner
        unsigned int iWinner = calcTrickWinner(m_aCardsOnTable[0],
                                               m_aCardsOnTable[1],
                                               m_aCardsOnTable[2],
                                               m_trumpSuit);

        // Increase the winner's score
        iWinner = (m_iActivePlayer + iWinner) % MAX_PLAYERS;
        m_score.incPlayerScore(iWinner);

        // Prepare for new trick
        m_iCardsOnTableCount = 0;
        m_currentSuit = CS_UNKNOWN;
        m_iActivePlayer = iWinner;
    }
}

void CGameState::setUpNewTrick()
{
    setUpCardsLeft();
    m_currentSuit = CS_UNKNOWN;
}

CCardPack CGameState::getActivePlayerValidTurns()
{
    // Get list of turns and filter out equivalent ones
    CCardPack validTurns = m_aPlayers[m_iActivePlayer]->getListOfValidTurns(m_currentSuit, m_trumpSuit);
    validTurns.filterOutEquivalentCards(*m_pCardsLeft);

    return validTurns;
}

CPath CGameState::playGameRecursive()
{
    // Check if this state was already visited
    if(m_pCache)
    {
        CPath cachedPath = m_pCache->getVisitedState(*this);
        if(cachedPath.isValid())
            return cachedPath;
    }

    // Cleanup and prepare for the new trick
    if(m_iCardsOnTableCount == 0)
        setUpCardsLeft();

    CCardPack possibleTurns = getActivePlayerValidTurns();

    // end of recursion, if no turns can be done
    if(possibleTurns.getCardsCount() == 0)
    {
        return CPath(m_score);
    }

    // Process all valid turns and select the most optimal one
    CPath path(m_aPlayers[m_iActivePlayer]->getPlayerStrategy());
    for(unsigned int i=0; i<possibleTurns.getCardsCount(); i++)
    {
        Card card = possibleTurns.getCard(i);

        // Make a copy of current state, and play the turn recursively
        CGameState newState(*this);
        newState.makeTurn(card);
        CPath subPath = newState.playGameRecursive();

        // Search for the best subpath
        path.addSubPath(card, subPath);
    }

    // Store found solution in the visited states cache
    if(m_pCache)
        m_pCache->addVisitedState(*this, path);

    return path;
}
