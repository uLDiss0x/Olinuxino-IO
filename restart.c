#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _DELAY 1
// call _>         .../cgi-bin/restart?output=2
int restart(int num, int delay);
int main() {
	char *data, param;
	char msg_ok[] = "done";
	int num;
	printf("Content-type: application/json\n\n");
	//=========parse params==================
	data = getenv("QUERY_STRING");
	if (sscanf(data, "output=%d", &num) != 1) {
		char msg[] = "Parameter can not be parsed./Invalid parameter for delay.";
		printf("{\"msg\":\"%s\"}", msg);
		return 0;
	}

	//=========toggle relay==================
	restart(num, _DELAY);
	//printf("Content-type: application/json\n\n");
	printf("{\"message\":\"%s\"}", msg_ok);
	return 0;
}
int restart(int num, int delay) {
	char param, command[50];
	switch (num) {
	case 1:
		param = 'E';
		break;
	case 2:
		param = 'D';
		break;
	case 3:
		param = 'B';
		break;
	case 4:
		param = '7';
		break;
	default:
		param = '0';
	}
	sprintf(command, "/opt/MOD-IO/mod-io -relay 0x58 0x01");
	FILE *fp;
	fp = popen(command, "r");
	if (fp == NULL) {
		char msg[] = "Can not open the mod-io program.";
		printf("{\"msg\":\"%s\"}", msg);
		return 1;
	}
	pclose(fp);
	sleep(delay);
	fp = popen("/opt/MOD-IO/mod-io -relay 0x58 0x0F", "r");
	if (fp == NULL) {
		char msg[] = "Can not open the mod-io program.";
		printf("{\"msg\":\"%s\"}", msg);
		return 1;
	}
	pclose(fp);
	return 0;
}

