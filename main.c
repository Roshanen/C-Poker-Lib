#include <stdio.h>
#include <time.h>
#include "pokerlib.h"
#include "utils.h"

int main(void)
{
    srand(time(NULL));
    Deck deck;
    Board board;
    Player *players = NULL;
    int nPlayers = 0;

    while (1)
    {
        initDeck(&deck);
        initBoard(&board);

        printf("Enter number of players (0 to exit): ");
        scanf("%d", &nPlayers);
        if (nPlayers == 0){
            break;
        }
        if (!isInRange(nPlayers, 1, 8))
        {
            printf("\033[31mInvalid number of players\033[37m\n");
            continue;
        }
        players = initPlayers(&players, nPlayers);

        for (int i = 0; i < nPlayers; i++)
        {
            for (int j = 0; j < MAX_CARDS; j++)
            {
                PokerCard card = randInDeck(&deck);
                givePlayerCard(&players[i], &deck, card);
            }
        }

        for (int i = 0; i < BOARD_SIZE; i++){
            PokerCard card = randInDeck(&deck);
            giveBoardCard(&board, &deck, card);
            printCard(card);
        }

        for (int i = 0; i < nPlayers; i++)
        {
            printf("Player %d's hand:\n", i + 1);
            showPlayerHand(&players[i]);
        }
    }
    printf("\033[33mEnd\033[37m\n");

    free(players);
    return 0;
}