#ifndef POKERSTAT_H
#define POKERSTAT_H

#include "pokerlib.h"

typedef enum {
    HIGH_CARD = 0,
    ONE_PAIR = 1,
    TWO_PAIR = 2,
    THREE_OF_A_KIND = 3,
    STRAIGHT = 4,
    FLUSH = 5,
    FULL_HOUSE = 6,
    FOUR_OF_A_KIND = 7,
    STRAIGHT_FLUSH = 8,
    ROYAL_FLUSH = 9
} PokerHandRank;

typedef struct {
    double highCard;
    double onePair;
    double twoPair;
    double threeOfAKind;
    double straight;
    double flush;
    double fullHouse;
    double fourOfAKind;
    double straightFlush;
    double royalFlush;
    double winRate;
} PokerProbabilities;

typedef struct {
    int rank;            // The rank of the hand (e.g., 1 = High Card, 2 = One Pair, etc.)
    int highCards[5];    // The high cards or kickers for tiebreakers (sorted in descending order)
} PokerHand;

void printHandEvaluation(Player *players, int nPlayers, Board *board);
void printPrediction(Player *players, int nPlayers, Board *board, Deck *deck);
void displayWinner(Player *players, int nPlayers, Board *board);

#endif
