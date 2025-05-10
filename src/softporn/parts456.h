#pragma once

void PartsFourToSix()
{
	switch (state.verb)
	{
	case go:
	{
		if (state.tiedToBed)
			writeTextLine(0);
		else if (!haveNoDirection)
		{
			if (youAreIn(bBedRoom) && state.direction == north && !state.hookerFucked)
				writeTextLine(1);
			else if (youAreIn(cHallway) && state.direction == north && !state.marriedToGirl)
				writeTextLine(2);
			else if (youAreIn(bBackRoom) && state.direction == up && state.TVChannel != 6)
			{
				if (state.TVChannel != 5)
				{
					if (state.money < 20 || !objectIsCarried(wallet))
						writeTextLine(3);
					else if (state.hookerFucked)
						writeTextLine(4);
					else
					{
						writeTextLine(5);
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
			writeTextLine(6);
		else if (!(youAreIn(bStreet) || youAreIn(cStreet) || youAreIn(dStreet)))
			writeTextLine(7);
		else
		{
			writeMessageLine(36);

			char dest[128];
			putchar('>');
			getUserInput(dest, 128);
			for (int i = 0; i < 4; i++)
				dest[i] = (char)toupper(dest[i]);
			dest[4] = 0;
			places newPlace = nowhere;
			if      (!strcmp(dest, "DISC")) newPlace = dStreet;
			else if (!strcmp(dest, "CASI")) newPlace = cStreet;
			else if (!strcmp(dest, "BAR" )) newPlace = bStreet;
			if (newPlace == nowhere || newPlace == state.yourPlace)
				writeTextLine(8);
			else if (objectIsCarried(wine))
			{
				wineInTaxi();
				state.yourPlace = newPlace;
				state.objectPlace[wine] = nowhere;
			}
			else
			{
				writeTextLine(9);
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
			for (int o = 0; o < objectCount; o++)
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
				writeLine("");
			else
				writeTextLine(13);
		}
		else if (state.noun == off)
			writeTextLine(14);
		else if (state.noun == all)
		{
			writeTextLine(15);
			delay(300);
			for (int o = 0; o < objectCount; o++)
			{
				if (objectIsHere((objects)o))
				{
					printf("%s: ", objectNames[o]);
					if (state.objectsCarried >= maxCarried)
						writeTextLine(16);
					else if (objectCanBeTaken((objects)o))
					{
						if (youAreIn(dPharmacy) && (o == magazine || o == rubber))
						{
							writeTextLine(17);
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
							writeTextLine(18);
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
			writeTextLine(19);
		else if (!objectCanBeTaken(state.noun))
			cantDoThat();
		else if (youAreIn(dPharmacy) && (state.noun == magazine || state.noun == rubber))
		{
			writeTextLine(20);
			purgatory();
		}
		else if (state.noun == water && !objectIsCarried(pitcher))
			writeTextLine(21);
		else if (state.noun == candy && youAreIn(bBedRoom) && !state.hookerFucked)
			writeTextLine(22);
		else if (state.noun == rope && state.ropeInUse)
			writeTextLine(23);
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
			for (int o = 0; o < objectCount; o++)
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
				writeTextLine(25);
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
					writeTextLine(26);
					state.candyGiven = true;
					//Bug in original: should remove candy from play, just like candy and ring?
					state.objectPlace[candy] = nowhere;
					break;
				}
				case flowers:
				{
					writeTextLine(27);
					state.flowersGiven = true;
					state.objectPlace[flowers] = nowhere;
					break;
				}
				case ring:
				{
					writeTextLine(28);
					state.ringGiven = true;
					state.objectPlace[ring] = nowhere;
					break;
				}
				}
				if (state.candyGiven && state.flowersGiven && state.ringGiven)
				{
					writeTextLine(29);
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
					writeTextLine(30);
			}
			else if (objectIsHere(businessMan) && state.noun == whiskey && state.objectPlace[controlUnit] == nowhere)
			{
				writeTextLine(31);
				state.objectPlace[controlUnit] = state.yourPlace;
			}
			else if (objectIsHere(blonde) && state.noun == pills)
			{
				writeMessageLine(57);
				state.objectPlace[blonde] = nowhere;
				state.objectPlace[pills] = nowhere;
			}
			else if (state.noun == apple && youAreIn(pJacuzzi) && objectIsHere(girl))
			{
				writeMessageLine(50);
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
			writeMessageLine(state.yourPlace + 1);
		}
		else if (state.noun == all)
			writeTextLine(32);
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
					writeTextLine(33);
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
				writeTextLine(35);
				break;
			}
			case toilet:
			{
				writeTextLine(36);
				break;
			}
			case businessMan:
			{
				writeTextLine(37);
				break;
			}
			case button:
			{
				writeTextLine(38);
				break;
			}
			case bartender:
			{
				writeTextLine(39);
				break;
			}
			case pimp:
			{
				writeTextLine(40);
				break;
			}
			case hooker:
			{
				writeMessageLine(31);
				break;
			}
			case billboard:
			{
				writeMessageLine(63);
				break;
			}
			case TV:
			{
				//if (!objectIsCarried(controlUnit))
				//	writeTextLine(41);
				//else if (!state.hookerFucked)
				//	writeTextLine(42);
				//else
					watchTV();
				break;
			}
			case slotMachines:
			{
				writeTextLine(43);
				break;
			}
			case ashtray:
			{
				seeSomething(passcard, NULL);
				break;
			}
			case blonde:
			{
				writeMessageLine(40);
				break;
			}
			case bum:
			{
				writeTextLine(44);
				break;
			}
			case peephole:
			{
				if (state.holePeeped)
					writeTextLine(45);
				else
				{
					writeMessageLine(55);
					state.holePeeped = true;
				}
				break;
			}
			case doorWest:
			{
				if (state.doorWestOpen)
					writeTextLine(46);
				else
				{
					writeTextLine(47);
					writeTextLine(48);
				}
				break;
			}
			case waitress:
			{
				writeTextLine(49);
				break;
			}
			case telephone:
			{
				if (youAreIn(dBooth))
					writeTextLine(50);
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
				writeTextLine(52);
				break;
			}
			case elevator:
			{
				writeTextLine(53);
				break;
			}
			case dealer:
			{
				writeTextLine(54);
				break;
			}
			case cabinet:
			{
				if (state.stoolClimbed)
				{
					if (state.cabinetOpen)
						seeSomething(pitcher, loadText(55));
					else
						writeTextLine(56);
				}
				else
					seeNothingSpecial();
				break;
			}
			case bushes:
			{
				writeTextLine(57);
				break;
			}
			case tree:
			{
				seeSomething(apple, NULL);
				break;
			}
			case sign:
			{
				writeTextLine(58);
				break;
			}
			case girl:
			{
				if (youAreIn(pJacuzzi))
					writeMessageLine(35);
				else if (youAreIn(dDisco) || youAreIn(cChapel))
					writeMessageLine(34);
				else
					writeTextLine(59);
				break;
			}
			case newspaper:
			{
				if (objectIsCarried(newspaper))
					writeMessageLine(32);
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
				writeTextLine(60);
				break;
			}
			case appleCore:
			{
				seeSomething(seeds, NULL);
				break;
			}
			case pills:
			{
				writeTextLine(61);
				writeTextLine(62);
				break;
			}
			case plant:
			{
				if (state.objectPlace[bushes] == nowhere)
				{
					writeTextLine(63);
					state.objectPlace[bushes] = state.yourPlace;
				}
				else
					seeNothingSpecial();
				break;
			}
			case radio:
			{
				writeTextLine(64);
				break;
			}
			case magazine:
			{
				if (objectIsCarried(magazine))
					writeMessageLine(33);
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
					writeTextLine(67);
				break;
			}
			case doll:
			{
				if (state.dollInflated)
					writeTextLine(68);
				else
					writeTextLine(69);
				break;
			}
			case pitcher:
			{
				if (state.pitcherFull)
					writeTextLine(70);
				else
					writeTextLine(71);
				break;
			}
			case rack:
			{
				if (objectIsHere(rack))
					seeSomething(magazine, NULL);
				else if (objectIsHere(hooker))
					writeTextLine(72);
				else if (objectIsHere(girl) || objectIsHere(blonde))
					writeTextLine(73);
				break;
			}
			case curtain:
			{
				writeTextLine(74);
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
			writeMessageLine(69); //tsk! Nice! -- Kawa
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
			writeTextLine(75);
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
				writeTextLine(76);
			else
			{
				writeTextLine(77);
				if (objectIsCarried(passcard))
				{
					writeTextLine(78);
					state.doorWestOpen = true;
					state.path[dEntrance][west] = dDisco;
				}
				else
					writeTextLine(79);
			}
		}
		case curtain:
		{
			writeTextLine(80);
			break;
		}
		case elevator:
		{
			writeTextLine(81);
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
				writeTextLine(82);
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
					writeTextLine(83);
				else
				{
					okay();
					state.dollInflated = true;
				}
			}
			else if (objectIsHere(doll))
				writeTextLine(84);
			else
				findMeOne();
		}
		else
			writeTextLine(85);
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
				writeTextLine(86);
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
			writeTextLine(87);
		break;
	}
	case press: //push
	{
		if (state.noun == button)
		{
			if (youAreIn(bBar))
			{
				writeTextLine(88);
				putchar('>');
				char password[128] = { 0 };
				getUserInput(password, 128);
				for (int i = 0; i < 6; i++)
					password[i] = (char)toupper(password[i]);
				password[6] = '\0';
				if (!strcmp(password, loadText(89)))
				{
					writeTextLine(90);
					state.path[bBar][east] = bBackRoom;
				}
				else if (!strcmp(password, loadText(91)) || !strcmp(password, loadText(92)))
					writeTextLine(93);
				else
					writeTextLine(94);
			}
			else if (youAreIn(cHtDesk) || youAreIn(pFoyer))
			{
				if (objectIsHere(blonde))
					writeTextLine(95);
				else
				{
					writeMessageLine(37);
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
			writeTextLine(96);
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
				writeTextLine(97);
		}
		else if (state.noun == elevator)
			writeTextLine(98);
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
			writeMessageLine(38);
			purgatory();
		}
		else if (state.noun == mushroom)
		{
			writeMessageLine(64);
			state.yourPlace = (places)getRandom(3);
			delay(600);
			state.placeVisited[state.yourPlace] = false;
		}
		else if (state.noun == garbage || state.noun == appleCore)
			writeTextLine(99);
		else if (state.noun == apple)
			writeTextLine(100);
		else if (state.noun == pills)
		{
			writeMessageLine(56);
			purgatory();
		}
		else
			writeTextLine(101);
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
		case whiskey: writeTextLine(102); break;
		case beer: writeTextLine(103); break;
		case wine: writeTextLine(104); break;
		case water: writeTextLine(105); break;
		default: writeTextLine(106); break;
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
					writeTextLine(107);
				else
				{
					writeTextLine(108);
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
					writeTextLine(109);
				else
				{
					writeTextLine(110);
					delay(3000);
					writeTextLine(111);
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
						writeTextLine(112);
					state.money--;
					state.objectPlace[state.noun] = youHaveIt;
				}
				else
					writeTextLine(113);
			}
			break;
		}
		case hooker:
		{
			if (objectIsHere(hooker))
			{
				if (state.paidPimp)
					writeTextLine(114);
				else
					writeTextLine(115);
			}
			else
				findMeOne();
			break;
		}
		default:
		{
			writeTextLine(116);
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
				writeTextLine(117);
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
				writeTextLine(118);
			else
			{
				state.waterOn = state.noun == on;
				if (state.waterOn)
				{
					writeTextLine(119);
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
			writeTextLine(120);
		else if (!objectIsHere(state.noun))
			findMeOne();
		else if (state.noun == seeds)
		{
			state.objectPlace[water] = nowhere;
			state.pitcherFull = true;
			if (youAreIn(pGarden))
			{
				writeTextLine(121);
				state.objectPlace[tree] = state.yourPlace;
				state.objectPlace[seeds] = nowhere;
			}
			else
				writeTextLine(122);
		}
		else
		{
			writeTextLine(123);
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
			writeTextLine(124);
		else if (!objectIsHere(sink))
			writeTextLine(125);
		else if (!state.waterOn)
			writeTextLine(126);
		else if (state.pitcherFull)
			writeTextLine(127);
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
			writeTextLine(128);
		else if (!state.pitcherFull)
			writeTextLine(129);
		else if (!youAreIn(pGarden) || !objectIsHere(seeds))
			writeTextLine(130);
		else
		{
			writeTextLine(131);
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
					writeTextLine(132);
				else
				{
					writeTextLine(133);
					state.radioListened = true;
				}
			}
			else
				writeTextLine(134);
		}
		else
			writeTextLine(135);
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
				writeTextLine(136);
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
			writeLine("Hup!");
		break;
	}
	case marry:
	{
		if (state.noun != girl)
			writeTextLine(137);
		else if (!objectIsHere(girl))
			writeTextLine(138);
		else if (!youAreIn(cChapel))
			maybeLater();
		else if (state.money < 30 || !objectIsCarried(wallet))
		{
			if (state.money < 20)
				writeTextLine(139);
			else
				writeTextLine(140);
		}
		else
		{
			writeMessageLine(66);
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
				writeTextLine(141);
			else
			{
				if (state.rubberWorn)
				{
					state.hookerFucked = true;
					state.score++;
					writeMessageLine(51);
				}
				else
				{
					writeTextLine(142);
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
					writeMessageLine(52);
					state.objectPlace[doll] = nowhere;
					state.objectsCarried--;
				}
				else
					writeTextLine(143);
			}
			else
				writeTextLine(144);
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
					writeMessageLine(54);
					state.girl2Fucked = true;
					state.score++;
					state.tiedToBed = true;
					state.objectPlace[girl] = pJacuzzi;
					state.objectPlace[rope] = state.yourPlace;
				}
				else
					writeTextLine(145);
				break;
			}
			case pJacuzzi:
			{
				if (state.appleGiven)
				{
					state.score++;
					writeMessageLine(53);
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
			//writeTextLine(146);
			//writeTextLine(147);
			//purgatory();
			writeTextLine(148);
			writeTextLine(149);
			break;
		}
		case you:
		{
			writeTextLine(150);
			break;
		}
		case waitress:
		{
			writeTextLine(151);
			writeTextLine(152);
			break;
		}
		case blonde:
		{
			writeTextLine(153);
			break;
		}
		case pimp:
		{
			writeTextLine(154);
			break;
		}
		case bum:
		{
			writeTextLine(155);
			break;
		}
		case businessMan:
		{
			writeTextLine(156);
			break;
		}
		case off:
		{
			writeTextLine(157);
			break;
		}
		default:
		{
			writeTextLine(158);
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
			writeTextLine(159);
			state.rubberWorn = true;
			state.objectPlace[rubber] = youHaveIt;
			break;
		}
		case toilet:
		{
			writeTextLine(160);
			writeTextLine(161);
			break;
		}
		case bed:
		{
			writeTextLine(162);
			delay(1000);
			writeTextLine(163);
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
					writeTextLine(164);
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
					writeTextLine(165);
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
			writeTextLine(166);
			delay(600);
			if (objectIsCarried(knife))
			{
				if (state.tiedToBed)
				{
					writeTextLine(167);
					state.tiedToBed = false;
				}
				else
				{
					writeTextLine(168);
					delay(600);
					writeTextLine(169);
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
			writeTextLine(175);
		break;
	}
	case call: //dial
	{
		if (youAreIn(pPntPch))
			writeTextLine(176);
		//Bug in original: CALL 555-XXXX works everywhere *but* the penthouse foyer.
		else if (!objectIsHere(telephone))
			writeTextLine(177);
		else if (!strcmp(fullNoun, "6969") && !state.called5556969)
		{
			writeTextLine(178);
			printf(loadText(179)); getUserInput(state.girlName,   32);
			printf(loadText(180)); getUserInput(state.girlPart,   32);
			printf(loadText(181)); getUserInput(state.girlDo,     32);
			printf(loadText(182)); getUserInput(state.yourPart,   32);
			printf(loadText(183)); getUserInput(state.yourObject, 32);
			writeTextLine(184);
			state.called5556969 = true;
		}
		else if (!strcmp(fullNoun, "0439") && !state.called5550439)
		{
			writeMessageLine(67);
			state.called5550439 = true;
		}
		else if (!strcmp(fullNoun, "0987") && state.marriedToGirl && !state.called5550987)
		{
			writeMessageLine(68);
			state.wineOrdered = true;
			state.called5550987 = true;
			state.objectPlace[wine] = cSuite;
		}
		else
		{
			writeTextLine(185);
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
				writeTextLine(186);
				state.path[bWindowLedge][south] = bInRoom;
			}
			else
				writeTextLine(187);
		}
		else
			cantDoThat();
		break;
	}
	case cut:
	{
		writeTextLine(188);
		delay(600);
		if (objectIsCarried(knife))
		{
			if (state.noun == rope && state.tiedToBed)
			{
				writeTextLine(189);
				state.tiedToBed = false;
			}
			else
			{
				writeTextLine(190);
				delay(600);
				writeTextLine(191);
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
			writeTextLine(192);
			delay(500);
			writeTextLine(193);
		}
		delay(1000);
		writeTextLine(194);
		break;
	}
	case kill:
	{
		//writeTextLine(195);
		writeTextLine(196);
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
				writeTextLine(197);
			else
				writeTextLine(198);
			break;
		}
		case hooker:
		{
			if (state.paidPimp)
				writeTextLine(199);
			else
				writeTextLine(200);
			break;
		}
		case blonde:
		case waitress:
		case girl:
		{
			writeTextLine(201);
			writeTextLine(202);
			purgatory();
			break;
		}
		case preacher:
		{
			writeTextLine(203);
			break;
		}
		case businessMan:
		{
			writeTextLine(204);
			break;
		}
		case bartender:
		{
			writeTextLine(205);
			break;
		}
		case dealer:
		{
			writeTextLine(206);
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
			writeTextLine(207);
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
					writeTextLine(208);
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
		case blonde: writeTextLine(209); break;
		case hooker: writeTextLine(210); break;
		case toilet: writeTextLine(211); break;
		case plant: writeTextLine(212); break;
		case garbage: writeTextLine(213); break;
		case flowers: writeTextLine(214); break;
		default: writeTextLine(215); break;
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
		writeTextLine(216);
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
		writeTextLine(217);
		break;
	}
	case save:
	{
		int slot = 0;
		writeTextLine(218);
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
		writeTextLine(222);
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