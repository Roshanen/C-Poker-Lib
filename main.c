#include <stdio.h>
#include <time.h>
#include "pokerlib.h"

int main(void){
    srand(time(NULL));
    PokerCard deck[DECK_SIZE];

    createDeck(deck);
    // for (int i = 0; i < DECK_SIZE; i++) {
    //     printCard(deck[i]);
    // }

    //Try init players
    int nPlayers = 3;
    Player* players = initPlayers(nPlayers);

    for (int i = 0; i < nPlayers; i++) {
        for (int j = 0; j < MAX_CARDS; j++) {
            PokerCard card = createCard(SPADES, (Rank)(rand() % 13 + 1));
            giveCard(&players[i], card);
        }
    }

    for (int i = 0; i < nPlayers; i++) {
        printf("Player %d's hand:\n", i + 1);
        showPlayerHand(&players[i]);
    }

    //Try reset with n players
    nPlayers = 5;
    resetPlayers(&players, nPlayers);

    for (int i = 0; i < nPlayers; i++) {
        for (int j = 0; j < MAX_CARDS; j++) {
            PokerCard card = createCard(SPADES, (Rank)(rand() % 13 + 1));
            giveCard(&players[i], card);
        }
    }

    for (int i = 0; i < nPlayers; i++) {
        printf("Player %d's hand:\n", i + 1);
        showPlayerHand(&players[i]);
    }
    
    free(players);
    return 0;
}