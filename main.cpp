#include <cstdlib>

//include c library
extern "C" {

//include your own C source files
#include "SPPoint.h"
#include "SPLogger.h"
#include "SPConfig.h"
#include "kdTree.h"
#include "AllocateManager.h"
}

//include C++ source files
#include "SPImageProc.h"

int main(int argc, char **argv) {
    sp::ImageProc* imageProc = NULL;
	char dir[MAXBUF];
	strcpy(dir, "../spcbir.config"); // todo change
	bool usingDefaultConfigFile = true;
	SP_CONFIG_MSG msg;
	if (argc > 1) {
		if (argc != 3) {
				printf("Invalid command line : use -c <config_filename>\n");
				return -1;
			} else {
				strcpy(dir, argv[2]);
				usingDefaultConfigFile = false;
				if (strcmp(argv[1], "-c") != 0) {
					printf("Invalid command line : use -c <config_filename>\n");
					return -1;
				}
			}
	}

	SPConfig config = spConfigCreate(dir, &msg);
	if (!config) {
		if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
			printf("The configuration file %s couldn't be open\n", usingDefaultConfigFile ? "spcbir.config" : dir);
		}
        return -1;
	}

	// config successfull
    imageProc = new sp::ImageProc(config);
    // todo check if needed to handle error





	if(spConfigIsExtractionMode(config, &msg)) {


	} else {
		// not in extraction mode
		if (msg == SP_CONFIG_INVALID_ARGUMENT) {
			// show error
			return -1;
		}

	}
}
