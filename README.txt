UI Components DEMO
----------------------

For some reason adding these UI components broke the scene transitions, and there are a slew of other bugs we have yet to sort out

Health Bar
- The health bar is displayed above the player's head
- Pressing the space bar will cause the player's health to be depleted

Text Box
- Press P to open a text box, which will follow above the player's head
- Pressing P once the box is open will continue to the next page of text
- Once there is no more text, the text box will close
- Currently an unresolved bug where if you close the game with the text box open, the TextBox destructor attempts to access an invalid pointer

Weapon Selection
- Press buttons 1-5 to select the appropriate weapon
- Currently an unresolved bug where the projectiles only appear when the text box is on screen