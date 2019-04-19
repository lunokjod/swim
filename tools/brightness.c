#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <stdio.h>
#include <fcntl.h>

int GetValue(char * filename) {
	FILE *fd;
	fd = fopen(filename, "r");
	if ( !fd ) {
		fprintf(stderr, "Unable to read file: %s\n", filename);
		return -1;
	}
	fseek( fd , 0L , SEEK_END);
	long fsize = ftell( fd );
	rewind( fd );
	char * data;
	data = malloc(fsize);
	fread( data , fsize, 1 , fd);
	int result = atoi(data);
	free(data);
	fclose(fd);
	return result;
}

void PutValue(char * filename, int value) {
	FILE *fd;
	fd = fopen(filename, "w");
	if ( !fd ) {
		fprintf(stderr, "Unable to write file: %s\n", filename);
		return;
	}
	fprintf(fd,"%d", value);
	fclose(fd);
}


int main(int argc, char*argv[]) {
	static char* rootNode = "/sys/class/backlight/";
	static char* maxBrightnessFile = "/max_brightness";
	static char* currentFile = "/actual_brightness";
	static char* changeFile = "/brightness";


	if ( argc < 2 ) { 
		fprintf(stderr, "No command supply:\n<up>   to pump up\n<down> to slow down\n");
		return EXIT_FAILURE;
	}
	bool direction = false;
	if ( 0 == strcmp(argv[1], "up")  ) {
		direction=true;
	}


	char * device = "intel_backlight"; // hardcoded @TODO use command line
	
	char * maxBrightnessFileName = malloc(strlen(rootNode)+strlen(device)+strlen(maxBrightnessFile));
	sprintf(maxBrightnessFileName, "%s%s%s", rootNode, device, maxBrightnessFile);
	int maxValue = GetValue(maxBrightnessFileName);
	fprintf(stdout, "file: %s\n", maxBrightnessFileName);
	free(maxBrightnessFileName);
	fprintf(stdout, "MaxValue? %d\n", maxValue);

	char * currentBrightnessFileName = malloc(strlen(rootNode)+strlen(device)+strlen(currentFile));
	sprintf(currentBrightnessFileName, "%s%s%s", rootNode, device, currentFile);
	int currentValue = GetValue(currentBrightnessFileName);
	fprintf(stdout, "file: %s\n", currentBrightnessFileName);
	free(currentBrightnessFileName);
	fprintf(stdout, "CurrentValue? %d\n", currentValue);

	fprintf(stdout, "Direction: %s\n",direction?"Up":"Down");
 	int increment = 3000;
 	if ( false == direction ) {
 		increment=increment*-1;
 	}
 	int finalValue = currentValue+increment;
 	if ( finalValue > maxValue) {
 		finalValue = maxValue;
 	}
	fprintf(stdout, "NuValue: %d\n",finalValue);


	char * changeFileName = malloc(strlen(rootNode)+strlen(device)+strlen(changeFile));
	sprintf(changeFileName, "%s%s%s", rootNode, device, changeFile);
	PutValue(changeFileName, finalValue);
	fprintf(stdout, "file: %s\n", changeFileName);
	free(changeFileName);




	


	return EXIT_SUCCESS;
}