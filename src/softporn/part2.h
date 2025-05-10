#pragma once

void giveHelp()
{
	clearScreen();
	writeMessageLine(70);
	getKeySilent();
}

void lookAtGraffiti()
{
	clearScreen();
	writeMessageLine(59);
	getKeySilent();
}

void purgatory()
{
	delay(700);
	int door = 0;
loop:
	if (door == 0)
		writeMessageLine(65);
	else
		writeLine("I'm still here?\n\r");
	writeLine("Choose your door: 1, 2, or 3?");
ask:
	putchar('>');
	char k = getOneOf('1', '2', '3');
	int c = k - '0';
	if (c > 0 && c < 4)
	{
		door = (getRandom(3) + c) % 3;
		state.gameEnded = door == 1;
	}
	else
	{
		writeLine("Just 1, 2, or 3.");
		goto ask;
	}
	if (door == 2)
		goto loop;
	else if (door == 1)
	{
		writeLine("Guess I'll die.");
		delay(200);
	}
	else if (door == 0)
	{
		writeLine("\"Congratulations, you get another shot! See you soon!\"");
		delay(200);
	}
}

void bumTellsStory()
{
	writeLine("He looks at you and starts to speak.");
	delay(400);
	writeMessageLine(39);
	delay(300);
	int i;
	for (i = 0; i < 128; i++)
	{
		for (int j = 0; j < i; j++)
			putchar(' ');
		writeLine("Like I did!");
	}
	for (i = 0; i < 5; i++)
		writeLine("");
	delay(500);
	writeLine("He throws up and gives me back the bottle of wine.");
}

void watchTV()
{
watch:
	writeMessageLine(41 + state.TVChannel);
	printf("Change the channel? >");
	if (getOneOf('Y', 'N', 0) == 'Y')
	{
		writeLine("*click*");
		state.TVChannel = (state.TVChannel + 1) % 9;
		writeLine("...");
		goto watch;
	}
}

void wineInTaxi()
{
	writeMessageLine(58);
	delay(500);
	writeLine("What do I do!?");
	delay(1000);
	writeLine("The idiot cabbie backs over me and kills me!");
	purgatory();
}

extern bool objectIsCarried(objects object);
void stabSomeone()
{
	if (!objectIsCarried(knife))
	{
		writeLine("With what, rapier wit?");
		return;
	}
	writeLine("Okay ya warmonger.");
	delay(1000);
	printf("Parry!");
	delay(500);
	printf(" Thrust!");
	delay(1000);
	writeLine(" ...and I just got myself.");
	purgatory();
}

void fallingDown()
{
	writeLine("Aaaaaauuuuuugh!");
	delay(300);
	writeLine("*SPLAT!*");
	if (state.verb != jump)
	{
		delay(500);
		writeLine("\n\r... I should've used a safety rope.");
	}
	purgatory();
}

void playSlots(int* money)
{
	const char slot[5] = { '!', '#', '*', '$', '^' };
	const int slotFigs = 5;
	int x1 = 0, x2 = 0, x3 = 0;
	bool cheat = false;
	char answer = 'Y';
	writeLine("This will cost $100 each time.");
	do
	{
		printf("You have $%d00. Would you like to play?\n\r>", *money);
		answer = getOneOf('Y','N','K');
		if (answer == 'K')
		{
			cheat = !cheat;
			answer = 'Y';
		}
		if (answer == 'Y')
		{
			savePos();
			for (int i = 0; i < 30; i++)
			{
				x1 = getRandom(slotFigs);
				x2 = getRandom(slotFigs);
				x3 = getRandom(slotFigs);
				if (cheat) x3 = x2 = x1;
				delay(100);
				restorePos();
				printf("%c %c %c", slot[x1], slot[x2], slot[x3]);
			}
			writeLine("");
			if (x1 == x2 && x2 == x3)
			{
				writeLine("Triples! You win $1500!");
				*money += 15;
			}
			else if (x1 == x2 || x2 == x3 || x1 == x3)
			{
				writeLine("A pair! You win $300!");
				*money += 3;
			}
			else
			{
				writeLine("You lose.");
				*money -= 1;
			}
		}
	} while (*money > 0 && answer != 'N');
	writeLine("");
	if (*money < 1)
	{
		writeLine("I'm broke! That means death!");
		purgatory();
	}
}

const char* const cardNames[] =
{
	"an Ace", "a 2", "a 3", "a 4", "a 5", "a 6", "a 7",
	"an 8", "a 9", "a 10", "a Jack", "a Queen", "a King"
};

void dealCard(int* y, int* z, int* ac, char** card)
{
	*z = getRandom(13) + 1;
	*y = 0;
	*ac = 0;
	*card = (char*)cardNames[*z - 1];
	if (*z > 10) *z = 10;
	if (*z == 1) *z = 11;
	if (*z == 0) *y = 1;
	if (*z == 11) *ac = 1;
}

void check(int* mi, int* md, int* ad, int* a, int* money, int* dollars, bool* gameOver)
{
	if (*md > 21 && *ad > 0)
	{
		*ad -= 1;
		*md -= 10;
	}
	delay(200);
	printf("The dealer has %d.\n\r", *md);
	if (*md < 17)
		*a = 6;
	else if (*md > 21 || *mi > *md)
	{
		delay(200);
		writeLine("You win!");
		*money += *dollars;
		*gameOver = true;
	}
	else if (*mi < *md)
	{
		delay(200);
		writeLine("You lose!");
		*money -= *dollars;
		*gameOver = true;
	}
	else if (*mi == *md)
	{
		delay(200);
		writeLine("Tie!");
		*gameOver = true;
	}
}

void checkHit(int* mi, int* md, int* yd, int* ym, int* ad, int* am, int* a, int* money, int* dollars, bool* gameOver)
{
	if (*mi > 21 && *am > 0)
	{
		*am -= 1;
		*mi -= 10;
	}
	delay(200);
	printf("Your total is %d.\n\r", *mi);
	if (*mi > 21)
	{
		delay(200);
		writeLine("Busted!");
		*money -= *dollars;
		*gameOver = true;
	}
	else if (*ym == 2 && (*mi == 21))
	{
		delay(200);
		writeLine("You got a ***BLACKJACK***");
		*money += *dollars + *dollars;
		*gameOver = true;
	}
	else if (*yd == 2 && (*md == 21))
	{
		delay(200);
		writeLine("The dealer has a ***BLACKJACK***");
		*money += *dollars - *dollars;
		*gameOver = true;
	}
	else
	{
		delay(200);
		printf("Would you like a hit?\n\r>");
		char answer = getOneOf('Y','N',0);
		if (answer == 'N')
			check(mi, md, ad, a, money, dollars, gameOver);
	}
}

void play21(int* money)
{
	bool answerOkay = false;
	int dollars;
	char dollarString[64];
	char *card;
	char answer = 'Y';
	do
	{
		do
		{
			answerOkay = false;
			printf("You have $%d00. How many would you like to bet?\n\r>", *money);
			getUserInput(dollarString, 64);
			dollars = atoi(dollarString);
			char *endBit = NULL;
			if (strlen(dollarString) > 2)
				endBit = dollarString + strlen(dollarString) - 2;
			if (dollars <= 0)
				writeLine("Huh?");
			else if (endBit == NULL || (endBit[0] != '0' || endBit[1] != '0'))
				writeLine("$100 increments only, please.");
			else if (dollars / 100 > *money)
				writeLine("You don't have that much.");
			else
				answerOkay = true;
		} while (!answerOkay);
		dollars /= 100;

		int mi = 0;
		int md = 0;
		int yd = 0;
		int ym = 0;
		int ad = 0;
		int am = 0;
		int a = 1;
		int y = 0;
		int z = 0;
		int ac;
		bool gameOver = false;
		do
		{
			dealCard(&y, &z, &ac, &card);
			delay(200);
			switch (a)
			{
			case 1:
			case 3:
			{
				mi += z;
				printf("You're dealt %s.\n\r", card);
				ym += y;
				am += ac;
				a++;
				break;
			}
			case 2:
			{
				md += z;
				printf("The dealer gets a card down.\n\r");
				yd += y;
				ad += ac;
				a++;
				break;
			}
			case 4:
			{
				md += z;
				printf("The dealer gets %s.\n\r", card);
				a = 5;
				ad += ac;
				yd += y;
				checkHit(&mi, &md, &yd, &ym, &ad, &am, &a, money, &dollars, &gameOver);
				break;
			}
			case 5:
			{
				mi += z;
				printf("You get %s.\n\r", card);
				am += ac;
				checkHit(&mi, &md, &yd, &ym, &ad, &am, &a, money, &dollars, &gameOver);
				break;
			}
			case 6:
			{
				md += z;
				printf("The dealer gets %s.\n\r", card);
				ad += ac;
				check(&mi, &md, &ad, &a, money, &dollars, &gameOver);
				break;
			}
			}
		} while (!gameOver);
		if (*money < 1)
		{
			writeLine("You're outta money! So long~");
			purgatory();
		}
		else
		{
			writeLine("Play again?");
			putchar('>');
			answer = getOneOf('Y', 'N', 0);
		}
	} while (answer == 'Y' && *money > 0);
}

void buyRubber()
{
	strcpy(state.rubberLubricated, "non-lubricated");
	strcpy(state.rubberRibbed, "smooth");
	writeLine("The man leans over the counter and whispers, \"What color, maaan?\"");
	putchar('>');
	getUserInput(state.rubberColor, 32);
	int i;
	for (i = 0; i < 32; i++)
		state.rubberColor[i] = (char)tolower(state.rubberColor[i]);
	writeLine("\"And what flavor?\"");
	putchar('>');
	getUserInput(state.rubberFlavor, 32);
	for (i = 0; i < 32; i++)
		state.rubberFlavor[i] = (char)tolower(state.rubberFlavor[i]);
	writeLine("\"Lubricated?\"");
	putchar('>');
	char yesNo = getOneOf('Y', 'N', 0);
	if (yesNo == 'Y')
		strcpy(state.rubberLubricated, "lubricated");
	writeLine("\"...Ribbed?\"");
	putchar('>');
	yesNo = getOneOf('Y', 'N', 0);
	if (yesNo == 'Y')
		strcpy(state.rubberRibbed, "ribbed");
	delay(1000);
	printf("Suddenly the clerk yells out, \"Hey everybody! This guy just bought\n\ra %s, %s-flavored, %s, %s rubber!\"\n\r", state.rubberColor, state.rubberFlavor, state.rubberLubricated, state.rubberRibbed);
	delay(1000);
	writeLine("\"WHAT A PERVERT!\"");
}

void okay()
{
	writeLine("Okay.");
}

void _open(bool* objectOpen)
{
	if (*objectOpen)
		writeLine("It's already open.");
	else
	{
		okay();
		*objectOpen = true;
	}
}

void _close(bool* objectOpen)
{
	if (!*objectOpen)
		writeLine("It's already closed.");
	else
	{
		okay();
		*objectOpen = false;
	}
}
