#ifndef POKERLIB_H
#define POKERLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DECK_SIZE 52
#define MAX_CARDS 2

typedef enum {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
    MIN_SUIT = HEARTS,
    MAX_SUIT = SPADES,
    NUM_SUIT = 4
} Suit;

typedef enum {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, 
    JACK = 11, QUEEN, KING,
    MIN_RANK = ACE,
    MAX_RANK = KING,
    NUM_RANK = 13
} Rank;

typedef struct {
    Suit suit;
    Rank rank;
} PokerCard;

typedef struct {
    PokerCard hand[MAX_CARDS];
    int cardCount;
} Player;

int encode(int, int);
PokerCard decode(int);

void printCard(PokerCard card);
PokerCard createCard(Suit suit, Rank rank);
unsigned char isValidCard(PokerCard card);

void createDeck(PokerCard deck[DECK_SIZE]);
unsigned char checkInDeck(PokerCard deck[DECK_SIZE], PokerCard card);
PokerCard removeFromDeck(PokerCard deck[DECK_SIZE], PokerCard card);

Player *initPlayers(Player**, int n);
void initPlayerHand(Player* player);
void giveCard(Player* player, PokerCard card);
void showPlayerHand(Player* player);

int isGameEnd(Player* players, int n);

#endif
