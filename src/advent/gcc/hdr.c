/*
 * hdr.c
 *
 * Miha Grčar 2022
 *
 * Stuff from hdr.h and glorkz
 */

#include "hdr.h"
#include "trav.h"

// WARNME: these need to be initialized in _init.c

int dseen[7], dloc[7], odloc[7];
int place[101], prop[101], link[201];
int atloc[LOCSIZ];
int fixed[101];
int hinted[20], hintlc[20];
int tk[21];
int abb[LOCSIZ];

// NOTE: these are initialized here (or don't need to be initialized)

/*
Text file contents (TEXT.BIN):
* Sect 1  ltext
* Sect 2  stext
* Sect 5  ptext
* Sect 6  rtext
* Sect 10 ctext
* Sect 12 mtext
*/

struct text ltext[LOCSIZ] = { // section 1
	/* 0 */ { 0, 0 },
	/* 1 */ { 0, 154 }, /* You are standing at the end of a road before a small brick building.
	Around you is a forest.  A small stream flows out of the building and
	down a gully. */
	/* 2 */ { 154, 141 }, /* You have walked up a hill, still in the forest.  The road slopes back
	down the other side of the hill.  There is a building in the distance. */
	/* 3 */ { 295, 59 }, /* You are inside a building, a well house for a large spring. */
	/* 4 */ { 354, 78 }, /* You are in a valley in the forest beside a stream tumbling along a
	rocky bed. */
	/* 5 */ { 432, 55 }, /* You are in open forest, with a deep valley to one side. */
	/* 6 */ { 487, 53 }, /* You are in open forest near both a valley and a road. */
	/* 7 */ { 540, 122 }, /* At your feet all the water of the stream splashes into a 2-inch slit
	in the rock.  Downstream the streambed is bare rock. */
	/* 8 */ { 662, 165 }, /* You are in a 20-foot depression floored with bare dirt.  Set into the
	dirt is a strong steel grate mounted in concrete.  A dry streambed
	leads into the depression. */
	/* 9 */ { 827, 120 }, /* You are in a small chamber beneath a 3x3 steel grate to the surface.
	A low crawl over cobbles leads inward to the west. */
	/* 10 */ { 947, 102 }, /* You are crawling over cobbles in a low passage.  There is a dim light
	at the east end of the passage. */
	/* 11 */ { 1049, 237 }, /* You are in a debris room filled with stuff washed in from the surface.
	A low wide passage with cobbles becomes plugged with mud and debris
	here, but an awkward canyon leads upward and west.  A note on the wall
	says "Magic word XYZZY". */
	/* 12 */ { 1286, 47 }, /* You are in an awkward sloping east/west canyon. */
	/* 13 */ { 1333, 179 }, /* You are in a splendid chamber thirty feet high.  The walls are frozen
	rivers of orange stone.  An awkward canyon and a good passage exit
	from east and west sides of the chamber. */
	/* 14 */ { 1512, 124 }, /* At your feet is a small pit breathing traces of white mist.  An east
	passage ends here except for a small crack leading on. */
	/* 15 */ { 1636, 340 }, /* You are at one end of a vast hall stretching forward out of sight to
	the west.  There are openings to either side.  Nearby, a wide stone
	staircase leads downward.  The hall is filled with wisps of white mist
	swaying to and fro almost as if alive.  A cold wind blows up the
	staircase.  There is a passage at the top of a dome behind you. */
	/* 16 */ { 1976, 45 }, /* The crack is far too small for you to follow. */
	/* 17 */ { 2021, 136 }, /* You are on the east bank of a fissure slicing clear across the hall.
	The mist is quite thick here, and the fissure is too wide to jump. */
	/* 18 */ { 2157, 99 }, /* This is a low room with a crude note on the wall.  The note says,
	"You won't get it up the steps". */
	/* 19 */ { 2256, 79 }, /* You are in the Hall of the Mountain King, with passages off in all
	directions. */
	/* 20 */ { 2335, 52 }, /* You are at the bottom of the pit with a broken neck. */
	/* 21 */ { 2387, 19 }, /* You didn't make it. */
	/* 22 */ { 2406, 24 }, /* The dome is unclimbable. */
	/* 23 */ { 2430, 118 }, /* You are at the west end of the Twopit Room.  There is a large hole in
	the wall above the pit at this end of the room. */
	/* 24 */ { 2548, 117 }, /* You are at the bottom of the eastern pit in the Twopit Room.  There is
	a small pool of oil in one corner of the pit. */
	/* 25 */ { 2665, 121 }, /* You are at the bottom of the western pit in the Twopit Room.  There is
	a large hole in the wall about 25 feet above you. */
	/* 26 */ { 2786, 64 }, /* You clamber up the plant and scurry through the hole at the top. */
	/* 27 */ { 2850, 61 }, /* You are on the west side of the fissure in the Hall of Mists. */
	/* 28 */ { 2911, 92 }, /* You are in a low N/S passage at a hole in the floor.  The hole goes
	down to an E/W passage. */
	/* 29 */ { 3003, 34 }, /* You are in the south side chamber. */
	/* 30 */ { 3037, 105 }, /* You are in the west side chamber of the Hall of the Mountain King.
	A passage continues west and up here. */
	/* 31 */ { 3142, 3 }, /* >$< */
	/* 32 */ { 3145, 27 }, /* You can't get by the snake. */
	/* 33 */ { 3172, 172 }, /* You are in a large room, with a passage to the south, a passage to the
	west, and a wall of broken rock to the east.  There is a large "Y2" on
	a rock in the room's center. */
	/* 34 */ { 3344, 52 }, /* You are in a jumble of rock, with cracks everywhere. */
	/* 35 */ { 3396, 476 }, /* You're at a low window overlooking a huge pit, which extends up out of
	sight.  A floor is indistinctly visible over 50 feet below.  Traces of
	white mist cover the floor of the pit, becoming thicker to the right.
	Marks in the dust around the window would seem to indicate that
	someone has been here recently.  Directly across the pit from you and
	25 feet away there is a similar window looking into a lighted room.  A
	shadowy figure can be seen there peering back at you. */
	/* 36 */ { 3872, 134 }, /* You are in a dirty broken passage.  To the east is a crawl.  To the
	west is a large passage.  Above you is a hole to another passage. */
	/* 37 */ { 4006, 74 }, /* You are on the brink of a small clean climbable pit.  A crawl leads
	west. */
	/* 38 */ { 4080, 102 }, /* You are in the bottom of a small pit with a little stream, which
	enters and exits through tiny slits. */
	/* 39 */ { 4182, 138 }, /* You are in a large room full of dusty rocks.  There is a big hole in
	the floor.  There are cracks everywhere, and a passage leading east. */
	/* 40 */ { 4320, 93 }, /* You have crawled through a very low wide passage parallel to and north
	of the Hall of Mists. */
	/* 41 */ { 4413, 156 }, /* You are at the west end of Hall of Mists.  A low wide crawl continues
	west and another goes north.  To the south is a little passage 6 feet
	off the floor. */
	/* 42 */ { 4569, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 43 */ { 4624, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 44 */ { 4679, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 45 */ { 4734, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 46 */ { 4789, 8 }, /* Dead end */
	/* 47 */ { 4797, 8 }, /* Dead end */
	/* 48 */ { 4805, 8 }, /* Dead end */
	/* 49 */ { 4813, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 50 */ { 4868, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 51 */ { 4923, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 52 */ { 4978, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 53 */ { 5033, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 54 */ { 5088, 8 }, /* Dead end */
	/* 55 */ { 5096, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 56 */ { 5151, 8 }, /* Dead end */
	/* 57 */ { 5159, 180 }, /* You are on the brink of a thirty foot pit with a massive orange column
	down one wall.  You could climb down here but you could not get back
	up.  The maze continues at this level. */
	/* 58 */ { 5339, 8 }, /* Dead end */
	/* 59 */ { 5347, 93 }, /* You have crawled through a very low wide passage parallel to and north
	of the Hall of Mists. */
	/* 60 */ { 5440, 169 }, /* You are at the east end of a very long hall apparently without side
	chambers.  To the east a low wide crawl slants up.  To the north a
	round two foot hole slants down. */
	/* 61 */ { 5609, 111 }, /* You are at the west end of a very long featureless hall.  The hall
	joins up with a narrow north/south passage. */
	/* 62 */ { 5720, 63 }, /* You are at a crossover of a high N/S passage and a low E/W one. */
	/* 63 */ { 5783, 8 }, /* Dead end */
	/* 64 */ { 5791, 209 }, /* You are at a complex junction.  A low hands and knees passage from the
	north joins a higher crawl from the east to make a walking passage
	going west.  There is also a large room above.  The air is damp here. */
	/* 65 */ { 6000, 124 }, /* You are in Bedquilt, a long east/west passage with holes everywhere.
	To explore at random select north, south, up, or down. */
	/* 66 */ { 6124, 155 }, /* You are in a room whose walls resemble Swiss cheese.  Obvious passages
	go west, east, NE, and NW.  Part of the room is occupied by a large
	bedrock block. */
	/* 67 */ { 6279, 334 }, /* You are at the east end of the Twopit Room.  The floor here is
	littered with thin rock slabs, which make it easy to descend the pits.
	There is a path here bypassing the pits to connect passages from east
	and west.  There are holes all over, but the only big one is on the
	wall directly over the west pit where you can't get to it. */
	/* 68 */ { 6613, 295 }, /* You are in a large low circular chamber whose floor is an immense slab
	fallen from the ceiling (Slab Room).  East and west there once were
	large passages, but they are now filled with boulders.  Low small
	passages go north and south, and the south one quickly bends west
	around the boulders. */
	/* 69 */ { 6908, 50 }, /* You are in a secret N/S canyon above a large room. */
	/* 70 */ { 6958, 55 }, /* You are in a secret N/S canyon above a sizable passage. */
	/* 71 */ { 7013, 143 }, /* You are in a secret canyon at a junction of three canyons, bearing
	north, south, and SE.  The north one is as tall as the other two
	combined. */
	/* 72 */ { 7156, 60 }, /* You are in a large low room.  Crawls lead north, SE, and SW. */
	/* 73 */ { 7216, 15 }, /* Dead end crawl. */
	/* 74 */ { 7231, 153 }, /* You are in a secret canyon which here runs e/w.  It crosses over a
	very tight canyon 15 feet below.  If you go down you may not be able
	to get back up. */
	/* 75 */ { 7384, 51 }, /* You are at a wide place in a very tight N/S canyon. */
	/* 76 */ { 7435, 54 }, /* The canyon here becomes too tight to go further south. */
	/* 77 */ { 7489, 89 }, /* You are in a tall E/W canyon.  A low tight crawl goes 3 feet north and
	seems to open up. */
	/* 78 */ { 7578, 52 }, /* The canyon runs into a mass of boulders -- dead end. */
	/* 79 */ { 7630, 107 }, /* The stream flows out through a pair of 1 foot diameter sewer pipes.
	It would be advisable to use the exit. */
	/* 80 */ { 7737, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 81 */ { 7792, 8 }, /* Dead end */
	/* 82 */ { 7800, 8 }, /* Dead end */
	/* 83 */ { 7808, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 84 */ { 7863, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 85 */ { 7918, 8 }, /* Dead end */
	/* 86 */ { 7926, 8 }, /* Dead end */
	/* 87 */ { 7934, 55 }, /* You are in a maze of twisty little passages, all alike. */
	/* 88 */ { 7989, 152 }, /* You are in a long, narrow corridor stretching out of sight to the
	west.  At the eastern end is a hole through which you can see a
	profusion of leaves. */
	/* 89 */ { 8141, 68 }, /* There is nothing here to climb.  Use "up" or "out" to leave the pit. */
	/* 90 */ { 8209, 49 }, /* You have climbed up the plant and out of the pit. */
	/* 91 */ { 8258, 173 }, /* You are at the top of a steep incline above a large room.  You could
	climb down here, but you would not be able to climb up.  There is a
	passage leading back to the north. */
	/* 92 */ { 8431, 208 }, /* You are in the Giant Room.  The ceiling here is too high up for your
	lamp to show it.  Cavernous passages lead east, north, and south.  On
	the west wall is scrawled the inscription, "Fee fie foe foo" [sic]. */
	/* 93 */ { 8639, 48 }, /* The passage here is blocked by a recent cave-in. */
	/* 94 */ { 8687, 53 }, /* You are at one end of an immense north/south passage. */
	/* 95 */ { 8740, 207 }, /* You are in a magnificent cavern with a rushing stream, which cascades
	over a sparkling waterfall into a roaring whirlpool which disappears
	through a hole in the floor.  Passages exit to the south and west. */
	/* 96 */ { 8947, 132 }, /* You are in the Soft Room.  The walls are covered with heavy curtains,
	the floor with a thick pile carpet.  Moss covers the ceiling. */
	/* 97 */ { 9079, 196 }, /* This is the Oriental Room.  Ancient oriental cave drawings cover the
	walls.  A gently sloping passage leads upward to the north, another
	passage leads SE, and a hands and knees crawl leads west. */
	/* 98 */ { 9275, 243 }, /* You are following a wide path around the outer edge of a large cavern.
	Far below, through a heavy white mist, strange splashing noises can be
	heard.  The mist rises up through a fissure in the ceiling.  The path
	exits to the south and west. */
	/* 99 */ { 9518, 201 }, /* You are in an alcove.  A small NW path seems to widen after a short
	distance.  An extremely tight tunnel leads east.  It looks like a very
	tight squeeze.  An eerie light can be seen at the other end. */
	/* 100 */ { 9719, 129 }, /* You're in a small chamber lit by an eerie green light.  An extremely
	narrow tunnel exits to the west.  A dark corridor leads ne. */
	/* 101 */ { 9848, 68 }, /* You're in the Dark-Room.  A corridor leading south is the only exit. */
	/* 102 */ { 9916, 142 }, /* You are in an arched hall.  A coral passage once continued up and east
	from here, but is now blocked by debris.  The air smells of sea water. */
	/* 103 */ { 10058, 263 }, /* You're in a large room carved out of sedimentary rock.  The floor and
	walls are littered with bits of shells embedded in the stone.  A
	shallow passage proceeds downward, and a somewhat steeper one leads
	up.  A low hands and knees passage enters from the south. */
	/* 104 */ { 10321, 59 }, /* You are in a long sloping corridor with ragged sharp walls. */
	/* 105 */ { 10380, 48 }, /* You are in a cul-de-sac about eight feet across. */
	/* 106 */ { 10428, 264 }, /* You are in an anteroom leading to a large passage to the east.  Small
	passages go west and up.  The remnants of recent digging are evident.
	A sign in midair here says "Cave under construction beyond this point.
	Proceed at own risk.  [Witt construction company]" */
	/* 107 */ { 10692, 59 }, /* You are in a maze of twisty little passages, all different. */
	/* 108 */ { 10751, 62 }, /* You are at Witt's End.  Passages lead off in *all* directions. */
	/* 109 */ { 10813, 474 }, /* You are in a north/south canyon about 25 feet across.  The floor is
	covered by white mist seeping in from the north.  The walls extend
	upward for well over 100 feet.  Suspended from some unseen point far
	above you, an enormous two-sided mirror is hanging parallel to and
	midway between the canyon walls.  (The mirror is obviously provided
	for the use of the dwarves, who as you know, are extremely vain.)  A
	small window can be seen in either wall, some fifty feet up. */
	/* 110 */ { 11287, 475 }, /* You're at a low window overlooking a huge pit, which extends up out of
	sight.  A floor is indistinctly visible over 50 feet below.  Traces of
	white mist cover the floor of the pit, becoming thicker to the left.
	Marks in the dust around the window would seem to indicate that
	someone has been here recently.  Directly across the pit from you and
	25 feet away there is a similar window looking into a lighted room.  A
	shadowy figure can be seen there peering back at you. */
	/* 111 */ { 11762, 204 }, /* A large stalactite extends from the roof and almost reaches the floor
	below.  You could climb down it, and jump from it to the floor, but
	having done so you would be unable to reach it to climb back up. */
	/* 112 */ { 11966, 61 }, /* You are in a little maze of twisting passages, all different. */
	/* 113 */ { 12027, 331 }, /* You are at the edge of a large underground reservoir.  An opaque cloud
	of white mist fills the room and rises rapidly upward.  The lake is
	fed by a stream, which tumbles out of a hole in the wall about 10 feet
	overhead and splashes noisily into the water somewhere within the
	mist.  The only passage goes back toward the south. */
	/* 114 */ { 12358, 8 }, /* Dead end */
	/* 115 */ { 12366, 750 }, /* You are at the northeast end of an immense room, even larger than the
	Giant Room.  It appears to be a repository for the "Adventure"
	program.  Massive torches far overhead bathe the room with smoky
	yellow light.  Scattered about you can be seen a pile of bottles (all
	of them empty), a nursery of young beanstalks murmuring quietly, a bed
	of oysters, a bundle of black rods with rusty stars on their ends, and
	a collection of brass lanterns.  Off to one side a great many dwarves
	are sleeping on the floor, snoring loudly.  A sign nearby reads: "Do
	not disturb the dwarves!"  An immense mirror is hanging against one
	wall, and stretches to the other end of the room, where various other
	sundry objects can be glimpsed dimly in the distance. */
	/* 116 */ { 13116, 504 }, /* You are at the southwest end of the Repository.  To one side is a pit
	full of fierce green snakes.  On the other side is a row of small
	wicker cages, each of which contains a little sulking bird.  In one
	corner is a bundle of black rods with rusty marks on their ends.  A
	large number of velvet pillows are scattered about on the floor.  A
	vast mirror stretches off to the northeast.  At your feet is a large
	steel grate, next to which is a sign which reads, "Treasure Vault.
	Keys in Main Office." */
	/* 117 */ { 13620, 183 }, /* You are on one side of a large, deep chasm.  A heavy white mist rising
	up from below obscures all view of the far side.  A SW path leads away
	from the chasm into a winding corridor. */
	/* 118 */ { 13803, 76 }, /* You are in a long winding corridor sloping out of sight in both
	directions. */
	/* 119 */ { 13879, 61 }, /* You are in a secret canyon which exits to the north and east. */
	/* 120 */ { 13940, 61 }, /* You are in a secret canyon which exits to the north and east. */
	/* 121 */ { 14001, 61 }, /* You are in a secret canyon which exits to the north and east. */
	/* 122 */ { 14062, 89 }, /* You are on the far side of the chasm.  A ne path leads away from the
	chasm on this side. */
	/* 123 */ { 14151, 91 }, /* You're in a long east/west corridor.  A faint rumbling noise can be
	heard in the distance. */
	/* 124 */ { 14242, 204 }, /* The path forks here.  The left fork leads northeast.  A dull rumbling
	seems to get louder in that direction.  The right fork leads southeast
	down a gentle slope.  The main corridor enters from the west. */
	/* 125 */ { 14446, 185 }, /* The walls are quite warm here.  From the north can be heard a steady
	roar, so loud that the entire cave seems to be trembling.  Another
	passage leads south, and a low crawl goes east. */
	/* 126 */ { 14631, 1310 }, /* You are on the edge of a breath-taking view.  Far below you is an
	active volcano, from which great gouts of molten lava come surging
	out, cascading back down into the depths.  The glowing rock fills the
	farthest reaches of the cavern with a blood-red glare, giving every-
	thing an eerie, macabre appearance.  The air is filled with flickering
	sparks of ash and a heavy smell of brimstone.  The walls are hot to
	the touch, and the thundering of the volcano drowns out all other
	sounds.  Embedded in the jagged roof far overhead are myriad twisted
	formations composed of pure white alabaster, which scatter the murky
	light into sinister apparitions upon the walls.  To one side is a deep
	gorge, filled with a bizarre chaos of tortured rock which seems to
	have been crafted by the devil himself.  An immense river of fire
	crashes out from the depths of the volcano, burns its way through the
	gorge, and plummets into a bottomless pit far off to your left.  To
	the right, an immense geyser of blistering steam erupts continuously
	from a barren island in the center of a sulfurous lake, which bubbles
	ominously.  The far right wall is aflame with an incandescence of its
	own, which lends an additional infernal splendor to the already
	hellish scene.  A dark, foreboding passage exits to the south. */
	/* 127 */ { 15941, 228 }, /* You are in a small chamber filled with large boulders.  The walls are
	very warm, causing the air in the room to be almost stifling from the
	heat.  The only exit is a crawl heading west, through which is coming
	a low rumbling. */
	/* 128 */ { 16169, 105 }, /* You are walking along a gently sloping north/south passage lined with
	oddly shaped limestone formations. */
	/* 129 */ { 16274, 126 }, /* You are standing at the entrance to a large, barren room.  A sign
	posted above the entrance reads:  "Caution!  Bear in room!" */
	/* 130 */ { 16400, 197 }, /* You are inside a barren room.  The center of the room is completely
	empty except for some dust.  Marks in the dust lead away toward the
	far end of the room.  The only exit is the way you came in. */
	/* 131 */ { 16597, 61 }, /* You are in a maze of twisting little passages, all different. */
	/* 132 */ { 16658, 59 }, /* You are in a little maze of twisty passages, all different. */
	/* 133 */ { 16717, 61 }, /* You are in a twisting maze of little passages, all different. */
	/* 134 */ { 16778, 61 }, /* You are in a twisting little maze of passages, all different. */
	/* 135 */ { 16839, 59 }, /* You are in a twisty little maze of passages, all different. */
	/* 136 */ { 16898, 59 }, /* You are in a twisty maze of little passages, all different. */
	/* 137 */ { 16957, 59 }, /* You are in a little twisty maze of passages, all different. */
	/* 138 */ { 17016, 61 }, /* You are in a maze of little twisting passages, all different. */
	/* 139 */ { 17077, 59 }, /* You are in a maze of little twisty passages, all different. */
	/* 140 */ { 17136, 8 } /* Dead end */
};

struct text stext[LOCSIZ] = { // section 2
	/* 0 */ { 0, 0 },
	/* 1 */ { 17144, 28 }, /* You're at end of road again. */
	/* 2 */ { 17172, 23 }, /* You're at hill in road. */
	/* 3 */ { 17195, 23 }, /* You're inside building. */
	/* 4 */ { 17218, 17 }, /* You're in valley. */
	/* 5 */ { 17235, 17 }, /* You're in forest. */
	/* 6 */ { 17252, 17 }, /* You're in forest. */
	/* 7 */ { 17269, 28 }, /* You're at slit in streambed. */
	/* 8 */ { 17297, 21 }, /* You're outside grate. */
	/* 9 */ { 17318, 23 }, /* You're below the grate. */
	/* 10 */ { 17341, 23 }, /* You're in Cobble Crawl. */
	/* 11 */ { 17364, 22 }, /* You're in Debris Room. */
	/* 12 */ { 0, 0 },
	/* 13 */ { 17386, 23 }, /* You're in Bird Chamber. */
	/* 14 */ { 17409, 27 }, /* You're at top of small pit. */
	/* 15 */ { 17436, 24 }, /* You're in Hall of Mists. */
	/* 16 */ { 0, 0 },
	/* 17 */ { 17460, 31 }, /* You're on east bank of fissure. */
	/* 18 */ { 17491, 30 }, /* You're in Nugget of Gold Room. */
	/* 19 */ { 17521, 26 }, /* You're in Hall of Mt King. */
	/* 20 */ { 0, 0 },
	/* 21 */ { 0, 0 },
	/* 22 */ { 0, 0 },
	/* 23 */ { 17547, 34 }, /* You're at west end of Twopit Room. */
	/* 24 */ { 17581, 19 }, /* You're in east pit. */
	/* 25 */ { 17600, 19 }, /* You're in west pit. */
	/* 26 */ { 0, 0 },
	/* 27 */ { 0, 0 },
	/* 28 */ { 0, 0 },
	/* 29 */ { 0, 0 },
	/* 30 */ { 0, 0 },
	/* 31 */ { 0, 0 },
	/* 32 */ { 0, 0 },
	/* 33 */ { 17619, 15 }, /* You're at "Y2". */
	/* 34 */ { 0, 0 },
	/* 35 */ { 17634, 24 }, /* You're at window on pit. */
	/* 36 */ { 17658, 24 }, /* You're in dirty passage. */
	/* 37 */ { 0, 0 },
	/* 38 */ { 0, 0 },
	/* 39 */ { 17682, 26 }, /* You're in Dusty Rock room. */
	/* 40 */ { 0, 0 },
	/* 41 */ { 17708, 36 }, /* You're at west end of Hall of Mists. */
	/* 42 */ { 0, 0 },
	/* 43 */ { 0, 0 },
	/* 44 */ { 0, 0 },
	/* 45 */ { 0, 0 },
	/* 46 */ { 0, 0 },
	/* 47 */ { 0, 0 },
	/* 48 */ { 0, 0 },
	/* 49 */ { 0, 0 },
	/* 50 */ { 0, 0 },
	/* 51 */ { 0, 0 },
	/* 52 */ { 0, 0 },
	/* 53 */ { 0, 0 },
	/* 54 */ { 0, 0 },
	/* 55 */ { 0, 0 },
	/* 56 */ { 0, 0 },
	/* 57 */ { 17744, 23 }, /* You're at brink of pit. */
	/* 58 */ { 0, 0 },
	/* 59 */ { 0, 0 },
	/* 60 */ { 17767, 32 }, /* You're at east end of Long Hall. */
	/* 61 */ { 17799, 32 }, /* You're at west end of Long Hall. */
	/* 62 */ { 0, 0 },
	/* 63 */ { 0, 0 },
	/* 64 */ { 17831, 27 }, /* You're at Complex Junction. */
	/* 65 */ { 0, 0 },
	/* 66 */ { 17858, 28 }, /* You're in Swiss Cheese room. */
	/* 67 */ { 17886, 34 }, /* You're at east end of Twopit Room. */
	/* 68 */ { 17920, 20 }, /* You're in Slab Room. */
	/* 69 */ { 0, 0 },
	/* 70 */ { 0, 0 },
	/* 71 */ { 17940, 43 }, /* You're at junction of three secret canyons. */
	/* 72 */ { 0, 0 },
	/* 73 */ { 0, 0 },
	/* 74 */ { 17983, 47 }, /* You're in secret E/W canyon above tight canyon. */
	/* 75 */ { 0, 0 },
	/* 76 */ { 0, 0 },
	/* 77 */ { 0, 0 },
	/* 78 */ { 0, 0 },
	/* 79 */ { 0, 0 },
	/* 80 */ { 0, 0 },
	/* 81 */ { 0, 0 },
	/* 82 */ { 0, 0 },
	/* 83 */ { 0, 0 },
	/* 84 */ { 0, 0 },
	/* 85 */ { 0, 0 },
	/* 86 */ { 0, 0 },
	/* 87 */ { 0, 0 },
	/* 88 */ { 18030, 26 }, /* You're in narrow corridor. */
	/* 89 */ { 0, 0 },
	/* 90 */ { 0, 0 },
	/* 91 */ { 18056, 41 }, /* You're at steep incline above large room. */
	/* 92 */ { 18097, 21 }, /* You're in Giant Room. */
	/* 93 */ { 0, 0 },
	/* 94 */ { 0, 0 },
	/* 95 */ { 18118, 32 }, /* You're in cavern with waterfall. */
	/* 96 */ { 18150, 20 }, /* You're in Soft Room. */
	/* 97 */ { 18170, 24 }, /* You're in Oriental Room. */
	/* 98 */ { 18194, 23 }, /* You're in Misty Cavern. */
	/* 99 */ { 18217, 17 }, /* You're in Alcove. */
	/* 100 */ { 18234, 22 }, /* You're in Plover Room. */
	/* 101 */ { 18256, 20 }, /* You're in Dark-Room. */
	/* 102 */ { 18276, 22 }, /* You're in Arched Hall. */
	/* 103 */ { 18298, 21 }, /* You're in Shell Room. */
	/* 104 */ { 0, 0 },
	/* 105 */ { 0, 0 },
	/* 106 */ { 18319, 19 }, /* You're in Anteroom. */
	/* 107 */ { 0, 0 },
	/* 108 */ { 18338, 21 }, /* You're at Witt's End. */
	/* 109 */ { 18359, 24 }, /* You're in Mirror Canyon. */
	/* 110 */ { 18383, 24 }, /* You're at window on pit. */
	/* 111 */ { 18407, 28 }, /* You're at top of stalactite. */
	/* 112 */ { 0, 0 },
	/* 113 */ { 18435, 20 }, /* You're at Reservoir. */
	/* 114 */ { 0, 0 },
	/* 115 */ { 18455, 17 }, /* You're at NE end. */
	/* 116 */ { 18472, 17 }, /* You're at SW end. */
	/* 117 */ { 18489, 27 }, /* You're on SW side of chasm. */
	/* 118 */ { 18516, 27 }, /* You're in sloping corridor. */
	/* 119 */ { 0, 0 },
	/* 120 */ { 0, 0 },
	/* 121 */ { 0, 0 },
	/* 122 */ { 18543, 27 }, /* You're on NE side of chasm. */
	/* 123 */ { 18570, 19 }, /* You're in corridor. */
	/* 124 */ { 18589, 23 }, /* You're at fork in path. */
	/* 125 */ { 18612, 35 }, /* You're at junction with warm walls. */
	/* 126 */ { 18647, 29 }, /* You're at Breath-taking View. */
	/* 127 */ { 18676, 30 }, /* You're in Chamber of Boulders. */
	/* 128 */ { 18706, 28 }, /* You're in limestone passage. */
	/* 129 */ { 18734, 31 }, /* You're in front of barren room. */
	/* 130 */ { 18765, 22 }, /* You're in Barren Room. */
	/* 131 */ { 0, 0 },
	/* 132 */ { 0, 0 },
	/* 133 */ { 0, 0 },
	/* 134 */ { 0, 0 },
	/* 135 */ { 0, 0 },
	/* 136 */ { 0, 0 },
	/* 137 */ { 0, 0 },
	/* 138 */ { 0, 0 },
	/* 139 */ { 0, 0 },
	/* 140 */ { 0, 0 }
};

struct text ptext[101] = { // section 5
	/* 0 */ { 0, 0 },
	/* 1 */ { 18787, 56 }, /* Set of keys
	000     There are some keys on the ground here. */
	/* 2 */ { 18843, 91 }, /* Brass lantern
	000     There is a shiny brass lamp nearby.
	100     There is a lamp shining nearby. */
	/* 3 */ { 18934, 56 }, /* *Grate
	000     The grate is locked.
	100     The grate is open. */
	/* 4 */ { 18990, 63 }, /* Wicker cage
	000     There is a small wicker cage discarded nearby. */
	/* 5 */ { 19053, 78 }, /* Black rod
	000     A three foot black rod with a rusty star on an end lies nearby. */
	/* 6 */ { 19131, 78 }, /* Black rod
	000     A three foot black rod with a rusty mark on an end lies nearby. */
	/* 7 */ { 19209, 89 }, /* *Steps
	000     Rough stone steps lead down the pit.
	100     Rough stone steps lead up the dome. */
	/* 8 */ { 19298, 113 }, /* Little bird in cage
	000     A cheerful little bird is sitting here singing.
	100     There is a little bird in the cage. */
	/* 9 */ { 19411, 134 }, /* *Rusty door
	000     The way north is barred by a massive, rusty, iron door.
	100     The way north leads through a massive, rusty, iron door. */
	/* 10 */ { 19545, 59 }, /* Velvet pillow
	000     A small velvet pillow lies on the floor. */
	/* 11 */ { 19604, 75 }, /* *Snake
	000     A huge green fierce snake bars the way!
	100     >$<  (Chased away) */
	/* 12 */ { 19679, 100 }, /* *Fissure
	000     >$<
	100     A crystal bridge now spans the fissure.
	200     The crystal bridge has vanished! */
	/* 13 */ { 19779, 130 }, /* *Stone tablet
	000     A massive stone tablet embedded in the wall reads:
	000     "Congratulations on bringing light into the Dark-Room!" */
	/* 14 */ { 19909, 87 }, /* Giant clam  >grunt!<
	000     There is an enormous clam here with its shell tightly closed. */
	/* 15 */ { 19996, 183 }, /* Giant oyster  >groan!<
	000     There is an enormous oyster here with its shell tightly closed.
	100     Interesting.  There seems to be something written on the underside of
	100     The oyster. */
	/* 16 */ { 20179, 88 }, /* "Spelunker Today"
	000     There are a few recent issues of "Spelunker Today" magazine here. */
	/* 17 */ { 0, 0 },
	/* 18 */ { 0, 0 },
	/* 19 */ { 20267, 35 }, /* Tasty food
	000     There is food here. */
	/* 20 */ { 20302, 122 }, /* Small bottle
	000     There is a bottle of water here.
	100     There is an empty bottle here.
	200     There is a bottle of oil here. */
	/* 21 */ { 20424, 19 }, /* Water in the bottle */
	/* 22 */ { 20443, 17 }, /* Oil in the bottle */
	/* 23 */ { 20460, 16 }, /* *Mirror
	000     >$< */
	/* 24 */ { 20476, 463 }, /* *Plant
	000     There is a tiny little plant in the pit, murmuring "Water, water, ..."
	100     The plant spurts into furious growth for a few seconds.
	200     There is a 12-foot-tall beanstalk stretching up out of the pit,
	200     bellowing "WATER!! WATER!!"
	300     The plant grows explosively, almost filling the bottom of the pit.
	400     There is a gigantic beanstalk stretching all the way up to the hole.
	500     You've over-watered the plant!  It's shriveling up!  It's, it's... */
	/* 25 */ { 20939, 212 }, /* *Phony plant (seen in twopit room only when tall enough)
	000     >$<
	100     The top of a 12-foot-tall beanstalk is poking out of the west pit.
	200     There is a huge beanstalk growing out of the west pit up to the hole. */
	/* 26 */ { 21151, 20 }, /* *Stalactite
	000     >$< */
	/* 27 */ { 21171, 85 }, /* *Shadowy figure
	000     The shadowy figure seems to be trying to attract your attention. */
	/* 28 */ { 21256, 94 }, /* Dwarf's axe
	000     There is a little axe here.
	100     There is a little axe lying beside the bear. */
	/* 29 */ { 21350, 23 }, /* *Cave drawings
	000     >$< */
	/* 30 */ { 21373, 16 }, /* *Pirate
	000     >$< */
	/* 31 */ { 21389, 232 }, /* *Dragon
	000     A huge green fierce dragon bars the way!
	100     Congratulations!  You have just vanquished a dragon with your bare
	100     hands!  (Unbelievable, isn't it?)
	200     The body of a huge green dead dragon is lying off to one side. */
	/* 32 */ { 21621, 239 }, /* *Chasm
	000     A rickety wooden bridge extends across the chasm, vanishing into the
	000     mist.  A sign posted on the bridge reads, "Stop! Pay troll!"
	100     The wreckage of a bridge (and a dead bear) can be seen at the bottom
	100     of the chasm. */
	/* 33 */ { 21860, 204 }, /* *Troll
	000     A burly troll stands by the bridge and insists you throw him a
	000     treasure before you may cross.
	100     The troll steps out from beneath the bridge and blocks your way.
	200     >$<  (Chased away) */
	/* 34 */ { 22064, 50 }, /* *Phony troll
	000     The troll is nowhere to be seen. */
	/* 35 */ { 22114, 247 }, /* >$<  (Bear uses rtext 141)
	000     There is a ferocious cave bear eying you from the far end of the room!
	100     There is a gentle cave bear sitting placidly in one corner.
	200     There is a contented-looking bear wandering about nearby.
	300     >$<  (Dead) */
	/* 36 */ { 22361, 170 }, /* *Message in second maze
	000     There is a message scrawled in the dust in a flowery script, reading:
	000     "This is not the maze where the pirate leaves his treasure chest." */
	/* 37 */ { 22531, 31 }, /* *Volcano and/or geyser
	000     >$< */
	/* 38 */ { 22562, 143 }, /* *Vending machine
	000     There is a massive vending machine here.  The instructions on it read:
	000     "Drop coins here to receive fresh batteries." */
	/* 39 */ { 22705, 103 }, /* Batteries
	000     There are fresh batteries here.
	100     Some worn-out batteries have been discarded nearby. */
	/* 40 */ { 22808, 28 }, /* *Carpet and/or moss
	000     >$< */
	/* 41 */ { 0, 0 },
	/* 42 */ { 0, 0 },
	/* 43 */ { 0, 0 },
	/* 44 */ { 0, 0 },
	/* 45 */ { 0, 0 },
	/* 46 */ { 0, 0 },
	/* 47 */ { 0, 0 },
	/* 48 */ { 0, 0 },
	/* 49 */ { 0, 0 },
	/* 50 */ { 22836, 70 }, /* Large gold nugget
	000     There is a large sparkling nugget of gold here! */
	/* 51 */ { 22906, 46 }, /* Several diamonds
	000     There are diamonds here! */
	/* 52 */ { 22952, 50 }, /* Bars of silver
	000     There are bars of silver here! */
	/* 53 */ { 23002, 53 }, /* Precious jewelry
	000     There is precious jewelry here! */
	/* 54 */ { 23055, 42 }, /* Rare coins
	000     There are many coins here! */
	/* 55 */ { 23097, 56 }, /* Treasure chest
	000     The pirate's treasure chest is here! */
	/* 56 */ { 23153, 119 }, /* Golden eggs
	000     There is a large nest here, full of golden eggs!
	100     The nest of golden eggs has vanished!
	200     Done! */
	/* 57 */ { 23272, 61 }, /* Jeweled trident
	000     There is a jewel-encrusted trident here! */
	/* 58 */ { 23333, 232 }, /* Ming vase
	000     There is a delicate, precious, Ming vase here!
	100     The vase is now resting, delicately, on a velvet pillow.
	200     The floor is littered with worthless shards of pottery.
	300     The Ming vase drops with a delicate crash. */
	/* 59 */ { 23565, 75 }, /* Egg-sized emerald
	000     There is an emerald here the size of a plover's egg! */
	/* 60 */ { 23640, 75 }, /* Platinum pyramid
	000     There is a platinum pyramid here, 8 inches on a side! */
	/* 61 */ { 23715, 62 }, /* Glistening pearl
	000     Off to one side lies a glistening pearl! */
	/* 62 */ { 23777, 115 }, /* Persian rug
	000     There is a Persian rug spread out on the floor!
	100     The dragon is sprawled out on a Persian rug!! */
	/* 63 */ { 23892, 44 }, /* Rare spices
	000     There are rare spices here! */
	/* 64 */ { 23936, 177 }, /* Golden chain
	000     There is a golden chain lying in a heap on the floor!
	100     The bear is locked to the wall with a golden chain!
	200     There is a golden chain locked to the wall! */
	/* 65 */ { 0, 0 },
	/* 66 */ { 0, 0 },
	/* 67 */ { 0, 0 },
	/* 68 */ { 0, 0 },
	/* 69 */ { 0, 0 },
	/* 70 */ { 0, 0 },
	/* 71 */ { 0, 0 },
	/* 72 */ { 0, 0 },
	/* 73 */ { 0, 0 },
	/* 74 */ { 0, 0 },
	/* 75 */ { 0, 0 },
	/* 76 */ { 0, 0 },
	/* 77 */ { 0, 0 },
	/* 78 */ { 0, 0 },
	/* 79 */ { 0, 0 },
	/* 80 */ { 0, 0 },
	/* 81 */ { 0, 0 },
	/* 82 */ { 0, 0 },
	/* 83 */ { 0, 0 },
	/* 84 */ { 0, 0 },
	/* 85 */ { 0, 0 },
	/* 86 */ { 0, 0 },
	/* 87 */ { 0, 0 },
	/* 88 */ { 0, 0 },
	/* 89 */ { 0, 0 },
	/* 90 */ { 0, 0 },
	/* 91 */ { 0, 0 },
	/* 92 */ { 0, 0 },
	/* 93 */ { 0, 0 },
	/* 94 */ { 0, 0 },
	/* 95 */ { 0, 0 },
	/* 96 */ { 0, 0 },
	/* 97 */ { 0, 0 },
	/* 98 */ { 0, 0 },
	/* 99 */ { 0, 0 },
	/* 100 */ { 0, 0 }
};

struct text rtext[RTXSIZ] = { // section 6
	/* 0 */ { 0, 0 },
	/* 1 */ { 24113, 791 }, /* Somewhere nearby is Colossal Cave, where others have found fortunes in
	treasure and gold, though it is rumored that some who enter are never
	seen again.  Magic is said to work in the cave.  I will be your eyes
	and hands.  Direct me with commands of 1 or 2 words.  I should warn
	you that I look at only the first five letters of each word, so you'll
	have to enter "northeast" as "ne" to distinguish it from "north".
	(Should you get stuck, type "help" for some general hints.  For
	information on how to end your adventure, etc., type "info".)
	- - -
	This program was originally developed by Will Crowther.  Most of the
	features of the current program were added by Don Woods.  This
	Iskra Delta Partner port is based on the UNIX version by Jim Gillogly.
	Ported by Miha Grcar in 2022. */
	/* 2 */ { 24904, 48 }, /* A little dwarf with a big knife blocks your way. */
	/* 3 */ { 24952, 117 }, /* A little dwarf just walked around a corner, saw you, threw a little
	axe at you (which missed), cursed, and ran away. */
	/* 4 */ { 25069, 57 }, /* There is a threatening little dwarf in the room with you! */
	/* 5 */ { 25126, 39 }, /* One sharp nasty knife is thrown at you! */
	/* 6 */ { 25165, 21 }, /* None of them hit you! */
	/* 7 */ { 25186, 21 }, /* One of them gets you! */
	/* 8 */ { 25207, 28 }, /* A hollow voice says "Plugh". */
	/* 9 */ { 25235, 37 }, /* There is no way to go that direction. */
	/* 10 */ { 25272, 70 }, /* I am unsure how you are facing.  Use compass points or nearby objects. */
	/* 11 */ { 25342, 110 }, /* I don't know in from out here.  Use compass points or name something
	in the general direction you want to go. */
	/* 12 */ { 25452, 41 }, /* I don't know how to apply that word here. */
	/* 13 */ { 25493, 24 }, /* I don't understand that! */
	/* 14 */ { 25517, 41 }, /* I'm game.  Would you care to explain how? */
	/* 15 */ { 25558, 103 }, /* Sorry, but I am not allowed to give more detail.  I will repeat the
	long description of your location. */
	/* 16 */ { 25661, 70 }, /* It is now pitch dark.  If you proceed you will likely fall into a pit. */
	/* 17 */ { 25731, 46 }, /* If you prefer, simply type w rather than west. */
	/* 18 */ { 25777, 33 }, /* Are you trying to catch the bird? */
	/* 19 */ { 25810, 111 }, /* The bird is frightened right now and you cannot catch it no matter
	what you try.  Perhaps you might try later. */
	/* 20 */ { 25921, 46 }, /* Are you trying to somehow deal with the snake? */
	/* 21 */ { 25967, 162 }, /* You can't kill the snake, or drive it away, or avoid it, or anything
	like that.  There is a way to get by, but you don't have the necessary
	resources right now. */
	/* 22 */ { 26129, 31 }, /* Do you really want to quit now? */
	/* 23 */ { 26160, 54 }, /* You fell into a pit and broke every bone in your body! */
	/* 24 */ { 26214, 28 }, /* You are already carrying it! */
	/* 25 */ { 26242, 21 }, /* You can't be serious! */
	/* 26 */ { 26263, 106 }, /* The bird was unafraid when you entered, but as you approach it becomes
	disturbed and you cannot catch it. */
	/* 27 */ { 26369, 48 }, /* You can catch the bird, but you cannot carry it. */
	/* 28 */ { 26417, 34 }, /* There is nothing here with a lock! */
	/* 29 */ { 26451, 23 }, /* You aren't carrying it! */
	/* 30 */ { 26474, 92 }, /* The little bird attacks the green snake, and in an astounding flurry
	drives the snake away. */
	/* 31 */ { 26566, 17 }, /* You have no keys! */
	/* 32 */ { 26583, 15 }, /* It has no lock. */
	/* 33 */ { 26598, 48 }, /* I don't know how to lock or unlock such a thing. */
	/* 34 */ { 26646, 22 }, /* It was already locked. */
	/* 35 */ { 26668, 24 }, /* The grate is now locked. */
	/* 36 */ { 26692, 26 }, /* The grate is now unlocked. */
	/* 37 */ { 26718, 24 }, /* It was already unlocked. */
	/* 38 */ { 26742, 28 }, /* You have no source of light. */
	/* 39 */ { 26770, 20 }, /* Your lamp is now on. */
	/* 40 */ { 26790, 21 }, /* Your lamp is now off. */
	/* 41 */ { 26811, 90 }, /* There is no way to get past the bear to unlock the chain, which is
	probably just as well. */
	/* 42 */ { 26901, 16 }, /* Nothing happens. */
	/* 43 */ { 26917, 6 }, /* Where? */
	/* 44 */ { 26923, 32 }, /* There is nothing here to attack. */
	/* 45 */ { 26955, 50 }, /* The little bird is now dead.  Its body disappears. */
	/* 46 */ { 27005, 60 }, /* Attacking the snake both doesn't work and is very dangerous. */
	/* 47 */ { 27065, 26 }, /* You killed a little dwarf. */
	/* 48 */ { 27091, 56 }, /* You attack a little dwarf, but he dodges out of the way. */
	/* 49 */ { 27147, 28 }, /* With what?  Your bare hands? */
	/* 50 */ { 27175, 49 }, /* Good try, but that is an old worn-out magic word. */
	/* 51 */ { 27224, 1315 }, /* I know of places, actions, and things.  Most of my vocabulary
	describes places and is used to move you there.  To move, try words
	like forest, building, downstream, enter, east, west, north, south,
	up, or down.  I know about a few special objects, like a black rod
	hidden in the cave.  These objects can be manipulated using some of
	the action words that I know.  Usually you will need to give both the
	object and action words (in either order), but sometimes I can infer
	the object from the verb alone.  Some objects also imply verbs; in
	particular, "inventory" implies "take inventory", which causes me to
	give you a list of what you're carrying.  The objects have side
	effects; for instance, the rod scares the bird.  Usually people having
	trouble moving just need to try a few more words.  Usually people
	trying unsuccessfully to manipulate an object are attempting something
	beyond their (or my!) capabilities and should try a completely
	different tack.  To speed the game you can sometimes move long
	distances with a single word.  For example, "building" usually gets
	you to the building from anywhere above ground except when lost in the
	forest.  Also, note that cave passages turn a lot, and that leaving a
	room to the north does not guarantee entering the next from the south.
	Good luck! */
	/* 52 */ { 28539, 10 }, /* It misses! */
	/* 53 */ { 28549, 12 }, /* It gets you! */
	/* 54 */ { 28561, 2 }, /* OK */
	/* 55 */ { 28563, 26 }, /* You can't unlock the keys. */
	/* 56 */ { 28589, 84 }, /* You have crawled around in some little holes and wound up back in the
	main passage. */
	/* 57 */ { 28673, 115 }, /* I don't know where the cave is, but hereabouts no stream can run on
	the surface for long.  I would try the stream. */
	/* 58 */ { 28788, 45 }, /* I need more detailed instructions to do that. */
	/* 59 */ { 28833, 118 }, /* I can only tell you what you see as you move about and manipulate
	things.  I cannot tell you where remote things are. */
	/* 60 */ { 28951, 23 }, /* I don't know that word. */
	/* 61 */ { 28974, 5 }, /* What? */
	/* 62 */ { 28979, 36 }, /* Are you trying to get into the cave? */
	/* 63 */ { 29015, 170 }, /* The grate is very solid and has a hardened steel lock.  You cannot
	enter without a key, and there are no keys nearby.  I would recommend
	looking elsewhere for the keys. */
	/* 64 */ { 29185, 363 }, /* The trees of the forest are large hardwood oak and maple, with an
	occasional grove of pine or spruce.  There is quite a bit of under-
	growth, largely birch and ash saplings plus nondescript bushes of
	various sorts.  This time of year visibility is quite restricted by
	all the leaves, but travel is quite easy if you detour around the
	spruce and berry bushes. */
	/* 65 */ { 29548, 52 }, /* Welcome to Adventure!!  Would you like instructions? */
	/* 66 */ { 29600, 89 }, /* Digging without a shovel is quite impractical.  Even with a shovel
	progress is unlikely. */
	/* 67 */ { 29689, 27 }, /* Blasting requires dynamite. */
	/* 68 */ { 29716, 27 }, /* I'm as confused as you are. */
	/* 69 */ { 29743, 162 }, /* Mist is a white vapor, usually water, seen from time to time in
	caverns.  It can be found anywhere but is frequently a sign of a deep
	pit leading down to water. */
	/* 70 */ { 29905, 22 }, /* Your feet are now wet. */
	/* 71 */ { 29927, 32 }, /* I think I just lost my appetite. */
	/* 72 */ { 29959, 28 }, /* Thank you, it was delicious! */
	/* 73 */ { 29987, 126 }, /* You have taken a drink from the stream.  The water tastes strongly of
	minerals, but is not unpleasant.  It is extremely cold. */
	/* 74 */ { 30113, 33 }, /* The bottle of water is now empty. */
	/* 75 */ { 30146, 92 }, /* Rubbing the electric lamp is not particularly rewarding.  Anyway,
	nothing exciting happens. */
	/* 76 */ { 30238, 38 }, /* Peculiar.  Nothing unexpected happens. */
	/* 77 */ { 30276, 43 }, /* Your bottle is empty and the ground is wet. */
	/* 78 */ { 30319, 20 }, /* You can't pour that. */
	/* 79 */ { 30339, 9 }, /* Watch it! */
	/* 80 */ { 30348, 10 }, /* Which way? */
	/* 81 */ { 30358, 168 }, /* Oh dear, you seem to have gotten yourself killed.  I might be able to
	help you out, but I've never really done this before.  Do you want me
	to try to reincarnate you? */
	/* 82 */ { 30526, 182 }, /* All right.  But don't blame me if something goes wr......
	--- Poof!! ---
	You are engulfed in a cloud of orange smoke.  Coughing and gasping,
	you emerge from the smoke and find.... */
	/* 83 */ { 30708, 129 }, /* You clumsy oaf, you've done it again!  I don't know how long I can
	keep this up.  Do you want me to try reincarnating you again? */
	/* 84 */ { 30837, 112 }, /* Okay, now where did I put my orange smoke?....  >poof!<
	Everything disappears in a dense cloud of orange smoke. */
	/* 85 */ { 30949, 137 }, /* Now you've really done it!  I'm out of orange smoke!  You don't expect
	me to do a decent reincarnation without any orange smoke, do you? */
	/* 86 */ { 31086, 55 }, /* Okay, if you're so smart, do it yourself!  I'm leaving! */
	/* 87 */ { 0, 0 },
	/* 88 */ { 0, 0 },
	/* 89 */ { 0, 0 },
	/* 90 */ { 31141, 58 }, /* >>> Messages 81 thru 90 are reserved for "obituaries". <<< */
	/* 91 */ { 31199, 64 }, /* Sorry, but I no longer seem to remember how it was you got here. */
	/* 92 */ { 31263, 68 }, /* You can't carry anything more.  You'll have to drop something first. */
	/* 93 */ { 31331, 42 }, /* You can't go through a locked steel grate! */
	/* 94 */ { 31373, 47 }, /* I believe what you want is right here with you. */
	/* 95 */ { 31420, 38 }, /* You don't fit through a two-inch slit! */
	/* 96 */ { 31458, 67 }, /* I respectfully suggest you go across the bridge instead of jumping. */
	/* 97 */ { 31525, 35 }, /* There is no way across the fissure. */
	/* 98 */ { 31560, 29 }, /* You're not carrying anything. */
	/* 99 */ { 31589, 40 }, /* You are currently holding the following: */
	/* 100 */ { 31629, 86 }, /* It's not hungry (it's merely pinin' for the fjords).  Besides, you
	have no bird seed. */
	/* 101 */ { 31715, 37 }, /* The snake has now devoured your bird. */
	/* 102 */ { 31752, 58 }, /* There's nothing here it wants to eat (except perhaps you). */
	/* 103 */ { 31810, 68 }, /* You fool, dwarves eat only coal!  Now you've made him *really* mad!! */
	/* 104 */ { 31878, 38 }, /* You have nothing in which to carry it. */
	/* 105 */ { 31916, 28 }, /* Your bottle is already full. */
	/* 106 */ { 31944, 52 }, /* There is nothing here with which to fill the bottle. */
	/* 107 */ { 31996, 33 }, /* Your bottle is now full of water. */
	/* 108 */ { 32029, 31 }, /* Your bottle is now full of oil. */
	/* 109 */ { 32060, 20 }, /* You can't fill that. */
	/* 110 */ { 32080, 20 }, /* Don't be ridiculous! */
	/* 111 */ { 32100, 48 }, /* The door is extremely rusty and refuses to open. */
	/* 112 */ { 32148, 70 }, /* The plant indignantly shakes the oil off its leaves and asks, "water?" */
	/* 113 */ { 32218, 59 }, /* The hinges are quite thoroughly rusted now and won't budge. */
	/* 114 */ { 32277, 98 }, /* The oil has freed up the hinges so that the door will now move,
	although it requires some effort. */
	/* 115 */ { 32375, 65 }, /* The plant has exceptionally deep roots and cannot be pulled free. */
	/* 116 */ { 32440, 64 }, /* The dwarves' knives vanish as they strike the walls of the cave. */
	/* 117 */ { 32504, 111 }, /* Something you're carrying won't fit through the tunnel with you.
	You'd best take inventory and drop something. */
	/* 118 */ { 32615, 62 }, /* You can't fit this five-foot clam through that little passage! */
	/* 119 */ { 32677, 64 }, /* You can't fit this five-foot oyster through that little passage! */
	/* 120 */ { 32741, 63 }, /* I advise you to put down the clam before opening it.  >strain!< */
	/* 121 */ { 32804, 65 }, /* I advise you to put down the oyster before opening it.  >wrench!< */
	/* 122 */ { 32869, 55 }, /* You don't have anything strong enough to open the clam. */
	/* 123 */ { 32924, 57 }, /* You don't have anything strong enough to open the oyster. */
	/* 124 */ { 32981, 198 }, /* A glistening pearl falls out of the clam and rolls away.  Goodness,
	this must really be an oyster.  (I never was very good at identifying
	bivalves.)  Whatever it is, it has now snapped shut again. */
	/* 125 */ { 33179, 92 }, /* The oyster creaks open, revealing nothing but oyster inside.  It
	promptly snaps shut again. */
	/* 126 */ { 33271, 132 }, /* You have crawled around in some little holes and found your way
	blocked by a recent cave-in.  You are now back in the main passage. */
	/* 127 */ { 33403, 61 }, /* There are faint rustling noises from the darkness behind you. */
	/* 128 */ { 33464, 223 }, /* Out from the shadows behind you pounces a bearded pirate!  "Har, har,"
	he chortles, "I'll just take all this booty and hide it away with me
	chest deep in the maze!"  He snatches your treasure and vanishes into
	the gloom. */
	/* 129 */ { 33687, 133 }, /* A sepulchral voice reverberating through the cave, says, "Cave closing
	soon.  All adventurers exit immediately through Main Office." */
	/* 130 */ { 33820, 114 }, /* A mysterious recorded voice groans into life and announces:
	"This exit is closed.  Please leave via Main Office." */
	/* 131 */ { 33934, 125 }, /* It looks as though you're dead.  Well, seeing as how it's so close to
	closing time anyway, I think we'll just call it a day. */
	/* 132 */ { 34059, 208 }, /* The sepulchral voice intones, "The cave is now closed."  As the echoes
	fade, there is a blinding flash of light (and a small puff of orange
	smoke). . . .    As your eyes refocus, you look around and find... */
	/* 133 */ { 34267, 271 }, /* There is a loud explosion, and a twenty-foot hole appears in the far
	wall, burying the dwarves in the rubble.  You march through the hole
	and find yourself in the Main Office, where a cheering band of
	friendly elves carry the conquering adventurer off into the sunset. */
	/* 134 */ { 34538, 211 }, /* There is a loud explosion, and a twenty-foot hole appears in the far
	wall, burying the snakes in the rubble.  A river of molten lava pours
	in through the hole, destroying everything in its path, including you! */
	/* 135 */ { 34749, 87 }, /* There is a loud explosion, and you are suddenly splashed across the
	walls of the room. */
	/* 136 */ { 34836, 177 }, /* The resulting ruckus has awakened the dwarves.  There are now several
	threatening little dwarves in the room with you!  Most of them throw
	knives at you!  All of them get you! */
	/* 137 */ { 35013, 38 }, /* Oh, leave the poor unhappy bird alone. */
	/* 138 */ { 35051, 54 }, /* I dare say whatever you want is around here somewhere. */
	/* 139 */ { 35105, 65 }, /* I don't know the word "stop".  Use "quit" if you want to give up. */
	/* 140 */ { 35170, 30 }, /* You can't get there from here. */
	/* 141 */ { 35200, 50 }, /* You are being followed by a very large, tame bear. */
	/* 142 */ { 35250, 1398 }, /* If you want to end your adventure early, say "quit".  To suspend your
	adventure such that you can continue later, say "suspend" (or "pause"
	or "save").  To see what hours the cave is normally open, say "hours".
	To see how well you're doing, say "score".  To get full credit for a
	treasure, you must have left it safely in the building, though you get
	partial credit just for locating it.  You lose points for getting
	killed, or for quitting, though the former costs you more.  There are
	also points based on how much (if any) of the cave you've managed to
	explore; in particular, there is a large bonus just for getting in (to
	distinguish the beginners from the rest of the pack), and there are
	other ways to determine whether you've been through some of the more
	harrowing sections.  If you think you've found all the treasures, just
	keep exploring for a while.  If nothing interesting happens, you
	haven't found them all yet.  If something interesting *does* happen,
	it means you're getting a bonus and have an opportunity to garner many
	more points in the Master's Section.  I may occasionally offer hints
	if you seem to be having trouble.  If I do, I'll warn you in advance
	how much it will affect your score to accept the hints.  Finally, to
	save paper, you may specify "brief", which tells me never to repeat
	the full description of a place unless you explicitly ask me to. */
	/* 143 */ { 36648, 31 }, /* Do you indeed wish to quit now? */
	/* 144 */ { 36679, 50 }, /* There is nothing here with which to fill the vase. */
	/* 145 */ { 36729, 67 }, /* The sudden change in temperature has delicately shattered the vase. */
	/* 146 */ { 36796, 35 }, /* It is beyond your power to do that. */
	/* 147 */ { 36831, 17 }, /* I don't know how. */
	/* 148 */ { 36848, 34 }, /* It is too far up for you to reach. */
	/* 149 */ { 36882, 80 }, /* You killed a little dwarf.  The body vanishes in a cloud of greasy
	black smoke. */
	/* 150 */ { 36962, 53 }, /* The shell is very strong and is impervious to attack. */
	/* 151 */ { 37015, 62 }, /* What's the matter, can't you read?  Now you'd best start over. */
	/* 152 */ { 37077, 57 }, /* The axe bounces harmlessly off the dragon's thick scales. */
	/* 153 */ { 37134, 61 }, /* The dragon looks rather nasty.  You'd best not try to get by. */
	/* 154 */ { 37195, 116 }, /* The little bird attacks the green dragon, and in an astounding flurry
	gets burnt to a cinder.  The ashes blow away. */
	/* 155 */ { 37311, 8 }, /* On what? */
	/* 156 */ { 37319, 126 }, /* Okay, from now on I'll only describe a place in full the first time
	you come to it.  To get the full description, say "look". */
	/* 157 */ { 37445, 136 }, /* Trolls are close relatives with the rocks and have skin as tough as
	that of a rhinoceros.  The troll fends off your blows effortlessly. */
	/* 158 */ { 37581, 138 }, /* The troll deftly catches the axe, examines it carefully, and tosses it
	back, declaring, "Good workmanship, but it's not valuable enough." */
	/* 159 */ { 37719, 63 }, /* The troll catches your treasure and scurries away out of sight. */
	/* 160 */ { 37782, 35 }, /* The troll refuses to let you cross. */
	/* 161 */ { 37817, 44 }, /* There is no longer any way across the chasm. */
	/* 162 */ { 37861, 234 }, /* Just as you reach the other side, the bridge buckles beneath the
	weight of the bear, which was still following you around.  You
	scrabble desperately for support, but as the bridge collapses you
	stumble back and fall into the chasm. */
	/* 163 */ { 38095, 139 }, /* The bear lumbers toward the troll, who lets out a startled shriek and
	scurries away.  The bear soon gives up the pursuit and wanders back. */
	/* 164 */ { 38234, 65 }, /* The axe misses and lands near the bear where you can't get at it. */
	/* 165 */ { 38299, 56 }, /* With what?  Your bare hands?  Against *his* bear hands?? */
	/* 166 */ { 38355, 54 }, /* The bear is confused; he only wants to be your friend. */
	/* 167 */ { 38409, 52 }, /* For crying out loud, the poor thing is already dead! */
	/* 168 */ { 38461, 120 }, /* The bear eagerly wolfs down your food, after which he seems to calm
	down considerably and even becomes rather friendly. */
	/* 169 */ { 38581, 38 }, /* The bear is still chained to the wall. */
	/* 170 */ { 38619, 26 }, /* The chain is still locked. */
	/* 171 */ { 38645, 26 }, /* The chain is now unlocked. */
	/* 172 */ { 38671, 24 }, /* The chain is now locked. */
	/* 173 */ { 38695, 55 }, /* There is nothing here to which the chain can be locked. */
	/* 174 */ { 38750, 29 }, /* There is nothing here to eat. */
	/* 175 */ { 38779, 21 }, /* Do you want the hint? */
	/* 176 */ { 38800, 41 }, /* Do you need help getting out of the maze? */
	/* 177 */ { 38841, 61 }, /* You can make the passages look less alike by dropping things. */
	/* 178 */ { 38902, 49 }, /* Are you trying to explore beyond the Plover Room? */
	/* 179 */ { 38951, 169 }, /* There is a way to explore that region without having to worry about
	falling into a pit.  None of the objects available is immediately
	useful in discovering the secret. */
	/* 180 */ { 39120, 37 }, /* Do you need help getting out of here? */
	/* 181 */ { 39157, 14 }, /* Don't go west. */
	/* 182 */ { 39171, 64 }, /* Gluttony is not one of the troll's vices.  Avarice, however, is. */
	/* 183 */ { 39235, 190 }, /* Your lamp is getting dim.  You'd best start wrapping this up, unless
	You can find some fresh batteries.  I seem to recall there's a vending
	machine in the maze.  Bring some coins with you. */
	/* 184 */ { 39425, 31 }, /* Your lamp has run out of power. */
	/* 185 */ { 39456, 130 }, /* There's not much point in wandering around out here, and you can't
	explore the cave without a lamp.  So let's just call it a day. */
	/* 186 */ { 39586, 321 }, /* There are faint rustling noises from the darkness behind you.  As you
	turn toward them, the beam of your lamp falls across a bearded pirate.
	He is carrying a large chest.  "Shiver me timbers!" he cries, "I've
	been spotted!  I'd best hie meself off to the maze to hide me chest!"
	With that, he vanishes into the gloom. */
	/* 187 */ { 39907, 66 }, /* Your lamp is getting dim.  You'd best go back for those batteries. */
	/* 188 */ { 39973, 78 }, /* Your lamp is getting dim.  I'm taking the liberty of replacing the
	batteries. */
	/* 189 */ { 40051, 97 }, /* Your lamp is getting dim, and you're out of spare batteries.  You'd
	best start wrapping this up. */
	/* 190 */ { 40148, 47 }, /* I'm afraid the magazine is written in Dwarvish. */
	/* 191 */ { 40195, 66 }, /* "This is not the maze where the pirate leaves his treasure chest." */
	/* 192 */ { 40261, 118 }, /* Hmmm, this looks like a clue, which means it'll cost you 10 points to
	read it.  Should I go ahead and read it anyway? */
	/* 193 */ { 40379, 123 }, /* It says, "there is something strange about this place, such that one
	of the words I've always known now has a new effect." */
	/* 194 */ { 40502, 37 }, /* It says the same thing it did before. */
	/* 195 */ { 40539, 30 }, /* I'm afraid I don't understand. */
	/* 196 */ { 40569, 55 }, /* "Congratulations on bringing light into the Dark-Room!" */
	/* 197 */ { 40624, 93 }, /* You strike the mirror a resounding blow, whereupon it shatters into a
	myriad tiny fragments. */
	/* 198 */ { 40717, 63 }, /* You have taken the vase and hurled it delicately to the ground. */
	/* 199 */ { 40780, 105 }, /* You prod the nearest dwarf, who wakes up grumpily, takes one look at
	you, curses, and grabs for his axe. */
	/* 200 */ { 40885, 19 }, /* Is this acceptable? */
	/* 201 */ { 40904, 52 }, /* There's no point in suspending a demonstration game. */
	/* 202 */ { 40956, 125 }, /* You awaken only to discover your bits have been dissolving while you
	slept.  You disappear in a cloud of greasy black smoke. */
	/* 203 */ { 0, 0 },
	/* 204 */ { 0, 0 }
};

struct text mtext[MAGSIZ] = { // section 12
	/* 0 */ { 0, 0 },
	/* 1 */ { 41553, 322 }, /* A large cloud of green smoke appears in front of you.  It clears away
	to reveal a tall wizard, clothed in grey.  He fixes you with a steely
	glare and declares, "This adventure has lasted too long."  With that
	he makes a single pass over you with his hands, and everything around
	you fades away into a grey nothingness. */
	/* 2 */ { 41875, 43 }, /* Even wizards have to wait longer than that! */
	/* 3 */ { 41918, 64 }, /* I'm terribly sorry, but Colossal Cave is closed.  Our hours are: */
	/* 4 */ { 41982, 53 }, /* Only wizards are permitted within the cave right now. */
	/* 5 */ { 42035, 97 }, /* We do allow visitors to make short explorations during our off hours.
	Would you like to do that? */
	/* 6 */ { 42132, 68 }, /* Colossal Cave is open to regular adventurers at the following hours: */
	/* 7 */ { 42200, 10 }, /* Very well. */
	/* 8 */ { 42210, 50 }, /* Only a wizard may continue an adventure this soon. */
	/* 9 */ { 42260, 52 }, /* I suggest you resume your adventure at a later time. */
	/* 10 */ { 42312, 29 }, /* Do you wish to see the hours? */
	/* 11 */ { 42341, 32 }, /* Do you wish to change the hours? */
	/* 12 */ { 42373, 41 }, /* New magic word (null to leave unchanged): */
	/* 13 */ { 42414, 43 }, /* New magic number (null to leave unchanged): */
	/* 14 */ { 42457, 45 }, /* Do you wish to change the message of the day? */
	/* 15 */ { 42502, 37 }, /* Okay.  You can save this version now. */
	/* 16 */ { 42539, 17 }, /* Are you a wizard? */
	/* 17 */ { 42556, 30 }, /* Prove it!  Say the magic word! */
	/* 18 */ { 42586, 70 }, /* That is not what I thought it was.  Do you know what I thought it was? */
	/* 19 */ { 42656, 69 }, /* Oh dear, you really *are* a wizard!  Sorry to have bothered you . . . */
	/* 20 */ { 42725, 37 }, /* Foo, you are nothing but a charlatan! */
	/* 21 */ { 42762, 137 }, /* New hours specified by defining "prime time".  Give only the hour
	(E.g. 14, not 14:00 or 2pm).  Enter a negative number after last pair. */
	/* 22 */ { 42899, 28 }, /* New hours for Colossal Cave: */
	/* 23 */ { 42927, 45 }, /* Limit lines to 70 chars.  End with null line. */
	/* 24 */ { 42972, 22 }, /* Line too long, retype: */
	/* 25 */ { 42994, 55 }, /* Not enough room for another line.  Ending message here. */
	/* 26 */ { 43049, 45 }, /* Do you wish to (re)schedule the next holiday? */
	/* 27 */ { 43094, 34 }, /* To begin how many days from today? */
	/* 28 */ { 43128, 43 }, /* To last how many days (zero if no holiday)? */
	/* 29 */ { 43171, 40 }, /* To be called what (up to 20 characters)? */
	/* 30 */ { 43211, 48 }, /* Too small!  Assuming minimum value (45 minutes). */
	/* 31 */ { 43259, 43 }, /* Break out of this and save your core-image. */
	/* 32 */ { 43302, 34 }, /* Be sure to save your core-image... */
	/* 33 */ { 0, 0 },
	/* 34 */ { 0, 0 }
};

int clsses = 10;

struct text ctext[CLSMAX] = { // section 10
	{ 0, 0 },
	{ 41081, 57 }, /* You are obviously a rank amateur.  Better luck next time. */
	{ 41138, 54 }, /* Your score qualifies you as a Novice class Adventurer. */
	{ 41192, 55 }, /* You have achieved the rating: "Experienced Adventurer". */
	{ 41247, 54 }, /* You may now consider yourself a "Seasoned Adventurer". */
	{ 41301, 40 }, /* You have reached "Junior Master" status. */
	{ 41341, 49 }, /* Your score puts you in Master Adventurer Class C. */
	{ 41390, 49 }, /* Your score puts you in Master Adventurer Class B. */
	{ 41439, 49 }, /* Your score puts you in Master Adventurer Class A. */
	{ 41488, 65 }, /* All of Adventuredom gives tribute to you, Adventurer Grandmaster! */
	{ 0, 0 }, 
	{ 0, 0 }
};

int cval[CLSMAX] = { // section 10
	0,
	35,
	100,
	130,
	200,
	250,
	300,
	330,
	349,
	9999,
	0,
	0
};

struct text voc[HTSIZE] = { // vocabulary (voc.bin)
	{ 0, 6 }, /* oil */
	{ 6, 36 }, /* broke geyse spice foe foe */
	{ 0, 0 },
	{ 42, 16 }, /* steps steps */
	{ 58, 32 }, /* touch pearl mumbl smash */
	{ 90, 31 }, /* hole carpe tride captu */
	{ 121, 15 }, /* water turn */
	{ 136, 23 }, /* back crack abrac */
	{ 0, 0 },
	{ 159, 22 }, /* dome nothi abra */
	{ 181, 16 }, /* slabr brief */
	{ 197, 17 }, /* go foo foo */
	{ 214, 16 }, /* troll troll */
	{ 230, 45 }, /* leave barre cave knife wave save */
	{ 275, 16 }, /* dwarv infor */
	{ 291, 16 }, /* acros shado */
	{ 307, 36 }, /* east food chest nest mist */
	{ 343, 8 }, /* debri */
	{ 351, 13 }, /* in orien */
	{ 364, 8 }, /* ratio */
	{ 372, 7 }, /* keep */
	{ 379, 32 }, /* valle strea persi suspe */
	{ 411, 11 }, /* y2 jar */
	{ 422, 15 }, /* look mirro */
	{ 437, 33 }, /* bed pit north slit hit */
	{ 470, 29 }, /* room vendi free tree */
	{ 0, 0 },
	{ 499, 15 }, /* view drago */
	{ 0, 0 },
	{ 514, 37 }, /* forwa passa ne secre messa */
	{ 551, 15 }, /* floor door */
	{ 566, 24 }, /* pillo explo follo */
	{ 0, 0 },
	{ 590, 59 }, /* snake stala carry take close shake fum fum */
	{ 649, 8 }, /* chain */
	{ 657, 7 }, /* fork */
	{ 0, 0 },
	{ 664, 15 }, /* oyste tote */
	{ 679, 31 }, /* stair over climb caver */
	{ 710, 23 }, /* attac perus info */
	{ 733, 7 }, /* quit */
	{ 740, 23 }, /* gully issue tame */
	{ 763, 21 }, /* on deton excav */
	{ 0, 0 },
	{ 784, 35 }, /* hill hall find fill stop */
	{ 819, 46 }, /* retre awkwa above exami plove say */
	{ 865, 8 }, /* magaz */
	{ 873, 28 }, /* nw cross strik toss */
	{ 901, 15 }, /* jewel lost */
	{ 916, 21 }, /* se fissu nugge */
	{ 0, 0 },
	{ 937, 8 }, /* drink */
	{ 945, 7 }, /* lamp */
	{ 952, 40 }, /* outsi nowhe batte potte distu */
	{ 992, 7 }, /* pour */
	{ 999, 22 }, /* h2o hocus pocus */
	{ 1021, 29 }, /* out insid ignit read */
	{ 1050, 16 }, /* bedqu emera */
	{ 1066, 7 }, /* down */
	{ 0, 0 },
	{ 1073, 47 }, /* bird bottl plant plant shard chant */
	{ 1120, 30 }, /* tunne key velve silve */
	{ 1150, 6 }, /* inv */
	{ 1156, 19 }, /* eggs break ? */
	{ 1175, 28 }, /* rod rod "spel steal */
	{ 1203, 46 }, /* house xyzzy vase blowu pause swim */
	{ 1249, 7 }, /* open */
	{ 1256, 12 }, /* dark sw */
	{ 1268, 36 }, /* up right gold light fight */
	{ 1304, 8 }, /* grate */
	{ 1312, 31 }, /* enter retur bear utter */
	{ 1343, 15 }, /* treas fuck */
	{ 1358, 21 }, /* south eat feed */
	{ 1379, 16 }, /* chasm plati */
	{ 0, 0 },
	{ 1395, 21 }, /* bars hours dig */
	{ 1416, 15 }, /* headl drop */
	{ 1431, 16 }, /* knive trave */
	{ 0, 0 },
	{ 0, 0 },
	{ 1447, 15 }, /* west blast */
	{ 1462, 23 }, /* cage pyram devou */
	{ 1485, 8 }, /* extin */
	{ 1493, 14 }, /* egg throw */
	{ 1507, 30 }, /* cobbl plugh box shatt */
	{ 1537, 22 }, /* lante axe drawi */
	{ 1559, 6 }, /* rub */
	{ 1565, 8 }, /* trees */
	{ 1573, 30 }, /* road exit crawl pirat */
	{ 1603, 44 }, /* conti surfa conti fee fee shaza */
	{ 0, 0 },
	{ 1647, 14 }, /* downs rug */
	{ 1661, 7 }, /* left */
	{ 1668, 16 }, /* disca score */
	{ 0, 0 },
	{ 1684, 16 }, /* downw unloc */
	{ 0, 0 },
	{ 1700, 31 }, /* desce proce wake sesam */
	{ 1731, 21 }, /* main run inven */
	{ 0, 0 },
	{ 1752, 12 }, /* build d */
	{ 1764, 4 }, /* e */
	{ 1768, 31 }, /* onwar inwar upwar slab */
	{ 1799, 37 }, /* fores rock offic keys lock */
	{ 0, 0 },
	{ 1836, 20 }, /* entra fie fie */
	{ 0, 0 },
	{ 1856, 7 }, /* walk */
	{ 1863, 43 }, /* null wall shell wand wand kill */
	{ 1906, 23 }, /* depre volca calm */
	{ 1929, 18 }, /* n descr off */
	{ 1947, 36 }, /* moss ming sing swing goto */
	{ 1983, 7 }, /* help */
	{ 1990, 7 }, /* clam */
	{ 1997, 16 }, /* upstr ascen */
	{ 2013, 26 }, /* low canyo s diamo */
	{ 2039, 22 }, /* jump catch dump */
	{ 2061, 28 }, /* u table spelu relea */
	{ 2089, 16 }, /* reser figur */
	{ 2105, 4 }, /* w */
	{ 2109, 6 }, /* get */
	{ 2115, 8 }, /* machi */
	{ 0, 0 },
	{ 2123, 24 }, /* beans coins opens */
	{ 2147, 8 }, /* giant */
	{ 2155, 23 }, /* placa slay where */
	{ 2178, 8 }, /* dwarf */
	{ 2186, 8 } /* outdo */
};

struct text travel[LOCSIZ] = { // section 3
	{ 0, 0 },
	{ 0, 36 },
	{ 36, 17 },
	{ 53, 24 },
	{ 77, 27 },
	{ 104, 25 },
	{ 129, 20 },
	{ 149, 33 },
	{ 182, 32 },
	{ 214, 29 },
	{ 243, 21 },
	{ 264, 37 },
	{ 301, 29 },
	{ 330, 28 },
	{ 358, 39 },
	{ 397, 40 },
	{ 437, 5 },
	{ 442, 29 },
	{ 471, 7 },
	{ 478, 45 },
	{ 523, 5 },
	{ 528, 5 },
	{ 533, 5 },
	{ 538, 23 },
	{ 561, 6 },
	{ 567, 16 },
	{ 583, 5 },
	{ 588, 33 },
	{ 621, 19 },
	{ 640, 7 },
	{ 647, 13 },
	{ 660, 10 },
	{ 670, 5 },
	{ 675, 32 },
	{ 707, 11 },
	{ 718, 11 },
	{ 729, 22 },
	{ 751, 13 },
	{ 764, 16 },
	{ 780, 18 },
	{ 798, 5 },
	{ 803, 24 },
	{ 827, 25 },
	{ 852, 15 },
	{ 867, 20 },
	{ 887, 26 },
	{ 913, 6 },
	{ 919, 6 },
	{ 925, 6 },
	{ 931, 10 },
	{ 941, 20 },
	{ 961, 20 },
	{ 981, 30 },
	{ 1011, 15 },
	{ 1026, 6 },
	{ 1032, 20 },
	{ 1052, 6 },
	{ 1058, 26 },
	{ 1084, 6 },
	{ 1090, 5 },
	{ 1095, 19 },
	{ 1114, 15 },
	{ 1129, 20 },
	{ 1149, 6 },
	{ 1155, 24 },
	{ 1179, 60 },
	{ 1239, 35 },
	{ 1274, 17 },
	{ 1291, 16 },
	{ 1307, 26 },
	{ 1333, 16 },
	{ 1349, 15 },
	{ 1364, 21 },
	{ 1385, 7 },
	{ 1392, 20 },
	{ 1412, 10 },
	{ 1422, 5 },
	{ 1427, 16 },
	{ 1443, 5 },
	{ 1448, 5 },
	{ 1453, 20 },
	{ 1473, 6 },
	{ 1479, 6 },
	{ 1485, 15 },
	{ 1500, 15 },
	{ 1515, 6 },
	{ 1521, 6 },
	{ 1527, 6 },
	{ 1533, 18 },
	{ 1551, 5 },
	{ 1556, 5 },
	{ 1561, 13 },
	{ 1574, 15 },
	{ 1589, 7 },
	{ 1596, 19 },
	{ 1615, 16 },
	{ 1631, 6 },
	{ 1637, 18 },
	{ 1655, 11 },
	{ 1666, 17 },
	{ 1683, 28 },
	{ 1711, 7 },
	{ 1718, 7 },
	{ 1725, 26 },
	{ 1751, 11 },
	{ 1762, 11 },
	{ 1773, 15 },
	{ 1788, 50 },
	{ 1838, 23 },
	{ 1861, 11 },
	{ 1872, 10 },
	{ 1882, 22 },
	{ 1904, 50 },
	{ 1954, 7 },
	{ 1961, 5 },
	{ 1966, 5 },
	{ 1971, 10 },
	{ 1981, 33 },
	{ 2014, 10 },
	{ 2024, 12 },
	{ 2036, 10 },
	{ 2046, 12 },
	{ 2058, 38 },
	{ 2096, 21 },
	{ 2117, 28 },
	{ 2145, 18 },
	{ 2163, 18 },
	{ 2181, 17 },
	{ 2198, 19 },
	{ 2217, 24 },
	{ 2241, 16 },
	{ 2257, 50 },
	{ 2307, 50 },
	{ 2357, 50 },
	{ 2407, 50 },
	{ 2457, 50 },
	{ 2507, 50 },
	{ 2557, 50 },
	{ 2607, 50 },
	{ 2657, 50 },
	{ 2707, 6 }
};

int hntmax = 9;

int hints[20][5] = { // section 11
	/* 0 */	{ 0, 0, 0, 0, 0 },
	/* 1 */	{ 0, 0, 0, 0, 0 },
	/* 2 */	{ 0, 9999, 10, 0, 0 },
	/* 3 */	{ 0, 9999, 5 ,0 ,0 },
	/* 4 */	{ 0, 4, 2, 62, 63 },
	/* 5 */	{ 0, 5, 2, 18, 19 },
	/* 6 */	{ 0, 8, 2, 20, 21 },
	/* 7 */	{ 0, 75, 4, 176, 177 },
	/* 8 */	{ 0, 25, 5, 178, 179 },
	/* 9 */	{ 0, 20, 3, 180, 181 },
	/* 10 */ { 0, 0, 0, 0, 0 },
	/* 11 */ { 0, 0, 0, 0, 0 },
	/* 12 */ { 0, 0, 0, 0, 0 },
	/* 13 */ { 0, 0, 0, 0, 0 },
	/* 14 */ { 0, 0, 0, 0, 0 },
	/* 15 */ { 0, 0, 0, 0, 0 },
	/* 16 */ { 0, 0, 0, 0, 0 },
	/* 17 */ { 0, 0, 0, 0, 0 },
	/* 18 */ { 0, 0, 0, 0, 0 },
	/* 19 */ { 0, 0, 0, 0, 0 }
};

int fixd[101] = { // section 7 1/2
	0, 0, 0, 9, 0, 0, 0, 15, 0, -1, 0, -1, 27, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, -1, 67, -1, 110, 0, -1, -1, 121, 122, 122, 0, -1, -1, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 121, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int plac[101] = { // section 7 2/2
	0, 3, 3, 8, 10, 11, 0, 14, 13, 94, 96, 19, 17, 101, 103, 0, 106, 0, 0, 3, 3, 0, 0, 109, 25, 23, 111, 35, 0, 97, 0, 119, 117, 117, 0, 130, 0, 126, 140, 0, 96, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 27, 28, 29, 30, 0, 92, 95, 97, 100, 101, 0, 119, 127, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	
};

int cond[LOCSIZ] = { // section 9
	0, 5, 1, 5, 5, 1, 1, 5, 17, 1, 1, 0, 0, 32, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 128, 128, 128, 128, 136, 136, 136, 128, 128, 128, 128, 128, 136, 128, 136, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 136, 0, 0, 8, 136, 128, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 256, 257, 256, 0, 0, 0, 0, 0, 0, 512, 0, 0, 0, 0, 4, 0, 1, 1, 0, 0, 0, 0, 0, 8, 8, 8, 8, 9, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int actspk[35] = { // section 8
	0, 24, 29, 0, 33, 0, 33, 38, 38, 42, 14, 43, 110, 29, 110, 73, 75, 29, 13, 59, 59, 174, 109, 67, 13, 147, 155, 195, 146, 110, 13, 13, 0, 0, 0
};

int saved = 0, savet = 0, mxscor = 0, latncy = 0;

int turns = 0, lmwarn = 0, iwest = 0, knfloc = 0, detail = 0, 
	abbnum = 0, maxdie = 0, numdie = 0, holdng = 0, dkill = 0, foobar = 0, bonus = 0, clock1 = 0, clock2 = 0, 
	closng = 0, panic = 0, closed = 0, scorng = 0;

int keys = 0, lamp = 0, grate = 0, cage = 0, rod = 0, rod2 = 0, steps = 0, 
	bird = 0, door = 0, pillow = 0, snake = 0, fissur = 0, tablet = 0, clam = 0, oyster = 0, magzin = 0, 
	dwarf = 0, knife = 0, food = 0, bottle = 0, water = 0, oil = 0, plant = 0, plant2 = 0, axe = 0, mirror = 0, dragon = 0, 
	chasm = 0, troll = 0, troll2 = 0, bear = 0, messag = 0, vend = 0, batter = 0, 
	nugget = 0, coins = 0, chest = 0, eggs = 0, tridnt = 0, vase = 0, emrald = 0, pyram = 0, pearl = 0, rug = 0, chain = 0, 
	spices = 0, 
	back = 0, look = 0, cave = 0, null = 0, entrnc = 0, dprssn = 0, 
	enter = 0,  stream = 0,  pour = 0, 
	say = 0, lock = 0, throw = 0, find = 0, invent = 0;

int maxtrs = 0, tally = 0, tally2 = 0;

int chloc = 0, chloc2 = 0, 
	dflag = 0, daltlc = 0;

int demo = 0, limit = 0;

int delhit = 0;

int loc = 0, newloc = 0, oldloc = 0, oldlc2 = 0, wzdark = 0, gaveup = 0, kq = 0, k = 0, k2 = 0;

int yea = 0;

int stick = 0, dtotal = 0, attack = 0;

int verb = 0, obj = 0, spk = 0;

struct trav_ptr _tkk = { 0, 0, 0, /*ptr*/0, /*eod*/0 };
struct trav_ptr *tkk = &_tkk;

char *wd1 = 0, *wd2 = 0;

int blklin = 0;

UINT8 buffer[BUFFER_SIZE];
char strbuf1[MAXSTR + 1], strbuf2[MAXSTR + 1];