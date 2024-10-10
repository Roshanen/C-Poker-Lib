#include "pokerlib.h"
#include "utils.h"

// Encode (PokerCard.suit, PokerCard.rank) to 0-51.
// order is HEARTS, DIAMONDS, CLUBS, SPADES
int encode(int suit, int rank)
{
    if (suit > 4 || suit < 0 || rank > 13 || rank < 1)
    {
        return -1;
    }
    return (suit * 13) + rank - 1;
}

// Decode 0-51 to PokerCard
// order is HEARTS, DIAMONDS, CLUBS, SPADES
PokerCard decode(int order)
{
    PokerCard card;
    if (order > 51 || order < 0)
    {
        card = createCard(-1, -1);
    }
    else
    {
        card = createCard(order / 13, (order % 13) + 1);
    }
    return card;
}

int randSuit(){
    return rand() % MAX_SUIT;
}

int randRank(){
    return (rand() % MAX_RANK) + 1;
}

PokerCard randCard(){
    PokerCard card = createCard(randSuit(), randRank());
    return card;
}

// Printf cards in hand with "Card: %Rank of %Suit"
void printCard(PokerCard card)
{
    if (card.suit > 3 || card.suit < 0 || card.rank > 13 || card.rank < 1){
        printf("\033[31mInvalid Card suit:%d rank:%d\033[37m", card.suit, card.rank);
        return;
    }
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

// Check 0 <= card.suit <= 3 and 1 <= card.rank <= 13 
bool isValidCard(PokerCard card){
    // debug display
    return (isInRange(card.suit, MIN_SUIT, MAX_SUIT) && isInRange(card.rank, MIN_RANK, MAX_RANK));
}

// Loop create cards fit to array size
void initDeck(Deck *deck)
{
    deck->cardCount = 0;
    for (Suit suit = MIN_SUIT; suit <= MAX_SUIT; suit++)
    {
        for (Rank rank = MIN_RANK; rank <= MAX_RANK; rank++)
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
void showPlayerHand(Player *player)
{
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
