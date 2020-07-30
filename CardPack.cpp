#include "CardPack.h"

#include <algorithm>

//CCardPack::CCardPack()
//{
//}

CCardPack::CCardPack(const char * cards)
{
    while(*cards != '\0')
    {
        // Skip white spaces
        while(*cards == ' ')
            cards++;

        // parse the card
        m_vCards.emplace_back(parseCard(cards));
        cards += 2;
    }

    // Sort cards, just in case
    std::sort(m_vCards.begin(), m_vCards.end());
}

//CCardPack::CCardPack(const CCardPack & rPack)
//{
//    // TODO: Make it default
//    m_vCards = rPack.m_vCards;
//}

//CCardPack::~CCardPack()
//{
//    // TODO: Make it default

//}
    
//CCardPack & CCardPack::operator=(const CCardPack & rPack)
//{
//    // TODO: Make it default
//    if(this != &rPack)
//    {
//        std::vector<Card> tmp(rPack.m_vCards);
//        std::swap(m_vCards, tmp);
//    }
    
//    return *this;
//}

std::string CCardPack::getPackStr() const
{
    std::string sRes;
    for(Card card : m_vCards)
    {
        sRes.append(std::string(" ") + getCardStr(card));
    }
    
    return sRes;
}

void CCardPack::removeCard(Card card)
{
    m_vCards.erase(std::find(m_vCards.begin(), m_vCards.end(), card));
}

bool CCardPack::areCardsEquivalent(Card left, Card right) const
{
    // Cards with different suit cannot be equivalent
    if(getSuit(left) != getSuit(right))
        return false;

    // Check that provided cards are located next to each other
    // TODO: Rewrite this with std::binary search
    for(size_t i=0; i < m_vCards.size() - 1; i++)
    {
        if(m_vCards[i] != left && m_vCards[i] != right)
            continue;
        
        if(m_vCards[i+1] == right || m_vCards[i+1] == left)
            return true;

        return false;
    }
    
    return false;
}

void CCardPack::filterOutEquivalentCards(const CCardPack & ref)
{
    // if the pack contains only 1 card (or 0) - nothing to do with filtering
    if(m_vCards.size() <= 1)
        return;

    unsigned int idx=0;
    unsigned int refIdx=0;
    unsigned int targetIdx = 0;
    do
    {
        // Get cards to compare
        Card card = m_vCards[idx++];
        Card nextCard = m_vCards[idx];

        // Search for the card in reference deck
        while(ref.m_vCards[refIdx] != card)
            refIdx++;

        // Skip equivalent cards
        if(getSuit(card) == getSuit(nextCard) &&
           card == ref.m_vCards[refIdx] &&
           nextCard == ref.m_vCards[refIdx + 1])
            continue;

        // Store non-equivalent cards
        m_vCards[targetIdx++] = card;
    }
    while(idx < m_vCards.size() - 1);

    // Fill the remaining card
    m_vCards[targetIdx] = m_vCards[idx];
    m_vCards.resize(targetIdx + 1);
}

CCardPack CCardPack::operator +(const CCardPack &rPack)
{
    // Make a copy first
    CCardPack res(*this);

    // Concatenate with other pack's array
    std::copy(rPack.m_vCards.begin(), rPack.m_vCards.end(), std::back_inserter(res.m_vCards));

    // And sort the result
    std::sort(res.m_vCards.begin(), res.m_vCards.end());

    return res;
}

CCardPack CCardPack::getSubset(CardSuit suit) const
{
    // If no suit specified - nothing to return
    if(suit == CS_UNKNOWN)
        return CCardPack();

    // Filter out everything but needed suit
    CCardPack res(*this);
    auto newEnd = std::remove_if(res.m_vCards.begin(),
                                 res.m_vCards.end(),
                                 [suit](const Card card) { return getSuit(card) != suit; });
    res.m_vCards.erase(newEnd, res.m_vCards.end());

    return res;
}
