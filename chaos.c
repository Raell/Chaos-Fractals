#include "display.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>

#define PI 3.14159265358979323846

struct point {
	
	double x;
	double y;
	
};

typedef struct point point;

struct corner {
	
	point *position;
	double rotation;
	
};

typedef struct corner corner;

int sides = 3;
double factor = 0.5;
int *rotationfactor = 0;
double scalingfactor = 1;
int iterations = 0;
bool repeatedcorner = true;
list *restrictions = NULL;
bool mirrored = false;
bool xor = false;
bool gridlines = false;

// Initialize positions and rotation factors of corners
void setCorners(int sides, corner *corners[sides], point *center, int radius) {
	
	double angle = (2 * PI) / sides;
	
	for(int i = 0; i < sides; i++) {
		
		corners[i] = malloc(sizeof(corner));
		corners[i]->position = malloc(sizeof(point));
		point *position = corners[i]->position;
		position->x = center->x + ((radius * cos((PI/2) + (i * angle))) * scalingfactor);
		position->y = center->y - ((radius * sin((PI/2) + (i * angle))) * scalingfactor);
		if(rotationfactor != 0)
			corners[i]->rotation = rotationfactor[i];
		else
			corners[i]->rotation = 0;
		
	}
	
}

// Gets a random starting position
point *getSeed() {
	
	point *seed = malloc(sizeof(point));
	*seed = (point) { rand() % 1001, rand() % 1001 };
	return seed;
}

// Checks if parameter is a number
bool isNumber(char *param) {	
	for(int i = 0; i < strlen(param); i++) {
		
		if (!isdigit(param[i]))
			return false;
		
	}
	return true;
}

// Returns double value of fraction
double processFractions(char *input, int fracpos) {
	char numerator[fracpos + 1];
	strncpy(numerator, input, fracpos);
	numerator[fracpos] = '\0';
	char *denominator = &input[strlen(input) - (strlen(input) - fracpos - 1)];
	if(atof(denominator) == 0) {
		return 0;
	}
	return (atof(numerator) / atof(denominator));
}

// Handles processing of fraction inputs
double checkFractions(char *input) {
	int fracpos = 0;
	for(int i = 0; i < strlen(input); i++) {
		
		if(input[i] == '/') {
			fracpos = i;
			continue;
		}
		
		if(!isdigit(input[i]) && input[i] != '.')
			return 0;
	}
	
	if(fracpos != 0)
		return processFractions(input, fracpos);
	
	else {	
		return atof(input);
	}
}

// Processes strings to be delimited into an array of ints
int processArrayParams(char *optarg, int *param, char *delimiter, bool skipfirst) {
	
	char *buf = strtok(optarg, delimiter);
	int i = 0;
	
	for(i = 0; buf != NULL; i++) {
		if(skipfirst) {
			skipfirst = false;
			continue;
		}
		if(isNumber(buf)) {											
			param = realloc(param, (i + 1) * sizeof(int));
			param[i] = atoi(buf);						
		}
		else
			return -1;
		
		buf = strtok(NULL, delimiter);
	}
	
	return i;
}

// Handles processing each restriction parameters into linked list
int setRestrictions(char *res) {
	
	int *restrictset = malloc(sizeof(int));
	
	if(strcmp(res, "-") == 0)
		restrictset[0] = 0;
	else 
		restrictset[0] = processArrayParams(res, restrictset, ",", true);
	
	
	
	if(restrictset[0] == -1)
		return -1;
	
	insertBefore(restrictions, restrictset);
	return 0;
	
}

// Handles processing of all restriction parameters
int initializeRestrictions(char *arguments) {
	
	restrictions = newList();
	char **buffer = malloc(sizeof(char));
	int count = 0;
	
	for(char *line = strtok(arguments, ":"); line != NULL; line = strtok(NULL, ":"), count++) {	
		buffer = realloc(buffer, (count + 1) * sizeof(line));
        buffer[count] = line;
	}
	
	for(int i = 0; i < count; i++) {
		
		if(setRestrictions(buffer[i]) == -1)
			return -1;
	}
	
	return 0;
}

// Checks if elements in set are unique
bool allUnique(int *set, int count) {
	
	for(int i = 1; i < count; i++) {		
		for(int j = 1; j < count; j++) {
			
			if(i == j)
				continue;
			if(set[i] == set[j])
				return false;
			
		}		
	}	
	return true;
}

// Checks is restrictions provided are valid
bool validRestrictions(bool mirrored) {
	
	start(restrictions);
	for(int i = 0; i < size(restrictions); i++) {
		
		int *rs = getAfter(restrictions);
		int count = rs[0];
		
		int maxrs = mirrored ? (2 * (count - 1)) : (count - 1);
		
		if(maxrs >= sides || !allUnique(rs, count))
			return false;
		
		for(int j = 1; j < count; j++) {
			int maxrange = mirrored ? (2 * rs[j]) : (rs[j] + 1);
			
			if(maxrange > sides)
				return false;		
		}
		
		forward(restrictions);
		
	}
	
	return true;
	
}

// Repeats rotation factor to all corners if 1 angle is specified
void repeatRotation() {
	int rot = rotationfactor[0];
	rotationfactor = realloc(rotationfactor, sides * sizeof(int));
	
	for(int i = 0; i < sides; i++) {
		rotationfactor[i] = rot;
	}
}

// Handles simple arguments processing
int basicArguments(char opt, char *optarg) {
	switch (opt) {		
		case 'c': 
			if(isNumber(optarg))
				sides = atoi(optarg);
			else
				return -1;
			break;
			
		case 'f':
			factor = checkFractions(optarg);
			if (factor <= 0 || factor >= 1)
				return -1;
			break;
			
		case 's':
			scalingfactor = checkFractions(optarg);
			if (scalingfactor <= 0)
				return -1;
			break;
			
		case 'i':
			if(isNumber(optarg))
				iterations = atoi(optarg);
			else
				return -1;
			break;
			
		case 'a':
			repeatedcorner = false;
			break;
			
		case 'x':
			xor = true;
			break;	
		
		case 'g':
			gridlines = true;
	}
	return 0;
}

// Main arguments processing function
int parseArguments(int n, char *argv[n]) {	
	int opt;
	int rotsize = 0;
	int exclusive = 0;	
	while ((opt = getopt(n, argv, "c:f:s:i:gr:haj:n:x")) != -1) {
		switch (opt) {					
			case 'c':
			case 'f':
			case 's':
			case 'i':
			case 'a':
			case 'g':
			case 'x':
				if(basicArguments(opt, optarg) == -1)
					return -1;
				break;		
			case 'r':
				rotationfactor = malloc(sizeof(int));
				rotsize = processArrayParams(optarg, rotationfactor, ",", false);
				if(rotsize == -1)
					return -1;
				break;					
			case 'j':
				mirrored = true;				
			case 'n':
				if(exclusive)
					return -1;
				if(initializeRestrictions(optarg) == -1)
					return -1;				
				exclusive = 1;
				break;					
			default:
				return -1;
		}	
	}	
	if(mirrored)
		exclusive++;
	
	bool invalidxor = !exclusive && xor;
	bool rot = sides != rotsize && rotsize > 1;
	bool res = true;
	
	if(rotsize == 1)
		repeatRotation();	
	if(exclusive)
		res = validRestrictions(mirrored);
	if(sides <= 0 || invalidxor || !res || rot)
		return -1;
	
	return 0;
}

// Prints helpmenu
void helpmenu(char *progname) {
	printf("Usage: %s [-c corners] [-f magfactor] [-s scale] [-i iterations] [-r a[,b,...,n]] [-a] [(-j delimitedset | -n delimitedset) [-x]] [-h]\n\n", progname);
	printf("Options:\n");
	printf("-c corners\n Number of corners (i.e 3 = triangle)\n\n");
	printf("-f magfactor\n Sets magnification factor (i.e. Distance to move towards corner to get next point, 1/2 = move half the distance to corner)\n\n");
	printf("-s scale\nAmount to scale final image (i.e 2 = image is 2x size)\n\n");
	printf("-i iterations\n Number of pixels to generate image\n\n");
	printf("-g\n Draw lines between corners in red\n\n");
	printf("-r a[,b,...,n]\n Set angle of rotation of corners.\n Each corner rotation can be set individually by seperating with ','\n Must be equal to number of corners in this case\n\n");
	printf("-a\n Set restriction that the same corner cannot be selected twice in a row\n\n");
	printf("-j delimitedset\n Restricts corners if within specified distance (both clockwise and anticlockwise) to previously selected corners\n\n Parameter is in the format a,b:c:d,e,f,g with ',' and ':' as delimiters\n Where in this example:\n\n a and b are the restrictions for the previously selected corner\n c is the restricton for the 2nd previously selected corner \n d,e,f and g are the restrictions for the 3rd previously selected corner\n\n Each restriction is delimited by ','\n More restrictions on earlier corners are added by delimiting with ':' as appropriate\n To set no restriction for a corner use '-'\n\n Note: Mutually exclusive with -n paramter below\n\n Example:\n -j 1,2:-:0:1 \n\n Means the next corner selected cannot be\n  1 or 2 distance away from the 1st previous corner AND\n  No restrictions on the 2nd previous corner AND\n  No repeat of 3rd previous corner AND\n  1 distance away from the 4th previous corner\n\n");
	printf("-n delimitedset\n Similar as -j parameter but only applies to clockwise distance only (not mirrored)\n\n Note: Mutually exclusive with -j parameter above\n\n Example:\n -n 1,4:2\n\n Means the next corner selected cannot be\n 1 or 4 distance away clockwise of the 1st previous corner AND\n 2 distance away clockwise of the 2nd previous corner\n\n");
	printf("-x\n Used with -j or -n parameters to change the AND conditions mentioned above to XOR (exclusive or)\n Must be used with -j or -n\n\n");
	printf("-h\n Open this help menu");
	
}

// Rotate seed point relative to corner by angle
void rotatePoint(point *seed, point *cornerpos, double rotation) {
	double angle = (rotation / 180 * PI);
	
	double xcoeff = seed->x - cornerpos->x;
	double ycoeff = seed->y - cornerpos->y;
	
	seed->x = (cos(angle) * xcoeff) - (sin(angle) * ycoeff) + cornerpos->x;
	seed->y = (sin(angle) * xcoeff) + (cos(angle) * ycoeff) + cornerpos->y;
}

// Generates next position of seed
void nextPosition(point *seed, point *cornerpos, double rotation) {
	
	seed->x = (500 * ((0.5 - factor) * 2)) + (seed->x + cornerpos->x) * factor;
	seed->y = (500 * ((0.5 - factor) * 2)) + (seed->y + cornerpos->y) * factor;
	rotatePoint(seed, cornerpos, rotation);
	
}

// Gets restricted corner by clockwise distance
int getClockwiseRestriction(int prevselection, int rangerestricted) {	
	int rc = prevselection + rangerestricted;	
	if(rc >= sides)
		rc -= sides;
	
	return rc;
}

// Gets restricted corner by anticlockwise distance
int getAnticlockwiseRestriction(int prevselection, int rangerestricted) {	
	int ra = prevselection - rangerestricted;
	if(ra < 0)
		ra += sides;
	
	return ra;
}

// Checks if selected corner is restricted
bool isRestricted(int chosenside, int prevselection, int restriction) {
	
	if(mirrored) {
		int rc = getClockwiseRestriction(prevselection, restriction);
		int ra = getAnticlockwiseRestriction(prevselection, restriction);
		return (chosenside == rc || chosenside == ra);
	}
	
	else {
		int rc = getClockwiseRestriction(prevselection, restriction);
		return (chosenside == rc);
	}
	
}

// Main restriction handling function
bool checkRestriction(int chosenside, int lastcorner, int *prevselections) {
	
	if(chosenside == lastcorner && !repeatedcorner)
		return false;
	
	if(restrictions != NULL) {		
		start(restrictions);	
		bool hasrestriction = false;
		for(int i = 0; i < size(restrictions); i++) {
			
			int *rst = getAfter(restrictions);
			int count = rst[0];
			
			for(int j = 1; j < count; j++) {			
				if(isRestricted(chosenside, prevselections[i], rst[j])) {
					if(!xor)
						return false;
					else {
						if(hasrestriction)
							return true;
						hasrestriction = true;
						break;
					}
				}
			}			
			forward(restrictions);		
		} 
		if(hasrestriction)
			return false;
	}	
	return true;
}

// Updates previous corner selections
void updatePreviousSelections(int *prevselections, int totrack, int lastchoice) {
	for(int i = totrack - 1; i > 0; i--)				
		prevselections[i] = prevselections[i - 1];
	
	prevselections[0] = lastchoice;
}

// Initialize all previous corner postions to -1
void initializePrevSelections(int count, int *prevselections) {
	for(int i = 0; i < count; i++)
		prevselections[i] = -1;
}

// Plots points to display
void iteratePoints(display *d, corner *corners[sides], int totrack) {
	
	point *seed = getSeed();
	int *prevselections = malloc(totrack * sizeof(int));
	initializePrevSelections(totrack, prevselections);
	
	int count = (iterations == 0) ? 10000 * (sides * sides) : iterations;
	int lastcorner = -1;
	
	int breakclause = 1000;
	
	for(int i = 0; i < count; i++) {
		int chosenside = rand() % sides;	
		
		if(!checkRestriction(chosenside, lastcorner, prevselections)) {
			i--;
			if(breakclause == 0)
				break;
			else
				breakclause--;
			continue;
		}
		
		breakclause = 1000;
		
		point *cornerpos = corners[chosenside]->position;
		nextPosition(seed, cornerpos, corners[chosenside]->rotation);
		if(i > 20)
			pixel(d, seed->x, seed->y);
		
		if(i % (50 * (sides * sides)) == 0)
			render(d);

		lastcorner = chosenside;
		
		updatePreviousSelections(prevselections, totrack, chosenside);
	}
	
}

// Prints red gridlines joining corners
void setGridLines(display *d, int sides, corner *corners[sides]) {
	colour(d, 0xFF0000FF);
	line(d, corners[0]->position->x, corners[0]->position->y, corners[sides - 1]->position->x, corners[sides - 1]->position->y);
	for(int i = 0; i < sides; i++) {
		if(i != sides - 1)
			line(d, corners[i]->position->x, corners[i]->position->y, corners[i + 1]->position->x, corners[i + 1]->position->y);
		else
			line(d, corners[i]->position->x, corners[i]->position->y, corners[0]->position->x, corners[0]->position->y);
	}	
	colour(d, 0x000000FF);
}

// Main function
int main(int n, char *argv[n]) {
	
	srand(time(NULL));
	
	if(parseArguments(n, argv) == -1) {
		helpmenu(argv[0]);
		return -1;
	}
	
	display *d = newDisplay("Sierpenski", 1000, 1000);	
	corner *corners[sides];
	
	point *center = malloc(sizeof(point));
    *center = (point) { 500, 500 };
	
	setCorners(sides, corners, center, 480);
	
	if(gridlines)
		setGridLines(d, sides, corners);
	
	int s = (restrictions != NULL) ? size(restrictions) : 0;
	
	printf("%d\n", s);
	
	iteratePoints(d, corners, s);
	
	key(d);
	terminateDisplay(d);

}



