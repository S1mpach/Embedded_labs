#ifndef LIGHTUTILS_H
#define LIGHTUTILS_H

typedef enum {
	ALL = 0,
	RED = 1,
	GREEN = 2,
	BLUE = 3
} Color;
	
void lightsOn(Color color);
void lightsOff(Color color);

#endif
