# Battleship
A game of strategy on the high sea!

## Backstory

Many of my fondest memories of strategy games come from the game of Battleship. Considering its simple mechanics and fun play I thought it a good candidate for a C project.

Following the "Question Driven Development" process this time around.

## Features

The feature list for the **MVP** would be as follows.

- [ ] NCurses UI
  - [ ] Player and opponent boards
  - [ ] Only updates chars that have changed on each turn
  - [ ] Game log stating outcome of attack, who took each turn, what was sunk, etc.
- [ ] Main game
  - [ ] Gives 2-char string input representing coordinates (eg "D3")
  - [ ] AI opponent (random guess strategy)
  - [ ] Ends stating winner/loser along with number of turns
- [ ] Easy to follow installation process

The game is standard battleship otherwise, each player takes turns firing a single shot at each other until one of them sinks all the other's ships.
