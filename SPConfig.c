#include "SPconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define DELIM "="

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	SPConfig res = (SPConfig) malloc(sizeof(SPConfig));
    char line[1024];
	FILE* configFile;
	if (!filename) {
		configFile = fopen("spcbir.config", "r");
	} else {
		configFile = fopen(filename, "r");
	}

	if (configFile == NULL) {
		// TODO return error
		return NULL;
	}
	while (fgets(line, sizeof(line), configFile)) {
	        printf("%s", line);
	    }
	return res;
}


int handleLine(SPConfig*, char* line) {

}

int main(int argc, char **argv) {
	spConfigCreate(NULL, NULL);
}

