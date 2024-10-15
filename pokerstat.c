#include "pokerstat.h"
#include "pokerlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TOTAL_CARDS_IN_DECK 52

#define MAX_RANK 14  // Ace is the highest rank

#define HIGH_CARD 1
#define ONE_PAIR 2
#define TWO_PAIR 3
#define THREE_OF_A_KIND 4
#define STRAIGHT 5
#define FLUSH 6
#define FULL_HOUSE 7
#define FOUR_OF_A_KIND 8
#define STRAIGHT_FLUSH 9

const char* formatProbability(double probability);

// Helper function to sort cards for hand evaluation
void sortCards(PokerCard *cards, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (cards[i].rank > cards[j].rank) {
                PokerCard temp = cards[i];
                cards[i] = cards[j];
                cards[j] = temp;
            }
        }
    }
}

// Function to evaluate the player's hand
int evaluateHand(PokerCard *playerCards, int playerCardCount, PokerCard *boardCards, int boardCardCount) {
    PokerCard allCards[7]; // 2 player cards + 5 board cards
    for (int i = 0; i < playerCardCount; i++) {
        allCards[i] = playerCards[i];
    }
    for (int i = 0; i < boardCardCount; i++) {
        allCards[playerCardCount + i] = boardCards[i];
    }

    // Sort all cards for easier evaluation
    sortCards(allCards, playerCardCount + boardCardCount);

    // Variables to count ranks and suits
    int rankCount[15] = {0}; // Assuming ranks go from 2 (0) to Ace (13)
    int suitCount[4] = {0}; // Assuming 4 suits
    bool hasStraight = false;

    // Count ranks and suits
    for (int i = 0; i < playerCardCount + boardCardCount; i++) {
        rankCount[allCards[i].rank]++;
        suitCount[allCards[i].suit]++;
    }

    // Check for flush
    bool isFlush = false;
    for (int i = 0; i < 4; i++) {
        if (suitCount[i] >= 5) {
            isFlush = true;
            break;
        }
    }

    // Check for straight
    for (int i = 2; i <= 10; i++) {
        if (rankCount[i] > 0 && rankCount[i + 1] > 0 && rankCount[i + 2] > 0 &&
            rankCount[i + 3] > 0 && rankCount[i + 4] > 0) {
            hasStraight = true;
            break;
        }
    }
    // Special case for Ace-low straight (A-2-3-4-5)
    if (rankCount[2] > 0 && rankCount[3] > 0 && rankCount[4] > 0 &&
        rankCount[5] > 0 && rankCount[14] > 0) { // Ace is rank 14
        hasStraight = true;
    }

    // Variables to check for specific hand types
    bool hasThreeOfAKind = false;
    bool hasFourOfAKind = false;
    int pairs = 0;

    for (int i = 2; i < 15; i++) {
        if (rankCount[i] == 4) {
            hasFourOfAKind = true;
        }
        if (rankCount[i] == 3) {
            hasThreeOfAKind = true;
        }
        if (rankCount[i] == 2) {
            pairs++;
        }
    }

    // Evaluate hand ranking
    if (isFlush && hasStraight) return STRAIGHT_FLUSH; // Straight Flush
    if (hasFourOfAKind) return FOUR_OF_A_KIND; // Four of a Kind
    if (hasThreeOfAKind && pairs == 1) return FULL_HOUSE; // Full House
    if (isFlush) return FLUSH; // Flush
    if (hasStraight) return STRAIGHT; // Straight
    if (hasThreeOfAKind) return THREE_OF_A_KIND; // Three of a Kind
    if (pairs == 2) return TWO_PAIR; // Two Pair
    if (pairs == 1) return ONE_PAIR; // One Pair
    return HIGH_CARD; // High Card
}


// Function to calculate the score for each hand rank
int getHandScore(int handRank) {
    switch (handRank) {
        case 1: return 1;   // High card
        case 2: return 2;   // One pair
        case 3: return 4;   // Two pair
        case 4: return 6;   // Three of a kind
        case 5: return 8;   // Straight
        case 6: return 10;  // Flush
        case 7: return 12;  // Full house
        case 8: return 14;  // Four of a kind
        case 9: return 16;  // Straight flush
        case 10: return 20; // Royal flush
        default: return 0;  // Unknown hand
    }
}

PokerProbabilities predictPlayerHand(Player *player, Board *board, Deck *deck, Player *allPlayers, int nPlayers) {
    PokerProbabilities probabilities = {0};
    int remainingCards = 5 - board->cardCount;
    int totalSimulations = 10000;
    int playerWins = 0;

    for (int sim = 0; sim < totalSimulations; sim++) {
        Deck simDeck = *deck;
        Board simBoard = *board;

        for (int k = 0; k < remainingCards; k++) {
            PokerCard newCard = randInDeck(&simDeck);
            giveBoardCard(&simBoard, &simDeck, newCard);
        }

        int bestPlayerIndex = -1;
        int bestHandRank = 0;

        for (int i = 0; i < nPlayers; i++) {
            int handRank = evaluateHand(allPlayers[i].hand, MAX_CARDS, simBoard.cards, simBoard.cardCount);
            if (handRank > bestHandRank) {
                bestHandRank = handRank;
                bestPlayerIndex = i;
            }
        }

        int playerHandRank = evaluateHand(player->hand, MAX_CARDS, simBoard.cards, simBoard.cardCount);
        if (playerHandRank >= bestHandRank) {
            playerWins++;
        }

        if (playerHandRank == 1) probabilities.highCard += 1.0;
        else if (playerHandRank == 2) probabilities.onePair += 1.0;
        else if (playerHandRank == 3) probabilities.twoPair += 1.0;
        else if (playerHandRank == 4) probabilities.threeOfAKind += 1.0;
        else if (playerHandRank == 5) probabilities.straight += 1.0;
        else if (playerHandRank == 6) probabilities.flush += 1.0;
        else if (playerHandRank == 7) probabilities.fullHouse += 1.0;
        else if (playerHandRank == 8) probabilities.fourOfAKind += 1.0;
        else if (playerHandRank == 9) probabilities.straightFlush += 1.0;
        else if (playerHandRank == 10) probabilities.royalFlush += 1.0;
    }

    probabilities.highCard = (probabilities.highCard / totalSimulations) * 100.0;
    probabilities.onePair = (probabilities.onePair / totalSimulations) * 100.0;
    probabilities.twoPair = (probabilities.twoPair / totalSimulations) * 100.0;
    probabilities.threeOfAKind = (probabilities.threeOfAKind / totalSimulations) * 100.0;
    probabilities.straight = (probabilities.straight / totalSimulations) * 100.0;
    probabilities.flush = (probabilities.flush / totalSimulations) * 100.0;
    probabilities.fullHouse = (probabilities.fullHouse / totalSimulations) * 100.0;
    probabilities.fourOfAKind = (probabilities.fourOfAKind / totalSimulations) * 100.0;
    probabilities.straightFlush = (probabilities.straightFlush / totalSimulations) * 100.0;
    probabilities.royalFlush = (probabilities.royalFlush / totalSimulations) * 100.0;

    probabilities.winRate = ((double)playerWins / totalSimulations) * 100.0;

    return probabilities;
}




// Function to print hand evaluation results
void printHandEvaluation(Player *players, int nPlayers, Board *board) {
    printf("--- Hand Evaluation after %d cards on board ---\n", board->cardCount);

    for (int i = 0; i < nPlayers; i++) {
        int handRank = evaluateHand(players[i].hand, MAX_CARDS, board->cards, board->cardCount);
        
        printf("Player %d: ", i + 1);
        switch (handRank) {
            case 1: printf("high card\n"); break;
            case 2: printf("one pair\n"); break;
            case 3: printf("two pair\n"); break;
            case 4: printf("three of a kind\n"); break;
            case 5: printf("straight\n"); break;
            case 6: printf("flush\n"); break;
            case 7: printf("full house\n"); break;
            case 8: printf("four of a kind\n"); break;
            case 9: printf("straight flush\n"); break;
            case 10: printf("royal flush\n"); break;
            default: printf("Unknown hand\n"); break;
        }
    }
}


void printPrediction(Player *players, int nPlayers, Board *board, Deck *deck) {
    printf("\n--- Prediction for remaining cards ---\n");

    double totalUnnormalizedWinRate = 0.0;

    PokerProbabilities playerProbabilities[nPlayers];
    for (int i = 0; i < nPlayers; i++) {
        int currentHandRank = evaluateHand(players[i].hand, MAX_CARDS, board->cards, board->cardCount);
        playerProbabilities[i] = predictPlayerHand(&players[i], board, deck, players, nPlayers);

        totalUnnormalizedWinRate += playerProbabilities[i].winRate;
    }

    for (int i = 0; i < nPlayers; i++) {
        playerProbabilities[i].winRate = (playerProbabilities[i].winRate / totalUnnormalizedWinRate) * 100.0;

        printf("Player %d: \n", i + 1);
        
        int currentHandRank = evaluateHand(players[i].hand, MAX_CARDS, board->cards, board->cardCount);
        printf("p high card to %s\n", (currentHandRank >= 1) ? "-" : formatProbability(playerProbabilities[i].highCard));
        printf("p one pair to %s\n", (currentHandRank >= 2) ? "-" : formatProbability(playerProbabilities[i].onePair));
        printf("p two pair to %s\n", (currentHandRank >= 3) ? "-" : formatProbability(playerProbabilities[i].twoPair));
        printf("p three of a kind to %s\n", (currentHandRank >= 4) ? "-" : formatProbability(playerProbabilities[i].threeOfAKind));
        printf("p straight to %s\n", (currentHandRank >= 5) ? "-" : formatProbability(playerProbabilities[i].straight));
        printf("p flush to %s\n", (currentHandRank >= 6) ? "-" : formatProbability(playerProbabilities[i].flush));
        printf("p full house to %s\n", (currentHandRank >= 7) ? "-" : formatProbability(playerProbabilities[i].fullHouse));
        printf("p four of a kind to %s\n", (currentHandRank >= 8) ? "-" : formatProbability(playerProbabilities[i].fourOfAKind));
        printf("p straight flush to %s\n", (currentHandRank >= 9) ? "-" : formatProbability(playerProbabilities[i].straightFlush));
        printf("p royal flush to %s\n", (currentHandRank >= 10) ? "-" : formatProbability(playerProbabilities[i].royalFlush));

        printf("p winrate %.2f%%\n", playerProbabilities[i].winRate);
    }
}

// Helper function to format probability to string
const char* formatProbability(double probability) {
    static char buffer[16];
    snprintf(buffer, sizeof(buffer), "%.2f%%", probability);
    return buffer;
}
PokerHand evaluateBestHand(PokerCard *cards, int totalCards) {
    PokerHand bestHand;
    bestHand.rank = HIGH_CARD;
    memset(bestHand.highCards, -1, sizeof(bestHand.highCards));

    int rankCount[13] = {0};
    int suitCount[4] = {0};
    int ranks[5] = {-1};        // For storing best rank cards (high cards)
    int flushSuit = -1;         // To store the suit of a flush (if any)
    bool isStraight = false;
    bool isFlush = false;
    
    // Count ranks and suits
    for (int i = 0; i < totalCards; i++) {
        rankCount[cards[i].rank]++;
        suitCount[cards[i].suit]++;
    }

    // Check for flush (5 cards of the same suit)
    for (int i = 0; i < 4; i++) {
        if (suitCount[i] >= 5) {
            flushSuit = i;
            isFlush = true;
            break;
        }
    }

    // Check for straight (5 consecutive ranks)
    int consecutive = 0;
    for (int i = 12; i >= 0; i--) {  // Start from highest rank (Ace)
        if (rankCount[i] > 0) {
            consecutive++;
            if (consecutive == 5) {
                isStraight = true;
                break;
            }
        } else {
            consecutive = 0;
        }
    }

    // Special case for Ace-low straight (A, 2, 3, 4, 5)
    if (!isStraight && rankCount[12] > 0 && rankCount[0] > 0 && 
        rankCount[1] > 0 && rankCount[2] > 0 && rankCount[3] > 0) {
        isStraight = true;
    }

    // Check for Four of a Kind, Full House, Three of a Kind, Two Pair, One Pair
    int fourOfAKind = -1, threeOfAKind = -1;
    int pairs[2] = {-1, -1};  // Can hold two pairs

    for (int i = 12; i >= 0; i--) {  // Start from highest rank
        if (rankCount[i] == 4) {
            fourOfAKind = i;
        } else if (rankCount[i] == 3) {
            threeOfAKind = i;
        } else if (rankCount[i] == 2) {
            if (pairs[0] == -1) {
                pairs[0] = i;
            } else {
                pairs[1] = i;
            }
        }
    }

    // Hand Ranking Logic
    if (isStraight && isFlush) {
        bestHand.rank = STRAIGHT_FLUSH;
        bestHand.highCards[0] = consecutive == 5 ? cards[4].rank : 12;  // High straight or Ace
    } else if (fourOfAKind != -1) {
        bestHand.rank = FOUR_OF_A_KIND;
        bestHand.highCards[0] = fourOfAKind;
        // Set kicker
        for (int i = 12; i >= 0; i--) {
            if (rankCount[i] > 0 && i != fourOfAKind) {
                bestHand.highCards[1] = i;
                break;
            }
        }
    } else if (threeOfAKind != -1 && pairs[0] != -1) {
        bestHand.rank = FULL_HOUSE;
        bestHand.highCards[0] = threeOfAKind;
        bestHand.highCards[1] = pairs[0];
    } else if (isFlush) {
        bestHand.rank = FLUSH;
        int flushCardIndex = 0;
        for (int i = 12; i >= 0 && flushCardIndex < 5; i--) {
            if (rankCount[i] > 0) {
                bestHand.highCards[flushCardIndex++] = i;
            }
        }
    } else if (isStraight) {
        bestHand.rank = STRAIGHT;
        bestHand.highCards[0] = consecutive == 5 ? cards[4].rank : 12;  // High straight or Ace
    } else if (threeOfAKind != -1) {
        bestHand.rank = THREE_OF_A_KIND;
        bestHand.highCards[0] = threeOfAKind;
        // Set remaining high cards as kickers
        int kickerIndex = 1;
        for (int i = 12; i >= 0 && kickerIndex < 3; i--) {
            if (rankCount[i] > 0 && i != threeOfAKind) {
                bestHand.highCards[kickerIndex++] = i;
            }
        }
    } else if (pairs[0] != -1 && pairs[1] != -1) {
        bestHand.rank = TWO_PAIR;
        bestHand.highCards[0] = pairs[0];
        bestHand.highCards[1] = pairs[1];
        // Set kicker
        for (int i = 12; i >= 0; i--) {
            if (rankCount[i] > 0 && i != pairs[0] && i != pairs[1]) {
                bestHand.highCards[2] = i;
                break;
            }
        }
    } else if (pairs[0] != -1) {
        bestHand.rank = ONE_PAIR;
        bestHand.highCards[0] = pairs[0];
        // Set remaining high cards as kickers
        int kickerIndex = 1;
        for (int i = 12; i >= 0 && kickerIndex < 4; i--) {
            if (rankCount[i] > 0 && i != pairs[0]) {
                bestHand.highCards[kickerIndex++] = i;
            }
        }
    } else {
        bestHand.rank = HIGH_CARD;
        int cardIndex = 0;
        for (int i = 12; i >= 0 && cardIndex < 5; i--) {
            if (rankCount[i] > 0) {
                bestHand.highCards[cardIndex++] = i;
            }
        }
    }

    return bestHand;
}

// Helper function to evaluate the player's final hand with the board cards
PokerHand evaluateFullHand(Player *player, Board *board) {
    PokerCard combined[MAX_CARDS + BOARD_SIZE];
    int totalCards = 0;
    for (int i = 0; i < MAX_CARDS; i++) {
        combined[totalCards++] = player->hand[i];
    }
    for (int i = 0; i < board->cardCount; i++) {
        combined[totalCards++] = board->cards[i];
    }

    return evaluateBestHand(combined, totalCards);
}

// Compare two hands and return 1 if hand1 is better, -1 if hand2 is better, 0 if equal
int compareHands(PokerHand *hand1, PokerHand *hand2) {
    if (hand1->rank > hand2->rank) return 1;
    if (hand1->rank < hand2->rank) return -1;

    for (int i = 0; i < 5; i++) {
        if (hand1->highCards[i] > hand2->highCards[i]) return 1;
        if (hand1->highCards[i] < hand2->highCards[i]) return -1;
    }

    return 0;
}

// Function to compare hands and return the winner
void displayWinner(Player *players, int nPlayers, Board *board) {
    int bestPlayerIndex = -1;
    PokerHand bestHand = {0};
    bool tie = false;

    printf("\n--- Final Evaluation and Winner ---\n");

    for (int i = 0; i < nPlayers; i++) {
        PokerHand currentHand = evaluateFullHand(&players[i], board);

        int comparisonResult = compareHands(&currentHand, &bestHand);
        if (bestPlayerIndex == -1 || comparisonResult > 0) {
            bestHand = currentHand;
            bestPlayerIndex = i;
            tie = false;
        } else if (comparisonResult == 0) {
            tie = true;
        }
    }

    if (!tie) {
        printf("The winner is Player %d!\n", bestPlayerIndex + 1);
        showPlayerHand(&players[bestPlayerIndex], bestPlayerIndex);
    } else {
        printf("It's a tie!\n");
        for (int i = 0; i < nPlayers; i++) {
            PokerHand currentHand = evaluateFullHand(&players[i], board);
            if (compareHands(&currentHand, &bestHand) == 0) {
                printf("Player %d ties with the best hand!\n", i + 1);
                showPlayerHand(&players[i], i);
            }
        }
    }
}
