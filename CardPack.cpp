#include "CardPack.h"

#include <algorithm>

CCardPack::CCardPack()
{
    m_iCardsCount = 0;
}

CCardPack::CCardPack(const char * cards)
{
    m_iCardsCount = 0;
    while(*cards != '\0')
    {
        // Skip white spaces
        while(*cards == ' ')
            cards++;

        // parse the card
        m_aCards[m_iCardsCount++] = parseCard(cards);
        cards += 2;
    }

    // Sort cards, just in case
    std::sort(m_aCards, m_aCards + m_iCardsCount * sizeof(Card));
}

CCardPack::CCardPack(const CCardPack & rPack)
{
    m_iCardsCount = rPack.m_iCardsCount;
    memcpy(m_aCards, rPack.m_aCards, m_iCardsCount * sizeof(Card));
}

CCardPack::~CCardPack()
{
}
    
CCardPack & CCardPack::operator=(const CCardPack & rPack)
{
    if(this != &rPack)
    {
        m_iCardsCount = rPack.m_iCardsCount;
        memcpy(m_aCards, rPack.m_aCards, m_iCardsCount * sizeof(Card));
    }	
    
    return *this;
}

std::string CCardPack::getPackStr() const
{
    std::string sRes;

    for(unsigned int i=0; i<m_iCardsCount; i++)
    {
        char temp[4] = "   ";
    
        temp[1] = getCardValueSymb(getCardValue(m_aCards[i]));
        temp[2] = getSuitSymb(getSuit(m_aCards[i]));
	
        sRes.append(std::string(temp));
    }
    
    return sRes;
}

#if 0
void CCardPack::shuffle()
{
    std::vector<Card> vTemp;
    for(unsigned int i=0; i<m_iCardsCount; i++)
	    vTemp.push_back(m_aCards[i]);
    
    m_iCardsCount = 0;
    
    while(vTemp.size() != 0)
    {
        int idx = rand() % vTemp.size();
        m_aCards[m_iCardsCount++] = vTemp[idx];
        vTemp.erase(vTemp.begin() + idx);
    }
}
#endif //0

void CCardPack::removeCard(Card card)
{
    for(unsigned int i=0; i<m_iCardsCount; i++)
    {
    	if(m_aCards[i] == card)
	    {
	        for(; i < m_iCardsCount - 1; i++)
		        m_aCards[i] = m_aCards[i+1];
	    
	        m_iCardsCount--;
	        break;
	    }
    }
}

bool CCardPack::areCardsEquivalent(Card left, Card right) const
{
    // Cards with different suit cannot be equivalent
    if(getSuit(left) != getSuit(right))
        return false;

    // Check that provided cards are located next to each other
    for(unsigned int i=0; i < m_iCardsCount - 1; i++)
    {
        if(m_aCards[i] != left && m_aCards[i] != right)
            continue;
        
        if(m_aCards[i+1] == right || m_aCards[i+1] == left)
            return true;

        return false;
    }
    
    return false;
}

void CCardPack::filterOutEquivalentCards(const CCardPack & ref)
{
    // if the pack contains only 1 card (or 0) - nothing to do with filtering
    if(m_iCardsCount <= 1)
        return;

    unsigned int idx=0;
    unsigned int refIdx=0;
    unsigned int targetIdx = 0;
    do
    {
        // Get cards to compare
        Card card = m_aCards[idx++];
        Card nextCard = m_aCards[idx];

        // Search for the card in reference deck
        while(ref.m_aCards[refIdx] != card)
            refIdx++;

        // Skip equivalent cards
        if(getSuit(card) == getSuit(nextCard) &&
           card == ref.m_aCards[refIdx] &&
           nextCard == ref.m_aCards[refIdx + 1])
            continue;

        // Store non-equivalent cards
        m_aCards[targetIdx++] = card;
    }
    while(idx < m_iCardsCount - 1);

    // Fill the remaining card
    m_aCards[targetIdx] = m_aCards[idx];
    m_iCardsCount = targetIdx + 1;
}

CCardPack CCardPack::operator +(const CCardPack &rPack)
{
    if(m_iCardsCount + rPack.m_iCardsCount > MAX_CARDS)
        throw "CCardPack::operator+(): Too many cards to combine";

    // Make a copy
    CCardPack res(*this);

    // Add another array
    std::copy(rPack.m_aCards, rPack.m_aCards + rPack.m_iCardsCount, res.m_aCards + res.m_iCardsCount);
    res.m_iCardsCount += rPack.m_iCardsCount;

    // Finally sort the result
    std::sort(res.m_aCards, res.m_aCards + res.m_iCardsCount);

    return res;
}

CCardPack CCardPack::getSubset(CardSuit suit) const
{
    CCardPack ret;

    // If no suit specified - nothing to return
    if(suit == CS_UNKNOWN)
        return ret;

    // Copy cards that match given suit
    unsigned int j = 0;
    for(unsigned int i=0; i<m_iCardsCount; i++)
    {
        if(getSuit(m_aCards[i]) == suit)
        {
            ret.m_aCards[j] = m_aCards[i];
            j++;
        }
    }

    ret.m_iCardsCount = j;
    return ret;
}
