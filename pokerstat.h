#ifndef POKERSTAT_H
#define POKERSTAT_H

#include "pokerlib.h"

#define SUIT_MASK 0x30
#define RANK_MASK 0x0F

typedef enum
{
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
} HandRank;

typedef struct
{
    HandRank curHand;
    float pHighCard;
    float pOnePair;
    float pTwoPair;
    float pThreeOfAKind;
    float pStraight;
    float pFlush;
    float pFullHouse;
    float pFourOfAKind;
    float pStraightFlush;
    float pWin;

} StatHolder;

unsigned char encodeByte(int suit, int rank);
PokerCard decodeByte(unsigned char encodedCard);

void setStatHolder(StatHolder *statHolder, HandRank curHand, float pHighCard, float pOnePair, float pTwoPair, float pThreeOfAKind,
                   float pStraight, float pFlush, float pFullHouse, float pFourOfAKind, float pStraightFlush, float pWin);
bool haveStraight(int *rankCount);
bool haveFlush(int *suitCount);
bool haveSameRanks(int *rankCount);
// bool haveStraight(int rankCount[]);
// bool haveFlush(int suitCount[]);
// bool haveThree(Board *board, Player *player);
// bool haveTwo(Board *board, Player *player);

void calculateHand(Board *board, Player *player);
void printStatHolder(StatHolder *statHolder);
void evaluateHands(unsigned char boardCard[], unsigned char playerCards[][2], int numPlayer, bool mapStraight[], bool mapFlush[], bool mapThree[], bool mapTwo[], bool mapTwoPair[], bool mapFour[]);

#endif