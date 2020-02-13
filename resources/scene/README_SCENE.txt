This is a short writeup on the scene JSON syntax.

Any key enclosed in ( ) can be anything the user enters, otherwise the key MUST be exactly as written here.
See files in "./resources/scene/" for examples.

{
	"(Entity ID)": {						This will be the ID assigned to the entity when constructed.
		"type":	"DisplayObjectContainer" | "Sprite" | "RGB" | "AnimatedSprite",
											DisplayObjectContainer will always be empty.

		"filepath": "/filepath",			As of now, only for Sprite type.
		"rgb": [red, green, blue],			Array of int values, only for RGB type.

											Object for Animations, only for AnimatedSprite type.
		"(animation)": {					(animation) is only used to parse the JSON, it is not the animation name in the AnimatedSprite.					
			"basepath": "/basepath",
			"animName": "name",
			"numFrames": frames,			int value
			"frameRate": rate,				int value
			"loop": true | false			bool value
		},

		"playing": "animName",				Name of the animation to initially play.

		"position": [x, y],					Array of ints
		"pivot": [x, y],					Array of ints
		"scale": [x, y],					Array of doubles
		"rotation": angle,					double value
		"alpha": 0-255,						int value
		"visible": true | false,			bool value

		"parent": "parent_id" | null		if null or parent was not found (might change this later), will set Scene object as parent
	}
}