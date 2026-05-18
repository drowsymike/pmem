#ifndef USAGE_H
#define USAGE_H

#include <getopt.h>

#define VERSION_FLAG 'v'
#define HELP_FLAG 'h'
#define PROCESS_PID_FLAG 'p'
#define LIVE_FLAG 'l'

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
#define PROCESS_PATHNAME_AFTERWORD ""

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

#endif