#include <stdio.h>
#include <time.h>
#include "pokerlib.h"
#include "pokerstat.h"
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

        printf("##### Enter number of players (0 to exit): ");
        scanf("%d", &nPlayers);
        if (nPlayers == 0){
            break;
        }
        if (!isInRange(nPlayers, 2, 8))
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
            // showPlayerHand(&players[i], i);
        }

        for (int i = 0; i < BOARD_SIZE; i++){
            PokerCard card = randInDeck(&deck);
            giveBoardCard(&board, &deck, card);
            // printCard(card);
            // eval at flop,turn,river
            if (i > 1){
                for (int j = 0; j < nPlayers; j++)
                {
                    showPlayerHand(&players[j], j);
                }
                printBoardCard(&board);
                printHandEvaluation(players, nPlayers, &board);
            }
            //prredict at flop,turn
            if (i == 2 || i == 3){
                printPrediction(players, nPlayers, &board, &deck);
            }
        }
        displayWinner(players, nPlayers, &board);
    }
    printf("\033[33mEnd\033[37m\n");

    free(players);
    return 0;
}