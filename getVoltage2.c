#include "stdio.h"
#include "stdlib.h"
float getVoltage(int num);
int main() {
	int num;
	char *data;
	float vcc;
	printf("Content-type: application/json\n\n");
	//get param
	data = getenv("QUERY_STRING");
	if (data == NULL) {
		num = 0;
//		char msg[] = "No arameters. Please enter params in your url.";
//		printf("{\"msg\":\"%s\"}", msg);
//		return 0;
	} else if (sscanf(data, "output=%d", &num) != 1) {
//		char msg[] =
//				"Error! Error in passing parameters from client to script.Invalid params.";
//		printf("{\"msg\":\"%s\"}", msg);
//		return 0;
		num = 0;
	}
	if (num > 0 && num < 5) {
		vcc = getVoltage(num);
		printf("{\"output%d\":\"%.3f\"}", num, vcc);
	}
	if (num == 0) {
		int i;
		char content[4][18];
		for (i = 0; i < 4; i++) {
			vcc = getVoltage(i + 1);
			sprintf(content[i], "\"output%d\":\"%.3f\"", i + 1, vcc);
		}
		printf("{%s,%s,%s,%s}", content[0], content[1], content[2], content[3]);
	}
	return 0;
}
float getVoltage(int num) {
	char result[11], path[80];
	float vcc;
	FILE *fp;
	int status;
	if (num < 1 || num > 4) {
		char msg[] = "Error!Invalid params. [1-4]";
		printf("{\"msg\":\"%s\"}", msg);
		return 0;
	}
	//execute program and read the output
	sprintf(path, "/opt/MOD-IO/mod-io -an%d 0x58", num);
	fp = popen(path, "r");
	char format[80];
	sprintf(format, "AN%d: %%fV", num);
	while (fgets(result, 11, fp) != NULL)
		sscanf(result, format, &vcc);
	status = pclose(fp);
	return vcc;
}
