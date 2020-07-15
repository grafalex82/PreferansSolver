#include "CardPack.h"

#include <stdlib.h>

CCardPack::CCardPack(const Card * pCards, unsigned int iCount)
{
    memcpy(m_aCards, pCards, iCount * sizeof(Card));
    for(unsigned int i=0; i<iCount; i++)
    {
        for(unsigned int j=i+1; j<iCount; j++)
            if(m_aCards[i] > m_aCards[j])
            {
                Card temp = m_aCards[j];
                m_aCards[j] = m_aCards[i];
                m_aCards[i] = temp;
            }
    }
    m_iCardsCount = iCount;
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

void CCardPack::shuffle()
{
    std::vector<Card> vTemp;
    for(unsigned int i=0; i<m_iCardsCount; i++)
	    vTemp.push_back(m_aCards[i]);
    
    m_iCardsCount = 0;
    
    while(vTemp.size() != 0)
    {
        int idx = random() % vTemp.size();
        m_aCards[m_iCardsCount++] = vTemp[idx];
        vTemp.erase(vTemp.begin() + idx);
    }
}

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

bool CCardPack::compareCards(Card left, Card right) const
{
    if(getSuit(left) != getSuit(right))
        return false;

    for(unsigned int i=0; i < m_iCardsCount - 1; i++)
    {
        if(m_aCards[i] != left)
            continue;
        
        if(m_aCards[i+1] == right)
            return true;
        
        return false;
    }
    
    return false;
}

CCardPack CCardPack::operator +(const CCardPack &rPack)
{
    if(m_iCardsCount + rPack.m_iCardsCount > MAX_CARDS)
        throw "CCardPack::operator+(): Too many cards to combine";

    Card temp[MAX_CARDS];
    
    unsigned int i = 0;
    unsigned int j = 0; 
    unsigned int idx = 0;
    while(i<m_iCardsCount || j<rPack.m_iCardsCount)
    {
        if(i < m_iCardsCount)
        {
            if(j < rPack.m_iCardsCount)
            {
                if(m_aCards[i] < rPack.m_aCards[j])
                    temp[idx++] = m_aCards[i++];
                else
                if(rPack.m_aCards[j] < m_aCards[i])
                    temp[idx++] = rPack.m_aCards[j++];
                else
                    throw "CCardPack::operator+(): one card is found in both packs";
            }
            else
            {
                temp[idx++] = m_aCards[i++];
            }
        }
        else
        {
            temp[idx++] = rPack.m_aCards[j++];
        }
    }
    
    return CCardPack(temp, idx);
}

