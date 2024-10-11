#ifndef POKERLIB_H
#define POKERLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DECK_SIZE 52
#define MAX_CARDS 2
#define BOARD_SIZE 5

typedef enum
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
    MIN_SUIT = HEARTS,
    MAX_SUIT = SPADES,
    NUM_SUIT = 4
} Suit;

typedef enum
{
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK = 11,
    QUEEN,
    KING,
    MIN_RANK = ACE,
    MAX_RANK = KING,
    NUM_RANK = 13
} Rank;

typedef struct
{
    Suit suit;
    Rank rank;
} PokerCard;

typedef struct
{
    PokerCard cards[DECK_SIZE];
    int cardCount;
} Deck;

typedef struct
{
    PokerCard cards[BOARD_SIZE];
    int cardCount;
} Board;

typedef struct
{
    PokerCard hand[MAX_CARDS];
    int cardCount;
} Player;

int encode(int, int);
PokerCard decode(int);
int randSuit();
int randRank();
PokerCard randCard();

void printCard(PokerCard card);
PokerCard createCard(Suit suit, Rank rank);
bool isValidCard(PokerCard card);

void initDeck(Deck *deck);
void initBoard(Board *board);
bool checkInDeck(Deck *deck, PokerCard card);
void giveBoardCard(Board *board, Deck *deck, PokerCard card);
PokerCard randInDeck(Deck *deck);
PokerCard removeFromDeck(Deck *deck, PokerCard card);

Player *initPlayers(Player **, int n);
void initPlayerHand(Player *player);
void givePlayerCard(Player *player, Deck *deck, PokerCard card);
void showPlayerHand(Player *player);

int isGameEnd(Player *players, int n);

#endif
