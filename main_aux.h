
#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_




#define EnterQuery "Please enter an image path:\n"
#define EXIT "Exiting..."
#define EXITSIGN "<>"
#define CANNTOPENCONFIG "The configuration file %s couldn't be open\n"
#define BESTCANDID "Best candidates for - %s - are:\n"
#define INVALID_ARGS "Invalid command line : use -c <config_filename>\n"
#define PARAMETER_NOT_SET_ERROR "Parameter %s is not set\n"
#define LOGGER_OUT_OF_MEMORY_ERROR "Logger out of memory\n"
#define LOGGER_CANNOT_OPEN_FILE_ERROR "Logger cannot open file\n"
#define ALLOCATION_FAILURE_ERROR "Allocation failure\n"



void removeNewline(char s1[]);

int cmpfunc(const void *a, const void *b);

void freeAll();

#endif /* MAIN_AUX_H_ */
