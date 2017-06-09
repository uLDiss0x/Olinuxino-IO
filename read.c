#include <stdio.h>
#include <stdlib.h>
int readConfig(char file[], int line, float *min, float* max);
int main() {
	int num;
	float min, max;
	char *data;

	printf("Content-type: application/json\n\n");
	char file[] = "C:\Users\uLDiss0x\Documents\visual studio 2017\Projects\Olinuxino\Olinuxino\Debug\config.txt";
	//==param
	//data = getenv("QUERY_STRING");
	data = 0;
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
		if (readConfig(file, num, &min, &max) > 0)
			return 0;
		printf("{\"output%d\":{\"minV\":\"%.3f\",\"maxV\":\"%.3f\"}}", num, min,
				max);
	}
	if (num == 0) {
		char content[4][42];
		int i;
		for (i = 1; i < 5; i++) {
			if (readConfig(file, i, &min, &max) > 0)
				return 0;
			sprintf(content[i - 1],
					"\"output%d\":{\"minV\":\"%.3f\",\"maxV\":\"%.3f\"}", i,
					min, max);

		}
		printf("{%s,%s,%s,%s}", content[0], content[1], content[2], content[3]);
	}
	return 0;
}

int readConfig(char file[], int num, float *min, float *max) {
	FILE *fp;
	int i = 0, status;
	char content[4][32];
	char format[32];
	fp = fopen(file, "r");
	if (fp == NULL) {
		char msg[] = "Error! Can not open the config file.";
		printf("{\"msg\":\"%s\"}", msg);
		return 1;
	}
	while (i < 4) {
		if (fgets(content[i], 32, fp) != NULL) {
			//printf("%s%d", content[i], strlen(content[i]));
		}
		i++;
	}
	sprintf(format, "output%d: minV=%%f maxV=%%f", num);
	if (sscanf(content[num - 1], format, min, max) != 2) {
		char msg[] =
				"Config file can not be parsed./Invalid parameters/file is empty.";
		printf("{\"msg\":\"%s\"}", msg);
		return 1;
	}
	status = fclose(fp);
	return 0;
}
