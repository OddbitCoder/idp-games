#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "metrics.h"

#include "utils.h"

#include "messages.h"
#include "part1.h"

int getRandom(int max)
{
	static bool initialized = false;
	if (!initialized)
	{
		srand(timer());
		initialized = true;
	}
	return rand() / ((RAND_MAX + 1u) / max);
}

void putw(const char*);
void writeLongMessage(int msg)
{
	putw(messages[msg]);
}
void writeLongMessageNoWrap(int msg)
{
	puts(messages[msg]);
}

#include "part2.h"
#include "part3.h"
#include "parts456.h"

int main()
{
	loadDict();
	if (dict == NULL)
	{
		puts("Is \"words.tok\" there?");
		getKey();
		return -1;
	}

	setWindowSize();
	initNewGame();

	state.gameEnded = false;
	while (!state.gameEnded)
	{
		if (state.yourPlace >= bHallway && state.yourPlace <= bBalcony)
		{
			state.objectPlace[sign] = bStreet;
			state.objectPlace[button] = bBar;
		}
		else if (state.yourPlace >= cStreet && state.yourPlace <= cHtDesk)
		{
			state.objectPlace[sign] = cStreet;
			state.objectPlace[button] = cHtDesk;
			state.objectPlace[elevator] = cHtDesk;
		}
		else if (state.yourPlace >= dBooth && state.yourPlace <= dPharmacy)
		{
			state.objectPlace[sign] = dStreet;
			state.objectPlace[telephone] = dBooth;
		}
		else if (state.yourPlace >= pFoyer && state.yourPlace <= pPntPch)
		{
			state.objectPlace[button] = pFoyer;
			state.objectPlace[elevator] = pFoyer;
			state.objectPlace[telephone] = pPntPch;
		}

		if (!youAreIn(bBar))
			state.path[bBar][east] = nowhere;
		if (!youAreIn(dEntrance))
		{
			state.path[dEntrance][west] = nowhere;
			state.doorWestOpen = false;
		}
		if (!objectIsHere(stool))
		{
			state.stoolClimbed = false;
		}
		if (state.rubberWorn && placeIsPublic(state.yourPlace))
		{
			if (getRandom(8) == 5)
			{
				putw("A passerby kills me for wearing my kinky rubber in public.");
				purgatory();
				if (state.gameEnded)
					goto quitGame;
			}
		}
		readAndParseCommand();

		if (state.noun == sign)
		{
			if (state.yourPlace == dEntrance)
				state.noun = doorWest;
			else if (state.yourPlace == pKitchen)
				state.noun = sink;
		}

		if (haveNoVerb)
			puts("I don't know how to do that.");
			//printf("I don't know how to %s something.\n", fullVerb);
		else if (haveVerbOnly && !verbIsStandalone(state.verb))
			puts("Give me a noun!");
		else if (!haveVerbOnly && (haveNoObject && haveNoDirection && (!verbIsSpecial(state.verb))))
		{
			puts("I don't know how to do that.");
			//addDefiniteArticleTo(fullNoun);
			//printf("I don't know to %s %s.\n", fullVerb, fullNoun);
		}
		else
		{
			PartsFourToSix();
		}
	}
quitGame:

	puts("\n\n\nThank you for playing, and good bye!\n");
	return 0;
}
