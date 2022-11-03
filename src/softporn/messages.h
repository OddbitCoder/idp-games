#pragma once

struct message 
{
	unsigned int pos;
	unsigned int len;
};

char *loadText(struct message *msg) 
{
	return "whatever"; // WARNME
}

const struct message messages[] =
{
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
};
