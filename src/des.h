/**
 * Nick Wright & Kevin Luu
 * 040911884 & 040905303
 * CST8244
 */

#ifndef DOOR_ENTRY_SYSTEM_H_
#define DOOR_ENTRY_SYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define NUM_STATES 13	// where n = number of states in your finite state machine (FSM)
#define IS -1 //invalid state
#define NUM_INPUTS 12	// where n equals the number of input commands that drive the FSM.
//From the assignment specification, notice the prompt.
//Each command, ls, rs, etc.

typedef enum {
	//assign an enum value, one for each state
	IDLE = 0,
    LEFT_SCAN = 1,
    RIGHT_SCAN = 2,
    GUARD_LEFT_UNLOCK = 3,
    GUARD_RIGHT_UNLOCK = 4,
    LEFT_OPEN = 5,
    RIGHT_OPEN = 6,
    WEIGHT_SCALE = 7,
    LEFT_CLOSED = 8,
    RIGHT_CLOSED = 9,
    GUARD_LEFT_LOCK = 10,
    GUARD_RIGHT_LOCK = 11,
	EXIT = 12
} State;

int state_table[][NUM_STATES] = {
    /*                           ls,  rs, glu, gru,  lo,  ro,  ws,  lc,  rc, gll, grl, exit*/
    /* 0 IDLE               */ {  1,   2,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  12},
    /* 1 LEFT_SCAN          */ { IS,  IS,   3,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  12},
    /* 2 RIGHT_SCAN         */ { IS,  IS,  IS,   4,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  12},
    /* 3 GUARD_LEFT_UNLOCK  */ { IS,  IS,  IS,  IS,   5,  IS,  IS,  IS,  IS,  IS,  IS,  12},
    /* 4 GUARD_RIGHT_UNLOCK */ { IS,  IS,  IS,  IS,  IS,   6,  IS,  IS,  IS,  IS,  IS,  12},
    /* 5 LEFT_OPEN          */ { IS,  IS,  IS,  IS,  IS,  IS,   7,   8,  IS,  IS,  IS,  12},
    /* 6 RIGHT_OPEN         */ { IS,  IS,  IS,  IS,  IS,  IS,   7,  IS,   9,  IS,  IS,  12},
    /* 7 WEIGHT_SCALE       */ { IS,  IS,  IS,  IS,  IS,  IS,  IS,   8,   9,  IS,  IS,  12},
    /* 8 LEFT_CLOSED        */ { IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,   0,  IS,  12},
    /* 9 RIGHT_CLOSED       */ { IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,   0,  12},
    /* 10 GUARD_LEFT_LOCK   */ { IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS},
    /* 11 GUARD_RIGHT_LOCK  */ { IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS},
    /* 12 EXIT              */ { IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS,  IS} };

typedef enum {
	//assign an enum value, one for each input command
	LEFT_SCAN = 0,
	RIGHT_SCAN = 1,
	GUARD_LEFT_UNLOCK = 2,
	GUARD_RIGHT_UNLOCK = 3,
	LEFT_OPEN = 4,
	RIGHT_OPEN = 5,
	WEIGHED = 6,
	LEFT_CLOSE = 7,
	RIGHT_CLOSE = 8,
	GUARD_LEFT_LOCK = 9,
	GUARD_RIGHT_LOCK = 10,
	EXIT = 11
} Input;

const char *inMessage[NUM_INPUTS] = {
	//each input command. For example, "ls"
	"ls",
	"rs",
	"glu",
	"gru",
	"lo",
	"ro",
	"ws",
	"lc",
	"rc",
	"gll",
	"grl",
	"exit"
};

#define NUM_OUTPUTS 11	// where n equals the number of output messages from the FSM.
typedef enum {
	//assign an enum value, one for each output message from the FSM
	ID_SCAN = 0,
	GUARD_LEFT_UNLOCK = 1,
	GUARD_RIGHT_UNLOCK = 2,
	LEFT_OPEN = 3,
	RIGHT_OPEN = 4,
	WEIGHED = 5,
	LEFT_CLOSE = 6,
	RIGHT_CLOSE = 7,
	GUARD_LEFT_LOCK = 8,
	GUARD_RIGHT_LOCK = 9,
	EXIT = 10
} Output;

const char *outMessage[NUM_OUTPUTS] = {
	//each output message. For example, "Person opened left door"
    "Person scanned ID, ID: =",
    "Guard left door unlocked.",
    "Guard right door unlocked.",
    "Person opened left door",
    "Person opened right door.",
    "Person weighed, weight =",
    "Left door closed (Automatically)",
    "Right door closed (Automatically)",
    "Guard left door locked.",
    "Guard right door locked.",
    "Exiting"
};

// inputs client sends a Person struct to its server, the controller
typedef struct {
	//TODO :: fields for person's ID (int), weight (int),
	//direction (inbound or outbound), and some way to remember
	//what state the Person is in. Suppose the Person in
	//"Left Scan" state. You need a way to represent that.
	int id;
	char input[64];
	int weight;
	int direction;
	State state;
} Person;

// controller client sends a Display struct to its server, the display
typedef struct {
	//fields for the output message and the Person.
	//For the output message, I used an int, which is the index
	//into the outMessages array. That way I don't pass strings
	//in my messages. The Person field is needed, as some output
	//message require information from the Person. Specifically,
	//those messages that display the Person's ID and weight.
	int output;
	Person person;
} Display;

#endif /* DOOR_ENTRY_SYSTEM_H_ */
