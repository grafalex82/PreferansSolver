#include "Player.h"
#include "Game.h"

#include <string>
#include <vector>

static unsigned int gCounter = 0;
static unsigned int progressByLevel[MAX_CARDS];
static unsigned int totalOnLevel[MAX_CARDS];
static unsigned int indent = 0;

extern unsigned int giHits;
extern unsigned int giTotal;

const unsigned int debugLevel = 2;


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

Card CPlayer::getOptimalTurn(CGame * pGame, CScore & retScore, bool bFirstHand)
{
    //Print the percent
    gCounter++;
    if((gCounter & 0xff) == 0)
    {
        double progress = 0.;
	
        double weight = 1;
        for(unsigned int i=0; i<indent; i++)
        {
            weight *= (double)totalOnLevel[i];
            progress += (100. * (double)progressByLevel[i]) / weight;
        }
        printf("%2.3lf%% %3.1lf%%\r", progress, (100. * (double)giHits)/(double)giTotal);
    }

    // Prepare debug indenting
    indent++;
    unsigned int i;
    std::string sIndent;
    if(indent < debugLevel)
    {
        std::cout << "Game state: " << *pGame << std::endl;
    
        for(i=0; i<indent; i++)
            sIndent += "  ";
    }

    //Retrive list of available cards
    Card availCards[MAX_CARDS];
    unsigned int availCardsCount = 0;
    Card availCardsPre[MAX_CARDS];
    unsigned int availCardsPreCount = 0;
    
    CardSuit suit = pGame->getCurrentSuit();
    CardSuit trump = pGame->getTrumpSuit();

    if(!bFirstHand)
    {    
        //Try given suit first
        if(suit != CS_UNKNOWN)
        {
            unsigned int iCardsCount = m_cardPack.getCardsCount();
            for(i=0; i<iCardsCount; i++)
            {
                if(getSuit(m_cardPack.getCard(i)) == suit)
                {
                    availCardsPre[availCardsPreCount] = m_cardPack.getCard(i);
                    availCardsPreCount++;
	            }
	        }
        }
    
        // Otherwise try trump
        if(trump != CS_UNKNOWN && availCardsPreCount == 0)
        {
            unsigned int iCardsCount = m_cardPack.getCardsCount();
            for(i=0; i<iCardsCount; i++)
	        {
	            if(getSuit(m_cardPack.getCard(i)) == trump)
	            {
		            availCardsPre[availCardsPreCount] = m_cardPack.getCard(i);
		            availCardsPreCount++;
	            }
	        }
        }
    }
    
    // If no trumps and given suit cards - try any cards
    if(availCardsPreCount == 0)
    {
        unsigned int iCardsCount = m_cardPack.getCardsCount();
        for(i=0; i<iCardsCount; i++)
        {
            availCardsPre[availCardsPreCount] = m_cardPack.getCard(i);
            availCardsPreCount++;
        }
    }
    
    if(availCardsPreCount == 0)
        throw "CPlayer::getOptimalTurn(): no available cards for turn";
    
    // Leave only those cards, that is not considered as equivalent
    Card lastCard;
    CCardPack & cardsLeft = pGame->getCardsLeft();
    for(i=0; i<availCardsPreCount; i++)
    {
        if(i == 0)
        {
            availCards[availCardsCount] = availCardsPre[i];
            availCardsCount++;
            lastCard = availCardsPre[i];
            continue;
        }
        
        if(cardsLeft.areCardsEquivalent(lastCard, availCardsPre[i]))
        {
            lastCard = availCardsPre[i];
            continue;
        }

        availCards[availCardsCount] = availCardsPre[i];
        availCardsCount++;
        lastCard = availCardsPre[i];
    }
    
    totalOnLevel[indent - 1] = availCardsCount;
    
    if(indent < debugLevel)
    {
        std::cout << sIndent << "Available cards for turn: ";
        for(i=0; i<availCardsCount; i++)
            std::cout << availCards[i] << " ";
        std::cout << std::endl;
    }
    
    //For each available card - guess the game score
    std::vector<CScore> vScores;
    CScore winScore;
    Card winCard;
    for(i=0; i<availCardsCount; i++)
    {
        if(indent < debugLevel)
            std::cout << sIndent << "Guessing card " << availCards[i] << std::endl;
	    
        progressByLevel[indent - 1] = i;
	    
        CGame game(*pGame);
        CScore score = game.guessTurn(availCards[i]);
        vScores.push_back(score);
        if(i == 0)
        {
            winCard = availCards[i];
            winScore = score;
        }
        else
        {
            if(winScore.isScoreHeigher(score, m_eStrategy))
            {
                winCard = availCards[i];
                winScore = score;
            }
        }
    }
    
    if(indent < debugLevel)
    {
        std::cout << sIndent << "Card Scores:" << std::endl;
        for(i=0; i<availCardsCount; i++)
            std::cout << sIndent << availCards[i] << " - " << vScores[i] << std::endl;

        std::cout << sIndent << "Optimal Card: " << winCard << " " << winScore << std::endl;
    }

    indent--;
    
    retScore = winScore;
    return winCard;
}

CCardPack CPlayer::getListOfValidTurns(CardSuit suit, CardSuit trump)
{
    // Search for requested suit first
    if(m_cardPack.hasSuit(suit))
        return m_cardPack.getSubset(suit);

    // Then search for trumps
    if(m_cardPack.hasSuit(trump))
        return m_cardPack.getSubset(trump);

    // Otherwise all the remaining cards are valid
    return m_cardPack;
}

