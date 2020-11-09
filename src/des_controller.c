/**
 * Nick Wright & Kevin Luu
 * 040911884 & 040905303
 * CST8244
 */

#include "des.h"

int main(void) {

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

	while (TRUE) {

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
