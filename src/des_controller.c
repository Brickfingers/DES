/**
 * Nick Wright & Kevin Luu
 * 040911884 & 040905303
 * CST8244
 */

#include "des.h"

int main(int argc, char* argv[]) {

	//des_display = server
	Display msg_send;
	Display msg_receive;
	pid_t serverpid;
	int coid;

	//des_inputs = client
	Person person_message;
	Output output;
	int rcvid;
	int chid;

	int* (*handler)(int coid, Person* person, Output* output) = {
			idle_handler,
			left_scan_handler,
			right_scan_handler,
			guard_left_unlock_handler,
			guard_right_unlock_handler,
			open_left_handler,
			open_right_handler,
			weight_handler,
			left_close_handler,
			right_close_handler,
			guard_left_lock_handler,
			guard_right_lock_handler,
			exit_handler
	};

	//Phase I

	//Get controller's PID from command-line arguments.
	serverpid = atoi(argv[1]);
	//On Failure: print usage message and EXIT_FAILURE
	if (argc != 2) {
		//print usage message
		fprintf(stderr,
				"Usage: ./des_inputs <pid of des_display> <enter>\n");
		exit(EXIT_FAILURE);
	}

	chid = ChannelCreate(0);
	if (chid == -1) {
		fprintf(stderr, "ChannelCreate failure.\n");
		exit(EXIT_FAILURE);
	}

	coid = ConnectAttach(ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		fprintf(stderr, "ConnectAttach had an error\n");
		exit(EXIT_FAILURE);
	}

	printf("controller PID is %d\tAnd the Channel ID (chid) is: %d\n", getpid(),
			chid);

	//PHASE II receive message & process message
	//SERVER WAIT FOR MESSAGE -> pass on to client

	while (1) {

		//TODO - get input event from Person object and advance state machine to next accepting state (or error state)

		if (person_message.state == IDLE_STATE){
			printf("Waiting for Person...\n");
		}

		//get message
		rcvid = MsgReceive(chid, &person_message, sizeof(person_message), NULL);
		if (rcvid == -1) {
			fprintf(stderr, "MsgReceive error.\n");
			exit(EXIT_FAILURE);
		}


		if (!strcmp(&person_message.input, inMessage[EX])){
			handler = &exit_handler;
		}

		handler = (*handler)(coid, &person_message, &output);
		MsgReply(rcvid, EOK, NULL, 0);
		//Reference - your CST8152 - Compiler notes (***)
		// use function pointers
		//TODO - complete rest of Phase II for controller

	}

	//PHASE III
	ChannelDestroy(chid);
	ConnectDetach(coid);
	exit(EXIT_SUCCESS);
}

void sendDisplay(Output* output, int coid, Person person){
	Display display;
	display.output = *output;
	display.person = person;
	if (MsgSend(coid, &display, sizeof(display), NULL, 0)){
		fprintf(stderr, "MsgSend had an error\n");
		perror(NULL);
	}
}

int* idle_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[LS])) {
		person->direction = IN;
		person->state = LEFT_SCAN_STATE;
		*output = ID_SCANNED;
		sendDisplay(output, coid, *person);
		return left_scan_handler;
	} else if (!strcmp(person->input, inMessage[RS])) {
		person->direction = OUT;
		person->state = RIGHT_SCAN_STATE;
		*output = ID_SCANNED;
		sendDisplay(output, coid, *person);
		return right_scan_handler;
	} else {
		//we could pass a error message for a wrong event
		return idle_handler;
	}
}

int* left_scan_handler(int coid, Person* person, Output* output){
	if (!strcmp(person->input, inMessage[GLU])) {
		*output = GUARD_LEFT_UNLOCK;
		person->state = GUARD_LEFT_UNLOCK_STATE;
		sendDisplay(output, coid, *person);
		return guard_left_unlock_handler;
	} else {
		//error
		return left_scan_handler;
	}
}

int* right_scan_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person->input, inMessage[GRU])) {
		*output = GUARD_RIGHT_UNLOCK;
		person->state = GUARD_RIGHT_UNLOCK_STATE;
		sendDisplay(output, coid, *person);
		return guard_right_unlock_handler;
	} else {
		return right_scan_handler;
	}
}

int* guard_left_unlock_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[LO])) {
		*output = LEFT_OPEN;
		person->state = LEFT_OPEN_STATE;
		sendDisplay(output, coid, *person);
		return open_left_handler;
	} else {
		//error
		return guard_left_unlock_handler;


int* guard_right_unlock_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[RO])) {
		*output = RIGHT_OPEN;
		person->state = RIGHT_OPEN_STATE;
		sendDisplay(output, coid, *person);
		return open_right_handler;
	} else {
		//error
		return guard_right_unlock_handler;
	}
}


int* open_left_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person->input, inMessage[WS])){
		*output = WEIGHED;
		person->state = WEIGHT_SCALE_STATE;
		sendDisplay(output, coid, *person);
		return weight_handler;
	} else if (!strcmp(person-> input, inMessage[LC])) {
		*output = LEFT_CLOSE;
		person->state = LEFT_CLOSED_STATE;
		sendDisplay(output, coid, *person);
		return left_close_handler;
	} else {
		return open_left_handler;
	}
}

int* open_right_handler(int coid, Person* person, Output* output){
	if(!strcmp(person->input, inMessage[WS])){
		*output = WEIGHED;
		person->state = WEIGHT_SCALE_STATE;
		sendDisplay(output, coid, *person);
		return weight_handler;
	} else if (!strcmp(person-> input, inMessage[RC])) {
		*output = RIGHT_CLOSE;
		person->state = RIGHT_CLOSED_STATE;
		sendDisplay(output, coid, *person);
		return right_close_handler;
	} else {
		return open_right_handler;
	}
}

int* weight_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[WS]) && person->direction == IN) {
		*output = WEIGHED;
		person->state = GUARD_RIGHT_UNLOCK_STATE;
		sendDisplay(output, coid, *person);
		return left_close_handler;
	} else if (!strcmp(person->input, inMessage[WS]) && person->direction == OUT){
		*output = WEIGHED;
		person->state = GUARD_LEFT_UNLOCK_STATE;
		sendDsiplay(output, coid, *person);
		return right_close_handler;
	} else{
		//error
		return weight_handler;
	}
}

int* left_close_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person->input, inMessage[GLL])){
		*output = GUARD_LEFT_LOCK;
		person->state = GUARD_LEFT_LOCK_STATE;
		sendDisplay(output, coid, *person);
		return guard_left_lock_handler;
	} else {
		return left_close_handler;
	}
}

int* right_close_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person->input, inMessage[GRL])){
		*output = GUARD_RIGHT_LOCK;
		person-> state = GUARD_RIGHT_LOCK_STATE;
		sendDisplay(output, coid, *person);
		return guard_right_lock_handler;
	} else {
		return left_close_handler;
	}
}

int* guard_left_lock_handler(int coid, Person* person, Output* output) {
    if(person->direction == OUT) {
        person->state = IDLE_STATE;
    	return idle_handler;
    } else if(!strcmp(person-> input, inMessage[GRU])) {
        *output = GUARD_RIGHT_UNLOCK;
        person->state = GUARD_RIGHT_UNLOCK_STATE;
        sendDisplay(output, coid, *person);
        return guard_right_unlock_handler;
    } else {
        return guard_left_lock_handler;
    }
}

int* guard_right_lock_handler(int coid, Person* person, Output* output) {
    if(person->direction == IN) {
        person->state = IDLE_STATE;
    	return idle_handler;
    } else if(!strcmp(person-> input, inMessage[GLU])) {
        *output = GUARD_LEFT_UNLOCK;
        person->state = GUARD_LEFT_UNLOCK_STATE;
        sendDisplay(output, coid, *person);
        return guard_left_unlock_handler;
    } else if (!strcmp(person -> input, inMessage[EX])){
        *output = EXIT;
        sendDisplay(output, coid, *person);
        return exit_handler;
    } else {
        return guard_right_lock_handler;
    }
}

int* exit_handler(int coid, Person* person, Output* output) {
		*output = EXIT;
		sendDisplay(output, coid, *person);
		exit(EXIT_SUCCESS);
}

