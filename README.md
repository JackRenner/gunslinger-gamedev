# The Gunslinger Development Tool

## Authors

- Evan Denison
- Minsoo Choi
- Reed Foster

##Important Notes

- Put all the images you want to access in /resources/sprites (spritesheet support coming soon)
- Trying to load a scene which does not exist will cause the program to crash (be careful when typing)
- The GUI is really janky and sometimes things require a couple clicks to respond
- Similar to the last note, typing into the fields sometimes requires a couple key presses so be careful
- If a button isn't working, most likely an entry box is selected, hit enter to deselect the entry box and try the button again
- There is no way to update the z value of sprites, thus add sprites in the order you want them to layer (bottom layer first)
- If you have any questions or encounter any bugs please reach out to me (Reed) via Slack or at (239) 250-0123.

##Instructions for Use

#Compile with make and run with ./bin/devTool (no guarantee this works on Windows)

#When the main window is in focus (click on it if it's not) and no sprite is selected:

- Use the arrow keys to move around the scene
- Use the Z and X keys to zoom in and out of the scene
- The bar at top displays all the .png images found in /resources/sprites (spritesheet support coming soon), the Q and W keys allow you to scroll through them
- Click on an image in the bar at top to create a new sprite with that texture
- Click on a sprite already in the scene to select it

#When a sprite is selected (if it's flashing) and main window is in focus:

- Use the arrow keys to move the selected sprite around the scene
- Press enter to deselect a sprite
- Click and drag anywhere in the window to have the sprite follow the mouse (kind of whack, but works) and click the mouse again to set it down
- Values in the preferences window will update automagically

#When a sprite is selected and preferences window is in focus (click on it if it's not):

-Click on an entry field to change its value (note: you can only use these entry boxes when a sprite is selected)
-Press enter once you've typed in the desired value and it will update in the scene (note: alpha only changes once a sprite is deselected)
-Press the delete sprite button to delete the sprite (note: this CANNOT be undone)

#Saving a scene
- Click on the preferences window to put it into focus
- Click on the entry field above the save scene button
- Type in the filename you want to save the scene as (i.e. example.txt or example.json)
- Hit enter to deselect the entry box
- Click on the save scene button (note: the scene is saved in resources/scene)

#Loading a scene

- Click on the preferences window to put it into focus
- Click on the entry field above the load scene button
- Type in the filename EXACTLY of the scene to load (from resources/scene)
- Hit enter to deselect the entry box
- Click on the load scene button