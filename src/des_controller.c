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
		printf("56");
		if (rcvid == -1) {
			fprintf(stderr, "MsgReceive error.\n");
			exit(EXIT_FAILURE);
		}
		MsgReply(rcvid, EOK, NULL, 0);

		int index = -1;
		for (int i = 0; i < NUM_INPUTS; i++){
			if (strcmp(&person_message, inMessage[i]) == 0) {
				index = i;
			}
		}
		printf("69");
		int next;
		//TODO - get input event from Person object and advance state machine to next accepting state (or error state)

		next = state_table[person_message.state][index];

		person_message.state = next;

		if (MsgSend(coid, &msg_send, sizeof(msg_send), NULL, 0) == -1){
			fprintf(stderr, "MsgSend had an error\n");
			perror(EXIT_FAILURE);
		}
		printf("81");
		//Reference - your CST8152 - Compiler notes (***)
		// use function pointers
		//TODO - complete rest of Phase II for controller

	}

	//PHASE III
	ChannelDestroy(chid);
	ConnectDetach(coid);
	exit(EXIT_SUCCESS);
}

void sendDisplay(Output* output, int coid, Person* person){
	Display display;
	display.output = *output;
	display.person = *person;
	if (MsgSend(coid, &display, size(display), NULL, 0)){
		fprintf(stderr, "MsgSend had an error\n");
		perror(NULL);
	}
}

int* idle_handler(int coid, Person* person, Output* output) {
	if (!strcmp(person->input, inMessage[LS])) {
		person->direction = IN;
		*output = ID_SCANNED;
		sendDisplay(output, coid, person);
		return left_scan_handler;
	} else if (!strcmp(person->input, inMessage[RS])) {
		person->direction = OUT;
		*output = ID_SCANNED;
		sendDisplay(ouput, coid, person);
		return right_scan_handler;
	} else {
		//we could pass a error message for a wrong event
		return idle_handler;
	}
}

int* left_scan_handler(int coid, Person* person, Output* output){
	if (!strcmp(person->input, inMessage[GLU])) {
		*output = GUARD_LEFT_UNLOCK;
	}
}

int* right_scan_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person->input, inMessage[GRU])) {
		*output = GUARD_RIGHT_UNLOCK;
		sendDisplay(output, coid, person);
		return guard_right_unlock
	} else {
		return right_scan_handler;
	}
}

int* guard_left_unlock_handler(int coid, Person* person, Output* output) {

}

int* guard_right_unlock_handler(int coid, Person* person, Output* output) {

}


int* open_left_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person->input, inMessage[WS])){
		*output = WEIGHED;
		sendDisplay(output, coid, person);
		return weight_handler;
	} else if (!strcmp(person-> input, inMessage[LC])) {
		*output = LEFT_CLOSE;
		sendDisplay(output, coid, person);
		return left_close_handler;
	} else {
		return open_left_handler;
	}
}

int* open_right_handler(int coid, Person* person, Output* output){
	if(!strcmp(person->input, inMessage[WS])){
		*output = WEIGHED;
		sendDisplay(output, coid, person);
		return weight_handler;
	} else if (!strcmp(person-> input, inMessage[RC])) {
		*output = RIGHT_CLOSE;
		sendDisplay(output, coid, person);
		return right_close_handler;
	} else {
		return open_right_handler;
	}
}

int* weight_handler(int coid, Person* person, Output* output) {

}

int* left_close_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person->input, inMessage[GLL])){
		*output = GUARD_LEFT_LOCK;
		sendDisplay(output, coid, person);
		return guard_left_lock_handler;
	} else {
		return left_close_handler;
	}

}

int* right_close_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person->input, inMessage[GRL])){
		*output = GUARD_RIGHT_LOCK;
		sendDisplay(output, coid, person);
		return guard_right_lock_handler;
	} else {
		return left_close_handler;
	}
}

int* guard_left_lock_handler(int coid, Person* person, Output* output) {
	if(!strcmp(person-> input, inMessage[EX])) {
		*output = EXIT;
		sendDisplay(output, coid, person);
	} else {
		return guard_left_lock_handler;
	}
}

int* guard_right_unlock_handler(int coid, Person* person, Output* output) {

}

int* exit_handler(int coid, Person* person, Output* output) {

}
