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
	int rcvid;
	int chid;

	StateFunc statefunc = idle_handler;

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

		//get message
		rcvid = MsgReceive(chid, &person_message, sizeof(person_message), NULL);

		if (rcvid == -1) {
			fprintf(stderr, "MsgReceive error.\n");
			exit(EXIT_FAILURE);
		}
		MsgReply(rcvid, EOK, NULL, 0);

		//TODO - get input event from Person object and advance state machine to next accepting state (or error state)


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

void* idle_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[LS])) {
		person->direction = IN;
		*output = ID_SCANNED;
		sendDisplay(output, coid, *person);
		return left_scan_handler;
	} else if (!strcmp(person->input, inMessage[RS])) {
		person->direction = OUT;
		*output = ID_SCANNED;
		sendDisplay(output, coid, *person);
		return right_scan_handler;
	} else {
		//we could pass a error message for a wrong event
		return idle_handler;
	}
}

void* left_scan_handler(int coid, Person* person, Output* output){
	if (!strcmp(person->input, inMessage[GLU])) {
		*output = GUARD_LEFT_UNLOCK;
		sendDisplay(output, coid, *person);
		return guard_left_unlock_handler;
	} else {
		//error
		return left_scan_handler;
	}
}

void* right_scan_handler(int coid, Person* person, Output* output) {

}

void* guard_left_unlock_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[LO])) {
		*output = LEFT_OPEN;
		sendDisplay(output, coid, *person);
		return open_left_handler;
	} else {
		//error
		return guard_left_unlock_handler;
	}
}

void* guard_right_unlock_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[RO])) {
		*output = RIGHT_OPEN;
		sendDisplay(output, coid, *person);
		return open_right_handler;
	} else {
		//error
		return guard_right_unlock_handler;
	}
}


void* open_left_handler(int coid, Person* person, Output* output) {

}

void* open_right_handler(int coid, Person* person, Output* output){

}

void* weight_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[WS]) && person.direction == IN) {
		*output = WEIGHED;
		sendDisplay(output, coid, *person);
		return left_close_handler;
	} else if (!strcmp(person->input, inMessage[WS]) && person.direction == OUT){
		*output = WEIGHED;
		sendDsiplay(output, coid, &person);
		return right_close_handler;
	} else{
		//error
		return weight_handler;
	}
}

void* left_close_handler(int coid, Person* person, Output* output) {

}

void* right_close_handler(int coid, Person* person, Output* output) {

}

void* guard_left_lock_handler(int coid, Person* person, Output* output) {

}

void* guard_right_lock_handler(int coid, Person* person, Output* output) {

}

void* exit_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[EX])) {
		*output = EXIT;
		sendDisplay(output, coid, &person);
		exit(EXIT_SUCCESS);
	}
}
