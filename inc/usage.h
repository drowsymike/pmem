#ifndef USAGE_H
#define USAGE_H

#include "main.h"

#define VERSION_FLAG 'v'
#define VERSION_ARG "version"
#define HELP_FLAG 'h'
#define HELP_ARG "help"
#define PROCESS_PID_FLAG 'p'
#define PROCESS_PID_ARG "process-id"
#define LIVE_FLAG 'l'
#define LIVE_ARG "live"
#define EXTENDED_OUTPUT_FLAG 'e'
#define EXTENDED_OUTPUT_ARG "extended-output"

/**
 * @brief Standard Linux directory with processes files
 * This is a foreword for the path to the process directory
 * 
 */
#define PROCESS_PATHNAME_FOREWORD "/proc/"

/**
 * @brief The afterword for configuring the pathname to the
 * concrete file inside the process directory
 * 
 */
#define PROCESS_PATHNAME_AFTERWORD "/status"

/*---------------------------------------------------------------------------*/

/**
 * @brief Function for printing utility version
 * 
 */
void print_version(void);

/**
 * @brief Function for printing the help for utility
 * 
 */
void print_help(void);

/**
 * @brief Function for checking the existing of process
 * 
 */
int8_t check_does_process_exist(char* process_dir);

/* --- */

/**
 * @brief Function for checking the access to the process directory
 * 
 */
int8_t check_process_directory_access(char* current_process);

/**
 *  @brief Fuinction for checking the process' status for the moment
 * 
 */
int8_t process_monitoring(char* process_dir, bool live_flag, bool extended_output);

/**
 *  @brief Fuinction for checking the process' status for the moment
 * 
 */
void parse_process_state(FILE *fp);

#endif