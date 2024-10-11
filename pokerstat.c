#include "pokerstat.h"

// Function to encode a card (rank and suit) into a byte 0x3D to 0x01
unsigned char encodeByte(int suit, int rank)
{
    return (suit << 4) | (rank & 0x0F);
}

// Function to decode a byte into rank and suit
PokerCard decodeByte(unsigned char encodedCard)
{
    PokerCard card = {(encodedCard >> 4) & 0x03, encodedCard & 0x0F};
    return card;
}

void setStatHolder(StatHolder *statHolder, HandRank curHand, float pHighCard, float pOnePair, float pTwoPair, float pThreeOfAKind,
                   float pStraight, float pFlush, float pFullHouse, float pFourOfAKind, float pStraightFlush, float pWin)
{
    statHolder->curHand = curHand;
    statHolder->pOnePair = pOnePair;
    statHolder->pTwoPair = pTwoPair;
    statHolder->pThreeOfAKind = pThreeOfAKind;
    statHolder->pStraight = pStraight;
    statHolder->pFlush = pFlush;
    statHolder->pFullHouse = pFullHouse;
    statHolder->pFourOfAKind = pFourOfAKind;
    statHolder->pStraightFlush = pStraightFlush;
    statHolder->pWin = pWin;
}

// have atleast 5 in order
bool haveStraight(int *rankCount)
{

    return false;
}

// have atleast 5 of a suit
bool haveFlush(int *suitCount)
{

    return false;
}

// have pair, three, four, twoPairs
bool haveSameRanks(int *rankCount)
{

    return false;
}

// bool haveFour(Board *board, Player *player, int totalCard)
// {
//     return false;
// }

// bool haveThree(Board *board, Player *player, int totalCard)
// {
//     return false;
// }

// bool haveTwoPair(Board *board, Player *player, int totalCard)
// {
//     return false;
// }

// bool havePair(Board *board, Player *player, int totalCard)
// {
//     return false;
// }

void calculateHand(Board *board, Player *player)
{
}

void printStatHolder(StatHolder *statHolder)
{
    printf("curHand: %5.2f\n", statHolder->curHand);
    printf("pOnePair: %5.2f\n", statHolder->pOnePair);
    printf("pTwoPair: %5.2f\n", statHolder->pTwoPair);
    printf("pThreeOfAKind: %5.2f\n", statHolder->pThreeOfAKind);
    printf("pStraight: %5.2f\n", statHolder->pStraight);
    printf("pFlush: %5.2f\n", statHolder->pFlush);
    printf("pFullHouse: %5.2f\n", statHolder->pFullHouse);
    printf("pFourOfAKind: %5.2f\n", statHolder->pFourOfAKind);
    printf("pStraightFlush: %5.2f\n", statHolder->pStraightFlush);
    printf("pWin: %5.2f\n", statHolder->pWin);
}

void evaluateHands(unsigned char boardCard[], unsigned char playerCards[][2], int numPlayer, bool mapStraight[], bool mapFlush[], bool mapThree[],
                   bool mapTwo[], bool mapTwoPair[], bool mapFour[])
{
    int NUM_CARDS = 5;
    for (int p = 0; p < numPlayer; ++p)
    {
        int rankCount[14] = {0};                    // to count occurrences of each rank
        int suitCount[4] = {0};                     // to count occurrences of each suit
        unsigned char combinedCards[NUM_CARDS + 2]; // board + player cards

        // Combine board and player cards
        for (int i = 0; i < NUM_CARDS; ++i)
        {
            combinedCards[i] = boardCard[i];
        }
        combinedCards[NUM_CARDS] = playerCards[p][0];
        combinedCards[NUM_CARDS + 1] = playerCards[p][1];

        // Count rank and suit occurrences
        for (int i = 0; i < NUM_CARDS + 2; ++i)
        {
            int suit = (combinedCards[i] & SUIT_MASK) >> 4;
            int rank = combinedCards[i] & RANK_MASK;
            rankCount[rank]++;
            suitCount[suit]++;
        }

        // Check for flush
        mapFlush[p] = false;
        for (int i = 0; i < 4; ++i)
        {
            if (suitCount[i] >= 5)
            {
                mapFlush[p] = true;
                break;
            }
        }

        // Check for straight
        mapStraight[p] = false;
        int consecutive = 0;
        for (int i = 0; i < 14; ++i)
        {
            if (rankCount[i] > 0)
            {
                consecutive++;
                if (consecutive == 5)
                {
                    mapStraight[p] = true;
                    break;
                }
            }
            else
            {
                consecutive = 0;
            }
        }
        // Special case: A-K-Q-J-10 royal
        if (!mapStraight[p] && rankCount[0] > 0 && rankCount[12] > 0 && rankCount[11] > 0 && rankCount[10] > 0 && rankCount[9] > 0)
        {
            mapStraight[p] = true;
        }

        // Check for pairs, three of a kind, two pairs
        int pairs = 0, threeOfKind = 0, fourOfKind = 0;
        for (int i = 0; i < 13; ++i)
        {
            if (rankCount[i] == 2)
            {
                pairs++;
            }
            else if (rankCount[i] == 3)
            {
                threeOfKind++;
            }
            else if (rankCount[i] == 4)
            {
                fourOfKind++;
            }
        }

        mapThree[p] = (threeOfKind > 0);
        mapTwo[p] = (pairs == 1);
        mapTwoPair[p] = (pairs == 2);
        mapFour[p] = (fourOfKind > 0);
    }
}
