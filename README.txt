SCENE TRANSITIONS DEMO
----------------------

TOWN
You will start out in the middle of the town area.
Each of the buildings along the top of the town are enterable by walking near the door.
You can exit the buildings by walking back to just below where you entered.
The lake area can be accessed by walking near the sign at the bottom left of the town.

LAKE
The lake area is a 3x3 grid of rooms. Upon entering from the town, you are in the top left room.
Walking to the edge of the screen at the border between two rooms will cause a scene transition.
We have not yet implemented collision detection at the border of the whole area, so the
character is able to walk off the screen at the outer perimeter.
Walking to the top of the screen when in the top left room will return you to the town.

Bounds are enforced on the camera so it does not go outside the playable area.
The scene transitions fade to black and then fade back in for each room.