#include "hdr.h"
#include "trav.h"

// WARNME: these need to be initialized in _init.c

int dseen[7], dloc[7], odloc[7];
int place[101], prop[101], link[201];
int atloc[LOCSIZ];
struct hashtab voc[HTSIZE];
int fixed[101];
int hinted[20], hintlc[20];
int tk[21];

// NOTE: these are initialized here (or don't need to be initialized)

/*
Text file contents (TEXT.BIN):
* Sect 1  ltext
* Sect 2  stext
* Sect 5  ptext
* Sect 6  rtext
* Sect 12 mtext
*/

struct text ltext[LOCSIZ] = { // section 1
	{ 0, 0 },
	{ 0, 152 }, /* You are standing at the end of a road before a small brick building.
	Around you is a forest.  A small stream flows out of the building and
	down a gully. */
	{ 152, 140 }, /* You have walked up a hill, still in the forest.  The road slopes back
	down the other side of the hill.  There is a building in the distance. */
	{ 292, 59 }, /* You are inside a building, a well house for a large spring. */
	{ 351, 77 }, /* You are in a valley in the forest beside a stream tumbling along a
	rocky bed. */
	{ 428, 55 }, /* You are in open forest, with a deep valley to one side. */
	{ 483, 53 }, /* You are in open forest near both a valley and a road. */
	{ 536, 121 }, /* At your feet all the water of the stream splashes into a 2-inch slit
	in the rock.  Downstream the streambed is bare rock. */
	{ 657, 163 }, /* You are in a 20-foot depression floored with bare dirt.  Set into the
	dirt is a strong steel grate mounted in concrete.  A dry streambed
	leads into the depression. */
	{ 820, 119 }, /* You are in a small chamber beneath a 3x3 steel grate to the surface.
	A low crawl over cobbles leads inward to the west. */
	{ 939, 101 }, /* You are crawling over cobbles in a low passage.  There is a dim light
	at the east end of the passage. */
	{ 1040, 234 }, /* You are in a debris room filled with stuff washed in from the surface.
	A low wide passage with cobbles becomes plugged with mud and debris
	here, but an awkward canyon leads upward and west.  A note on the wall
	says "Magic word XYZZY". */
	{ 1274, 47 }, /* You are in an awkward sloping east/west canyon. */
	{ 1321, 177 }, /* You are in a splendid chamber thirty feet high.  The walls are frozen
	rivers of orange stone.  An awkward canyon and a good passage exit
	from east and west sides of the chamber. */
	{ 1498, 123 }, /* At your feet is a small pit breathing traces of white mist.  An east
	passage ends here except for a small crack leading on. */
	{ 1621, 336 }, /* You are at one end of a vast hall stretching forward out of sight to
	the west.  There are openings to either side.  Nearby, a wide stone
	staircase leads downward.  The hall is filled with wisps of white mist
	swaying to and fro almost as if alive.  A cold wind blows up the
	staircase.  There is a passage at the top of a dome behind you. */
	{ 1957, 45 }, /* The crack is far too small for you to follow. */
	{ 2002, 135 }, /* You are on the east bank of a fissure slicing clear across the hall.
	The mist is quite thick here, and the fissure is too wide to jump. */
	{ 2137, 98 }, /* This is a low room with a crude note on the wall.  The note says,
	"You won't get it up the steps". */
	{ 2235, 78 }, /* You are in the Hall of the Mountain King, with passages off in all
	directions. */
	{ 2313, 52 }, /* You are at the bottom of the pit with a broken neck. */
	{ 2365, 19 }, /* You didn't make it. */
	{ 2384, 24 }, /* The dome is unclimbable. */
	{ 2408, 117 }, /* You are at the west end of the Twopit Room.  There is a large hole in
	the wall above the pit at this end of the room. */
	{ 2525, 116 }, /* You are at the bottom of the eastern pit in the Twopit Room.  There is
	a small pool of oil in one corner of the pit. */
	{ 2641, 120 }, /* You are at the bottom of the western pit in the Twopit Room.  There is
	a large hole in the wall about 25 feet above you. */
	{ 2761, 64 }, /* You clamber up the plant and scurry through the hole at the top. */
	{ 2825, 61 }, /* You are on the west side of the fissure in the Hall of Mists. */
	{ 2886, 91 }, /* You are in a low N/S passage at a hole in the floor.  The hole goes
	down to an E/W passage. */
	{ 2977, 34 }, /* You are in the south side chamber. */
	{ 3011, 104 }, /* You are in the west side chamber of the Hall of the Mountain King.
	A passage continues west and up here. */
	{ 3115, 3 }, /* >$< */
	{ 3118, 27 }, /* You can't get by the snake. */
	{ 3145, 170 }, /* You are in a large room, with a passage to the south, a passage to the
	west, and a wall of broken rock to the east.  There is a large "Y2" on
	a rock in the room's center. */
	{ 3315, 52 }, /* You are in a jumble of rock, with cracks everywhere. */
	{ 3367, 470 }, /* You're at a low window overlooking a huge pit, which extends up out of
	sight.  A floor is indistinctly visible over 50 feet below.  Traces of
	white mist cover the floor of the pit, becoming thicker to the right.
	Marks in the dust around the window would seem to indicate that
	someone has been here recently.  Directly across the pit from you and
	25 feet away there is a similar window looking into a lighted room.  A
	shadowy figure can be seen there peering back at you. */
	{ 3837, 133 }, /* You are in a dirty broken passage.  To the east is a crawl.  To the
	west is a large passage.  Above you is a hole to another passage. */
	{ 3970, 73 }, /* You are on the brink of a small clean climbable pit.  A crawl leads
	west. */
	{ 4043, 101 }, /* You are in the bottom of a small pit with a little stream, which
	enters and exits through tiny slits. */
	{ 4144, 137 }, /* You are in a large room full of dusty rocks.  There is a big hole in
	the floor.  There are cracks everywhere, and a passage leading east. */
	{ 4281, 92 }, /* You have crawled through a very low wide passage parallel to and north
	of the Hall of Mists. */
	{ 4373, 154 }, /* You are at the west end of Hall of Mists.  A low wide crawl continues
	west and another goes north.  To the south is a little passage 6 feet
	off the floor. */
	{ 4527, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 4582, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 4637, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 4692, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 4747, 8 }, /* Dead end */
	{ 4755, 8 }, /* Dead end */
	{ 4763, 8 }, /* Dead end */
	{ 4771, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 4826, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 4881, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 4936, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 4991, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 5046, 8 }, /* Dead end */
	{ 5054, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 5109, 8 }, /* Dead end */
	{ 5117, 178 }, /* You are on the brink of a thirty foot pit with a massive orange column
	down one wall.  You could climb down here but you could not get back
	up.  The maze continues at this level. */
	{ 5295, 8 }, /* Dead end */
	{ 5303, 92 }, /* You have crawled through a very low wide passage parallel to and north
	of the Hall of Mists. */
	{ 5395, 167 }, /* You are at the east end of a very long hall apparently without side
	chambers.  To the east a low wide crawl slants up.  To the north a
	round two foot hole slants down. */
	{ 5562, 110 }, /* You are at the west end of a very long featureless hall.  The hall
	joins up with a narrow north/south passage. */
	{ 5672, 63 }, /* You are at a crossover of a high N/S passage and a low E/W one. */
	{ 5735, 8 }, /* Dead end */
	{ 5743, 207 }, /* You are at a complex junction.  A low hands and knees passage from the
	north joins a higher crawl from the east to make a walking passage
	going west.  There is also a large room above.  The air is damp here. */
	{ 5950, 123 }, /* You are in Bedquilt, a long east/west passage with holes everywhere.
	To explore at random select north, south, up, or down. */
	{ 6073, 153 }, /* You are in a room whose walls resemble Swiss cheese.  Obvious passages
	go west, east, NE, and NW.  Part of the room is occupied by a large
	bedrock block. */
	{ 6226, 330 }, /* You are at the east end of the Twopit Room.  The floor here is
	littered with thin rock slabs, which make it easy to descend the pits.
	There is a path here bypassing the pits to connect passages from east
	and west.  There are holes all over, but the only big one is on the
	wall directly over the west pit where you can't get to it. */
	{ 6556, 291 }, /* You are in a large low circular chamber whose floor is an immense slab
	fallen from the ceiling (Slab Room).  East and west there once were
	large passages, but they are now filled with boulders.  Low small
	passages go north and south, and the south one quickly bends west
	around the boulders. */
	{ 6847, 50 }, /* You are in a secret N/S canyon above a large room. */
	{ 6897, 55 }, /* You are in a secret N/S canyon above a sizable passage. */
	{ 6952, 141 }, /* You are in a secret canyon at a junction of three canyons, bearing
	north, south, and SE.  The north one is as tall as the other two
	combined. */
	{ 7093, 60 }, /* You are in a large low room.  Crawls lead north, SE, and SW. */
	{ 7153, 15 }, /* Dead end crawl. */
	{ 7168, 151 }, /* You are in a secret canyon which here runs e/w.  It crosses over a
	very tight canyon 15 feet below.  If you go down you may not be able
	to get back up. */
	{ 7319, 51 }, /* You are at a wide place in a very tight N/S canyon. */
	{ 7370, 54 }, /* The canyon here becomes too tight to go further south. */
	{ 7424, 88 }, /* You are in a tall E/W canyon.  A low tight crawl goes 3 feet north and
	seems to open up. */
	{ 7512, 52 }, /* The canyon runs into a mass of boulders -- dead end. */
	{ 7564, 106 }, /* The stream flows out through a pair of 1 foot diameter sewer pipes.
	It would be advisable to use the exit. */
	{ 7670, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 7725, 8 }, /* Dead end */
	{ 7733, 8 }, /* Dead end */
	{ 7741, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 7796, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 7851, 8 }, /* Dead end */
	{ 7859, 8 }, /* Dead end */
	{ 7867, 55 }, /* You are in a maze of twisty little passages, all alike. */
	{ 7922, 150 }, /* You are in a long, narrow corridor stretching out of sight to the
	west.  At the eastern end is a hole through which you can see a
	profusion of leaves. */
	{ 8072, 68 }, /* There is nothing here to climb.  Use "up" or "out" to leave the pit. */
	{ 8140, 49 }, /* You have climbed up the plant and out of the pit. */
	{ 8189, 171 }, /* You are at the top of a steep incline above a large room.  You could
	climb down here, but you would not be able to climb up.  There is a
	passage leading back to the north. */
	{ 8360, 206 }, /* You are in the Giant Room.  The ceiling here is too high up for your
	lamp to show it.  Cavernous passages lead east, north, and south.  On
	the west wall is scrawled the inscription, "Fee fie foe foo" [sic]. */
	{ 8566, 48 }, /* The passage here is blocked by a recent cave-in. */
	{ 8614, 53 }, /* You are at one end of an immense north/south passage. */
	{ 8667, 205 }, /* You are in a magnificent cavern with a rushing stream, which cascades
	over a sparkling waterfall into a roaring whirlpool which disappears
	through a hole in the floor.  Passages exit to the south and west. */
	{ 8872, 131 }, /* You are in the Soft Room.  The walls are covered with heavy curtains,
	the floor with a thick pile carpet.  Moss covers the ceiling. */
	{ 9003, 194 }, /* This is the Oriental Room.  Ancient oriental cave drawings cover the
	walls.  A gently sloping passage leads upward to the north, another
	passage leads SE, and a hands and knees crawl leads west. */
	{ 9197, 240 }, /* You are following a wide path around the outer edge of a large cavern.
	Far below, through a heavy white mist, strange splashing noises can be
	heard.  The mist rises up through a fissure in the ceiling.  The path
	exits to the south and west. */
	{ 9437, 199 }, /* You are in an alcove.  A small NW path seems to widen after a short
	distance.  An extremely tight tunnel leads east.  It looks like a very
	tight squeeze.  An eerie light can be seen at the other end. */
	{ 9636, 128 }, /* You're in a small chamber lit by an eerie green light.  An extremely
	narrow tunnel exits to the west.  A dark corridor leads ne. */
	{ 9764, 68 }, /* You're in the Dark-Room.  A corridor leading south is the only exit. */
	{ 9832, 141 }, /* You are in an arched hall.  A coral passage once continued up and east
	from here, but is now blocked by debris.  The air smells of sea water. */
	{ 9973, 260 }, /* You're in a large room carved out of sedimentary rock.  The floor and
	walls are littered with bits of shells embedded in the stone.  A
	shallow passage proceeds downward, and a somewhat steeper one leads
	up.  A low hands and knees passage enters from the south. */
	{ 10233, 59 }, /* You are in a long sloping corridor with ragged sharp walls. */
	{ 10292, 48 }, /* You are in a cul-de-sac about eight feet across. */
	{ 10340, 261 }, /* You are in an anteroom leading to a large passage to the east.  Small
	passages go west and up.  The remnants of recent digging are evident.
	A sign in midair here says "Cave under construction beyond this point.
	Proceed at own risk.  [Witt construction company]" */
	{ 10601, 59 }, /* You are in a maze of twisty little passages, all different. */
	{ 10660, 62 }, /* You are at Witt's End.  Passages lead off in *all* directions. */
	{ 10722, 468 }, /* You are in a north/south canyon about 25 feet across.  The floor is
	covered by white mist seeping in from the north.  The walls extend
	upward for well over 100 feet.  Suspended from some unseen point far
	above you, an enormous two-sided mirror is hanging parallel to and
	midway between the canyon walls.  (The mirror is obviously provided
	for the use of the dwarves, who as you know, are extremely vain.)  A
	small window can be seen in either wall, some fifty feet up. */
	{ 11190, 469 }, /* You're at a low window overlooking a huge pit, which extends up out of
	sight.  A floor is indistinctly visible over 50 feet below.  Traces of
	white mist cover the floor of the pit, becoming thicker to the left.
	Marks in the dust around the window would seem to indicate that
	someone has been here recently.  Directly across the pit from you and
	25 feet away there is a similar window looking into a lighted room.  A
	shadowy figure can be seen there peering back at you. */
	{ 11659, 202 }, /* A large stalactite extends from the roof and almost reaches the floor
	below.  You could climb down it, and jump from it to the floor, but
	having done so you would be unable to reach it to climb back up. */
	{ 11861, 61 }, /* You are in a little maze of twisting passages, all different. */
	{ 11922, 327 }, /* You are at the edge of a large underground reservoir.  An opaque cloud
	of white mist fills the room and rises rapidly upward.  The lake is
	fed by a stream, which tumbles out of a hole in the wall about 10 feet
	overhead and splashes noisily into the water somewhere within the
	mist.  The only passage goes back toward the south. */
	{ 12249, 8 }, /* Dead end */
	{ 12257, 740 }, /* You are at the northeast end of an immense room, even larger than the
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
	{ 12997, 497 }, /* You are at the southwest end of the Repository.  To one side is a pit
	full of fierce green snakes.  On the other side is a row of small
	wicker cages, each of which contains a little sulking bird.  In one
	corner is a bundle of black rods with rusty marks on their ends.  A
	large number of velvet pillows are scattered about on the floor.  A
	vast mirror stretches off to the northeast.  At your feet is a large
	steel grate, next to which is a sign which reads, "Treasure Vault.
	Keys in Main Office." */
	{ 13494, 181 }, /* You are on one side of a large, deep chasm.  A heavy white mist rising
	up from below obscures all view of the far side.  A SW path leads away
	from the chasm into a winding corridor. */
	{ 13675, 75 }, /* You are in a long winding corridor sloping out of sight in both
	directions. */
	{ 13750, 61 }, /* You are in a secret canyon which exits to the north and east. */
	{ 13811, 61 }, /* You are in a secret canyon which exits to the north and east. */
	{ 13872, 61 }, /* You are in a secret canyon which exits to the north and east. */
	{ 13933, 88 }, /* You are on the far side of the chasm.  A ne path leads away from the
	chasm on this side. */
	{ 14021, 90 }, /* You're in a long east/west corridor.  A faint rumbling noise can be
	heard in the distance. */
	{ 14111, 202 }, /* The path forks here.  The left fork leads northeast.  A dull rumbling
	seems to get louder in that direction.  The right fork leads southeast
	down a gentle slope.  The main corridor enters from the west. */
	{ 14313, 183 }, /* The walls are quite warm here.  From the north can be heard a steady
	roar, so loud that the entire cave seems to be trembling.  Another
	passage leads south, and a low crawl goes east. */
	{ 14496, 1292 }, /* You are on the edge of a breath-taking view.  Far below you is an
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
	{ 15788, 225 }, /* You are in a small chamber filled with large boulders.  The walls are
	very warm, causing the air in the room to be almost stifling from the
	heat.  The only exit is a crawl heading west, through which is coming
	a low rumbling. */
	{ 16013, 104 }, /* You are walking along a gently sloping north/south passage lined with
	oddly shaped limestone formations. */
	{ 16117, 125 }, /* You are standing at the entrance to a large, barren room.  A sign
	posted above the entrance reads:  "Caution!  Bear in room!" */
	{ 16242, 195 }, /* You are inside a barren room.  The center of the room is completely
	empty except for some dust.  Marks in the dust lead away toward the
	far end of the room.  The only exit is the way you came in. */
	{ 16437, 61 }, /* You are in a maze of twisting little passages, all different. */
	{ 16498, 59 }, /* You are in a little maze of twisty passages, all different. */
	{ 16557, 61 }, /* You are in a twisting maze of little passages, all different. */
	{ 16618, 61 }, /* You are in a twisting little maze of passages, all different. */
	{ 16679, 59 }, /* You are in a twisty little maze of passages, all different. */
	{ 16738, 59 }, /* You are in a twisty maze of little passages, all different. */
	{ 16797, 59 }, /* You are in a little twisty maze of passages, all different. */
	{ 16856, 61 }, /* You are in a maze of little twisting passages, all different. */
	{ 16917, 59 }, /* You are in a maze of little twisty passages, all different. */
	{ 16976, 8 } /* Dead end */
};

struct text stext[LOCSIZ] = { // section 2
	{ 16984, 0 },
	{ 16984, 28 }, /* You're at end of road again. */
	{ 17012, 23 }, /* You're at hill in road. */
	{ 17035, 23 }, /* You're inside building. */
	{ 17058, 17 }, /* You're in valley. */
	{ 17075, 17 }, /* You're in forest. */
	{ 17092, 17 }, /* You're in forest. */
	{ 17109, 28 }, /* You're at slit in streambed. */
	{ 17137, 21 }, /* You're outside grate. */
	{ 17158, 23 }, /* You're below the grate. */
	{ 17181, 23 }, /* You're in Cobble Crawl. */
	{ 17204, 22 }, /* You're in Debris Room. */
	{ 17226, 0 },
	{ 17226, 23 }, /* You're in Bird Chamber. */
	{ 17249, 27 }, /* You're at top of small pit. */
	{ 17276, 24 }, /* You're in Hall of Mists. */
	{ 17300, 0 },
	{ 17300, 31 }, /* You're on east bank of fissure. */
	{ 17331, 30 }, /* You're in Nugget of Gold Room. */
	{ 17361, 26 }, /* You're in Hall of Mt King. */
	{ 17387, 0 },
	{ 17387, 0 },
	{ 17387, 0 },
	{ 17387, 34 }, /* You're at west end of Twopit Room. */
	{ 17421, 19 }, /* You're in east pit. */
	{ 17440, 19 }, /* You're in west pit. */
	{ 17459, 0 },
	{ 17459, 0 },
	{ 17459, 0 },
	{ 17459, 0 },
	{ 17459, 0 },
	{ 17459, 0 },
	{ 17459, 0 },
	{ 17459, 15 }, /* You're at "Y2". */
	{ 17474, 0 },
	{ 17474, 24 }, /* You're at window on pit. */
	{ 17498, 24 }, /* You're in dirty passage. */
	{ 17522, 0 },
	{ 17522, 0 },
	{ 17522, 26 }, /* You're in Dusty Rock room. */
	{ 17548, 0 },
	{ 17548, 36 }, /* You're at west end of Hall of Mists. */
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 0 },
	{ 17584, 23 }, /* You're at brink of pit. */
	{ 17607, 0 },
	{ 17607, 0 },
	{ 17607, 32 }, /* You're at east end of Long Hall. */
	{ 17639, 32 }, /* You're at west end of Long Hall. */
	{ 17671, 0 },
	{ 17671, 0 },
	{ 17671, 27 }, /* You're at Complex Junction. */
	{ 17698, 0 },
	{ 17698, 28 }, /* You're in Swiss Cheese room. */
	{ 17726, 34 }, /* You're at east end of Twopit Room. */
	{ 17760, 20 }, /* You're in Slab Room. */
	{ 17780, 0 },
	{ 17780, 0 },
	{ 17780, 43 }, /* You're at junction of three secret canyons. */
	{ 17823, 0 },
	{ 17823, 0 },
	{ 17823, 47 }, /* You're in secret E/W canyon above tight canyon. */
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 0 },
	{ 17870, 26 }, /* You're in narrow corridor. */
	{ 17896, 0 },
	{ 17896, 0 },
	{ 17896, 41 }, /* You're at steep incline above large room. */
	{ 17937, 21 }, /* You're in Giant Room. */
	{ 17958, 0 },
	{ 17958, 0 },
	{ 17958, 32 }, /* You're in cavern with waterfall. */
	{ 17990, 20 }, /* You're in Soft Room. */
	{ 18010, 24 }, /* You're in Oriental Room. */
	{ 18034, 23 }, /* You're in Misty Cavern. */
	{ 18057, 17 }, /* You're in Alcove. */
	{ 18074, 22 }, /* You're in Plover Room. */
	{ 18096, 20 }, /* You're in Dark-Room. */
	{ 18116, 22 }, /* You're in Arched Hall. */
	{ 18138, 21 }, /* You're in Shell Room. */
	{ 18159, 0 },
	{ 18159, 0 },
	{ 18159, 19 }, /* You're in Anteroom. */
	{ 18178, 0 },
	{ 18178, 21 }, /* You're at Witt's End. */
	{ 18199, 24 }, /* You're in Mirror Canyon. */
	{ 18223, 24 }, /* You're at window on pit. */
	{ 18247, 28 }, /* You're at top of stalactite. */
	{ 18275, 0 },
	{ 18275, 20 }, /* You're at Reservoir. */
	{ 18295, 0 },
	{ 18295, 17 }, /* You're at NE end. */
	{ 18312, 17 }, /* You're at SW end. */
	{ 18329, 27 }, /* You're on SW side of chasm. */
	{ 18356, 27 }, /* You're in sloping corridor. */
	{ 18383, 0 },
	{ 18383, 0 },
	{ 18383, 0 },
	{ 18383, 27 }, /* You're on NE side of chasm. */
	{ 18410, 19 }, /* You're in corridor. */
	{ 18429, 23 }, /* You're at fork in path. */
	{ 18452, 35 }, /* You're at junction with warm walls. */
	{ 18487, 29 }, /* You're at Breath-taking View. */
	{ 18516, 30 }, /* You're in Chamber of Boulders. */
	{ 18546, 28 }, /* You're in limestone passage. */
	{ 18574, 31 }, /* You're in front of barren room. */
	{ 18605, 22 } /* You're in Barren Room. */
};

struct text ptext[101] = { // section 5
	{ 18627, 0 },
	{ 18627, 51 }, /* Set of keys
	000     There are some keys on the ground here. */
	{ 18678, 81 }, /* Brass lantern
	000     There is a shiny brass lamp nearby.
	100     There is a lamp shining nearby. */
	{ 18759, 46 }, /* *Grate
	000     The grate is locked.
	100     The grate is open. */
	{ 18805, 58 }, /* Wicker cage
	000     There is a small wicker cage discarded nearby. */
	{ 18863, 73 }, /* Black rod
	000     A three foot black rod with a rusty star on an end lies nearby. */
	{ 18936, 73 }, /* Black rod
	000     A three foot black rod with a rusty mark on an end lies nearby. */
	{ 19009, 79 }, /* *Steps
	000     Rough stone steps lead down the pit.
	100     Rough stone steps lead up the dome. */
	{ 19088, 103 }, /* Little bird in cage
	000     A cheerful little bird is sitting here singing.
	100     There is a little bird in the cage. */
	{ 19191, 124 }, /* *Rusty door
	000     The way north is barred by a massive, rusty, iron door.
	100     The way north leads through a massive, rusty, iron door. */
	{ 19315, 54 }, /* Velvet pillow
	000     A small velvet pillow lies on the floor. */
	{ 19369, 65 }, /* *Snake
	000     A huge green fierce snake bars the way!
	100     >$<  (Chased away) */
	{ 19434, 85 }, /* *Fissure
	000     >$<
	100     A crystal bridge now spans the fissure.
	200     The crystal bridge has vanished! */
	{ 19519, 120 }, /* *Stone tablet
	000     A massive stone tablet embedded in the wall reads:
	000     "Congratulations on bringing light into the Dark-Room!" */
	{ 19639, 82 }, /* Giant clam  >grunt!<
	000     There is an enormous clam here with its shell tightly closed. */
	{ 19721, 168 }, /* Giant oyster  >groan!<
	000     There is an enormous oyster here with its shell tightly closed.
	100     Interesting.  There seems to be something written on the underside of
	100     The oyster. */
	{ 19889, 83 }, /* "Spelunker Today"
	000     There are a few recent issues of "Spelunker Today" magazine here. */
	{ 19972, 0 },
	{ 19972, 0 },
	{ 19972, 30 }, /* Tasty food
	000     There is food here. */
	{ 20002, 107 }, /* Small bottle
	000     There is a bottle of water here.
	100     There is an empty bottle here.
	200     There is a bottle of oil here. */
	{ 20109, 19 }, /* Water in the bottle */
	{ 20128, 17 }, /* Oil in the bottle */
	{ 20145, 11 }, /* *Mirror
	000     >$< */
// WARNME: there seems to be a comment here: "*Phony plant (seen in twopit room only when tall enough)"
	{ 20156, 428 }, /* *Plant
	000     There is a tiny little plant in the pit, murmuring "Water, water, ..."
	100     The plant spurts into furious growth for a few seconds.
	200     There is a 12-foot-tall beanstalk stretching up out of the pit,
	200     bellowing "WATER!! WATER!!"
	300     The plant grows explosively, almost filling the bottom of the pit.
	400     There is a gigantic beanstalk stretching all the way up to the hole.
	500     You've over-watered the plant!  It's shriveling up!  It's, it's... */
	{ 20584, 197 }, /* *Phony plant (seen in twopit room only when tall enough)
	000     >$<
	100     The top of a 12-foot-tall beanstalk is poking out of the west pit.
	200     There is a huge beanstalk growing out of the west pit up to the hole. */
	{ 20781, 15 }, /* *Stalactite
	000     >$< */
	{ 20796, 80 }, /* *Shadowy figure
	000     The shadowy figure seems to be trying to attract your attention. */
	{ 20876, 84 }, /* Dwarf's axe
	000     There is a little axe here.
	100     There is a little axe lying beside the bear. */
	{ 20960, 18 }, /* *Cave drawings
	000     >$< */
	{ 20978, 11 }, /* *Pirate
	000     >$< */
	{ 20989, 212 }, /* *Dragon
	000     A huge green fierce dragon bars the way!
	100     Congratulations!  You have just vanquished a dragon with your bare
	100     hands!  (Unbelievable, isn't it?)
	200     The body of a huge green dead dragon is lying off to one side. */
	{ 21201, 219 }, /* *Chasm
	000     A rickety wooden bridge extends across the chasm, vanishing into the
	000     mist.  A sign posted on the bridge reads, "Stop! Pay troll!"
	100     The wreckage of a bridge (and a dead bear) can be seen at the bottom
	100     of the chasm. */
	{ 21420, 184 }, /* *Troll
	000     A burly troll stands by the bridge and insists you throw him a
	000     treasure before you may cross.
	100     The troll steps out from beneath the bridge and blocks your way.
	200     >$<  (Chased away) */
	{ 21604, 45 }, /* *Phony troll
	000     The troll is nowhere to be seen. */
	{ 21649, 227 }, /* >$<  (Bear uses rtext 141)
	000     There is a ferocious cave bear eying you from the far end of the room!
	100     There is a gentle cave bear sitting placidly in one corner.
	200     There is a contented-looking bear wandering about nearby.
	300     >$<  (Dead) */
	{ 21876, 160 }, /* *Message in second maze
	000     There is a message scrawled in the dust in a flowery script, reading:
	000     "This is not the maze where the pirate leaves his treasure chest." */
	{ 22036, 26 }, /* *Volcano and/or geyser
	000     >$< */
	{ 22062, 133 }, /* *Vending machine
	000     There is a massive vending machine here.  The instructions on it read:
	000     "Drop coins here to receive fresh batteries." */
	{ 22195, 93 }, /* Batteries
	000     There are fresh batteries here.
	100     Some worn-out batteries have been discarded nearby. */
	{ 22288, 23 }, /* *Carpet and/or moss
	000     >$< */
	{ 22311, 0 },
	{ 22311, 0 },
	{ 22311, 0 },
	{ 22311, 0 },
	{ 22311, 0 },
	{ 22311, 0 },
	{ 22311, 0 },
	{ 22311, 0 },
	{ 22311, 0 },
	{ 22311, 65 }, /* Large gold nugget
	000     There is a large sparkling nugget of gold here! */
	{ 22376, 41 }, /* Several diamonds
	000     There are diamonds here! */
	{ 22417, 45 }, /* Bars of silver
	000     There are bars of silver here! */
	{ 22462, 48 }, /* Precious jewelry
	000     There is precious jewelry here! */
	{ 22510, 37 }, /* Rare coins
	000     There are many coins here! */
	{ 22547, 51 }, /* Treasure chest
	000     The pirate's treasure chest is here! */
	{ 22598, 104 }, /* Golden eggs
	000     There is a large nest here, full of golden eggs!
	100     The nest of golden eggs has vanished!
	200     Done! */
	{ 22702, 56 }, /* Jeweled trident
	000     There is a jewel-encrusted trident here! */
	{ 22758, 212 }, /* Ming vase
	000     There is a delicate, precious, Ming vase here!
	100     The vase is now resting, delicately, on a velvet pillow.
	200     The floor is littered with worthless shards of pottery.
	300     The Ming vase drops with a delicate crash. */
	{ 22970, 70 }, /* Egg-sized emerald
	000     There is an emerald here the size of a plover's egg! */
	{ 23040, 70 }, /* Platinum pyramid
	000     There is a platinum pyramid here, 8 inches on a side! */
	{ 23110, 57 }, /* Glistening pearl
	000     Off to one side lies a glistening pearl! */
	{ 23167, 105 }, /* Persian rug
	000     There is a Persian rug spread out on the floor!
	100     The dragon is sprawled out on a Persian rug!! */
	{ 23272, 39 }, /* Rare spices
	000     There are rare spices here! */
	{ 23311, 162 } /* Golden chain
	000     There is a golden chain lying in a heap on the floor!
	100     The bear is locked to the wall with a golden chain!
	200     There is a golden chain locked to the wall! */
};

struct text rtext[RTXSIZ] = { // section 6
	/* 0 */ { 23857, 0 },
	/* 1 */ { 23857, 747 }, /* Somewhere nearby is Colossal Cave, where others have found fortunes in
	treasure and gold, though it is rumored that some who enter are never
	seen again.  Magic is said to work in the cave.  I will be your eyes
	and hands.  Direct me with commands of 1 or 2 words.  I should warn
	you that I look at only the first five letters of each word, so you'll
	have to enter "northeast" as "ne" to distinguish it from "north".
	(Should you get stuck, type "help" for some general hints.  For
	information on how to end your adventure, etc., type "info".)
	- - -
	This program was originally developed by Will Crowther.  Most of the
	features of the current program were added by Don Woods.  Address
	complaints about the UNIX version to Jim Gillogly (jim@rand.org). */
	/* 2 */ { 24604, 48 }, /* A little dwarf with a big knife blocks your way. */
	/* 3 */ { 24652, 116 }, /* A little dwarf just walked around a corner, saw you, threw a little
	axe at you (which missed), cursed, and ran away. */
	/* 4 */ { 24768, 57 }, /* There is a threatening little dwarf in the room with you! */
	/* 5 */ { 24825, 39 }, /* One sharp nasty knife is thrown at you! */
	/* 6 */ { 24864, 21 }, /* None of them hit you! */
	/* 7 */ { 24885, 21 }, /* One of them gets you! */
	/* 8 */ { 24906, 28 }, /* A hollow voice says "Plugh". */
	/* 9 */ { 24934, 37 }, /* There is no way to go that direction. */
	/* 10 */ { 24971, 70 }, /* I am unsure how you are facing.  Use compass points or nearby objects. */
	/* 11 */ { 25041, 109 }, /* I don't know in from out here.  Use compass points or name something
	in the general direction you want to go. */
	/* 12 */ { 25150, 41 }, /* I don't know how to apply that word here. */
	/* 13 */ { 25191, 24 }, /* I don't understand that! */
	/* 14 */ { 25215, 41 }, /* I'm game.  Would you care to explain how? */
	/* 15 */ { 25256, 102 }, /* Sorry, but I am not allowed to give more detail.  I will repeat the
	long description of your location. */
	/* 16 */ { 25358, 70 }, /* It is now pitch dark.  If you proceed you will likely fall into a pit. */
	/* 17 */ { 25428, 46 }, /* If you prefer, simply type w rather than west. */
	/* 18 */ { 25474, 33 }, /* Are you trying to catch the bird? */
	/* 19 */ { 25507, 110 }, /* The bird is frightened right now and you cannot catch it no matter
	what you try.  Perhaps you might try later. */
	/* 20 */ { 25617, 46 }, /* Are you trying to somehow deal with the snake? */
	/* 21 */ { 25663, 160 }, /* You can't kill the snake, or drive it away, or avoid it, or anything
	like that.  There is a way to get by, but you don't have the necessary
	resources right now. */
	/* 22 */ { 25823, 31 }, /* Do you really want to quit now? */
	/* 23 */ { 25854, 54 }, /* You fell into a pit and broke every bone in your body! */
	/* 24 */ { 25908, 28 }, /* You are already carrying it! */
	/* 25 */ { 25936, 21 }, /* You can't be serious! */
	/* 26 */ { 25957, 105 }, /* The bird was unafraid when you entered, but as you approach it becomes
	disturbed and you cannot catch it. */
	/* 27 */ { 26062, 48 }, /* You can catch the bird, but you cannot carry it. */
	/* 28 */ { 26110, 34 }, /* There is nothing here with a lock! */
	/* 29 */ { 26144, 23 }, /* You aren't carrying it! */
	/* 30 */ { 26167, 91 }, /* The little bird attacks the green snake, and in an astounding flurry
	drives the snake away. */
	/* 31 */ { 26258, 17 }, /* You have no keys! */
	/* 32 */ { 26275, 15 }, /* It has no lock. */
	/* 33 */ { 26290, 48 }, /* I don't know how to lock or unlock such a thing. */
	/* 34 */ { 26338, 22 }, /* It was already locked. */
	/* 35 */ { 26360, 24 }, /* The grate is now locked. */
	/* 36 */ { 26384, 26 }, /* The grate is now unlocked. */
	/* 37 */ { 26410, 24 }, /* It was already unlocked. */
	/* 38 */ { 26434, 28 }, /* You have no source of light. */
	/* 39 */ { 26462, 20 }, /* Your lamp is now on. */
	/* 40 */ { 26482, 21 }, /* Your lamp is now off. */
	/* 41 */ { 26503, 89 }, /* There is no way to get past the bear to unlock the chain, which is
	probably just as well. */
	/* 42 */ { 26592, 16 }, /* Nothing happens. */
	/* 43 */ { 26608, 6 }, /* Where? */
	/* 44 */ { 26614, 32 }, /* There is nothing here to attack. */
	/* 45 */ { 26646, 50 }, /* The little bird is now dead.  Its body disappears. */
	/* 46 */ { 26696, 60 }, /* Attacking the snake both doesn't work and is very dangerous. */
	/* 47 */ { 26756, 26 }, /* You killed a little dwarf. */
	/* 48 */ { 26782, 56 }, /* You attack a little dwarf, but he dodges out of the way. */
	/* 49 */ { 26838, 28 }, /* With what?  Your bare hands? */
	/* 50 */ { 26866, 49 }, /* Good try, but that is an old worn-out magic word. */
	/* 51 */ { 26915, 1296 }, /* I know of places, actions, and things.  Most of my vocabulary
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
	/* 52 */ { 28211, 10 }, /* It misses! */
	/* 53 */ { 28221, 12 }, /* It gets you! */
	/* 54 */ { 28233, 2 }, /* OK */
	/* 55 */ { 28235, 26 }, /* You can't unlock the keys. */
	/* 56 */ { 28261, 83 }, /* You have crawled around in some little holes and wound up back in the
	main passage. */
	/* 57 */ { 28344, 114 }, /* I don't know where the cave is, but hereabouts no stream can run on
	the surface for long.  I would try the stream. */
	/* 58 */ { 28458, 45 }, /* I need more detailed instructions to do that. */
	/* 59 */ { 28503, 117 }, /* I can only tell you what you see as you move about and manipulate
	things.  I cannot tell you where remote things are. */
	/* 60 */ { 28620, 23 }, /* I don't know that word. */
	/* 61 */ { 28643, 5 }, /* What? */
	/* 62 */ { 28648, 36 }, /* Are you trying to get into the cave? */
	/* 63 */ { 28684, 168 }, /* The grate is very solid and has a hardened steel lock.  You cannot
	enter without a key, and there are no keys nearby.  I would recommend
	looking elsewhere for the keys. */
	/* 64 */ { 28852, 358 }, /* The trees of the forest are large hardwood oak and maple, with an
	occasional grove of pine or spruce.  There is quite a bit of under-
	growth, largely birch and ash saplings plus nondescript bushes of
	various sorts.  This time of year visibility is quite restricted by
	all the leaves, but travel is quite easy if you detour around the
	spruce and berry bushes. */
	/* 65 */ { 29210, 52 }, /* Welcome to Adventure!!  Would you like instructions? */
	/* 66 */ { 29262, 88 }, /* Digging without a shovel is quite impractical.  Even with a shovel
	progress is unlikely. */
	/* 67 */ { 29350, 27 }, /* Blasting requires dynamite. */
	/* 68 */ { 29377, 27 }, /* I'm as confused as you are. */
	/* 69 */ { 29404, 160 }, /* Mist is a white vapor, usually water, seen from time to time in
	caverns.  It can be found anywhere but is frequently a sign of a deep
	pit leading down to water. */
	/* 70 */ { 29564, 22 }, /* Your feet are now wet. */
	/* 71 */ { 29586, 32 }, /* I think I just lost my appetite. */
	/* 72 */ { 29618, 28 }, /* Thank you, it was delicious! */
	/* 73 */ { 29646, 125 }, /* You have taken a drink from the stream.  The water tastes strongly of
	minerals, but is not unpleasant.  It is extremely cold. */
	/* 74 */ { 29771, 33 }, /* The bottle of water is now empty. */
	/* 75 */ { 29804, 91 }, /* Rubbing the electric lamp is not particularly rewarding.  Anyway,
	nothing exciting happens. */
	/* 76 */ { 29895, 38 }, /* Peculiar.  Nothing unexpected happens. */
	/* 77 */ { 29933, 43 }, /* Your bottle is empty and the ground is wet. */
	/* 78 */ { 29976, 20 }, /* You can't pour that. */
	/* 79 */ { 29996, 9 }, /* Watch it! */
	/* 80 */ { 30005, 10 }, /* Which way? */
	/* 81 */ { 30015, 166 }, /* Oh dear, you seem to have gotten yourself killed.  I might be able to
	help you out, but I've never really done this before.  Do you want me
	to try to reincarnate you? */
	/* 82 */ { 30181, 179 }, /* All right.  But don't blame me if something goes wr......
	--- Poof!! ---
	You are engulfed in a cloud of orange smoke.  Coughing and gasping,
	you emerge from the smoke and find.... */
	/* 83 */ { 30360, 128 }, /* You clumsy oaf, you've done it again!  I don't know how long I can
	keep this up.  Do you want me to try reincarnating you again? */
	/* 84 */ { 30488, 111 }, /* Okay, now where did I put my orange smoke?....  >poof!<
	Everything disappears in a dense cloud of orange smoke. */
	/* 85 */ { 30599, 136 }, /* Now you've really done it!  I'm out of orange smoke!  You don't expect
	me to do a decent reincarnation without any orange smoke, do you? */
	/* 86 */ { 30735, 55 }, /* Okay, if you're so smart, do it yourself!  I'm leaving! */
    { 30790, 0 },
    { 30790, 0 },
    { 30790, 0 },
	/* 90 */ { 30790, 58 }, /* >>> Messages 81 thru 90 are reserved for "obituaries". <<< */
	/* 91 */ { 30848, 64 }, /* Sorry, but I no longer seem to remember how it was you got here. */
	/* 92 */ { 30912, 68 }, /* You can't carry anything more.  You'll have to drop something first. */
	/* 93 */ { 30980, 42 }, /* You can't go through a locked steel grate! */
	/* 94 */ { 31022, 47 }, /* I believe what you want is right here with you. */
	/* 95 */ { 31069, 38 }, /* You don't fit through a two-inch slit! */
	/* 96 */ { 31107, 67 }, /* I respectfully suggest you go across the bridge instead of jumping. */
	/* 97 */ { 31174, 35 }, /* There is no way across the fissure. */
	/* 98 */ { 31209, 29 }, /* You're not carrying anything. */
	/* 99 */ { 31238, 40 }, /* You are currently holding the following: */
	/* 100 */ { 31278, 85 }, /* It's not hungry (it's merely pinin' for the fjords).  Besides, you
	have no bird seed. */
	/* 101 */ { 31363, 37 }, /* The snake has now devoured your bird. */
	/* 102 */ { 31400, 58 }, /* There's nothing here it wants to eat (except perhaps you). */
	/* 103 */ { 31458, 68 }, /* You fool, dwarves eat only coal!  Now you've made him *really* mad!! */
	/* 104 */ { 31526, 38 }, /* You have nothing in which to carry it. */
	/* 105 */ { 31564, 28 }, /* Your bottle is already full. */
	/* 106 */ { 31592, 52 }, /* There is nothing here with which to fill the bottle. */
	/* 107 */ { 31644, 33 }, /* Your bottle is now full of water. */
	/* 108 */ { 31677, 31 }, /* Your bottle is now full of oil. */
	/* 109 */ { 31708, 20 }, /* You can't fill that. */
	/* 110 */ { 31728, 20 }, /* Don't be ridiculous! */
	/* 111 */ { 31748, 48 }, /* The door is extremely rusty and refuses to open. */
	/* 112 */ { 31796, 70 }, /* The plant indignantly shakes the oil off its leaves and asks, "water?" */
	/* 113 */ { 31866, 59 }, /* The hinges are quite thoroughly rusted now and won't budge. */
	/* 114 */ { 31925, 97 }, /* The oil has freed up the hinges so that the door will now move,
	although it requires some effort. */
	/* 115 */ { 32022, 65 }, /* The plant has exceptionally deep roots and cannot be pulled free. */
	/* 116 */ { 32087, 64 }, /* The dwarves' knives vanish as they strike the walls of the cave. */
	/* 117 */ { 32151, 110 }, /* Something you're carrying won't fit through the tunnel with you.
	You'd best take inventory and drop something. */
	/* 118 */ { 32261, 62 }, /* You can't fit this five-foot clam through that little passage! */
	/* 119 */ { 32323, 64 }, /* You can't fit this five-foot oyster through that little passage! */
	/* 120 */ { 32387, 63 }, /* I advise you to put down the clam before opening it.  >strain!< */
	/* 121 */ { 32450, 65 }, /* I advise you to put down the oyster before opening it.  >wrench!< */
	/* 122 */ { 32515, 55 }, /* You don't have anything strong enough to open the clam. */
	/* 123 */ { 32570, 57 }, /* You don't have anything strong enough to open the oyster. */
	/* 124 */ { 32627, 196 }, /* A glistening pearl falls out of the clam and rolls away.  Goodness,
	this must really be an oyster.  (I never was very good at identifying
	bivalves.)  Whatever it is, it has now snapped shut again. */
	/* 125 */ { 32823, 91 }, /* The oyster creaks open, revealing nothing but oyster inside.  It
	promptly snaps shut again. */
	/* 126 */ { 32914, 131 }, /* You have crawled around in some little holes and found your way
	blocked by a recent cave-in.  You are now back in the main passage. */
	/* 127 */ { 33045, 61 }, /* There are faint rustling noises from the darkness behind you. */
	/* 128 */ { 33106, 220 }, /* Out from the shadows behind you pounces a bearded pirate!  "Har, har,"
	he chortles, "I'll just take all this booty and hide it away with me
	chest deep in the maze!"  He snatches your treasure and vanishes into
	the gloom. */
	/* 129 */ { 33326, 132 }, /* A sepulchral voice reverberating through the cave, says, "Cave closing
	soon.  All adventurers exit immediately through Main Office." */
	/* 130 */ { 33458, 113 }, /* A mysterious recorded voice groans into life and announces:
	"This exit is closed.  Please leave via Main Office." */
	/* 131 */ { 33571, 124 }, /* It looks as though you're dead.  Well, seeing as how it's so close to
	closing time anyway, I think we'll just call it a day. */
	/* 132 */ { 33695, 206 }, /* The sepulchral voice intones, "The cave is now closed."  As the echoes
	fade, there is a blinding flash of light (and a small puff of orange
	smoke). . . .    As your eyes refocus, you look around and find... */
	/* 133 */ { 33901, 268 }, /* There is a loud explosion, and a twenty-foot hole appears in the far
	wall, burying the dwarves in the rubble.  You march through the hole
	and find yourself in the Main Office, where a cheering band of
	friendly elves carry the conquering adventurer off into the sunset. */
	/* 134 */ { 34169, 209 }, /* There is a loud explosion, and a twenty-foot hole appears in the far
	wall, burying the snakes in the rubble.  A river of molten lava pours
	in through the hole, destroying everything in its path, including you! */
	/* 135 */ { 34378, 86 }, /* There is a loud explosion, and you are suddenly splashed across the
	walls of the room. */
	/* 136 */ { 34464, 175 }, /* The resulting ruckus has awakened the dwarves.  There are now several
	threatening little dwarves in the room with you!  Most of them throw
	knives at you!  All of them get you! */
	/* 137 */ { 34639, 38 }, /* Oh, leave the poor unhappy bird alone. */
	/* 138 */ { 34677, 54 }, /* I dare say whatever you want is around here somewhere. */
	/* 139 */ { 34731, 65 }, /* I don't know the word "stop".  Use "quit" if you want to give up. */
	/* 140 */ { 34796, 30 }, /* You can't get there from here. */
	/* 141 */ { 34826, 50 }, /* You are being followed by a very large, tame bear. */
	/* 142 */ { 34876, 1379 }, /* If you want to end your adventure early, say "quit".  To suspend your
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
	/* 143 */ { 36255, 31 }, /* Do you indeed wish to quit now? */
	/* 144 */ { 36286, 50 }, /* There is nothing here with which to fill the vase. */
	/* 145 */ { 36336, 67 }, /* The sudden change in temperature has delicately shattered the vase. */
	/* 146 */ { 36403, 35 }, /* It is beyond your power to do that. */
	/* 147 */ { 36438, 17 }, /* I don't know how. */
	/* 148 */ { 36455, 34 }, /* It is too far up for you to reach. */
	/* 149 */ { 36489, 79 }, /* You killed a little dwarf.  The body vanishes in a cloud of greasy
	black smoke. */
	/* 150 */ { 36568, 53 }, /* The shell is very strong and is impervious to attack. */
	/* 151 */ { 36621, 62 }, /* What's the matter, can't you read?  Now you'd best start over. */
	/* 152 */ { 36683, 57 }, /* The axe bounces harmlessly off the dragon's thick scales. */
	/* 153 */ { 36740, 61 }, /* The dragon looks rather nasty.  You'd best not try to get by. */
	/* 154 */ { 36801, 115 }, /* The little bird attacks the green dragon, and in an astounding flurry
	gets burnt to a cinder.  The ashes blow away. */
	/* 155 */ { 36916, 8 }, /* On what? */
	/* 156 */ { 36924, 125 }, /* Okay, from now on I'll only describe a place in full the first time
	you come to it.  To get the full description, say "look". */
	/* 157 */ { 37049, 135 }, /* Trolls are close relatives with the rocks and have skin as tough as
	that of a rhinoceros.  The troll fends off your blows effortlessly. */
	/* 158 */ { 37184, 137 }, /* The troll deftly catches the axe, examines it carefully, and tosses it
	back, declaring, "Good workmanship, but it's not valuable enough." */
	/* 159 */ { 37321, 63 }, /* The troll catches your treasure and scurries away out of sight. */
	/* 160 */ { 37384, 35 }, /* The troll refuses to let you cross. */
	/* 161 */ { 37419, 44 }, /* There is no longer any way across the chasm. */
	/* 162 */ { 37463, 231 }, /* Just as you reach the other side, the bridge buckles beneath the
	weight of the bear, which was still following you around.  You
	scrabble desperately for support, but as the bridge collapses you
	stumble back and fall into the chasm. */
	/* 163 */ { 37694, 138 }, /* The bear lumbers toward the troll, who lets out a startled shriek and
	scurries away.  The bear soon gives up the pursuit and wanders back. */
	/* 164 */ { 37832, 65 }, /* The axe misses and lands near the bear where you can't get at it. */
	/* 165 */ { 37897, 56 }, /* With what?  Your bare hands?  Against *his* bear hands?? */
	/* 166 */ { 37953, 54 }, /* The bear is confused; he only wants to be your friend. */
	/* 167 */ { 38007, 52 }, /* For crying out loud, the poor thing is already dead! */
	/* 168 */ { 38059, 119 }, /* The bear eagerly wolfs down your food, after which he seems to calm
	down considerably and even becomes rather friendly. */
	/* 169 */ { 38178, 38 }, /* The bear is still chained to the wall. */
	/* 170 */ { 38216, 26 }, /* The chain is still locked. */
	/* 171 */ { 38242, 26 }, /* The chain is now unlocked. */
	/* 172 */ { 38268, 24 }, /* The chain is now locked. */
	/* 173 */ { 38292, 55 }, /* There is nothing here to which the chain can be locked. */
	/* 174 */ { 38347, 29 }, /* There is nothing here to eat. */
	/* 175 */ { 38376, 21 }, /* Do you want the hint? */
	/* 176 */ { 38397, 41 }, /* Do you need help getting out of the maze? */
	/* 177 */ { 38438, 61 }, /* You can make the passages look less alike by dropping things. */
	/* 178 */ { 38499, 49 }, /* Are you trying to explore beyond the Plover Room? */
	/* 179 */ { 38548, 167 }, /* There is a way to explore that region without having to worry about
	falling into a pit.  None of the objects available is immediately
	useful in discovering the secret. */
	/* 180 */ { 38715, 37 }, /* Do you need help getting out of here? */
	/* 181 */ { 38752, 14 }, /* Don't go west. */
	/* 182 */ { 38766, 64 }, /* Gluttony is not one of the troll's vices.  Avarice, however, is. */
	/* 183 */ { 38830, 188 }, /* Your lamp is getting dim.  You'd best start wrapping this up, unless
	You can find some fresh batteries.  I seem to recall there's a vending
	machine in the maze.  Bring some coins with you. */
	/* 184 */ { 39018, 31 }, /* Your lamp has run out of power. */
	/* 185 */ { 39049, 129 }, /* There's not much point in wandering around out here, and you can't
	explore the cave without a lamp.  So let's just call it a day. */
	/* 186 */ { 39178, 317 }, /* There are faint rustling noises from the darkness behind you.  As you
	turn toward them, the beam of your lamp falls across a bearded pirate.
	He is carrying a large chest.  "Shiver me timbers!" he cries, "I've
	been spotted!  I'd best hie meself off to the maze to hide me chest!"
	With that, he vanishes into the gloom. */
	/* 187 */ { 39495, 66 }, /* Your lamp is getting dim.  You'd best go back for those batteries. */
	/* 188 */ { 39561, 77 }, /* Your lamp is getting dim.  I'm taking the liberty of replacing the
	batteries. */
	/* 189 */ { 39638, 96 }, /* Your lamp is getting dim, and you're out of spare batteries.  You'd
	best start wrapping this up. */
	/* 190 */ { 39734, 47 }, /* I'm afraid the magazine is written in Dwarvish. */
	/* 191 */ { 39781, 66 }, /* "This is not the maze where the pirate leaves his treasure chest." */
	/* 192 */ { 39847, 117 }, /* Hmmm, this looks like a clue, which means it'll cost you 10 points to
	read it.  Should I go ahead and read it anyway? */
	/* 193 */ { 39964, 122 }, /* It says, "there is something strange about this place, such that one
	of the words I've always known now has a new effect." */
	/* 194 */ { 40086, 37 }, /* It says the same thing it did before. */
	/* 195 */ { 40123, 30 }, /* I'm afraid I don't understand. */
	/* 196 */ { 40153, 55 }, /* "Congratulations on bringing light into the Dark-Room!" */
	/* 197 */ { 40208, 92 }, /* You strike the mirror a resounding blow, whereupon it shatters into a
	myriad tiny fragments. */
	/* 198 */ { 40300, 63 }, /* You have taken the vase and hurled it delicately to the ground. */
	/* 199 */ { 40363, 104 }, /* You prod the nearest dwarf, who wakes up grumpily, takes one look at
	you, curses, and grabs for his axe. */
	/* 200 */ { 40467, 19 }, /* Is this acceptable? */
	/* 201 */ { 40486, 52 }, /* There's no point in suspending a demonstration game. */
	/* 202 */ { 40538, 124 } /* You awaken only to discover your bits have been dissolving while you
	slept.  You disappear in a cloud of greasy black smoke. */
};

struct text mtext[MAGSIZ] = { // section 12
	{ 40278, 0 },
	{ 40278, 318 }, /* A large cloud of green smoke appears in front of you.  It clears away
	to reveal a tall wizard, clothed in grey.  He fixes you with a steely
	glare and declares, "This adventure has lasted too long."  With that
	he makes a single pass over you with his hands, and everything around
	you fades away into a grey nothingness. */
	{ 40596, 43 }, /* Even wizards have to wait longer than that! */
	{ 40639, 64 }, /* I'm terribly sorry, but Colossal Cave is closed.  Our hours are: */
	{ 40703, 53 }, /* Only wizards are permitted within the cave right now. */
	{ 40756, 96 }, /* We do allow visitors to make short explorations during our off hours.
	Would you like to do that? */
	{ 40852, 68 }, /* Colossal Cave is open to regular adventurers at the following hours: */
	{ 40920, 10 }, /* Very well. */
	{ 40930, 50 }, /* Only a wizard may continue an adventure this soon. */
	{ 40980, 52 }, /* I suggest you resume your adventure at a later time. */
	{ 41032, 29 }, /* Do you wish to see the hours? */
	{ 41061, 32 }, /* Do you wish to change the hours? */
	{ 41093, 41 }, /* New magic word (null to leave unchanged): */
	{ 41134, 43 }, /* New magic number (null to leave unchanged): */
	{ 41177, 45 }, /* Do you wish to change the message of the day? */
	{ 41222, 37 }, /* Okay.  You can save this version now. */
	{ 41259, 17 }, /* Are you a wizard? */
	{ 41276, 30 }, /* Prove it!  Say the magic word! */
	{ 41306, 70 }, /* That is not what I thought it was.  Do you know what I thought it was? */
	{ 41376, 69 }, /* Oh dear, you really *are* a wizard!  Sorry to have bothered you . . . */
	{ 41445, 37 }, /* Foo, you are nothing but a charlatan! */
	{ 41482, 136 }, /* New hours specified by defining "prime time".  Give only the hour
	(E.g. 14, not 14:00 or 2pm).  Enter a negative number after last pair. */
	{ 41618, 28 }, /* New hours for Colossal Cave: */
	{ 41646, 45 }, /* Limit lines to 70 chars.  End with null line. */
	{ 41691, 22 }, /* Line too long, retype: */
	{ 41713, 55 }, /* Not enough room for another line.  Ending message here. */
	{ 41768, 45 }, /* Do you wish to (re)schedule the next holiday? */
	{ 41813, 34 }, /* To begin how many days from today? */
	{ 41847, 43 }, /* To last how many days (zero if no holiday)? */
	{ 41890, 40 }, /* To be called what (up to 20 characters)? */
	{ 41930, 48 }, /* Too small!  Assuming minimum value (45 minutes). */
	{ 41978, 43 } /* Break out of this and save your core-image. */
};

int clsses = 10;

struct text ctext[CLSMAX] = { // section 10
	{ 0, 0 },
	{ "You are obviously a rank amateur.  Better luck next time.", 57 }, 
	{ "Your score qualifies you as a Novice class Adventurer.", 54 },
	{ "You have achieved the rating: \"Experienced Adventurer\".", 55 }, 
	{ "You may now consider yourself a \"Seasoned Adventurer\".", 54 }, 
	{ "You have reached \"Junior Master\" status.", 40 }, 
	{ "Your score puts you in Master Adventurer Class C.", 49 }, 
	{ "Your score puts you in Master Adventurer Class B.", 49 }, 
	{ "Your score puts you in Master Adventurer Class A.", 49 }, 
	{ "All of Adventuredom gives tribute to you, Adventurer Grandmaster!", 65 } 
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
	9999
};

struct travptr tptr;

char *newtravel[LOCSIZ] = { // section 3
	"\x00",
	"\x24\x82\x02\x00\x00\x00\x2C\x1D\x83\x03\x00\x00\x00\x0C\x13\x2B\x85\x04\x00\x00\x00\x0D\x0E\x2E\x1E\x86\x05\x00\x00\x00\x2D\x2B\xBF\x08\x00\x00\x00",
	"\x11\x82\x01\x00\x00\x00\x0C\x07\x2B\x2D\x1E\x86\x05\x00\x00\x00\x2D\x2E",
	"\x18\x83\x01\x00\x00\x00\x0B\x20\x2C\xBE\x0B\x00\x00\x00\xC1\x21\x00\x00\x00\x85\x4F\x00\x00\x00\x0E",
	"\x1B\x84\x01\x00\x00\x00\x0C\x2D\x86\x05\x00\x00\x00\x2B\x2C\x1D\x85\x07\x00\x00\x00\x2E\x1E\xBF\x08\x00\x00\x00",
	"\x19\x89\x04\x00\x00\x00\x2B\x1E\x86\x05\x00\x32\x00\x07\x2D\x86\x06\x00\x00\x00\xAC\x05\x00\x00\x00\x2E",
	"\x14\x82\x01\x00\x00\x00\x2D\x89\x04\x00\x00\x00\x2B\x2C\x1E\x86\x05\x00\x00\x00\x2E",
	"\x21\x8C\x01\x00\x00\x00\x84\x04\x00\x00\x00\x2D\x86\x05\x00\x00\x00\x2B\x2C\x85\x08\x00\x00\x00\x0F\x10\x2E\xBC\x53\x02\x00\x00\x0E\x1E",
	"\x20\x86\x05\x00\x00\x00\x2B\x2C\x2E\x8C\x01\x00\x00\x00\x84\x07\x00\x00\x00\x0D\x2D\x83\x09\x00\x2F\x01\x13\x1E\x83\x51\x02\x00\x00",
	"\x1D\x8B\x08\x00\x2F\x01\x1D\x8B\x51\x02\x00\x00\x91\x0A\x00\x00\x00\x12\x13\x2C\x9F\x0E\x00\x00\x00\xB3\x0B\x00\x00\x00",
	"\x15\x8B\x09\x00\x00\x00\x14\x15\x2B\x93\x0B\x00\x00\x00\x16\x2C\x33\x9F\x0E\x00\x00\x00",
	"\x25\xBF\x08\x00\x2F\x01\xC0\x09\x00\x00\x00\x91\x0A\x00\x00\x00\x12\x17\x18\x2B\x99\x0C\x00\x00\x00\x13\x1D\x2C\xBE\x03\x00\x00\x00\x9F\x0E\x00\x00\x00",
	"\x1D\xBF\x08\x00\x2F\x01\xC0\x09\x00\x00\x00\x9E\x0B\x00\x00\x00\x2B\x33\x93\x0D\x00\x00\x00\x1D\x2C\x9F\x0E\x00\x00\x00",
	"\x1C\xBF\x08\x00\x2F\x01\xC0\x09\x00\x00\x00\xB3\x0B\x00\x00\x00\x99\x0C\x00\x00\x00\x2B\x97\x0E\x00\x00\x00\x1F\x2C",
	"\x27\xBF\x08\x00\x2F\x01\xC0\x09\x00\x00\x00\xB3\x0B\x00\x00\x00\x97\x0D\x00\x00\x00\x2B\x9E\x14\x00\x96\x00\x1F\x22\x9E\x0F\x00\x00\x00\xA1\x10\x00\x00\x00\x2C",
	"\x28\xA4\x12\x00\x00\x00\x2E\x87\x11\x00\x00\x00\x26\x2C\x8A\x13\x00\x00\x00\x1E\x2D\x9D\x16\x00\x96\x00\x1F\x22\x23\x17\x2B\x9D\x0E\x00\x00\x00\xB7\x22\x00\x00\x00",
	"\x05\x81\x0E\x00\x00\x00",
	"\x1D\xA6\x0F\x00\x00\x00\x2B\xA7\x54\x02\x38\x01\x87\x15\x00\x9C\x01\xA9\x55\x02\x9C\x01\x2A\x2C\x45\xA9\x1B\x00\x00\x00",
	"\x07\xA6\x0F\x00\x00\x00\x0B\x2D",
	"\x2D\x8A\x0F\x00\x00\x00\x1D\x2B\xAD\x1C\x00\x37\x01\x24\xAE\x1D\x00\x37\x01\x25\xAC\x1E\x00\x37\x01\x07\xAD\x20\x00\x00\x00\xB1\x4A\x00\x23\x00\xB1\x20\x00\xD3\x00\xC2\x4A\x00\x00\x00",
	"\x05\x81\x00\x00\x00\x00",
	"\x05\x81\x00\x00\x00\x00",
	"\x05\x81\x0F\x00\x00\x00",
	"\x17\xAB\x43\x00\x00\x00\x2A\xAC\x44\x00\x00\x00\x3D\x9E\x19\x00\x00\x00\x1F\xB4\x88\x02\x00\x00",
	"\x06\x9D\x43\x00\x00\x00\x0B",
	"\x10\x9D\x17\x00\x00\x00\x0B\xB8\x1F\x00\xD4\x02\xB8\x1A\x00\x00\x00",
	"\x05\x81\x58\x00\x00\x00",
	"\x21\xA7\x54\x02\x38\x01\x87\x15\x00\x9C\x01\xA9\x55\x02\x9C\x01\x2A\x2B\x45\xA9\x11\x00\x00\x00\xAD\x28\x00\x00\x00\xAC\x29\x00\x00\x00",
	"\x13\xA6\x13\x00\x00\x00\x0B\x2E\xAD\x21\x00\x00\x00\x37\x9E\x24\x00\x00\x00\x34",
	"\x07\xA6\x13\x00\x00\x00\x0B\x2D",
	"\x0D\xA6\x13\x00\x00\x00\x0B\x2B\xAC\x3E\x00\x00\x00\x1D",
	"\x0A\x81\x59\x00\x0C\x02\x81\x5A\x00\x00\x00",
	"\x05\x81\x13\x00\x00\x00",
	"\x20\xC1\x03\x00\x00\x00\xAE\x1C\x00\x00\x00\xAB\x22\x00\x00\x00\x35\x36\xAC\x23\x00\x00\x00\xC7\x2E\x01\x9F\x00\xC7\x64\x00\x00\x00",
	"\x0B\x9E\x21\x00\x00\x00\x37\x9D\x0F\x00\x00\x00",
	"\x0B\xAB\x21\x00\x00\x00\x37\xA7\x14\x00\x00\x00",
	"\x16\xAB\x25\x00\x00\x00\x11\x9D\x1C\x00\x00\x00\x34\xAC\x27\x00\x00\x00\xC6\x41\x00\x00\x00",
	"\x0D\xAC\x24\x00\x00\x00\x11\x9E\x26\x00\x00\x00\x1F\x38",
	"\x10\xB8\x25\x00\x00\x00\x1D\x0B\xBC\x53\x02\x00\x00\x0E\x1E\x04\x05",
	"\x12\xAB\x24\x00\x00\x00\x17\x9E\x40\x00\x00\x00\x34\x3A\xC6\x41\x00\x00\x00",
	"\x05\x81\x29\x00\x00\x00",
	"\x18\xAE\x2A\x00\x00\x00\x1D\x17\x38\xAB\x1B\x00\x00\x00\xAD\x3B\x00\x00\x00\xAC\x3C\x00\x00\x00\x11",
	"\x19\x9D\x29\x00\x00\x00\xAD\x2A\x00\x00\x00\xAB\x2B\x00\x00\x00\xAE\x2D\x00\x00\x00\xAC\x50\x00\x00\x00",
	"\x0F\xAC\x2A\x00\x00\x00\xAE\x2C\x00\x00\x00\xAB\x2D\x00\x00\x00",
	"\x14\xAB\x2B\x00\x00\x00\x9E\x30\x00\x00\x00\xAE\x32\x00\x00\x00\xAD\x52\x00\x00\x00",
	"\x1A\xAC\x2A\x00\x00\x00\xAD\x2B\x00\x00\x00\xAB\x2E\x00\x00\x00\xAE\x2F\x00\x00\x00\x9D\x57\x00\x00\x00\x1E",
	"\x06\xAC\x2D\x00\x00\x00\x0B",
	"\x06\xAB\x2D\x00\x00\x00\x0B",
	"\x06\x9D\x2C\x00\x00\x00\x0B",
	"\x0A\xAB\x32\x00\x00\x00\xAC\x33\x00\x00\x00",
	"\x14\xAB\x2C\x00\x00\x00\xAC\x31\x00\x00\x00\x9E\x33\x00\x00\x00\xAE\x34\x00\x00\x00",
	"\x14\xAC\x31\x00\x00\x00\x9D\x32\x00\x00\x00\xAB\x34\x00\x00\x00\xAE\x35\x00\x00\x00",
	"\x1E\xAC\x32\x00\x00\x00\xAB\x33\x00\x00\x00\xAE\x34\x00\x00\x00\x9D\x35\x00\x00\x00\xAD\x37\x00\x00\x00\x9E\x56\x00\x00\x00",
	"\x0F\xAC\x33\x00\x00\x00\xAD\x34\x00\x00\x00\xAE\x36\x00\x00\x00",
	"\x06\xAC\x35\x00\x00\x00\x0B",
	"\x14\xAC\x34\x00\x00\x00\xAD\x37\x00\x00\x00\x9E\x38\x00\x00\x00\xAB\x39\x00\x00\x00",
	"\x06\x9D\x37\x00\x00\x00\x0B",
	"\x1A\x9E\x0D\x00\x00\x00\x38\xAC\x37\x00\x00\x00\xAE\x3A\x00\x00\x00\xAD\x53\x00\x00\x00\xAB\x54\x00\x00\x00",
	"\x06\xAB\x39\x00\x00\x00\x0B",
	"\x05\x81\x1B\x00\x00\x00",
	"\x13\xAB\x29\x00\x00\x00\x1D\x11\xAC\x3D\x00\x00\x00\xAD\x3E\x00\x00\x00\x1E\x34",
	"\x0F\xAB\x3C\x00\x00\x00\xAD\x3E\x00\x00\x00\xAE\x6B\x00\x64\x00",
	"\x14\xAC\x3C\x00\x00\x00\xAD\x3F\x00\x00\x00\xAB\x1E\x00\x00\x00\xAE\x3D\x00\x00\x00",
	"\x06\xAE\x3E\x00\x00\x00\x0B",
	"\x18\x9D\x27\x00\x00\x00\x38\x3B\xAC\x41\x00\x00\x00\x46\xAD\x67\x00\x00\x00\x4A\xAB\x6A\x00\x00\x00",
	"\x3C\xAB\x40\x00\x00\x00\xAC\x42\x00\x00\x00\xAE\x2C\x02\x50\x00\xBD\x44\x00\x00\x00\x9D\x2C\x02\x50\x00\x9D\x46\x00\x32\x00\x9D\x27\x00\x00\x00\xAD\x2C\x02\x3C\x00\xAD\x48\x00\x4B\x00\xAD\x47\x00\x00\x00\x9E\x2C\x02\x50\x00\x9E\x6A\x00\x00\x00",
	"\x23\xAF\x41\x00\x00\x00\xAC\x43\x00\x00\x00\xAE\x2C\x02\x50\x00\x99\x4D\x00\x00\x00\xAB\x60\x00\x00\x00\xB2\x2C\x02\x32\x00\xC8\x61\x00\x00\x00",
	"\x11\xAB\x42\x00\x00\x00\xAC\x17\x00\x00\x00\x2A\x9E\x18\x00\x00\x00\x1F",
	"\x10\xAE\x17\x00\x00\x00\x9D\x45\x00\x00\x00\x38\xAD\x41\x00\x00\x00",
	"\x1A\x9E\x44\x00\x00\x00\x3D\xAE\x78\x00\x4B\x01\xAE\x77\x00\x00\x00\xAD\x6D\x00\x00\x00\xCB\x71\x00\x00\x00",
	"\x10\xAD\x47\x00\x00\x00\x9E\x41\x00\x00\x00\x17\xAE\x6F\x00\x00\x00",
	"\x0F\xB0\x41\x00\x00\x00\xAE\x46\x00\x00\x00\xAD\x6E\x00\x00\x00",
	"\x15\xC6\x41\x00\x00\x00\xB1\x76\x00\x00\x00\xAD\x49\x00\x00\x00\xB0\x61\x00\x00\x00\x48",
	"\x07\xAE\x48\x00\x00\x00\x11\x0B",
	"\x14\xAB\x13\x00\x00\x00\xAC\x78\x00\x4B\x01\xAC\x79\x00\x00\x00\x9E\x4B\x00\x00\x00",
	"\x0A\xAE\x4C\x00\x00\x00\xAD\x4D\x00\x00\x00",
	"\x05\xAD\x4B\x00\x00\x00",
	"\x10\xAB\x4B\x00\x00\x00\xAC\x4E\x00\x00\x00\xAD\x42\x00\x00\x00\x11",
	"\x05\xAE\x4D\x00\x00\x00",
	"\x05\x81\x03\x00\x00\x00",
	"\x14\xAD\x2A\x00\x00\x00\xAC\x50\x00\x00\x00\xAE\x50\x00\x00\x00\xAB\x51\x00\x00\x00",
	"\x06\xAC\x50\x00\x00\x00\x0B",
	"\x06\xAE\x2C\x00\x00\x00\x0B",
	"\x0F\xAE\x39\x00\x00\x00\xAB\x54\x00\x00\x00\xAC\x55\x00\x00\x00",
	"\x0F\xAD\x39\x00\x00\x00\xAC\x53\x00\x00\x00\xB2\x72\x00\x00\x00",
	"\x06\xAB\x53\x00\x00\x00\x0B",
	"\x06\x9D\x34\x00\x00\x00\x0B",
	"\x06\x9D\x2D\x00\x00\x00\x1E",
	"\x12\x9E\x19\x00\x00\x00\x38\x2B\xA7\x14\x00\x00\x00\xAC\x5C\x00\x00\x00\x1B",
	"\x05\x81\x19\x00\x00\x00",
	"\x05\x81\x17\x00\x00\x00",
	"\x0D\xAD\x5F\x00\x00\x00\x49\x17\x9E\x48\x00\x00\x00\x38",
	"\x0F\xAE\x58\x00\x00\x00\xAB\x5D\x00\x00\x00\xAD\x5E\x00\x00\x00",
	"\x07\xAE\x5C\x00\x00\x00\x1B\x0B",
	"\x13\xAE\x5C\x00\x00\x00\x1B\x17\xAD\x5F\x00\x35\x01\x03\x49\xAD\x63\x02\x00\x00",
	"\x10\xAE\x5E\x00\x00\x00\x0B\x9B\x5C\x00\x00\x00\xAC\x5B\x00\x00\x00",
	"\x06\xAC\x42\x00\x00\x00\x0B",
	"\x12\xB0\x42\x00\x00\x00\xAC\x48\x00\x00\x00\x11\x9D\x62\x00\x00\x00\x2D\x49",
	"\x0B\xAE\x61\x00\x00\x00\x48\xAC\x63\x00\x00\x00",
	"\x11\xB2\x62\x00\x00\x00\x49\xAB\x2D\x01\x00\x00\x17\xAB\x64\x00\x00\x00",
	"\x1C\xAC\x2D\x01\x00\x00\x17\x0B\xAC\x63\x00\x00\x00\xC7\x2E\x01\x9F\x00\xC7\x21\x00\x00\x00\xAF\x65\x00\x00\x00\x16",
	"\x07\xAE\x64\x00\x00\x00\x47\x0B",
	"\x07\x9E\x67\x00\x00\x00\x4A\x0B",
	"\x1A\x9D\x66\x00\x00\x00\x26\x9E\x68\x00\x00\x00\xAE\x6A\x02\x72\x00\xAE\x6B\x02\x73\x00\xAE\x40\x00\x00\x00",
	"\x0B\x9D\x67\x00\x00\x00\x4A\x9E\x69\x00\x00\x00",
	"\x0B\x9D\x68\x00\x00\x00\x0B\xCA\x67\x00\x00\x00",
	"\x0F\x9D\x40\x00\x00\x00\xAC\x41\x00\x00\x00\xAB\x6C\x00\x00\x00",
	"\x32\xAE\x83\x00\x00\x00\xB1\x84\x00\x00\x00\xAF\x85\x00\x00\x00\xB0\x86\x00\x00\x00\x9D\x87\x00\x00\x00\xB2\x88\x00\x00\x00\xAB\x89\x00\x00\x00\xAC\x8A\x00\x00\x00\xAD\x8B\x00\x00\x00\x9E\x3D\x00\x00\x00",
	"\x17\xAB\x2C\x02\x5F\x00\x2D\x2E\x2F\x30\x31\x32\x1D\x1E\xAB\x6A\x00\x00\x00\xAC\x72\x02\x00\x00",
	"\x0B\xAE\x45\x00\x00\x00\xAD\x71\x00\x00\x00\x4B",
	"\x0A\xAC\x47\x00\x00\x00\xA7\x14\x00\x00\x00",
	"\x16\xAD\x46\x00\x00\x00\x9E\x32\x00\x28\x00\x27\x38\x9E\x35\x00\x32\x00\x9E\x2D\x00\x00\x00",
	"\x32\xB1\x83\x00\x00\x00\xAD\x84\x00\x00\x00\xAB\x85\x00\x00\x00\xB2\x86\x00\x00\x00\xB0\x87\x00\x00\x00\xAF\x88\x00\x00\x00\xAC\x89\x00\x00\x00\x9E\x8A\x00\x00\x00\x9D\x8B\x00\x00\x00\xAE\x8C\x00\x00\x00",
	"\x07\xAE\x6D\x00\x00\x00\x0B\x6D",
	"\x05\xB0\x54\x00\x00\x00",
	"\x05\xB1\x74\x00\x00\x00",
	"\x0A\xAF\x73\x00\x00\x00\x9E\x51\x02\x00\x00",
	"\x21\xB1\x76\x00\x00\x00\xA9\x94\x02\xE9\x00\x2A\x45\x2F\xA9\x95\x02\x4C\x01\xA9\x2F\x01\x00\x00\xA7\x15\x00\x4C\x01\xA7\x54\x02\x00\x00",
	"\x0A\x9E\x48\x00\x00\x00\x9D\x75\x00\x00\x00",
	"\x0C\xAD\x45\x00\x00\x00\x0B\xAB\x8D\x02\x00\x00\x07",
	"\x0A\xAD\x45\x00\x00\x00\xAB\x4A\x00\x00\x00",
	"\x0C\xAB\x4A\x00\x00\x00\x0B\xAD\x8D\x02\x00\x00\x07",
	"\x26\xAF\x7B\x00\x00\x00\xA9\x94\x02\xE9\x00\x2A\x45\x31\xA9\x2F\x01\x00\x00\xA7\x54\x02\x00\x00\xCD\x7C\x00\x00\x00\x9C\x7E\x00\x00\x00\xA8\x81\x00\x00\x00",
	"\x15\xAC\x7A\x00\x00\x00\xAB\x7C\x00\x00\x00\x4D\x9C\x7E\x00\x00\x00\xA8\x81\x00\x00\x00",
	"\x1C\xAC\x7B\x00\x00\x00\xAF\x7D\x00\x00\x00\x24\xB0\x80\x00\x00\x00\x25\x1E\x9C\x7E\x00\x00\x00\xA8\x81\x00\x00\x00",
	"\x12\xAE\x7C\x00\x00\x00\x4D\xAD\x7E\x00\x00\x00\x1C\xAB\x7F\x00\x00\x00\x11",
	"\x12\xAE\x7D\x00\x00\x00\x17\x0B\xCD\x7C\x00\x00\x00\x9E\x62\x02\x00\x00\x27",
	"\x11\xAC\x7D\x00\x00\x00\x0B\x11\xCD\x7C\x00\x00\x00\x9C\x7E\x00\x00\x00",
	"\x13\xAD\x7C\x00\x00\x00\x1D\x4D\xAE\x81\x00\x00\x00\x1E\x28\x9C\x7E\x00\x00\x00",
	"\x18\xAC\x80\x00\x00\x00\x1D\xCD\x7C\x00\x00\x00\xAB\x82\x00\x00\x00\x13\x28\x03\x9C\x7E\x00\x00\x00",
	"\x10\xAC\x81\x00\x00\x00\x0B\xCD\x7C\x00\x00\x00\x9C\x7E\x00\x00\x00",
	"\x32\xAC\x6B\x00\x00\x00\xB0\x84\x00\x00\x00\xB2\x85\x00\x00\x00\xB1\x86\x00\x00\x00\xAF\x87\x00\x00\x00\x9D\x88\x00\x00\x00\x9E\x89\x00\x00\x00\xAD\x8A\x00\x00\x00\xAE\x8B\x00\x00\x00\xAB\x70\x00\x00\x00",
	"\x32\xB2\x6B\x00\x00\x00\x9D\x83\x00\x00\x00\xAD\x85\x00\x00\x00\xAE\x86\x00\x00\x00\xAC\x87\x00\x00\x00\xB1\x88\x00\x00\x00\xAF\x89\x00\x00\x00\xAB\x8A\x00\x00\x00\x9E\x8B\x00\x00\x00\xB0\x70\x00\x00\x00",
	"\x32\x9D\x6B\x00\x00\x00\x9E\x83\x00\x00\x00\xAC\x84\x00\x00\x00\xAF\x86\x00\x00\x00\xB1\x87\x00\x00\x00\xAB\x88\x00\x00\x00\xAD\x89\x00\x00\x00\xB2\x8A\x00\x00\x00\xB0\x8B\x00\x00\x00\xAE\x70\x00\x00\x00",
	"\x32\xAF\x6B\x00\x00\x00\xAD\x83\x00\x00\x00\xB2\x84\x00\x00\x00\xB0\x85\x00\x00\x00\xAB\x87\x00\x00\x00\x9E\x88\x00\x00\x00\xAE\x89\x00\x00\x00\x9D\x8A\x00\x00\x00\xAC\x8B\x00\x00\x00\xB1\x70\x00\x00\x00",
	"\x32\xAD\x6B\x00\x00\x00\xB0\x83\x00\x00\x00\x9E\x84\x00\x00\x00\xAE\x85\x00\x00\x00\xAB\x86\x00\x00\x00\xAC\x88\x00\x00\x00\xB1\x89\x00\x00\x00\xAF\x8A\x00\x00\x00\xB2\x8B\x00\x00\x00\x9D\x70\x00\x00\x00",
	"\x32\xAB\x6B\x00\x00\x00\xAC\x83\x00\x00\x00\x9D\x84\x00\x00\x00\xB1\x85\x00\x00\x00\x9E\x86\x00\x00\x00\xAE\x87\x00\x00\x00\xB2\x89\x00\x00\x00\xB0\x8A\x00\x00\x00\xAF\x8B\x00\x00\x00\xAD\x70\x00\x00\x00",
	"\x32\xB0\x6B\x00\x00\x00\xAF\x83\x00\x00\x00\xAE\x84\x00\x00\x00\x9E\x85\x00\x00\x00\x9D\x86\x00\x00\x00\xB2\x87\x00\x00\x00\xAD\x88\x00\x00\x00\xB1\x8A\x00\x00\x00\xAB\x8B\x00\x00\x00\xAC\x70\x00\x00\x00",
	"\x32\x9E\x6B\x00\x00\x00\xAB\x83\x00\x00\x00\xAF\x84\x00\x00\x00\x9D\x85\x00\x00\x00\xAC\x86\x00\x00\x00\xAD\x87\x00\x00\x00\xAE\x88\x00\x00\x00\xB0\x89\x00\x00\x00\xB1\x8B\x00\x00\x00\xB2\x70\x00\x00\x00",
	"\x32\xB1\x6B\x00\x00\x00\xB2\x83\x00\x00\x00\xAB\x84\x00\x00\x00\xAC\x85\x00\x00\x00\xAD\x86\x00\x00\x00\x9E\x87\x00\x00\x00\xB0\x88\x00\x00\x00\x9D\x89\x00\x00\x00\xAE\x8A\x00\x00\x00\xAF\x70\x00\x00\x00",
	"\x06\xAD\x70\x00\x00\x00\x0B"
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
/* 9 */	{ 0, 20, 3, 180, 181 }
};

int fixd[101] = { // section 7 1/2
	0, 0, 0, 9, 0, 0, 0, 15, 0, -1, 0, -1, 27, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, -1, 67, -1, 110, 0, -1, -1, 121, 122, 122, 0, -1, -1, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 121, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int plac[101] = { // section 7 2/2
	0, 3, 3, 8, 10, 11, 0, 14, 13, 94, 96, 19, 17, 101, 103, 0, 106, 0, 0, 3, 3, 0, 0, 109, 25, 23, 111, 35, 0, 97, 0, 119, 117, 117, 0, 130, 0, 126, 140, 0, 96, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 27, 28, 29, 30, 0, 92, 95, 97, 100, 101, 0, 119, 127, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	
};

int cond[LOCSIZ] = { // section 9
	0, 2, 2, 2, 2, 2, 2, 2, 6, 2, 2, 0, 0, 7, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 1, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10, 10, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1, 1, 1, 1, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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

char buffer[BUFFER_SIZE];