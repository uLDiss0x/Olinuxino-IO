#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/file.h>

#define DELAY 1
int readConfig(char file[], char log[], int line, float *min, float* max);
float getVoltage(int num, char log[]);
int restart(char num, int delay);
int main(void) {

	/* Our process ID and Session ID */
	pid_t pid, sid;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		exit (EXIT_FAILURE);
	}
	/* If we got a good PID, then
	 we can exit the parent process. */
	if (pid > 0) {
		exit (EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	/* Open any logs here */

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		/* Log the failure */
		exit (EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		/* Log the failure */
		exit (EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	close (STDERR_FILENO);

	/* Daemon-specific initialization goes here */

//	char str[10];
//	int lfp = open("/var/run/daemon.pid", O_RDWR | O_CREAT);
//	if (lfp < 0)
//		exit(1); /* can not open */
//	if (flock(lfp, LOCK_EX) < 0)
//		exit(0); /* can not lock */
	/* only first instance continues */

//	sprintf(str, "%d\n", setsid());
//	write(lfp, str, strlen(str)); /* record pid to lockfile */
	/* The Big Loop */
	//turn al realys ON
	FILE *fpIO;
	fpIO = popen("/opt/MOD-IO/mod-io -relay 0x58 0x01", "r");
	pclose(fpIO);
	while (1) {
		/* Do some task here ... */
		float min, max, vcc;
		FILE * log;
		int i;
		char hex;

		char path_conf[] = "/opt/MOD-IO/config.txt"; //change
		char path_log[] = "/opt/MOD-IO/log.txt";
		log = fopen("/opt/MOD-IO/log.txt", "a"); //change
                hex = 0x0F;
		for (i = 1; i < 5; i++) {
			readConfig(path_conf, path_log, i, &min, &max);
			vcc = getVoltage(i, path_log);
			if (vcc < min) {
				hex = hex & ~(1<<(i-1));
				fprintf(log, "\n[WARNING]	Danger! Voltage is too low."); //turn off?
				fprintf(log, "\n[INFO]	Vcc%d=%.3f min=%.3f max=%.3f", i, vcc,
						min, max);
			}
			if (vcc > max) {
				hex = hex & ~(1<<(i-1));
				fprintf(log, "\n[WARNING]	Danger! Voltage is too high."); //turn off?
				fprintf(log, "\n[INFO]	Vcc%d=%.3f min=%.3f max=%.3f", i, vcc,min, max);
			}
		}
		restart(hex, 1);
		fclose(log);
		sleep(0); /* wait 30 seconds */
	}
	exit (EXIT_SUCCESS);
}
int readConfig(char file[], char log[], int num, float *min, float *max) {
	FILE *fp, *fl;
	int i = 0, status;
	char content[4][32];
	char format[32];
	fp = fopen(file, "r");
	fl = fopen(log, "a");
	if (fp == NULL) {
		fprintf(fl, "/n[ERROR]	Can't open config file.");
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
		fprintf(fl,
				"\n[ERROR]	Config file can not be parsed./Invalid parameters/file is empty.");
		return 1;
	}
	status = fclose(fp);
	fclose(fl);
	return 0;
}
float getVoltage(int num, char log[]) {
	char result[11], path[80];
	float vcc;
	FILE *fp, *fl;
	int status;
	fl = fopen(log, "a");
	if (num < 1 || num > 4) {
		fprintf(fl, "Error!Invalid params. [1-4]");
		return -1;
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
int restart(char num, int delay) {
	char command[50];
	sprintf(command, "/opt/MOD-IO/mod-io -relay 0x58 0x%02x", num);
        //sprintf(command, "/opt/MOD-IO/mod-io -relay 0x58 0x02");
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
