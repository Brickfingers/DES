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

void sendDisplay(Output* output, int coid, int value){
	display_t display;
	display.output = *output;
	if (MsgSend(coid, &display, size(display), NULL, 0)){
		fprintf(stderr, "MsgSend had an error\n");
		perror(NULL);
	}
}

int idle(int coid, Person* person, Input* input) {
	if (strcmp(person->input, inMessage[0]) && strcmp(person->input, inMessage[1])) {
		input =
	}
}

int left_scan(int input, int coid, Display msg_send, Person person){
	if (MsgSend(coid, &msg_send, sizeof(msg_send), NULL, 0) == -1){
		fprintf(stderr, "MsgSend had an error\n");
		perror(EXIT_FAILURE);
	}
	if(state_table[person.state][input] != IS) {
		return state_table[person.state][input];
	}
	return -1;
}

int right_scan(int input, int coid, Display msg_send, Person person) {
	if (MsgSend(coid, &msg_send, sizeof(msg_send), NULL, 0) == -1){
		fprintf(stderr, "MsgSend had an error\n");
		perror(EXIT_FAILURE);
	}
	if(state_table[person.state][input] != IS) {
		return state_table[person.state][input];
	}
	return -1;
}

int guard_left_unlock(int input, int coid, Display msg_send, Person person){
	if (MsgSend(coid, &msg_send, sizeof(msg_send), NULL, 0) == -1){
		fprintf(stderr, "MsgSend had an error\n");
		perror(EXIT_FAILURE);
	}
	if(state_table[person.state][input] != IS) {
		return state_table[person.state][input];
	}
	return -1;
}

int guard_right_unlock(int input, int coid, Display msg_send, Person person) {
	if (MsgSend(coid, &msg_send, sizeof(msg_send), NULL, 0) == -1){
		fprintf(stderr, "MsgSend had an error\n");
		perror(EXIT_FAILURE);
	}
	if(state_table[person.state][input] != IS) {
		return state_table[person.state][input];
	}
	return -1;
}


int left_open(int input, int coid, Display msg_send, Person person){
	if (MsgSend(coid, &msg_send, sizeof(msg_send), NULL, 0) == -1){
		fprintf(stderr, "MsgSend had an error\n");
		perror(EXIT_FAILURE);
	}
	if(state_table[person.state][input] != IS) {
		return state_table[person.state][input];
	}
	return -1;
}
