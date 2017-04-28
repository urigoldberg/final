#include "SPLogger.h"

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		printf(R_LOGGER_OUT_OF_MEMORY_ERROR);
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL || strcmp(filename, "stdout") == 0) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			printf(R_LOGGER_CANNOT_OPEN_FILE_ERROR);
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) { //Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger); //free allocation
	logger = NULL;
}


// TODO check if should malloc string or is it good like this
SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line) {
	if (!logger) {
		return SP_LOGGER_UNDIFINED;
	}
	if (msg == NULL || file == NULL || function == NULL || line < 0) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if (fprintf(logger->outputChannel,
			"---ERROR---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
			file, function, line, msg) < 0) {
		return SP_LOGGER_WRITE_FAIL;
	}
	return SP_LOGGER_SUCCESS;

}

SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line) {
	if (!logger) {
		return SP_LOGGER_UNDIFINED;
	}
	if (msg == NULL || file == NULL || function == NULL || line < 0) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if (logger->level != SP_LOGGER_ERROR_LEVEL) {
		if (fprintf(logger->outputChannel,
				"---WARNING---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
				file, function, line, msg) < 0) {
			return SP_LOGGER_WRITE_FAIL;
		}
	}
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg) {
	if (!logger) {
		return SP_LOGGER_UNDIFINED;
	}
	if (msg == NULL) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if (logger->level != SP_LOGGER_ERROR_LEVEL
			&& logger->level != SP_LOGGER_WARNING_ERROR_LEVEL) {
		if (fprintf(logger->outputChannel, "---INFO---\n- message: %s\n", msg)
				< 0) {
			return SP_LOGGER_WRITE_FAIL;
		}
	}
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line) {
	if (!logger) {
		return SP_LOGGER_UNDIFINED;
	}
	if (msg == NULL || file == NULL || function == NULL || line < 0) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if (logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) {
		if (fprintf(logger->outputChannel,
				"---DEBUG---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
				file, function, line, msg) < 0) {
			return SP_LOGGER_WRITE_FAIL;
		}
	}
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintMsg(const char* msg) {
	if (!logger) {
		return SP_LOGGER_UNDIFINED;
	}
	if (msg == NULL) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if (fprintf(logger->outputChannel, "%s\n", msg) < 0) {
		return SP_LOGGER_WRITE_FAIL;
	}

	return SP_LOGGER_SUCCESS;
}

