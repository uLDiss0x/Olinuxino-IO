#include "stdlib.h"
#include "stdio.h"
#include <string.h>

int main() {
	char *data;
	int status, num, i = 0;
	float min, max;
	FILE *fp, *fr;
	char content[4][32];
	char current[32];
	printf("Content-type: application/json\n\n");
	//=======read config file-======================
	fr = fopen("/opt/MOD-IO/config.txt", "r");
	while (i < 4) {
		if (fgets(content[i], 32, fr) != NULL) {
			//printf("%s%d", content[i], strlen(content[i]));
		}
		i++;
	}
	fclose(fr);
	//=======parse params===========================
	data = getenv("QUERY_STRING");
	if (data == NULL) {
		char msg[] = "No arameters. Please enter params in your url.";
		printf("{\"msg\":\"%s\"}", msg);
		return 0;
	} else if (sscanf(data, "min=%f&max=%f&output=%d", &min, &max, &num) != 3) {
		char msg[] =
				"Error! Error in passing parameters from client to script.Invalid params.";
		printf("{\"msg\":\"%s\"}", msg);
		return 0;
	} else {
		//=======write to config file====================
		fp = fopen("/opt/MOD-IO/config.txt", "w");
		if (fp == NULL) {
			char msg[] = "Error! Can not open the config file.";
			printf("{\"msg\":\"%s\"}", msg);
			return 0;
		}
		sprintf(current, "output%d: minV=%.3f maxV=%.3f\n", num, min, max);
		strcpy(content[num - 1], current);
		fprintf(fp, "%s%s%s%s", content[0], content[1], content[2], content[3]);
		status = fclose(fp);
		char msg[] = "Done";
		printf("{\"msg\":\"%s\"}", msg);
	}
	return 0;
}
