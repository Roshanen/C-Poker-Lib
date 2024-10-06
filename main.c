#include <stdio.h>
#include <time.h>
#include "pokerlib.h"

int main(void)
{
    srand(time(NULL));
    PokerCard deck[DECK_SIZE];
    Player *players = NULL;
    int nPlayers = 0;

    createDeck(deck);

    while (1)
    {
        printf("Enter number of player(s): ");
        scanf("%d", &nPlayers);
        if (nPlayers < 1)
        {
            break;
        }
        players = initPlayers(&players, nPlayers);

        for (int i = 0; i < nPlayers; i++)
        {
            for (int j = 0; j < MAX_CARDS; j++)
            {
                PokerCard card = createCard(SPADES, (Rank)(rand() % 13 + 1));
                giveCard(&players[i], card);
            }
        }

        for (int i = 0; i < nPlayers; i++)
        {
            printf("Player %d's hand:\n", i + 1);
            showPlayerHand(&players[i]);
        }
    }
    printf("\nEnd");

    free(players);
    return 0;
}