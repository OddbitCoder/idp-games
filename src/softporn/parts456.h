#pragma once

void PartsFourToSix()
{
	switch (state.verb)
	{
	case go:
	{
		if (state.tiedToBed)
			printTextLine(0);
		else if (!haveNoDirection)
		{
			if (youAreIn(bBedRoom) && state.direction == north && !state.hookerFucked)
				printTextLine(1);
			else if (youAreIn(cHallway) && state.direction == north && !state.marriedToGirl)
				printTextLine(2);
			else if (youAreIn(bBackRoom) && state.direction == up && state.TVChannel != 6)
			{
				if (state.TVChannel != 5)
				{
					if (state.money < 20 || !objectIsCarried(wallet))
						printTextLine(3);
					else if (state.hookerFucked)
						printTextLine(4);
					else
					{
						printTextLine(5);
						state.paidPimp = true;
						state.money -= 20;
						state.yourPlace = bBedRoom;
					}
				}
				else
				{ 
					state.yourPlace = bBedRoom;
				}
			}
			else if (youAreIn(bBalcony) && state.direction == west && !state.ropeInUse)
				fallingDown();
			else
			{
				places newPlace = (places)state.path[state.yourPlace][state.direction];
				if (newPlace != nowhere)
					state.yourPlace = newPlace;
				else
					cantGoThatWay();
			}
		}
		else
			cantDoThat();
		break;
	}
	case hail:
	{
		if (state.noun != taxi)
			printTextLine(6);
		else if (!(youAreIn(bStreet) || youAreIn(cStreet) || youAreIn(dStreet)))
			printTextLine(7);
		else
		{
			printLongMessageLine(36);

			char dest[128];
			putchar('>');
			getString(dest, 128);
			for (int i = 0; i < 4; i++)
				dest[i] = (char)toupper(dest[i]);
			dest[4] = 0;
			places newPlace = nowhere;
			if      (!strcmp(dest, "DISC")) newPlace = dStreet;
			else if (!strcmp(dest, "CASI")) newPlace = cStreet;
			else if (!strcmp(dest, "BAR" )) newPlace = bStreet;
			if (newPlace == nowhere || newPlace == state.yourPlace)
				printTextLine(8);
			else if (objectIsCarried(wine))
			{
				wineInTaxi();
				state.yourPlace = newPlace;
				state.objectPlace[wine] = nowhere;
			}
			else
			{
				printTextLine(9);
				state.yourPlace = newPlace;
			}
		}
		break;
	}
	case take: //get, grab
	{
		if (state.noun == inventory)
		{
			bool anythingCarried = false;
			for (int o = firstObject; o < lastObject; o++)
			{
				if (objectIsCarried((objects)o))
				{
					if (!anythingCarried)
						printf(loadText(10));
					anythingCarried = true;
					printf(loadText(11), objectNames[o]);
					if (o == wallet && state.money > 0)
						printf(loadText(12), state.money);
				}
			}
			if (anythingCarried)
				puts("");
			else
				printTextLine(13);
		}
		else if (state.noun == off)
			printTextLine(14);
		else if (state.noun == all)
		{
			printTextLine(15);
			delay(300);
			for (int o = firstObject; o < lastObject; o++)
			{
				if (objectIsHere((objects)o))
				{
					printf("%s: ", objectNames[o]);
					if (state.objectsCarried >= maxCarried)
						printTextLine(16);
					else if (objectCanBeTaken((objects)o))
					{
						if (youAreIn(dPharmacy) && (o == magazine || o == rubber))
						{
							printTextLine(17);
							purgatory();
						}
						else
						{
							state.objectPlace[o] = youHaveIt;
							state.objectsCarried++;
							if (o == water)
								state.pitcherFull = true;
							else if (o == pitcher && state.pitcherFull)
								state.objectPlace[water] = youHaveIt;
							printTextLine(18);
						}
					}
					else
						cantDoThat();
				}
			}
		}
		else if (objectIsCarried(state.noun))
			alreadyHaveIt();
		else if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.objectsCarried >= maxCarried)
			printTextLine(19);
		else if (!objectCanBeTaken(state.noun))
			cantDoThat();
		else if (youAreIn(dPharmacy) && (state.noun == magazine || state.noun == rubber))
		{
			printTextLine(20);
			purgatory();
		}
		else if (state.noun == water && !objectIsCarried(pitcher))
			printTextLine(21);
		else if (state.noun == candy && youAreIn(bBedRoom) && !state.hookerFucked)
			printTextLine(22);
		else if (state.noun == rope && state.ropeInUse)
			printTextLine(23);
		else
		{
			okay();
			state.objectPlace[state.noun] = youHaveIt;
			state.objectsCarried++;
			if (state.noun == water)
				state.pitcherFull = true;
			else if (state.noun == pitcher && state.pitcherFull)
				state.objectPlace[water] = youHaveIt;
		}
		break;
	}
	case drop: //leave, plant, give
	{
		if (state.noun == inventory || state.noun == taxi || state.noun == on || state.noun == off)
			huh();
		else if (state.noun == all)
		{
			bool anythingCarried = false;
			for (int o = firstObject; o < lastObject; o++)
			{
				if (objectIsCarried((objects)o))
				{
					anythingCarried = true;
					printf(loadText(24), objectNames[o]);
					state.objectPlace[o] = state.yourPlace;
					state.objectsCarried--;
					//Bug in original: DROP ALL won't synchronize things like rubberWorn.
					if (o == pitcher && state.pitcherFull)
						state.objectPlace[water] = state.yourPlace;
					else if (o == rubber)
						state.rubberWorn = false;
					//Anything given *to* others, like Fawn's gifts, we don't actually *give* even if we typed "give".
				}
			}
			if (!anythingCarried)
				printTextLine(25);
		}
		else if (!objectIsCarried(state.noun))
			dontHaveIt();
		else
		{
			state.objectPlace[state.noun] = state.yourPlace;
			state.objectsCarried--;
			if (state.noun == pitcher && state.pitcherFull)
				state.objectPlace[water] = state.yourPlace;
			else if (state.noun == rubber)
				state.rubberWorn = false;
			else if (youAreIn(dDisco) && objectIsHere(girl) && (state.noun == candy || state.noun == flowers || state.noun == ring))
			{
				switch (state.noun)
				{
				case candy:
				{
					printTextLine(26);
					state.candyGiven = true;
					//Bug in original: should remove candy from play, just like candy and ring?
					state.objectPlace[candy] = nowhere;
					break;
				}
				case flowers:
				{
					printTextLine(27);
					state.flowersGiven = true;
					state.objectPlace[flowers] = nowhere;
					break;
				}
				case ring:
				{
					printTextLine(28);
					state.ringGiven = true;
					state.objectPlace[ring] = nowhere;
					break;
				}
				}
				if (state.candyGiven && state.flowersGiven && state.ringGiven)
				{
					printTextLine(29);
					state.objectPlace[girl] = cChapel;
				}
			}
			else if (objectIsHere(bum) && state.noun == wine)
			{
				if (state.objectPlace[knife] == nowhere)
				{
					bumTellsStory();
					state.objectPlace[knife] = state.yourPlace;
				}
				else
					printTextLine(30);
			}
			else if (objectIsHere(businessMan) && state.noun == whiskey && state.objectPlace[controlUnit] == nowhere)
			{
				printTextLine(31);
				state.objectPlace[controlUnit] = state.yourPlace;
			}
			else if (objectIsHere(blonde) && state.noun == pills)
			{
				printLongMessageLine(57);
				state.objectPlace[blonde] = nowhere;
				state.objectPlace[pills] = nowhere;
			}
			else if (state.noun == apple && youAreIn(pJacuzzi) && objectIsHere(girl))
			{
				printLongMessageLine(50);
				state.appleGiven = true;
			}
			else
				okay();
		}
		break;
	}
	case look: //search, examine, read, watch
	{
		if (haveNoObject)
		{
			printLongMessageLine(state.yourPlace);
			noImAt = true;
		}
		else if (state.noun == all)
			printTextLine(32);
		else if (state.noun == inventory || state.noun == on || state.noun == off)
			huh();
		else if (!objectIsHere(state.noun) && !objectIsCarried(state.noun))
			findMeOne();
		else
		{
			switch (state.noun)
			{
			case desk:
			{
				if (state.drawerOpen)
					seeSomething(newspaper, NULL);
				else
					printTextLine(33);
				break;
			}
			case washBasin:
			{
				seeSomething(ring, loadText(34));
				break;
			}
			case graffiti:
			{
				lookAtGraffiti();
				break;
			}
			case mirror:
			{
				printTextLine(35);
				break;
			}
			case toilet:
			{
				printTextLine(36);
				break;
			}
			case businessMan:
			{
				printTextLine(37);
				break;
			}
			case button:
			{
				printTextLine(38);
				break;
			}
			case bartender:
			{
				printTextLine(39);
				break;
			}
			case pimp:
			{
				printTextLine(40);
				break;
			}
			case hooker:
			{
				printLongMessageLine(31);
				break;
			}
			case billboard:
			{
				printLongMessageLine(63);
				break;
			}
			case TV:
			{
				//if (!objectIsCarried(controlUnit))
				//	printTextLine(41);
				//else if (!state.hookerFucked)
				//	printTextLine(42);
				//else
					watchTV();
				break;
			}
			case slotMachines:
			{
				printTextLine(43);
				break;
			}
			case ashtray:
			{
				seeSomething(passcard, NULL);
				break;
			}
			case blonde:
			{
				printLongMessageLine(40);
				break;
			}
			case bum:
			{
				printTextLine(44);
				break;
			}
			case peephole:
			{
				if (state.holePeeped)
					printTextLine(45);
				else
				{
					printLongMessageLine(55);
					state.holePeeped = true;
				}
				break;
			}
			case doorWest:
			{
				if (state.doorWestOpen)
					printTextLine(46);
				else
				{
					printTextLine(47);
					printTextLine(48);
				}
				break;
			}
			case waitress:
			{
				printTextLine(49);
				break;
			}
			case telephone:
			{
				if (youAreIn(dBooth))
					printTextLine(50);
				else
					seeNothingSpecial();
				break;
			}
			case closet:
			{
				if (state.closetOpen)
					seeSomething(doll, loadText(51));
				break;
			}
			case sink:
			{
				printTextLine(52);
				break;
			}
			case elevator:
			{
				printTextLine(53);
				break;
			}
			case dealer:
			{
				printTextLine(54);
				break;
			}
			case cabinet:
			{
				if (state.stoolClimbed)
				{
					if (state.cabinetOpen)
						seeSomething(pitcher, loadText(55));
					else
						printTextLine(56);
				}
				else
					seeNothingSpecial();
				break;
			}
			case bushes:
			{
				printTextLine(57);
				break;
			}
			case tree:
			{
				seeSomething(apple, NULL);
				break;
			}
			case sign:
			{
				printTextLine(58);
				break;
			}
			case girl:
			{
				if (youAreIn(pJacuzzi))
					printLongMessageLine(35);
				else if (youAreIn(dDisco) || youAreIn(cChapel))
					printLongMessageLine(34);
				else
					printTextLine(59);
				break;
			}
			case newspaper:
			{
				if (objectIsCarried(newspaper))
					printLongMessageLine(32);
				else
					dontHaveIt();
				break;
			}
			case garbage:
			{
				seeSomething(appleCore, NULL);
				break;
			}
			case flowers:
			{
				printTextLine(60);
				break;
			}
			case appleCore:
			{
				seeSomething(seeds, NULL);
				break;
			}
			case pills:
			{
				printTextLine(61);
				printTextLine(62);
				break;
			}
			case plant:
			{
				if (state.objectPlace[bushes] == nowhere)
				{
					printTextLine(63);
					state.objectPlace[bushes] = state.yourPlace;
				}
				else
					seeNothingSpecial();
				break;
			}
			case radio:
			{
				printTextLine(64);
				break;
			}
			case magazine:
			{
				if (objectIsCarried(magazine))
					printLongMessageLine(33);
				else
					dontHaveIt();
				break;
			}
			case rubber:
			{
				if (objectIsCarried(rubber))
					printf(loadText(65), state.rubberColor, state.rubberFlavor, state.rubberLubricated, state.rubberRibbed);
				else
					dontHaveIt();
				break;
			}
			case wallet:
			{
				if (state.money > 0)
					printf(loadText(66), state.money);
				else
					printTextLine(67);
				break;
			}
			case doll:
			{
				if (state.dollInflated)
					printTextLine(68);
				else
					printTextLine(69);
				break;
			}
			case pitcher:
			{
				if (state.pitcherFull)
					printTextLine(70);
				else
					printTextLine(71);
				break;
			}
			case rack:
			{
				if (objectIsHere(rack))
					seeSomething(magazine, NULL);
				else if (objectIsHere(hooker))
					printTextLine(72);
				else if (objectIsHere(girl) || objectIsHere(blonde))
					printTextLine(73);
				break;
			}
			case curtain:
			{
				printTextLine(74);
				break;
			}
			default:
			{
				seeNothingSpecial();
				break;
			}
			}
		}
		break;
	}
	case flush:
	{
		if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.noun == toilet)
		{
			printLongMessageLine(69); //tsk! Nice! -- Kawa
			purgatory();
		}
		else
			cantDoThat();
		break;
	}
	case open: //pull
	{
		if (!objectIsHere(state.noun))
		{
			findMeOne();
			break;
		}
		switch (state.noun)
		{
		case window:
		{
			printTextLine(75);
			break;
		}
		case desk:
		{
			_open(&state.drawerOpen);
			break;
		}
		case doorWest:
		{
			if (state.doorWestOpen)
				printTextLine(76);
			else
			{
				printTextLine(77);
				if (objectIsCarried(passcard))
				{
					printTextLine(78);
					state.doorWestOpen = true;
					state.path[dEntrance][west] = dDisco;
				}
				else
					printTextLine(79);
			}
		}
		case curtain:
		{
			printTextLine(80);
			break;
		}
		case elevator:
		{
			printTextLine(81);
			break;
		}
		case closet:
		{
			_open(&state.closetOpen);
			break;
		}
		case cabinet:
		{
			if (state.stoolClimbed)
				_open(&state.cabinetOpen);
			else
				printTextLine(82);
		}
		default:
		{
			cantDoThat();
			break;
		}
		}
		break;
	}
	case inflate:
	{
		if (state.noun == doll)
		{
			if (objectIsCarried(doll))
			{
				if (state.dollInflated)
					printTextLine(83);
				else
				{
					okay();
					state.dollInflated = true;
				}
			}
			else if (objectIsHere(doll))
				printTextLine(84);
			else
				findMeOne();
		}
		else
			printTextLine(85);
		break;
	}
	case play:
	{
		if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.noun == slotMachines)
		{
			if (objectIsHere(slotMachines))
			{
				if (state.money > 0 && objectIsCarried(wallet))
					playSlots(&state.money);
				else
					noMoney();
			}
			else
				printTextLine(86);
		}
		else if (state.noun == cards)
		{
			if (youAreIn(c21Room))
			{
				if (state.money > 0 && objectIsCarried(wallet))
					; //play21(&state.money);
				else
					noMoney();
			}
			else
				maybeLater();
		}
		else
			printTextLine(87);
		break;
	}
	case press: //push
	{
		if (state.noun == button)
		{
			if (youAreIn(bBar))
			{
				printTextLine(88);
				putchar('>');
				char password[128] = { 0 };
				getString(password, 128);
				for (int i = 0; i < 6; i++)
					password[i] = (char)toupper(password[i]);
				password[6] = '\0';
				if (!strcmp(password, loadText(89)))
				{
					printTextLine(90);
					state.path[bBar][east] = bBackRoom;
				}
				else if (!strcmp(password, loadText(91)) || !strcmp(password, loadText(92)))
					printTextLine(93);
				else
					printTextLine(94);
			}
			else if (youAreIn(cHtDesk) || youAreIn(pFoyer))
			{
				if (objectIsHere(blonde))
					printTextLine(95);
				else
				{
					printLongMessageLine(37);
					if (youAreIn(cHtDesk))
						state.yourPlace = pFoyer;
					else
						state.yourPlace = cHtDesk;
				}
			}
			else
				maybeLater();
		}
		else
			printTextLine(96);
		break;
	}
	case enter:
	{
		if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.noun == bushes)
			state.yourPlace = pGarden;
		else if (state.noun == window)
		{
			if (state.windowBroken)
				state.yourPlace = bInRoom;
			else
				maybeLater();
		}
		else if (state.noun == doorWest)
		{
			if (state.doorWestOpen)
				state.yourPlace = dDisco;
			else
				printTextLine(97);
		}
		else if (state.noun == elevator)
			printTextLine(98);
		else
			cantDoThat();
		break;
	}
	case eat:
	{
		if (!objectIsHere(state.noun) && !objectIsCarried(state.noun))
			findMeOne();
		else if (state.noun == blonde || state.noun == waitress || state.noun == hooker || state.noun == girl)
		{
			printLongMessageLine(38);
			purgatory();
		}
		else if (state.noun == mushroom)
		{
			printLongMessageLine(64);
			state.yourPlace = (places)getRandom(3);
			delay(600);
			state.placeVisited[state.yourPlace] = false;
		}
		else if (state.noun == garbage || state.noun == appleCore)
			printTextLine(99);
		else if (state.noun == apple)
			printTextLine(100);
		else if (state.noun == pills)
		{
			printLongMessageLine(56);
			purgatory();
		}
		else
			printTextLine(101);
		break;
	}
	case drink:
	{
		if (!objectIsCarried(state.noun))
		{
			dontHaveIt();
			break;
		}
		switch (state.noun)
		{
		case whiskey: printTextLine(102); break;
		case beer: printTextLine(103); break;
		case wine: printTextLine(104); break;
		case water: printTextLine(105); break;
		default: printTextLine(106); break;
		}
		if (state.noun == beer || state.noun == water)
		{
			state.objectPlace[state.noun] = nowhere;
			state.objectsCarried--;
		}
		break;
	}
	case buy: //order
	{
		if (state.money < 1 || !objectIsCarried(wallet))
		{
			noMoney();
			break;
		}
		switch (state.noun)
		{
		case whiskey:
		case beer:
		{
			if (!youAreIn(bBar))
				maybeLater();
			else
			{
				if (state.objectPlace[state.noun] != nowhere)
					printTextLine(107);
				else
				{
					printTextLine(108);
					state.money--;
					state.objectPlace[state.noun] = state.yourPlace;
				}
			}
			break;
		}
		case wine:
		{
			if (!youAreIn(dDisco))
				maybeLater();
			else
			{
				if (state.objectPlace[state.noun] != nowhere)
					printTextLine(109);
				else
				{
					printTextLine(110);
					delay(3000);
					printTextLine(111);
					delay(2000);
					state.money--;
					state.objectPlace[state.noun] = state.yourPlace;
				}
			}
			break;
		}
		case rubber:
		case magazine:
		{
			if (!youAreIn(dPharmacy))
				maybeLater();
			else
			{
				if (objectIsHere(state.noun))
				{
					if (state.noun == rubber)
						buyRubber();
					else
						printTextLine(112);
					state.money--;
					state.objectPlace[state.noun] = youHaveIt;
				}
				else
					printTextLine(113);
			}
			break;
		}
		case hooker:
		{
			if (objectIsHere(hooker))
			{
				if (state.paidPimp)
					printTextLine(114);
				else
					printTextLine(115);
			}
			else
				findMeOne();
			break;
		}
		default:
		{
			printTextLine(116);
			break;
		}
		}
		break;
	}
	case climb:
	{
		if (state.noun == stool)
		{
			if (objectIsHere(stool))
			{
				okay();
				state.stoolClimbed = true;
			}
			else
				printTextLine(117);
		}
		else if (objectIsHere(state.noun) || objectIsCarried(state.noun))
			cantDoThat();
		else
			findMeOne();
		break;
	}
	case _water:
	{
		if (state.noun == on || state.noun == off)
		{
			if (!objectIsHere(sink))
				printTextLine(118);
			else
			{
				state.waterOn = state.noun == on;
				if (state.waterOn)
				{
					printTextLine(119);
					state.objectPlace[water] = state.yourPlace;
				}
				else if (!state.pitcherFull)
				{
					okay();
					state.objectPlace[water] = nowhere;
				}
			}
		}
		else if (!objectIsCarried(water))
			printTextLine(120);
		else if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.noun == seeds)
		{
			state.objectPlace[water] = nowhere;
			state.pitcherFull = true;
			if (youAreIn(pGarden))
			{
				printTextLine(121);
				state.objectPlace[tree] = state.yourPlace;
				state.objectPlace[seeds] = nowhere;
			}
			else
				printTextLine(122);
		}
		else
		{
			printTextLine(123);
			state.objectPlace[water] = nowhere;
			state.pitcherFull = false;
		}
		break;
	}
	case fill:
	{
		if (state.noun != pitcher)
			cantDoThat();
		else if (!objectIsCarried(pitcher))
			printTextLine(124);
		else if (!objectIsHere(sink))
			printTextLine(125);
		else if (!state.waterOn)
			printTextLine(126);
		else if (state.pitcherFull)
			printTextLine(127);
		else
		{
			okay();
			state.pitcherFull = true;
		}
		break;
	}
	case pour:
	{
		if (state.noun != water)
			cantDoThat();
		else if (!objectIsCarried(pitcher))
			printTextLine(128);
		else if (!state.pitcherFull)
			printTextLine(129);
		else if (!youAreIn(pGarden) || !objectIsHere(seeds))
			printTextLine(130);
		else
		{
			printTextLine(131);
			state.objectPlace[tree] = state.yourPlace;
			//Bug in original: WATER SEEDS removes seeds from play, but POUR WATER does not.
			state.objectPlace[seeds] = nowhere;
		}
		break;
	}
	case _listen:
	{
		if (!objectIsHere(state.noun) && !objectIsCarried(state.noun))
			findMeOne();
		else if (state.noun == radio)
		{
			if (objectIsCarried(radio))
			{
				if (state.radioListened)
					printTextLine(132);
				else
				{
					printTextLine(133);
					state.radioListened = true;
				}
			}
			else
				printTextLine(134);
		}
		else
			printTextLine(135);
		break;
	}
	case close:
	{
		if (!objectIsHere(state.noun))
		{
			findMeOne();
			break;
		}
		switch (state.noun)
		{
		case desk:
		{
			_close(&state.drawerOpen);
			if (objectIsHere(newspaper))
				state.objectPlace[newspaper] = nowhere;
			break;
		}
		case closet:
		{
			_close(&state.closetOpen);
			if (objectIsHere(doll))
				state.objectPlace[doll] = nowhere;
			break;
		}
		case cabinet:
		{
			if (state.stoolClimbed)
			{
				_close(&state.cabinetOpen);
				if (objectIsHere(pitcher))
					state.objectPlace[pitcher] = nowhere;
			}
			else
				printTextLine(136);
			break;
		}
		case doorWest:
		{
			_close(&state.doorWestOpen);
			state.path[dEntrance][west] = nowhere;
		}
		default:
		{
			cantDoThat();
			break;
		}
		}
		break;
	}
	case jump:
	{
		if (youAreIn(bBalcony) || youAreIn(bWindowLedge))
			fallingDown();
		else
			puts("Hup!");
		break;
	}
	case marry:
	{
		if (state.noun != girl)
			printTextLine(137);
		else if (!objectIsHere(girl))
			printTextLine(138);
		else if (!youAreIn(cChapel))
			maybeLater();
		else if (state.money < 30 || !objectIsCarried(wallet))
		{
			if (state.money < 20)
				printTextLine(139);
			else
				printTextLine(140);
		}
		else
		{
			printLongMessageLine(66);
			state.money -= 30;
			state.objectPlace[girl] = cSuite;
			state.marriedToGirl = true;
			state.path[cHallway][south] = cSuite;
		}
		break;
	}
	case fuck: //seduce, rape, screw
	{
		if (!objectIsHere(state.noun) && !objectIsCarried(state.noun) && state.noun != you)
		{
			findMeOne();
			break;
		}
		switch (state.noun)
		{
		case hooker:
		{
			if (state.hookerFucked)
				printTextLine(141);
			else
			{
				if (state.rubberWorn)
				{
					state.hookerFucked = true;
					state.score++;
					printLongMessageLine(51);
				}
				else
				{
					printTextLine(142);
					purgatory();
				}
			}
			break;
		}
		case doll:
		{
			if (objectIsCarried(doll))
			{
				if (state.dollInflated)
				{
					printLongMessageLine(52);
					state.objectPlace[doll] = nowhere;
					state.objectsCarried--;
				}
				else
					printTextLine(143);
			}
			else
				printTextLine(144);
			break;
		}
		case girl:
		{
			switch (state.yourPlace)
			{
			case cSuite:
			{
				if (state.wineOrdered)
				{
					printLongMessageLine(54);
					state.girl2Fucked = true;
					state.score++;
					state.tiedToBed = true;
					state.objectPlace[girl] = pJacuzzi;
					state.objectPlace[rope] = state.yourPlace;
				}
				else
					printTextLine(145);
				break;
			}
			case pJacuzzi:
			{
				if (state.appleGiven)
				{
					state.score++;
					printLongMessageLine(53);
					state.gameEnded = true;
				}
				else
					maybeLater();
				break;
			}
			default:
			{
				maybeLater();
				break;
			}
			}
			break;
		}
		case bartender:
		{
			//printTextLine(146);
			//printTextLine(147);
			//purgatory();
			printTextLine(148);
			printTextLine(149);
			break;
		}
		case you:
		{
			printTextLine(150);
			break;
		}
		case waitress:
		{
			printTextLine(151);
			printTextLine(152);
			break;
		}
		case blonde:
		{
			printTextLine(153);
			break;
		}
		case pimp:
		{
			printTextLine(154);
			break;
		}
		case bum:
		{
			printTextLine(155);
			break;
		}
		case businessMan:
		{
			printTextLine(156);
			break;
		}
		case off:
		{
			printTextLine(157);
			break;
		}
		default:
		{
			printTextLine(158);
			break;
		}
		}
		break;
	}
	case wear: //use
	{
		if (!objectIsHere(state.noun) && !objectIsCarried(state.noun) && state.noun != you)
		{
			findMeOne();
			break;
		}
		switch (state.noun)
		{
		case rubber:
		{
			printTextLine(159);
			state.rubberWorn = true;
			state.objectPlace[rubber] = youHaveIt;
			break;
		}
		case toilet:
		{
			printTextLine(160);
			printTextLine(161);
			break;
		}
		case bed:
		{
			printTextLine(162);
			delay(1000);
			printTextLine(163);
			break;
		}
		case rope:
		{
			if (objectIsCarried(rope))
			{
				if (youAreIn(bBalcony))
				{
					state.objectPlace[rope] = state.yourPlace;
					state.ropeInUse = true;
					printTextLine(164);
				}
				else
					maybeLater();
			}
			else
				dontHaveIt();
			break;
		}
		case passcard:
		{
			if (objectIsCarried(passcard))
			{
				if (youAreIn(dEntrance))
				{
					printTextLine(165);
					state.path[dEntrance][west] = dDisco;
				}
				else
					maybeLater();
			}
			else
				dontHaveIt();
			break;
		}
		case knife:
		{
			printTextLine(166);
			delay(600);
			if (objectIsCarried(knife))
			{
				if (state.tiedToBed)
				{
					printTextLine(167);
					state.tiedToBed = false;
				}
				else
				{
					printTextLine(168);
					delay(600);
					printTextLine(169);
					purgatory();
				}
			}
			else
				dontHaveIt();
			break;
		}
		default:
		{
			cantDoThat();
			break;
		}
		}
		break;
	}
	case answer:
	{
		if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.noun != telephone)
			cantDoThat();
		else if (state.telephoneRinging)
		{
			printf(loadText(170), state.girlName);
			printf(loadText(171), state.girlDo);
			printf(loadText(172), state.yourPart);
			printf(loadText(173), state.yourObject, state.girlPart);
			printf(loadText(174));
			state.telephoneRinging = false;
			state.telephoneAnswered = true;
		}
		else
			printTextLine(175);
		break;
	}
	case call: //dial
	{
		if (youAreIn(pPntPch))
			printTextLine(176);
		//Bug in original: CALL 555-XXXX works everywhere *but* the penthouse foyer.
		else if (!objectIsHere(telephone))
			printTextLine(177);
		else if (!strcmp(fullNoun, "6969") && !state.called5556969)
		{
			printTextLine(178);
			printf(loadText(179)); getString(state.girlName,   32);
			printf(loadText(180)); getString(state.girlPart,   32);
			printf(loadText(181)); getString(state.girlDo,     32);
			printf(loadText(182)); getString(state.yourPart,   32);
			printf(loadText(183)); getString(state.yourObject, 32);
			printTextLine(184);
			state.called5556969 = true;
		}
		else if (!strcmp(fullNoun, "0439") && !state.called5550439)
		{
			printLongMessageLine(67);
			state.called5550439 = true;
		}
		else if (!strcmp(fullNoun, "0987") && state.marriedToGirl && !state.called5550987)
		{
			printLongMessageLine(68);
			state.wineOrdered = true;
			state.called5550987 = true;
			state.objectPlace[wine] = cSuite;
		}
		else
		{
			printTextLine(185);
		}
		break;
	}
	case _break:
	{
		if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.noun == window)
		{
			if (objectIsCarried(hammer))
			{
				printTextLine(186);
				state.path[bWindowLedge][south] = bInRoom;
			}
			else
				printTextLine(187);
		}
		else
			cantDoThat();
		break;
	}
	case cut:
	{
		printTextLine(188);
		delay(600);
		if (objectIsCarried(knife))
		{
			if (state.noun == rope && state.tiedToBed)
			{
				printTextLine(189);
				state.tiedToBed = false;
			}
			else
			{
				printTextLine(190);
				delay(600);
				printTextLine(191);
				purgatory();
			}
		}
		else
			dontHaveIt();
		break;
	}
	case dance:
	{
		for (int i = 0; i < 3; i++)
		{
			delay(500);
			printTextLine(192);
			delay(500);
			printTextLine(193);
		}
		delay(1000);
		printTextLine(194);
		break;
	}
	case kill:
	{
		//printTextLine(195);
		printTextLine(196);
		break;
	}
	case pay:
	{
		if (!objectIsHere(state.noun))
		{
			findMeOne();
			break;
		}
		switch (state.noun)
		{
		case pimp:
		{
			if (state.hookerFucked)
				printTextLine(197);
			else
				printTextLine(198);
			break;
		}
		case hooker:
		{
			if (state.paidPimp)
				printTextLine(199);
			else
				printTextLine(200);
			break;
		}
		case blonde:
		case waitress:
		case girl:
		{
			printTextLine(201);
			printTextLine(202);
			purgatory();
			break;
		}
		case preacher:
		{
			printTextLine(203);
			break;
		}
		case businessMan:
		{
			printTextLine(204);
			break;
		}
		case bartender:
		{
			printTextLine(205);
			break;
		}
		case dealer:
		{
			printTextLine(206);
			break;
		}
		default:
		{
			cantDoThat();
			break;
		}
		}
		break;
	}
	case smoke:
	{
		if (state.noun == plant)
		{
			printTextLine(207);
			purgatory();
		}
		else
			cantDoThat();
		break;
	}
	case show:
	{
		if (state.noun == passcard)
		{
			if (objectIsCarried(passcard))
			{
				if (youAreIn(dEntrance))
				{
					printTextLine(208);
					state.path[dEntrance][west] = dDisco;
				}
				else
					maybeLater();
			}
			else
				dontHaveIt();
		}
		else
			cantDoThat();
		break;
	}
	case smell:
	{
		if (!objectIsHere(state.noun) && !objectIsCarried(state.noun))
		{
			findMeOne();
			break;
		}
		switch (state.noun)
		{
		case blonde: printTextLine(209); break;
		case hooker: printTextLine(210); break;
		case toilet: printTextLine(211); break;
		case plant: printTextLine(212); break;
		case garbage: printTextLine(213); break;
		case flowers: printTextLine(214); break;
		default: printTextLine(215); break;
		}
		break;
	}
	case help:
	{
		giveHelp();
		break;
	}
	case kiss:
	{
		printTextLine(216);
		break;
	}
	case stab:
	{
		stabSomeone();
		break;
	}
	case quit:
	{
		state.gameEnded = true;
		break;
	}
	case showScore:
	{
		printTextLine(217);
		break;
	}
	case save:
	{
		int slot = 0;
		printTextLine(218);
		while (true)
		{
			char resp = getKeySilent();
			if (resp >= '0' && resp <= '9')
			{
				slot = resp - '0';
				break;
			}
		}
		char fileName[24] = "SOFTP-#.SAV";
		fileName[6] = '0' + slot;
// WARNME
// #ifdef HAVESAFE
// 		FILE *f = NULL;
// 		if (fopen_s(&f, fileName, "wb+"))
// #else
// 		FILE *f = fopen(fileName, "wb+");
// 		if (f != 0)
// #endif
// 		{
// 			printf(loadText(220), fileName);
// 			break;
// 		}
// 		fwrite(&state, sizeof(gameState), 1, f);
// 		fclose(f);
		printf(loadText(221), fileName);
		break;
	}
	case restore: //load
	{
		int slot = 0;
		printTextLine(222);
		while (true)
		{
			char resp = getKeySilent();
			if (resp >= '0' && resp <= '9')
			{
				slot = resp - '0';
				break;
			}
		}
		char fileName[24] = "SOFTP-#.SAV";
		fileName[6] = '0' + slot;
// WARNME
// #ifdef HAVESAFE
// 		FILE *f = NULL;
// 		if (fopen_s(&f, fileName, "rb+"))
// #else
// 		FILE *f = fopen(fileName, "rb+");
// 		if (f != 0)
// #endif
// 		{
// 			printf(loadText(224), fileName);
// 			break;
// 		}
// 		fread(&state, sizeof(gameState), 1, f);
// 		fclose(f);
		printf(loadText(225), fileName);

		//force full description
		state.placeVisited[state.yourPlace] = false;
		break;
	}
	default:
	{
		cantDoThat();
		break;
	}
	}
}