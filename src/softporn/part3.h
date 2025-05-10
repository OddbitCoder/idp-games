#pragma once

void cantGoThatWay()
{
	writeLine("I can't go that way.");
}

void cantDoThat()
{
	const char* const responses[] =
	{
		"Huh?",
		"Umm... huh?",
		"You're nuts.",
		"You can't be serious.",
		"Not freakin' likely."
		"I don't know how.",
		"An interesting idea.",
		"I can't do that."
	};
	writeLine(responses[getRandom(7)]);
}

void huh()
{
	writeLine("Huh?");
}

void dontKnowThatWord()
{
	writeLine("I don't think I know that word.");
}

void findMeOne()
{
	const char* const responses[] =
	{
		"Find me one.",
		"I don't see one here.",
		"Can't find it here.",
		"You'd have to find it first."
	};
	writeLine(responses[getRandom(4)]);
}

void dontHaveIt()
{
	writeLine("I don't have it.");
}

void alreadyHaveIt()
{
	writeLine("I already have it.");
}

void seeNothingSpecial()
{
	writeLine("I see nothing special.");
}

void seeSomething(objects object, const char* message)
{
	if (state.objectPlace[object] == nowhere)
	{
		writeLine("Oh! I see something!");
		state.objectPlace[object] = state.yourPlace;
	}
	else if (message == NULL)
		seeNothingSpecial();
	else
		writeLine(message);
}

void maybeLater()
{
	writeLine("Maybe not just yet.");
}

void noMoney()
{
	writeLine("Sorry, no money.");
}

bool objectIsHere(objects object)
{
	return state.objectPlace[object] == state.yourPlace;
}

bool objectIsCarried(objects object)
{
	return state.objectPlace[object] == youHaveIt;
}

bool objectCanBeTaken(objects object)
{
	const objects them[] = {
		newspaper, ring, whiskey, beer, hammer, garbage, flowers,
		appleCore, seeds, candy, pills, plant, passcard, radio, knife,
		magazine, rubber, wine, wallet, doll, apple, pitcher, stool,
		rope, rack, mushroom, controlUnit, water
	};
	for (int i = 0; i < 28; i++)
		if (them[i] == object)
			return true;
	return false;
}

bool placeIsPublic(places place)
{
	const places there[] = {
		bStreet, cStreet, cCasino, c21Room, cLobby, cHtDesk, dStreet, dEntrance
	};
	for (int i = 0; i < 8; i++)
		if (there[i] == place)
			return true;
	return false;
}

bool youAreIn(places place)
{
	return state.yourPlace == place;
}

void initNewGame()
{
	clearScreen();

  writeLine("\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r");

	setColor(BOLDCOLOR);
	writeLine("Welcome to SOFTPORN ADVENTURE!\n\r");
	setColor(REGULARCOLOR);
	writeLine("Do you need instructions?");
	write("> ");
	char yesNo = getOneOf('Y', 'N', 0);
	if (yesNo == 'Y')
	{
		giveHelp();
		writeLine("");
	}
	else
		writeLine("");

	memset((void*)&state, 0, sizeof(gameState));

	int i;
	for (i = 0; i < objectCount; i++)
		state.objectPlace[i] = origObjectPlace[i];
	for (i = 0; i < placeCount; i++)
	{
		state.placeVisited[i] = false;
		for (int j = 0; j < directionCount; j++)
			state.path[i][j] = origPath[i][j];
	}

	state.yourPlace = bBar;
	state.money = 10; //$1000
}

void writeHeader()
{
	savePos();
	char status[80];
	strcpy(status, "Score: #/3");
	status[7] = state.score + '0'; 
	writeHeaderLine(0, placeHeaders[state.yourPlace], status);
	int statusLine = 1;
	strcpy(status, "Things here: ");
	int ct = 0;
	int i;
	for (i = 0; i < objectCount; i++)
	{
		if (objectIsHere((objects)i))
		{
			if (ct)
				strcat(status, ", ");
			ct++;
			if (strlen(status) + strlen(objectNames[i]) > COLS - 1)
			{
				writeHeaderLine(statusLine, status, NULL);
				strcpy(status, "             ");
				statusLine++;
			}
			strcat(status, objectNames[i]);
		}
	}
	if (ct == 0)
		strcat(status, "Nothing interesting.");
	writeHeaderLine(statusLine, status, NULL);
	statusLine++;
	strcpy(status, "Exits: ");
	ct = 0;
	for (i = 0; i < directionCount; i++)
	{
		if (state.path[state.yourPlace][i] != nowhere)
			ct++;
	}
	int exits = ct;
	if (exits == 0)
		strcat(status, "By magic!");
	else
	{
		for (int i = 0; i < directionCount; i++)
		{
			if (state.path[state.yourPlace][i] != nowhere)
			{
				if (ct < exits)
				{
					if (ct > 1)
						strcat(status, ", ");
					else if (exits > 1)
						strcat(status, " and ");
				}
				ct--;
				strcat(status, directionNames[i]);
			}
		}
	}
	writeHeaderLine(statusLine, status, NULL);
	restorePos();
}

places prevPlace = (places)-1;

void lookAround()
{
	if (prevPlace != state.yourPlace) {
	  prevPlace = state.yourPlace;
	  setColor(BOLDCOLOR);
	  write(">> ");
	  writeLine(placeHeaders[state.yourPlace]);
	  setColor(REGULARCOLOR);
	}
	if (!state.placeVisited[state.yourPlace])
	{
		writeLine("");
		writeMessageLine(state.yourPlace + 1);
	}
	if (youAreIn(pPntPch) && state.called5550439)
	{
		if (!state.telephoneAnswered && getRandom(4) == 2)
			state.telephoneRinging = true;
		if (state.telephoneRinging)
			writeLine("\n\rThe telephone rings.");
	}
	state.placeVisited[state.yourPlace] = true;
}

/*
procedure take_inventory;
(*=====================*)
var objcount  : integer;
    obj       : objects;

begin
  with game_position do
    begin
      writeln;
      writeln( 'I''m carrying: ');
      objcount := 0;
      for obj := first_object to last_object do
        begin
          if is_carried(obj) then
            begin
              objcount := objcount + 1;
              writeln(object_name[obj]);
            end;
        end;
      if objcount=0 then writeln('Nothing') else writeln;
    end;

end;  { take_inventory }
*/

bool verbIsStandalone(verbs verb)
{
	const verbs verbs[] =
	{
		look, jump, dance, help, quit, showScore, save, restore
	};
	for (int i = 0; i < 8; i++)
		if (verbs[i] == verb)
			return true;
	return false;
}

bool verbIsSpecial(verbs verb)
{
	const verbs verbs[] =
	{
		take /*inven*/, hail /*taxi*/, call /*number*/, play /*slot/21*/, buy, save, restore
	};
	for (int i = 0; i < 7; i++)
		if (verbs[i] == verb)
			return true;
	return false;
}

char* trimWhiteSpace(char* str)
{
	char *end;

	int startSpaces = 0;
	unsigned int i;
	for (i = 0; i < strlen(str); i++)
	{
		if (isspace(str[i]))
			startSpaces++;
		else
			break;
	}

	strcpy(str, str + startSpaces);

	if (*str == 0)
		return str;

	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;

	end[1] = '\0';

	for (i = 1; i < strlen(str); i++)
	{
		while (isspace(str[i]) && isspace(str[i - 1]))
		{
			strcpy(str + i, str + i + 1);
			//for (int j = i; j < strlen(str); j++)
			//	str[i] = str[i + 1];
		}
	}

	return str;
}

extern void parse(char*);
extern int results[];
bool agiParse(char* str)
{
	for (unsigned int i = 0; i < strlen(str); i++)
		str[i] = (char)tolower(str[i]);

	if (strlen(str) == 1)
	{
		switch (str[0])
		{
		case 'i': strcpy(str, "take inventory"); break;
		case 'n': strcpy(str, "north"); break;
		case 's': strcpy(str, "south"); break;
		case 'e': strcpy(str, "east"); break;
		case 'w': strcpy(str, "west"); break;
		case 'u': strcpy(str, "up"); break;
		case 'd': strcpy(str, "down"); break;
		}
	}

	parse(str);

	if (results[0] == -1)
	{
		writeLine("I didn't catch that.");
		return false;
	}

	if (results[0] == 2) //taek
	{
		writeLine("Learn to spell, numbnut!");
		return false;
	}
	else if (results[1] == 3) //lady
	{
		writeLine("That's no lady, that's my sister!");
		return false;
	}

	if (results[0] == 264)
	{
		//take inventory
		results[0] = 302;
		results[1] = 264;
	}
	else if (results[0] >= 100 && results[0] <= 199)
	{
		//go _____
		results[1] = results[0];
		results[0] = 300;
	}

	haveNoVerb = true;
	haveNoObject = true;
	haveNoDirection = true;

	if (results[0] >= 300 && results[0] <= 399)
	{
		haveNoVerb = false;
		state.verb = (verbs)(results[0] - 300);
	}
	if (results[1] >= 200 && results[1] <= 299)
	{
		haveNoObject = false;
		state.noun = (objects)(results[1] - 200);
	}
	if (haveNoObject && results[1] >= 100 && results[1] <= 199)
	{
		haveNoDirection = false;
		state.direction = (directions)(results[1] - 100);
	}

	haveVerbOnly = !haveNoVerb && (haveNoObject && haveNoDirection);
	return true;
}

void readAndParseCommand()
{
	bool commandOK = false;
	char userInput[80];

	lookAround();

	do
	{
		do
		{
			do
			{
				write("\n\r> ");
				writeHeader();
				getUserInput(userInput, 38);
				writeLine("");
				if (strlen(userInput) == 0)
					writeLine("Beg your pardon?");
			} while (strlen(userInput) == 0);

			for (unsigned int i = 0; i < strlen(userInput); i++)
			{
				if (ispunct(userInput[i]))
					userInput[i] = ' ';
				trimWhiteSpace(userInput);
			}
		} while (strlen(userInput) == 0);

		commandOK = agiParse(userInput);

	} while (!commandOK);
}
