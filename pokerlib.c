#include "pokerlib.h"

void printCard(PokerCard card) {
    const char* suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
    const char* ranks[] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };

    // ANSI \033[33m = yellow
    // ANSI \033[37m = white
    printf("Card: \033[33m%6s \033[37mof \033[33m%s \033[37m\n", ranks[card.rank - 1], suits[card.suit]);
}

PokerCard createCard(Suit suit, Rank rank) {
    PokerCard card;
    card.suit = suit;
    card.rank = rank;
    return card;
}

void createDeck(PokerCard deck[DECK_SIZE]) {
    int index = 0;
    
    for (Suit suit = HEARTS; suit <= SPADES; suit++) {
        for (Rank rank = ACE; rank <= KING; rank++) {
            deck[index++] = createCard(suit, rank);
        }
    }
}

int checkInDeck(PokerCard deck[DECK_SIZE], PokerCard card) {
    for (int i = 0; i < DECK_SIZE; i++) {
        if (deck[i].rank == card.rank && deck[i].suit == card.suit) {
            return 1;
        }
    }
    return 0;
}

PokerCard removeFromDeck(PokerCard deck[DECK_SIZE], PokerCard card) {
    PokerCard removedCard = createCard(-1, -1);
    
    for (int i = 0; i < DECK_SIZE; i++) {
        if (deck[i].rank == card.rank && deck[i].suit == card.suit) {
            removedCard = deck[i];
            deck[i] = createCard(-1, -1);
            break;
        }
    }
    
    return removedCard;
}


Player* initPlayers(int n) {
    Player* players = (Player*)malloc(n * sizeof(Player));
    for (int i = 0; i < n; i++) {
        initPlayerHand(&players[i]);
    }
    return players;
}

void initPlayerHand(Player* player) {
    player->cardCount = 0;
    for (int i = 0; i < MAX_CARDS; i++) {
        player->hand[i] = createCard(-1, -1);
    }
}

void giveCard(Player* player, PokerCard card) {
    if (player->cardCount < MAX_CARDS) {
        player->hand[player->cardCount++] = card;
    }
}

void showPlayerHand(Player* player) {
    for (int i = 0; i < player->cardCount; i++) {
        printCard(player->hand[i]);
    }
}

int isGameEnd(Player* players, int n) {
    for (int i = 0; i < n; i++) {
        if (players[i].cardCount < MAX_CARDS) {
            return 0;
        }
    }
    return 1;
}

void resetPlayers(Player** oldPlayers, int newNPlayers) {
    free(*oldPlayers);
    
    *oldPlayers = (Player*)malloc(newNPlayers * sizeof(Player));
    
    for (int i = 0; i < newNPlayers; i++) {
        initPlayerHand(&(*oldPlayers)[i]);
    }

    printf("Players reset with %d players.\n", newNPlayers);
}
