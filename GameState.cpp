#include "GameState.h"

#include <map>
#include <sstream>

typedef std::map<CGameState, CScore> MapGameToScore;
typedef MapGameToScore::const_iterator MapGameToScoreCIt;
typedef MapGameToScore::value_type MapGameToScoreVt;

static MapGameToScore gVisitedStates;
unsigned int giHits;
unsigned int giTotal;

std::string currentPath;

unsigned int iLeafsCount = 0;

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
//    if(rGame.m_currentSuit < m_currentSuit)
//	return false;

    return false;
}

std::ostream& operator<< (std::ostream& out, const CGameState & game)
{
    out << std::endl;
    out << "0: " << *game.m_aPlayers[0] << std::endl;
    out << "1: " << *game.m_aPlayers[1] << std::endl;
    out << "2: " << *game.m_aPlayers[2] << std::endl;
    out << "Active player: " << game.m_iActivePlayer;
    
    return out;
}

Card CGameState::getOptimalTurn(CScore & score)
{
    return m_aPlayers[m_iActivePlayer]->getOptimalTurn(this, score, (m_iCardsOnTableCount == 0));
}

CScore CGameState::guessTurn(Card card)
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

void CGameState::makeTurn(Card card)
{
    // Update players and table information
    m_aPlayers[m_iActivePlayer]->removeCard(card);
    m_aCardsOnTable[m_iCardsOnTableCount++] = card;
    m_iActivePlayer = (++m_iActivePlayer) % MAX_PLAYERS;
    
    // First card on the table defines current trick suit
    if(m_iCardsOnTableCount == 1)
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
        for(unsigned int i=0; i<3; i++)
            m_pCardsLeft->removeCard(m_aCardsOnTable[i]);

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

void CGameState::playGameRecursive()
{
//    std::cout << "Current state: " << *this << std::endl;

    // Cleanup and prepare for the new trick
    if(m_iCardsOnTableCount == 0)
        setUpCardsLeft();

    CCardPack possibleTurns = getActivePlayerValidTurns();
    //std::cout << "Player's valid turns are: " << possibleTurns << std::endl;

    // end of recursion, if no turns can be done
    if(possibleTurns.getCardsCount() == 0)
    {
        iLeafsCount++;
        if(iLeafsCount % 1000 == 0)
            std::cout << "\rReached end of the path " << iLeafsCount << ": " << currentPath << std::endl;
        return;
    }

    for(unsigned int i=0; i<possibleTurns.getCardsCount(); i++)
    {
        Card card = possibleTurns.getCard(i);

        //std::cout << "Current Path: " << currentPath << std::endl;
        //std::cout << "Player " << m_iActivePlayer << " plays " << card << std::endl;

        std::string currentPathBackup = currentPath;
        std::stringstream str;
        str << card;
        currentPath += str.str();

        // Make a copy of current state, and play the turn recursively
        CGameState newState(*this);
        newState.makeTurn(card);
        newState.playGameRecursive();

        currentPath = currentPathBackup;
    }
}
