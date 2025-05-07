#pragma once

void PartsFourToSix()
{
	switch (state.verb)
	{
	case go:
	{
		if (state.tiedToBed)
			printText(0);
		else if (!haveNoDirection)
		{
			if (youAreIn(bBedRoom) && state.direction == north && !state.hookerFucked)
				printText(1);
			else if (youAreIn(cHallway) && state.direction == north && !state.marriedToGirl)
				printText(2);
			else if (youAreIn(bBackRoom) && state.direction == up && state.TVChannel != 6)
			{
				if (state.TVChannel != 5)
				{
					if (state.money < 20 || !objectIsCarried(wallet))
						printText(3);
					else if (state.hookerFucked)
						printText(4);
					else
					{
						printText(5);
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
			printText(6);
		else if (!(youAreIn(bStreet) || youAreIn(cStreet) || youAreIn(dStreet)))
			printText(7);
		else
		{
			writeLongMessage(36);

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
				printText(8);
			else if (objectIsCarried(wine))
			{
				wineInTaxi();
				state.yourPlace = newPlace;
				state.objectPlace[wine] = nowhere;
			}
			else
			{
				printText(9);
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
				printText(13);
		}
		else if (state.noun == off)
			printText(14);
		else if (state.noun == all)
		{
			printText(15);
			delay(300);
			for (int o = firstObject; o < lastObject; o++)
			{
				if (objectIsHere((objects)o))
				{
					printf("%s: ", objectNames[o]);
					if (state.objectsCarried >= maxCarried)
						printText(16);
					else if (objectCanBeTaken((objects)o))
					{
						if (youAreIn(dPharmacy) && (o == magazine || o == rubber))
						{
							printText(17);
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
							printText(18);
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
			printText(19);
		else if (!objectCanBeTaken(state.noun))
			cantDoThat();
		else if (youAreIn(dPharmacy) && (state.noun == magazine || state.noun == rubber))
		{
			printText(20);
			purgatory();
		}
		else if (state.noun == water && !objectIsCarried(pitcher))
			printText(21);
		else if (state.noun == candy && youAreIn(bBedRoom) && !state.hookerFucked)
			printText(22);
		else if (state.noun == rope && state.ropeInUse)
			printText(23);
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
				printText(25);
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
					printText(26);
					state.candyGiven = true;
					//Bug in original: should remove candy from play, just like candy and ring?
					state.objectPlace[candy] = nowhere;
					break;
				}
				case flowers:
				{
					printText(27);
					state.flowersGiven = true;
					state.objectPlace[flowers] = nowhere;
					break;
				}
				case ring:
				{
					printText(28);
					state.ringGiven = true;
					state.objectPlace[ring] = nowhere;
					break;
				}
				}
				if (state.candyGiven && state.flowersGiven && state.ringGiven)
				{
					printText(29);
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
					printText(30);
			}
			else if (objectIsHere(businessMan) && state.noun == whiskey && state.objectPlace[controlUnit] == nowhere)
			{
				printText(31);
				state.objectPlace[controlUnit] = state.yourPlace;
			}
			else if (objectIsHere(blonde) && state.noun == pills)
			{
				writeLongMessage(57);
				state.objectPlace[blonde] = nowhere;
				state.objectPlace[pills] = nowhere;
			}
			else if (state.noun == apple && youAreIn(pJacuzzi) && objectIsHere(girl))
			{
				writeLongMessage(50);
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
			writeLongMessage(state.yourPlace + 1); // WARNME: bug?
			noImAt = true;
		}
		else if (state.noun == all)
			printText(32);
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
					printText(33);
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
				printText(35);
				break;
			}
			case toilet:
			{
				printText(36);
				break;
			}
			case businessMan:
			{
				printText(37);
				break;
			}
			case button:
			{
				printText(38);
				break;
			}
			case bartender:
			{
				printText(39);
				break;
			}
			case pimp:
			{
				printText(40);
				break;
			}
			case hooker:
			{
				writeLongMessage(31);
				break;
			}
			case billboard:
			{
				writeLongMessage(63);
				break;
			}
			case TV:
			{
				//if (!objectIsCarried(controlUnit))
				//	printText(41);
				//else if (!state.hookerFucked)
				//	printText(42);
				//else
					watchTV();
				break;
			}
			case slotMachines:
			{
				printText(43);
				break;
			}
			case ashtray:
			{
				seeSomething(passcard, NULL);
				break;
			}
			case blonde:
			{
				writeLongMessage(40);
				break;
			}
			case bum:
			{
				printText(44);
				break;
			}
			case peephole:
			{
				if (state.holePeeped)
					printText(45);
				else
				{
					writeLongMessage(55);
					state.holePeeped = true;
				}
				break;
			}
			case doorWest:
			{
				if (state.doorWestOpen)
					printText(46);
				else
				{
					printText(47);
					printText(48);
				}
				break;
			}
			case waitress:
			{
				printText(49);
				break;
			}
			case telephone:
			{
				if (youAreIn(dBooth))
					printText(50);
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
				printText(52);
				break;
			}
			case elevator:
			{
				printText(53);
				break;
			}
			case dealer:
			{
				printText(54);
				break;
			}
			case cabinet:
			{
				if (state.stoolClimbed)
				{
					if (state.cabinetOpen)
						seeSomething(pitcher, loadText(55));
					else
						printText(56);
				}
				else
					seeNothingSpecial();
				break;
			}
			case bushes:
			{
				printText(57);
				break;
			}
			case tree:
			{
				seeSomething(apple, NULL);
				break;
			}
			case sign:
			{
				printText(58);
				break;
			}
			case girl:
			{
				if (youAreIn(pJacuzzi))
					writeLongMessage(35);
				else if (youAreIn(dDisco) || youAreIn(cChapel))
					writeLongMessage(34);
				else
					printText(59);
				break;
			}
			case newspaper:
			{
				if (objectIsCarried(newspaper))
					writeLongMessage(32);
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
				printText(60);
				break;
			}
			case appleCore:
			{
				seeSomething(seeds, NULL);
				break;
			}
			case pills:
			{
				printText(61);
				printText(62);
				break;
			}
			case plant:
			{
				if (state.objectPlace[bushes] == nowhere)
				{
					printText(63);
					state.objectPlace[bushes] = state.yourPlace;
				}
				else
					seeNothingSpecial();
				break;
			}
			case radio:
			{
				printText(64);
				break;
			}
			case magazine:
			{
				if (objectIsCarried(magazine))
					writeLongMessage(33);
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
					printText(67);
				break;
			}
			case doll:
			{
				if (state.dollInflated)
					printText(68);
				else
					printText(69);
				break;
			}
			case pitcher:
			{
				if (state.pitcherFull)
					printText(70);
				else
					printText(71);
				break;
			}
			case rack:
			{
				if (objectIsHere(rack))
					seeSomething(magazine, NULL);
				else if (objectIsHere(hooker))
					printText(72);
				else if (objectIsHere(girl) || objectIsHere(blonde))
					printText(73);
				break;
			}
			case curtain:
			{
				printText(74);
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
			writeLongMessage(69); //tsk! Nice! -- Kawa
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
			printText(75);
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
				printText(76);
			else
			{
				printText(77);
				if (objectIsCarried(passcard))
				{
					printText(78);
					state.doorWestOpen = true;
					state.path[dEntrance][west] = dDisco;
				}
				else
					printText(79);
			}
		}
		case curtain:
		{
			printText(80);
			break;
		}
		case elevator:
		{
			printText(81);
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
				printText(82);
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
					printText(83);
				else
				{
					okay();
					state.dollInflated = true;
				}
			}
			else if (objectIsHere(doll))
				printText(84);
			else
				findMeOne();
		}
		else
			printText(85);
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
				printText(86);
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
			printText(87);
		break;
	}
	case press: //push
	{
		if (state.noun == button)
		{
			if (youAreIn(bBar))
			{
				printText(88);
				putchar('>');
				char password[128] = { 0 };
				getString(password, 128);
				for (int i = 0; i < 6; i++)
					password[i] = (char)toupper(password[i]);
				password[6] = '\0';
				if (!strcmp(password, loadText(89)))
				{
					printText(90);
					state.path[bBar][east] = bBackRoom;
				}
				else if (!strcmp(password, loadText(91)) || !strcmp(password, loadText(92)))
					printText(93);
				else
					printText(94);
			}
			else if (youAreIn(cHtDesk) || youAreIn(pFoyer))
			{
				if (objectIsHere(blonde))
					printText(95);
				else
				{
					writeLongMessage(37);
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
			printText(96);
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
				printText(97);
		}
		else if (state.noun == elevator)
			printText(98);
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
			writeLongMessage(38);
			purgatory();
		}
		else if (state.noun == mushroom)
		{
			writeLongMessage(64);
			state.yourPlace = (places)getRandom(3);
			delay(600);
			state.placeVisited[state.yourPlace] = false;
		}
		else if (state.noun == garbage || state.noun == appleCore)
			printText(99);
		else if (state.noun == apple)
			printText(100);
		else if (state.noun == pills)
		{
			writeLongMessage(56);
			purgatory();
		}
		else
			printText(101);
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
		case whiskey: printText(102); break;
		case beer: printText(103); break;
		case wine: printText(104); break;
		case water: printText(105); break;
		default: printText(106); break;
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
					printText(107);
				else
				{
					printText(108);
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
					printText(109);
				else
				{
					printText(110);
					delay(3000);
					printText(111);
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
						printText(112);
					state.money--;
					state.objectPlace[state.noun] = youHaveIt;
				}
				else
					printText(113);
			}
			break;
		}
		case hooker:
		{
			if (objectIsHere(hooker))
			{
				if (state.paidPimp)
					printText(114);
				else
					printText(115);
			}
			else
				findMeOne();
			break;
		}
		default:
		{
			printText(116);
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
				printText(117);
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
				printText(118);
			else
			{
				state.waterOn = state.noun == on;
				if (state.waterOn)
				{
					printText(119);
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
			printText(120);
		else if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.noun == seeds)
		{
			state.objectPlace[water] = nowhere;
			state.pitcherFull = true;
			if (youAreIn(pGarden))
			{
				printText(121);
				state.objectPlace[tree] = state.yourPlace;
				state.objectPlace[seeds] = nowhere;
			}
			else
				printText(122);
		}
		else
		{
			printText(123);
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
			printText(124);
		else if (!objectIsHere(sink))
			printText(125);
		else if (!state.waterOn)
			printText(126);
		else if (state.pitcherFull)
			printText(127);
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
			printText(128);
		else if (!state.pitcherFull)
			printText(129);
		else if (!youAreIn(pGarden) || !objectIsHere(seeds))
			printText(130);
		else
		{
			printText(131);
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
					printText(132);
				else
				{
					printText(133);
					state.radioListened = true;
				}
			}
			else
				printText(134);
		}
		else
			printText(135);
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
				printText(136);
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
			printText(137);
		else if (!objectIsHere(girl))
			printText(138);
		else if (!youAreIn(cChapel))
			maybeLater();
		else if (state.money < 30 || !objectIsCarried(wallet))
		{
			if (state.money < 20)
				printText(139);
			else
				printText(140);
		}
		else
		{
			writeLongMessage(66);
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
				printText(141);
			else
			{
				if (state.rubberWorn)
				{
					state.hookerFucked = true;
					state.score++;
					writeLongMessage(51);
				}
				else
				{
					printText(142);
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
					writeLongMessage(52);
					state.objectPlace[doll] = nowhere;
					state.objectsCarried--;
				}
				else
					printText(143);
			}
			else
				printText(144);
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
					writeLongMessage(54);
					state.girl2Fucked = true;
					state.score++;
					state.tiedToBed = true;
					state.objectPlace[girl] = pJacuzzi;
					state.objectPlace[rope] = state.yourPlace;
				}
				else
					printText(145);
				break;
			}
			case pJacuzzi:
			{
				if (state.appleGiven)
				{
					state.score++;
					writeLongMessage(53);
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
			//printText(146);
			//printText(147);
			//purgatory();
			printText(148);
			printText(149);
			break;
		}
		case you:
		{
			printText(150);
			break;
		}
		case waitress:
		{
			printText(151);
			printText(152);
			break;
		}
		case blonde:
		{
			printText(153);
			break;
		}
		case pimp:
		{
			printText(154);
			break;
		}
		case bum:
		{
			printText(155);
			break;
		}
		case businessMan:
		{
			printText(156);
			break;
		}
		case off:
		{
			printText(157);
			break;
		}
		default:
		{
			printText(158);
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
			printText(159);
			state.rubberWorn = true;
			state.objectPlace[rubber] = youHaveIt;
			break;
		}
		case toilet:
		{
			printText(160);
			printText(161);
			break;
		}
		case bed:
		{
			printText(162);
			delay(1000);
			printText(163);
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
					printText(164);
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
					printText(165);
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
			printText(166);
			delay(600);
			if (objectIsCarried(knife))
			{
				if (state.tiedToBed)
				{
					printText(167);
					state.tiedToBed = false;
				}
				else
				{
					printText(168);
					delay(600);
					printText(169);
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
			printText(175);
		break;
	}
	case call: //dial
	{
		if (youAreIn(pPntPch))
			printText(176);
		//Bug in original: CALL 555-XXXX works everywhere *but* the penthouse foyer.
		else if (!objectIsHere(telephone))
			printText(177);
		else if (!strcmp(fullNoun, "6969") && !state.called5556969)
		{
			printText(178);
			printf(loadText(179)); getString(state.girlName,   32);
			printf(loadText(180)); getString(state.girlPart,   32);
			printf(loadText(181)); getString(state.girlDo,     32);
			printf(loadText(182)); getString(state.yourPart,   32);
			printf(loadText(183)); getString(state.yourObject, 32);
			printText(184);
			state.called5556969 = true;
		}
		else if (!strcmp(fullNoun, "0439") && !state.called5550439)
		{
			writeLongMessage(67);
			state.called5550439 = true;
		}
		else if (!strcmp(fullNoun, "0987") && state.marriedToGirl && !state.called5550987)
		{
			writeLongMessage(68);
			state.wineOrdered = true;
			state.called5550987 = true;
			state.objectPlace[wine] = cSuite;
		}
		else
		{
			printText(185);
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
				printText(186);
				state.path[bWindowLedge][south] = bInRoom;
			}
			else
				printText(187);
		}
		else
			cantDoThat();
		break;
	}
	case cut:
	{
		printText(188);
		delay(600);
		if (objectIsCarried(knife))
		{
			if (state.noun == rope && state.tiedToBed)
			{
				printText(189);
				state.tiedToBed = false;
			}
			else
			{
				printText(190);
				delay(600);
				printText(191);
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
			printText(192);
			delay(500);
			printText(193);
		}
		delay(1000);
		printText(194);
		break;
	}
	case kill:
	{
		//printText(195);
		printText(196);
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
				printText(197);
			else
				printText(198);
			break;
		}
		case hooker:
		{
			if (state.paidPimp)
				printText(199);
			else
				printText(200);
			break;
		}
		case blonde:
		case waitress:
		case girl:
		{
			printText(201);
			printText(202);
			purgatory();
			break;
		}
		case preacher:
		{
			printText(203);
			break;
		}
		case businessMan:
		{
			printText(204);
			break;
		}
		case bartender:
		{
			printText(205);
			break;
		}
		case dealer:
		{
			printText(206);
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
			printText(207);
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
					printText(208);
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
		case blonde: printText(209); break;
		case hooker: printText(210); break;
		case toilet: printText(211); break;
		case plant: printText(212); break;
		case garbage: printText(213); break;
		case flowers: printText(214); break;
		default: printText(215); break;
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
		printText(216);
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
		printText(217);
		break;
	}
	case save:
	{
		int slot = 0;
		printText(218);
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
// 		FILE *f = fopen(fileName, "wb+");
// 		if (f != 0)
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
		printText(222);
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
// 		FILE *f = fopen(fileName, "rb+");
// 		if (f != 0)
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