# C-Poker-Lib

C-Poker-Lib is a C programming library designed to simulate poker gameplay, calculate possible higher poker hands, and evaluate win rates. The library allows users to run poker simulations and assess the likelihood of achieving specific hands.

## Features
- Simulates poker gameplay for 2 to 8 players.
- Calculates the probability of reaching higher poker hands.
- Evaluates the win rate based on the current hand and remaining cards.

## Requirements
- A C compiler (e.g., GCC)
- Make tool for building the project

## Installation
- **Clone the repository**:
   
   ```bash
   git clone https://github.com/yourusername/C-Poker-Lib.git
   cd C-Poker-Lib
   ```

## Usage
- Run the simulation: After building the project, run the program with:
  
  ```bash
  make run
  ```
- Input the number of players: You will be prompted to input the number of players (between 2 and 8). If you input a number outside this range, you will be prompted to enter it again.
- End the program: To terminate the program, input 0 when asked for the number of players.
