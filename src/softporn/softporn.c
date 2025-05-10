#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "utils.h"

#include "messages.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "parts456.h"

int main()
{
	loadDict();
	if (dict == NULL)
	{
		writeLine("Is \"words.bin\" there?");
		getKey();
		return -1;
	}

	initNewGame();

	state.gameEnded = false;
	while (!state.gameEnded)
	{
		if (/*state.yourPlace >= bHallway &&*/ state.yourPlace <= bBalcony)
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
				writeLine("A passerby kills me for wearing my kinky rubber in public.");
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
			writeLine("I don't know how to do that.");
			//write("I don't know how to %s something.\n\r", fullVerb);
		else if (haveVerbOnly && !verbIsStandalone(state.verb))
			writeLine("Give me a noun!");
		else if (!haveVerbOnly && (haveNoObject && haveNoDirection && (!verbIsSpecial(state.verb))))
		{
			writeLine("I don't know how to do that.");
			//addDefiniteArticleTo(fullNoun);
			//write("I don't know to %s %s.\n\r", fullVerb, fullNoun);
		}
		else
		{
			PartsFourToSix();
		}
	}
quitGame:

	writeLine("Thank you for playing, and good bye!");
	doneWithFiles();
	return 0;
}
