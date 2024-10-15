#include "pokerlib.h"
#include "utils.h"

int randSuit(){
    return rand() % SPADES;
}

int randRank(){
    return (rand() % KING) + 1;
}

PokerCard randCard(){
    PokerCard card = createCard(randSuit(), randRank());
    return card;
}

// Printf cards in hand with "Card: %Rank of %Suit"
void printCard(PokerCard card)
{
    const char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    const char *ranks[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};

    // ANSI \033[33m = yellow
    // ANSI \033[37m = white
    printf("Card: \033[33m%6s \033[37mof \033[33m%s \033[37m\n", ranks[card.rank - 1], suits[card.suit]);
}

// Card contructor
PokerCard createCard(Suit suit, Rank rank)
{
    PokerCard card;
    card.suit = suit;
    card.rank = rank;
    return card;
}

// Loop create cards fit to array size
void initDeck(Deck *deck)
{
    deck->cardCount = 0;
    for (Suit suit = HEARTS; suit <= SPADES; suit++)
    {
        for (Rank rank = ACE; rank <= KING; rank++)
        {
            deck->cards[deck->cardCount++] = createCard(suit, rank);
        }
    }
}

void initBoard(Board *board){
    board->cardCount = 0;
}

void giveBoardCard(Board *board, Deck *deck, PokerCard card){
    removeFromDeck(deck, card);
    board->cards[board->cardCount++] = card;
}

void printBoardCard(Board *board){
    printf("Community Card\n");
    for (int i = 0; i < board->cardCount; i++){
        printCard(board->cards[i]);
    }
}

// Check card exist in deck or not
bool checkInDeck(Deck *deck, PokerCard card)
{
    for (int i = 0; i < DECK_SIZE; i++)
    {
        if (deck->cards[i].rank == card.rank && deck->cards[i].suit == card.suit)
        {
            return true;
        }
    }
    return false;
}

PokerCard randInDeck(Deck *deck){
    PokerCard card = randCard();

    while (!checkInDeck(deck, card))
    {
        card = randCard();
    }
    return card;
}

// Remove card from deck by replace with card(-1, -1) then return the card
PokerCard removeFromDeck(Deck *deck, PokerCard card)
{
    PokerCard removedCard = createCard(-1, -1);

    for (int i = 0; i < DECK_SIZE; i++)
    {
        if (deck->cards[i].rank == card.rank && deck->cards[i].suit == card.suit)
        {
            removedCard = deck->cards[i];
            deck->cards[i] = createCard(-1, -1);
            deck->cardCount--;
            break;
        }
    }
    return removedCard;
}

// Init player with n players with empty hand (initPlayerHand)
Player *initPlayers(Player **players, int n)
{
    if (*players != NULL)
    {
        free(*players);
    }
    *players = (Player *)malloc(n * sizeof(Player));
    for (int i = 0; i < n; i++)
    {
        initPlayerHand(&(*players)[i]);
    }
    return *players;
}

// Init players with empty hand (-1,-1)
void initPlayerHand(Player *player)
{
    player->cardCount = 0;
    for (int i = 0; i < MAX_CARDS; i++)
    {
        player->hand[i] = createCard(-1, -1);
    }
}

// give card to player hard in order if card count < max
void givePlayerCard(Player *player,Deck *deck, PokerCard card)
{
    if (player->cardCount < MAX_CARDS)
    {
        removeFromDeck(deck, card);
        player->hand[player->cardCount++] = card;
    }
}

//  Show player hand with printCard()
void showPlayerHand(Player *player, int pIndex)
{
    printf("Player %d's hand:\n", pIndex + 1);
    for (int i = 0; i < player->cardCount; i++)
    {
        printCard(player->hand[i]);
    }
}

// Just test
int isGameEnd(Player *players, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (players[i].cardCount < MAX_CARDS)
        {
            return 0;
        }
    }
    return 1;
}
