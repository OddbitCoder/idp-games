#pragma once

typedef struct {
	uint16_t pos;
	uint16_t len;
} message;

FILE *m_file = NULL;
char m_buffer[2048];

void doneWithFiles() {
	if (m_file != NULL) {
		fclose(m_file);
	}
}

const message messages[] = {
	/* 0 */ { 0, 0 }, //
	/* 1 */ { 0, 303 }, // The paint is peeling off the walls and the floor hasn't been cleaned in months.
	// Cockroaches run across the floor, jumping as the loosely installed lightbulb
	// crackles and flickers. An old desk sits pushed against the wall. A businessman
	// sits on a broken chair next to the desk. He seems kind of drunk.
	/* 2 */ { 303, 302 }, // The stench is unbelievable! Graffiti is all over the walls. Cockroaches don't
	// seem to survive in this place - their dead bodies are strewn everywhere. The
	// sink's faucets are broken - in fact the sink hangs from the wall by its rusted
	// plumbing. A toilet sits in the corner. This baby looks dangerous!
	/* 3 */ { 605, 251 }, // Behind the bar sits a bartender. A sign hanging over him says "beer $100,
	// whiskey $100". The place isn't furnished too well. A curtain hangs on one wall.
	// Next to the curtain is a button. A fan whirls slowly overhead, moving the
	// stagnant air around.
	/* 4 */ { 856, 364 }, // A couple stray dogs wander around. A cat is crouched in the entrance watching
	// the dogs. Old beat-up cars drive by. The sirens of police cars and ambulances
	// wail in the distance. Some paper blows by from the overfilled garbage dumpster
	// next to me. I just miss stepping on a dogs 'calling card'. The dogs look at
	// me -- I hope they don't think I'm a fire hydrant!
	/* 5 */ { 1220, 255 }, // There's this big dude in here with me. He's wearing a button -- I can't see
	// what it says, maybe I should take a closer look at him. Stairs lead up to
	// the second floor. There's also a TV in the corner. I get the feeling
	// loitering is not encouraged here!
	/* 6 */ { 1475, 366 }, // There's a fire escape ladder above me which lowers automatically whenever
	// weight is put on it. As a result, I find myself in the garbage dumpster which
	// some fool placed under it! The trash in this thing is absolutely foul! I'm
	// sitting in a sea of coffee grinds and egg shells. Various pieces of trash
	// surround me... I don't like this! I think I'm gonna throw up!
	/* 7 */ { 1841, 278 }, // The entrance on the far side of the room is locked shut. Through the door's
	// peephole I see a hallway with people in it, so that won't make a good exit. The
	// plaster's falling off the wall - the usual decor for this building. Through the
	// window leads the safety rope I'm using.
	/* 8 */ { 2119, 187 }, // My safety rope leads back to the fire escape. While it helps me some I could
	// still fall and kill myself, so be careful! The window looks into a room, but I
	// can't see too much from here.
	/* 9 */ { 2306, 237 }, // There's a hooker in here. The bed's a mess and the hooker's about the same! The
	// room is painted bright pink and the ceiling is covered with mirrors. A fire
	// exit is to the north. A sign says, "Take precautions! The clap could be fatal!"
	/* 10 */ { 2543, 271 }, // Off in the distance I see a brightly lit billboard. A fire escape ladder is at
	// one end of the balcony. A sign says "use only in extreme emergency!" Looking
	// over towards the west end of the balcony I see a window ledge. It looks too
	// dangerous to go there; I might fall!
	/* 11 */ { 2814, 346 }, // People from all walks of life are milling about. Cadillacs, limos and Mercedes
	// sportcars drive up and down the street bringing gamblers to and from the
	// casinos. To the north is an establishment - an advertisement says "use our
	// services to enter into a blissful life!" To the east is "The Adventurers
	// Hotel". Seems like quite the classy place!
	/* 12 */ { 3160, 211 }, // A flashing neon sign says "Why wait? Marry the girl of your dream today! You
	// provide the girl - we provide a legal marriage for only $1000!". A plaque hangs
	// below the sign, proclaiming "over 1 million served."
	/* 13 */ { 3371, 312 }, // Row upon row of slot machines fill the room. The bells of the slot machines
	// clamor as winnings are paid out to the winners.The police cart off one of the
	// losers. Vagrancy is not tolerated here -- vagrants are exterminated! It keeps
	// the crime rate low. But everyone seems happy here. It's a gambler's paradise!
	/* 14 */ { 3683, 283 }, // Tables for playing blackjack are everywhere. The noise of people winning and
	// losing fortunes fills the room. A table stands in front of me -- the dealer
	// waits for me to join in. People gather as they want to see me gamble my fortune
	// away! A voice within me says "Go for it, fool!"
	/* 15 */ { 3966, 308 }, // There's a stairway going up to the hotel desk. The only other exit leads back
	// to the casino. Over in the corner is a flourishing plant which is sitting in a
	// pot. Couches and tables are also in the room, as are other items one would
	// normally find in a lobby. The lobby is empty - everybody is out gambling.
	/* 16 */ { 4274, 262 }, // The decor is fabulous! A giant heart shaped bed sits in one corner of the room.
	// The floor is covered with a deep shag rug. A tiffany lamp provides just the
	// right amount of light to complete the atmosphere. A breeze flows through the
	// curtains on the east wall.
	/* 17 */ { 4536, 248 }, // Doors line each side. Most have "do not disturb" signs hung on the doorknobs.
	// Waiters from room service pass by bringing food and drink to the hotel guests.
	// An ashtray stands next to the wall. To the south is the entrance to the
	// honeymoon suite.
	/* 18 */ { 4784, 219 }, // A high wooden fence surrounds the porch. There's a little hole in the fence. An
	// arrow points at it - written next to it is a message: "Look here!" The sun is
	// out -- it's rays beat down on the porch making it very hot.
	/* 19 */ { 5003, 167 }, // A sign says "no vacancy". There's an elevator next to the counter marked
	// "penthouse - private". A button is next to the elevator. The smell of perfume
	// fills the air.
	/* 20 */ { 5170, 177 }, // The directory is all ripped up and piled on the floor. There's some numbers
	// scribbled on the side of the telephone. The only ones which are legible read
	// 555-6969 and 555-0439.
	/* 21 */ { 5347, 292 }, // There's a crazy DJ playing the newest hits. The dance floor is filled with guys
	// and gals doin' the best steps in town. The crowd is really getting into it -
	// everybody's having fun. There's a table to stand at over by the dance floor. A
	// little cardboard sign at the table says: "wine $100".
	/* 22 */ { 5639, 251 }, // To the north is a fancy doorway - the entrance to the Disco Club. An old bum
	// sits by the entrance - he's definitely seen better days. The guy really looks
	// tormented! To the east is a pharmacy. Apartment houses and homes line the rest
	// of the street.
	/* 23 */ { 5890, 224 }, // Pictures line the entrance way, showing the happy singles who attend the club.
	// Singles pass by me into the club. Couples wander out - kissing and making eyes
	// at each other. A door is to the west. The door has a sign on it.
	/* 24 */ { 6114, 261 }, // A pharmacist sits behing the counter. On one wall sits a magazine rack. A sign
	// reads "this is not a library - no reading". A mirror to protect against
	// shoplifting is mounted in the corner. Kids stop and buy candy. Others buy
	// newspapers, cigarettes, and such.
	/* 25 */ { 6375, 155 }, // Over in the corner is a spiral staircase. Next to the elevator is a button. The
	// place is nicely decorated - no expense spared! The kitchen is to the east.
	/* 26 */ { 6530, 233 }, // Oh boy does this feel good! Water swirls around me - its warmth soaks into my
	// body. The feeling of relaxation is almost numbing. Over on the other side of
	// the jacuzzi is a most beautiful girl! I hope she doesn't mind me being here.
	/* 27 */ { 6763, 177 }, // There's a sink to one side. High over the sink is mounted a cabinet. There's no
	// dishes in sight - the place is kept quite tidy. Real nice. There's a little
	// sign over the sink.
	/* 28 */ { 6940, 295 }, // The air is filled with the aroma of all sorts of plants. Green ferns are
	// everywhere. Roses and other flowers emit their wonderful fragrances. If ever
	// there was a garden of Eden - this certainly has to be the place! The entrance
	// disappeared just as I walked in! Hmmm, how do I get out of here?
	/* 29 */ { 7235, 126 }, // There's a closet on one wall. Nobody is here, but I think there is a gurgling
	// noise coming from somewhere. A very nice place!
	/* 30 */ { 7361, 200 }, // The sun shines in amongst the plants and trees - birds flutter about. There's a
	// jacuzzi in the middle of the porch. Well, there's the source of the gurgling
	// noise! A wooden fence surrounds the area.
	/* 31 */ { 7561, 215 }, // Oh no! I paid for this?! OTHER people pay for this!? This beast is really ugly!
	// Jeez! I hope I don't get the clap from this hooker. Well... she seems to be
	// annoyed that I haven't jumped on her yet. Go to it, stud!
	/* 32 */ { 7776, 229 }, // It's the Gambler's Gazette! There's an article here which tells how to activate
	// the games at the Adventurer's Hotel. It says that Blackjack can be played by
	// entering "Play 21". The slot machines can be started with "Play Slot".
	/* 33 */ { 8005, 394 }, // Hmmmm. An interesting magazine with a nice centerfold! The feature article is
	// about how to pick up an innocent girl at a disco. It says, "Shower her with
	// presents. Dancing won't hurt either. And wine is always good to get thing
	// moving!"
	//
	// (Kawa would like to interrupt that this kind of article is absolute garbage
	// amd should not be taken as serious advice to apply in real life. Play on.)
	/* 34 */ { 8399, 327 }, // Cute and innocent! Just the way I like my women. Oh, this girl is great! She
	// has a beautiful California tan, pert little breasts, a trim waist, and well-
	// rounded hips! I dream about getting this nice a girl. I hope you play this game
	// well enough so I can have my jolly with her! You could make your puppet a very
	// happy man!
	/* 35 */ { 8726, 317 }, // What a beautiful face! She's leaning back in the jacuzzi with her eyes closed
	// and seems extremely relaxed. The water is bubbling up around her. She's so
	// beautiful, a guy really could fall in love with a girl like this. I presume her
	// name is Eve -- at least that's what the towel next to her has embrodiered on
	// it.
	/* 36 */ { 9043, 211 }, // A taxi pulls up and screeches to a halt! I get in the back and sit down. A sign
	// says "We service 3 destinations. When asked, please specify - Disco, Casino, or
	// Bar". The driver turns and asks: 'Where to, Mac?'
	/* 37 */ { 9254, 216 }, // The elevator doors open and I get in. As the doors close the music starts
	// playing -- it's the usual elevator stuff. Boring! We start to move and after a
	// few seconds the elevator stops. The doors open and I get out.
	/* 38 */ { 9470, 189 }, // "Me first" she says. She takes my throbbing tool into her mouth and starts
	// going to work. Feels so good! Then she smiles and bites it off. She says "No
	// oral sex in this game! Suffeeerrr!"
	/* 39 */ { 9659, 340 }, // "Well, sonny... here's my story. I came here many years ago - and my goals
	// were the same as yours. But this adventure was too much for me! Here's a gift.
	// Carry it with you at all times! There's some kinky girls in this town! And you
	// never know when you may need to use this to defend yourself! After all you may
	// get in a program bug..."
	/* 40 */ { 9999, 271 }, // She's wearing the tightest jeans! Wow, what a body! 36-24-35! This girl's
	// derriere is sensational! And the shirt? See-through, and what I see I like! As
	// my eyes reluctantly roam from her body, I see bright blue eyes - and a smile
	// that dazzles me. I think she likes me!
	/* 41 */ { 10270, 185 }, // A masked man runs across the screen. Jumping up he lands on his horse and yells
	// "Hi-ho Plutonium!" He rides off into a green sky. Ah, nothing like a good ol'
	// western to pass the time.
	/* 42 */ { 10455, 246 }, // It's "The Price is Fright!" "And now for out favorite host, Haunty Male!"
	// Haunty jumps up the stage and asks "and who's our first lucky contestant?" The
	// announcer points out a lady. The crowd screams in ecstacy as she's dragged to
	// the stage...
	/* 43 */ { 10701, 333 }, // Captain Jerk looks at this door from which behind the noise is coming. Throwing
	// open the door - his face turns a deep red! He says: "Scotty! What are you
	// doing?" Scotty replies, "But Captain, me girl and I - we're engaged!" Jerk
	// commands, "Well, then disengage!" as the starship thrusted forward, penetrating
	// deeper into space...
	/* 44 */ { 11034, 410 }, // The News! Today the prime rate was raised once again, to 257%! This does not
	// come near the record set in 1996 - when it broke the 1000% mark. The birth rate
	// has taken a dramatic fall. This is due to the increased usage of computers as
	// sexual partners. However, rapes of innocent people are on the increase! And who
	// is the rapist?? Computerized banking machines lead the list, followed by home
	// computers...
	/* 45 */ { 11444, 82 }, // "I love you, you love me, I will kill your family." Oh god no, anything but
	// that!
	/* 46 */ { 11526, 192 }, // Cable TV! They're showing the kinkiest X-rated movies! This one's titled "Deep
	// Nostril". The pimp seems to like this one! He's engrossed in the action he sees!
	// Seems down-right distracted...
	/* 47 */ { 11718, 242 }, // It's Happy Daze! Richie turns to Gonzo and says "But you always had it made
	// with the girls. What's your secret?" The Gonz replies "Ayyy, I didn't get my
	// name for nuthin!" Reaching into his pocket he pulls out a funny looking
	// cigarette...
	//
	/* 48 */ { 11960, 343 }, // Mrs Smith and Mrs Jones are comparing detergents. "See this blouse? We're
	// making it this dirty to see who's works better." A dog is thrown onto the
	// blouse. In his excitement he defecates all over it. "Do you think yours will
	// work, Mrs Smith?" The camera pans to Mrs Smith. She throws up. "Mrs Jones?"
	// A shot shows her taking the dog and...
	/* 49 */ { 12303, 246 }, // It's the Superb Owl! "The center snaps the ball! The quarterback fades back!
	// It's a bomb! The ball sails through the air... the receiver runs to get it...
	// It explodes in his hands! What a bomb! Tell me, Howard, have you ever seen
	// this before?"
	/* 50 */ { 12549, 231 }, // She takes the apple and raises it to her mouth. With an outrageously innocent
	// look she takes a small bite out of it. A smile comes across her face! She's
	// really starting to look quite sexy! She winks and lays back in the jacuzzi.
	/* 51 */ { 12780, 220 }, // It's a good thing I was wearing that rubber! She was okay, I guess, but really?
	// Can't you do better that this? The score is now 1 out of a possible 3... so
	// congratulations! Well, go to it ya stud! Find me another girl!
	/* 52 */ { 13000, 269 }, // Oh boy, it's got three spots to try! I thrust into the doll. Kinky, eh? I start
	// to increase my tempo... faster and faster I go! Suddenly there's a flatulent
	// noise and the doll becomes a popped balloon soaring around the room! It flies
	// out of the room and disappears!
	/* 53 */ { 13269, 327 }, // She hops out of the tub, steam rising from her skin. Her body is the best
	// looking I've ever seen! Then she comes up to me and gives the best time of my
	// life! Well, I guess that's it! As your puppet in this game I thank you for the
	// pleasure you have brought me! So long, I've got to get back to my new girl
	// here! Keep it up!
	/* 54 */ { 13596, 390 }, // She says "Lay down honey - let me give you a special surprise!" I lay down and
	// she says "Okay - now close your eyes". I close my eyes and she starts to go to
	// work on me. I'm really enjoying myself when suddenly she ties me to the bed!
	// Then she says "So long, turkey!" and runs out of the room! Well, the score is
	// now 2 out of a possible 3... but I'm also tied to the bed and can't move!
	/* 55 */ { 13986, 355 }, // Hmmmm. This is a Peeping Tom's paradise! Across the road is another hotel. Aha!
	// The curtains are open at one window! The bathroom door opens and a girl walks
	// out. Holy cow, her boobs are huge! And look at the sway as she strides across
	// the room! Now she's taking a large sausage-shaped object and looks at it
	// longinly! ... Damn, she shuts the curtain!
	/* 56 */ { 14341, 211 }, // This stuff is good! I'm breathing heavily - I've never been this horny! I've
	// just got to do something! Aha, there goes a female german shepard! That gives
	// me an idea!
	//
	// Oooh, kinky doggie! Chewed me to death!
	/* 57 */ { 14552, 282 }, // The blonde looks at the pills and says "Thanks! I love this stuff!" She takes a
	// pill. Shortly, her nipples start to stand up! Wow! She's breathing heavily. I
	// hope she takes me! But instead she says "So long! I'm going to see my
	// boyfriend!" and disappears down the stairs... Damn.
	/* 58 */ { 14834, 227 }, // The driver looks at me and says "Hey! What's that you got? Wine?" He grabs the
	// bottle and guzzles the wine down! Oh no! He's swerving towards a huge truck! I
	// grab the wheel! We struggle... and the truck just barely misses us!
	/* 59 */ { 15061, 1631 }, // +-------------------------------------------------------------------+
	// |                                            Linguists do it        |
	// |    Here I sit all broken hearted             cunningly            |
	// |      Tried to shit but only farted                                |
	// |                                                       DONT  DEAD  |
	// |                                    The password is    OPEN INSIDE |
	// |  Away with                           bellybutton                  |
	// |   thee foul      for a   time                        G            |
	// |    stench         weird                               O  B        |
	// |                      call 555-0439            ZIP         R       |
	// |   ?          ?                                 GO          O      |
	// |   xnjn vf n yvggyr            CALL APOGEE      CLICK        N     |
	// |     obggbz ovgpu  ?           SAY AARDWOLF                   C    |
	// |   ?                                                           O   |
	// |       mr sandman                                               S  |
	// |     man me a sand        real hackers          the rain           |
	// |                      do it with butterflies      in spain         |
	// |                                               falls mainly        |
	// |       __                                       on the spaniards   |
	// |      '  '   /___ do it                                            |
	// |     |    |  \              whos been drawing dicks?               |
	// |      '__'                                                         |
	// +-------------------------------------------------------------------+
	/* 60 */ { 16692, 0 }, //
	/* 61 */ { 16692, 0 }, //
	/* 62 */ { 16692, 0 }, //
	/* 63 */ { 16692, 438 }, // +---------------------------------------------------+
	// |          For those who desire the best:           |
	// |    Announcing, the most exclusive, the exciting,  |
	// |             the hottest spot in town,             |
	// |            ***************************            |
	// |            * SWINGING SINGLE'S DISCO *            |
	// |            ***************************            |
	// +---------------------------------------------------+
	/* 64 */ { 17130, 62 }, // Holy cow! Psychedelic! Pretty colors appear and I'm elsewhere!
	/* 65 */ { 17192, 342 }, // "Welcome to Purgatory," a Monty Hall-looking demon announces to me. "Here at
	// this crossroad you have three options. Before you are three doors. Each will
	// bring you to a different place."
	//  A - To Hell (where the game ends)
	//  B - Back to life, unharmed
	//  C - You stay here and must choose again
	// "The doors are randomly different each time!"
	/* 66 */ { 17534, 170 }, // Okay, why am I doing this? The preacher takes $1000 and winks! The girl grabs
	// $2000 and says "Meet me at the Honeymoon Suite! I've got connections to get a
	// room there!"
	/* 67 */ { 17704, 150 }, // Hi there! This is Kawa. I can't believe you bothered to check if I'd replaced
	// Chuck's reply. Find me on Twitter, @kawa_oneechan, and say 'love boat'!
	/* 68 */ { 17854, 75 }, // A voice answers and says "Wine for the nervous newlyweds! Coming right up!"
	/* 69 */ { 17929, 96 }, // Okay, here goes. ...Oh no, it's overflowing! It's filling the room with gross
	// sewage! EEEUUUGH!
	/* 70 */ { 18025, 956 }, // The object of SOFTPORN ADVENTURE is to find and seduce three different girls.
	// They have very different personalities, so tricks that work on one girl usually
	// won't work on another girl. The game has three different areas -- Disco, Casino
	// and Bar. You start off in the Bar with $1000. You'll need more money than that,
	// so you'll have to make more money during the game.
	//
	// You give your puppet commands like "Go north", "Buy beer" etc. A phrase like
	// "Look at the table" is equivalent to "Look table". Some commands can be abbre-
	// viated, like "N" for "Go north", "L" for "Look", "I" for "Inventory" etc. To
	// see how well you're doing, look up. To save and restore a game position,
	// just type "Save" and "Restore".
	//
	// If you're stuck, try looking at everything in sight, object by object. This
	// will usually help you find missing objects and/or clues etc. The "Help"
	// command will repeat this message.
	//
	// ========= ******** GOOD LUCK ! ******** ========

	// TEXTS

	/* 0 */ { 0, 25 }, // But I'm tied to the bed!?
    /* 1 */ { 25, 30 }, // "Don't go there; do me first!"
    /* 2 */ { 55, 24 }, // The door is locked shut.
    /* 3 */ { 79, 43 }, // The pimp says I can't go until I get $2000.
    /* 4 */ { 122, 57 }, // "Bruh, ain'tcha had enough for one night?" the pimp asks.
    /* 5 */ { 179, 43 }, // The pimp takes $2000 and gives me the okay.
    /* 6 */ { 222, 20 }, // Who are you kidding?
    /* 7 */ { 242, 22 }, // I'm not in the street.
    /* 8 */ { 264, 18 }, // Huh? Hail another!
    /* 9 */ { 282, 24 }, // We arrive and I get out.
    /* 10 */ { 306, 27 }, // I'm carrying the following:
    /* 11 */ { 333, 6 }, //
    //  > %s
    /* 12 */ { 340, 11 }, //  with $%d00
    /* 13 */ { 351, 26 }, // I'm not carrying anything!
    /* 14 */ { 377, 25 }, // You're not a bird, silly.
    /* 15 */ { 402, 8 }, // You hog.
    /* 16 */ { 410, 22 }, // I'm carrying too much!
    /* 17 */ { 432, 48 }, // The clerk shouts "Shoplifter!" and guns me down.
    /* 18 */ { 480, 6 }, // Taken.
    /* 19 */ { 486, 22 }, // I'm carrying too much!
    /* 20 */ { 508, 48 }, // The clerk shouts "Shoplifter!" and guns me down.
    /* 21 */ { 556, 37 }, // Get me a pitcher so I don't spill it.
    /* 22 */ { 593, 58 }, // The hooker calls out to you, "Don't take it, do me first!"
    /* 23 */ { 651, 25 }, // It's tied to the balcony.
    /* 24 */ { 676, 13 }, // %s: Dropped.
    //
    /* 25 */ { 690, 26 }, // I'm not carrying anything!
    /* 26 */ { 716, 29 }, // She smiles and eats a couple.
    /* 27 */ { 745, 48 }, // She blushes profusely and puts them in her hair.
    /* 28 */ { 793, 44 }, // She blushes and puts it safely in her purse.
    /* 29 */ { 837, 43 }, // She says, "Meet me at the marriage center!"
    /* 30 */ { 880, 61 }, // "That stuff made me puke," the bum mutters. "Git outta here."
    /* 31 */ { 941, 37 }, // The guy gives me a TV remote control.
    /* 32 */ { 978, 44 }, // That's too much! One item at a time, please.
    /* 33 */ { 1022, 19 }, // Its drawer is shut.
    /* 34 */ { 1041, 15 }, // Dead roaches...
    /* 35 */ { 1056, 40 }, // There's some pervert staring back at me.
    /* 36 */ { 1096, 46 }, // Doesn't seem to be cleaned in ages. It stinks!
    /* 37 */ { 1142, 46 }, // Looks like a whiskey-drinking kinda guy to me.
    /* 38 */ { 1188, 15 }, // It says 'push'.
    /* 39 */ { 1203, 37 }, // He's waiting for me to buy something.
    /* 40 */ { 1240, 67 }, // He has a pin on his coat. "Support your local pimp -- gimme $2000".
    /* 41 */ { 1307, 37 }, // I'd need the remote control for that.
    /* 42 */ { 1344, 31 }, // The pimp says I can't watch TV.
    /* 43 */ { 1375, 34 }, // Playing these might be more fun...
    /* 44 */ { 1409, 53 }, // "Tell ya my story for a bottle o' wine," he grumbles.
    /* 45 */ { 1462, 66 }, // All windows at the hotel across the road have their curtains shut.
    /* 46 */ { 1528, 17 }, // The door is open.
    /* 47 */ { 1545, 26 }, // The sign on the door says:
    /* 48 */ { 1571, 64 }, // "Entry by showing passcard - club members and their guests only"
    /* 49 */ { 1635, 16 }, // She ignores you.
    /* 50 */ { 1651, 33 }, // "For a good time, call 555-6969."
    /* 51 */ { 1684, 10 }, // It's open.
    /* 52 */ { 1694, 42 }, // Type "water on" or "water off" to operate.
    /* 53 */ { 1736, 21 }, // Its doors are closed.
    /* 54 */ { 1757, 28 }, // He's waiting for me to play.
    /* 55 */ { 1785, 10 }, // It's open.
    /* 56 */ { 1795, 12 }, // It's closed.
    /* 57 */ { 1807, 25 }, // These bushes look lushes.
    /* 58 */ { 1832, 16 }, // "Hail taxi here"
    /* 59 */ { 1848, 25 }, // She slaps me for staring.
    /* 60 */ { 1873, 20 }, // They look beautiful!
    /* 61 */ { 1893, 29 }, // The label on the bottle says:
    /* 62 */ { 1922, 50 }, // "Want to drive someone crazy with lust? Try this!"
    /* 63 */ { 1972, 43 }, // Hello? There's a group of bushes behind it!
    /* 64 */ { 2015, 22 }, // Maybe I should listen?
    /* 65 */ { 2037, 34 }, // It's %s, %s-flavored, %s, and %s.
    //
    /* 66 */ { 2072, 19 }, // It contains $%d00.
    //
    /* 67 */ { 2092, 11 }, // It's empty.
    /* 68 */ { 2103, 14 }, // It's inflated.
    /* 69 */ { 2117, 36 }, // It's all rolled up in a little ball.
    /* 70 */ { 2153, 19 }, // It's full of water.
    /* 71 */ { 2172, 11 }, // It's empty.
    /* 72 */ { 2183, 26 }, // It's... something alright.
    /* 73 */ { 2209, 24 }, // Lookin' goo-- *SLAP* Ow.
    /* 74 */ { 2233, 22 }, // It's on the east wall.
    /* 75 */ { 2255, 15 }, // It won't budge.
    /* 76 */ { 2270, 18 }, // It's already open.
    /* 77 */ { 2288, 53 }, // A voice asks, "Passcard?". I search my pockets and...
    /* 78 */ { 2341, 26 }, // I have it! The door opens.
    /* 79 */ { 2367, 16 }, // I don't have it!
    /* 80 */ { 2383, 35 }, // It seems to be remotely controlled.
    /* 81 */ { 2418, 43 }, // Push the button to open the elevator doors.
    /* 82 */ { 2461, 17 }, // I can't reach it.
    /* 83 */ { 2478, 30 }, // It's already inflated, dimwit.
    /* 84 */ { 2508, 36 }, // I can't if I'm not holding it close.
    /* 85 */ { 2544, 36 }, // But the prime rate is already 257%%!
    /* 86 */ { 2580, 26 }, // Okay, show me your slot...
    /* 87 */ { 2606, 24 }, // Playful li'l bugger, eh?
    /* 88 */ { 2630, 59 }, // A voice from behind the curtain says "What's the password?"
    /* 89 */ { 2689, 6 }, // BELLYB
    /* 90 */ { 2695, 23 }, // The curtain pulls back!
    /* 91 */ { 2718, 6 }, // AL SEN
    /* 92 */ { 2724, 6 }, // KEN SE
    /* 93 */ { 2730, 35 }, // "Cute, bruh. Real cute, but wrong."
    /* 94 */ { 2765, 14 }, // "Wrong, bruh!"
    /* 95 */ { 2779, 43 }, // The blonde calls out, "You can't go there!"
    /* 96 */ { 2822, 16 }, // Pushy chump, eh?
    /* 97 */ { 2838, 18 }, // The door's closed.
    /* 98 */ { 2856, 38 }, // Push the button to enter the elevator.
    /* 99 */ { 2894, 9 }, // Eccch no!
    /* 100 */ { 2903, 20 }, // I'm not that hungry.
    /* 101 */ { 2923, 13 }, // Tastes awful.
    /* 102 */ { 2936, 57 }, // This stuff's rot-gut! I'd rather give it to someone else.
    /* 103 */ { 2993, 31 }, // Heheheheeey, this stuff's okay!
    /* 104 */ { 3024, 14 }, // Sour grapes...
    /* 105 */ { 3038, 7 }, // Thanks!
    /* 106 */ { 3045, 31 }, // ...Huh? Get your head examined.
    /* 107 */ { 3076, 17 }, // "Sorry, all out."
    /* 108 */ { 3093, 54 }, // I give the bartender $100 and he places it on the bar.
    /* 109 */ { 3147, 17 }, // "Sorry, all out."
    /* 110 */ { 3164, 56 }, // The waitress takes $100 and says she'll be back shortly.
    /* 111 */ { 3220, 30 }, // ...gee, that's taking a while.
    /* 112 */ { 3250, 47 }, // The clerk takes $100 and gives me the magazine.
    /* 113 */ { 3297, 18 }, // "Sorry, sold out."
    /* 114 */ { 3315, 34 }, // I already paid the pimp, numbnuts.
    /* 115 */ { 3349, 24 }, // After all that trickery?
    /* 116 */ { 3373, 27 }, // Money can't buy everything.
    /* 117 */ { 3400, 27 }, // It's not on the floor here.
    /* 118 */ { 3427, 26 }, // Gotta find a working sink.
    /* 119 */ { 3453, 29 }, // Water is running in the sink.
    /* 120 */ { 3482, 16 }, // I have no water.
    /* 121 */ { 3498, 39 }, // Somehow, a tree sprouts within seconds!
    /* 122 */ { 3537, 35 }, // The seeds need better soil to grow.
    /* 123 */ { 3572, 25 }, // It pours into the ground.
    /* 124 */ { 3597, 16 }, // I don't have it.
    /* 125 */ { 3613, 26 }, // Gotta find a working sink.
    /* 126 */ { 3639, 24 }, // The water isn't running.
    /* 127 */ { 3663, 28 }, // The pitcher is already full.
    /* 128 */ { 3691, 31 }, // I have nothing to pour it with.
    /* 129 */ { 3722, 21 }, // The pitcher is empty.
    /* 130 */ { 3743, 25 }, // It pours into the ground.
    /* 131 */ { 3768, 39 }, // Somehow, a tree sprouts within seconds!
    /* 132 */ { 3807, 16 }, // Eugh, punk rock.
    /* 133 */ { 3823, 64 }, // An advertisement says "call 555-0987 for all your liquor needs!"
    /* 134 */ { 3887, 47 }, // I should take it so I can hold it up to my ear.
    /* 135 */ { 3934, 25 }, // Quiet as a mouse in heat.
    /* 136 */ { 3959, 17 }, // I can't reach it.
    /* 137 */ { 3976, 15 }, // No way, weirdo.
    /* 138 */ { 3991, 21 }, // There's no girl here.
    /* 139 */ { 4012, 111 }, // The girl says, "But you'll need $2000 for the honeymoon suite!"
    // The preacher chimes in, "I'll need $1000, too."
    /* 140 */ { 4124, 46 }, // The preacher says, "I'll need $1000 for that."
    /* 141 */ { 4170, 44 }, // She doesn't look like she can take any more.
    /* 142 */ { 4214, 43 }, // Oh no! She gave me the dreaded Atomic Clap!
    /* 143 */ { 4257, 24 }, // Inflate it first, dummy.
    /* 144 */ { 4281, 36 }, // I can't unless I'm holding it close.
    /* 145 */ { 4317, 40 }, // "I'm nervous. Don't you have some wine?"
    /* 146 */ { 4357, 42 }, // He jumps over the bar and strikes me down!
    /* 147 */ { 4399, 48 }, // (Now that's putting the 'ender' in 'bartender'!)
    /* 148 */ { 4447, 54 }, // "Buddy, don't make me put the 'ender' in 'bartender'."
    /* 149 */ { 4501, 20 }, // ... he got me there.
    /* 150 */ { 4521, 39 }, // The whole point was to stop doing that!
    /* 151 */ { 4560, 36 }, // She kicks me in the nads for trying.
    /* 152 */ { 4596, 19 }, // "Wisen up, Buster!"
    /* 153 */ { 4615, 24 }, // "Fuck off, I'm working!"
    /* 154 */ { 4639, 66 }, // "Bruh, even if I'd let ya you couldn't afford me, foo'!" he mocks.
    /* 155 */ { 4705, 40 }, // Let's not. I don't know where he's been!
    /* 156 */ { 4745, 24 }, // "Don't make this weird."
    /* 157 */ { 4769, 40 }, // Yeah? How 'bout *you* fuck off, asshole?
    /* 158 */ { 4809, 8 }, // Pervert.
    /* 159 */ { 4817, 11 }, // It tickles!
    /* 160 */ { 4828, 37 }, // ~I got the constipation bluuueees...~
    /* 161 */ { 4865, 16 }, // Ah, much better.
    /* 162 */ { 4881, 12 }, // Ah, sleep...
    /* 163 */ { 4893, 51 }, // No wait! I can't sleep yet, I gotta find me a girl!
    /* 164 */ { 4944, 45 }, // Okay, the safety rope is tied to the balcony.
    /* 165 */ { 4989, 39 }, // I flash my passcard and the door opens.
    /* 166 */ { 5028, 32 }, // Let me see if I still have it...
    /* 167 */ { 5060, 33 }, // Yeah, I do. And it works! Thanks!
    /* 168 */ { 5093, 28 }, // Samurai sex fiend, YEEEAAAH!
    /* 169 */ { 5121, 33 }, // ...I accidentally stabbed myself.
    /* 170 */ { 5154, 46 }, // A girl says, "Hi honey! This is %s. Dear, why
    //
    /* 171 */ { 5201, 43 }, // don't you forget this game and %s with me?
    //
    /* 172 */ { 5245, 44 }, // After all, your %s has always turned me on.
    //
    /* 173 */ { 5290, 41 }, // So bring a %s and come play with my %s."
    //
    /* 174 */ { 5332, 13 }, // She hangs up.
    /* 175 */ { 5345, 24 }, // It's not ringing though?
    /* 176 */ { 5369, 31 }, // This only takes incoming calls.
    /* 177 */ { 5400, 34 }, // I mean, I could talk to my hand...
    /* 178 */ { 5434, 84 }, // A voice on the line says "Hello. Please answer the questions with
    // one-word answers."
    /* 179 */ { 5519, 37 }, // "What's your favorite girls name?"  >
    /* 180 */ { 5556, 37 }, // "Name a nice part of her anatomy."  >
    /* 181 */ { 5593, 37 }, // "What do you like to do with her?"  >
    /* 182 */ { 5630, 37 }, // "And the best part of your body?"   >
    /* 183 */ { 5667, 37 }, // "Finally, your favorite object?"    >
    /* 184 */ { 5704, 12 }, // He hangs up.
    /* 185 */ { 5716, 15 }, // Nobody answers.
    /* 186 */ { 5731, 47 }, // Using the hammer, I smash the window to pieces.
    /* 187 */ { 5778, 42 }, // If I had something to break it with, sure!
    /* 188 */ { 5820, 40 }, // Let me see if I still have that knife...
    /* 189 */ { 5860, 33 }, // Yeah, I do. And it works! Thanks!
    /* 190 */ { 5893, 28 }, // Samurai sex fiend, YEEEAAAH!
    /* 191 */ { 5921, 33 }, // ...I accidentally stabbed myself.
    /* 192 */ { 5954, 7 }, // Woohoo!
    /* 193 */ { 5961, 17 }, // Yeah, yeah, yeah~
    /* 194 */ { 5978, 29 }, //
    // Yeah, I got the moves, dude!
    /* 195 */ { 6008, 18 }, // Try using a knife.
    /* 196 */ { 6026, 25 }, // What, with my bare hands?
    /* 197 */ { 6051, 30 }, // "I don't need yo money, bruh."
    /* 198 */ { 6081, 36 }, // He'll take the money when you go up.
    /* 199 */ { 6117, 35 }, // You already paid the pimp, dum-dum.
    /* 200 */ { 6152, 24 }, // After all that trickery?
    /* 201 */ { 6176, 46 }, // "What do you take me for, some common skank!?"
    /* 202 */ { 6222, 42 }, // She punches me right in the vulnerables...
    /* 203 */ { 6264, 54 }, // Bring a girl here to marry, he'll take the money then.
    /* 204 */ { 6318, 40 }, // He's too drunk to do business right now.
    /* 205 */ { 6358, 41 }, // Buy something, he'll take the money then.
    /* 206 */ { 6399, 44 }, // Why not play 21 instead? You'll lose anyway!
    /* 207 */ { 6443, 37 }, // A cop comes running and guns me down!
    /* 208 */ { 6480, 39 }, // I flash my passcard and the door opens.
    /* 209 */ { 6519, 10 }, // Hmm. Nice!
    /* 210 */ { 6529, 29 }, // Okay, who's eating tuna fish?
    /* 211 */ { 6558, 21 }, // Augh, I'm gonna puke!
    /* 212 */ { 6579, 34 }, // ...Achoo! Hmm. Guess I'm allergic?
    /* 213 */ { 6613, 9 }, // Yeeeccch!
    /* 214 */ { 6622, 20 }, // Smells like perfume.
    /* 215 */ { 6642, 12 }, // Smells okay.
    /* 216 */ { 6654, 34 }, // Don't do that, it gets me excited!
    /* 217 */ { 6688, 43 }, // Can't you see the bar on top of the screen?
    /* 218 */ { 6731, 25 }, // Save to which slot? (0-9)
    /* 219 */ { 6756, 11 }, // SOFTP-#.SAV
    /* 220 */ { 6767, 18 }, // Couldn't open %s.
    //
    /* 221 */ { 6786, 18 }, // Saved game to %s.
    //
    /* 222 */ { 6805, 30 }, // Restore from which slot? (0-9)
    /* 223 */ { 6835, 11 }, // SOFTP-#.SAV
    /* 224 */ { 6846, 18 }, // Couldn't open %s.
    //
    /* 225 */ { 6865, 21 }, // Loaded game from %s.
    //
};

char *loadMessageFromFile(uint16_t pos, uint16_t len) {
	if (m_file == NULL) {
		m_file = fopen("MESSAGES.BIN");
	}
    fread(m_file, m_buffer, pos, len);
    m_buffer[len] = 0;
    return m_buffer;
}

char *loadMessage(int idx) {
    return loadMessageFromFile(messages[idx].pos, messages[idx].len);
}

char *loadText(int idx) {
	return loadMessageFromFile(
		messages[idx + 71].pos + 18025 + 956, 
		messages[idx + 71].len
	);
}

void printText(int idx) {
	puts(loadText(idx));
}