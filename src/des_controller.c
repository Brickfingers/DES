/**
 * Nick Wright
 * 040911884
 * CST8244
 */

#include "des.h"

int main(void) {

	client_send_t msg_send;
	server_response_t msg_receive;
	pid_t serverpid;
	int coid;

	client_send_t client_message;
	server_response_t response;
	int rcvid;
	int chid;

	chid = ChannelCreate(0);
	if (chid == -1) {
		fprintf(stderr, "ChannelCreate failure.\n");
		exit(EXIT_FAILURE);
	}

	printf("controller PID is %d\tAnd the Channel ID (chid) is: %d\n", getpid(),
			chid);

}
