#ifndef POKERLIB_H
#define POKERLIB_H

#include <stdio.h>
#include <stdlib.h>

#define DECK_SIZE 52
#define MAX_CARDS 2

typedef enum {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
} Suit;

typedef enum {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, 
    JACK = 11, QUEEN, KING
} Rank;

typedef struct {
    Suit suit;
    Rank rank;
} PokerCard;

typedef struct {
    PokerCard hand[MAX_CARDS];
    int cardCount;
} Player;

void printCard(PokerCard card);
PokerCard createCard(Suit suit, Rank rank);

void createDeck(PokerCard deck[DECK_SIZE]);
int checkInDeck(PokerCard deck[DECK_SIZE], PokerCard card);
PokerCard removeFromDeck(PokerCard deck[DECK_SIZE], PokerCard card);

Player* initPlayers(int n);
void initPlayerHand(Player* player);
void giveCard(Player* player, PokerCard card);
void showPlayerHand(Player* player);
void resetPlayers(Player** oldPlayers, int newNPlayers);

int isGameEnd(Player* players, int n);

#endif
