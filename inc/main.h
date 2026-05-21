#ifndef MAIN_H
#define MAIN_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define PROGRAM_NAME "pmem"
#define PRIGRAM_VERSION "0.1"

/*--Interface----------------------------------------------------------------*/

/**
 * @brief Disabling and enabling the option to dump the memory sector in which 
 * the program was executed
 * 
 */
//#define DUMP_ENABLE

/**
 * @brief Disabling or enabling the option to connect external debugging software
 *  based on PTrace technology
 * 
 */
//#define PTRACE_DEBUG_DISABLE

/**
 * @brief Limiting the size of memory used
 * 
 */
//#define SANDBOX_STACK_LIMIT

/**
 * @brief Clearing environment variables when executing program code
 * 
 */
//#define CLEANING_ENVIRONMENT_VARIABLES

/**
 * @brief Enable or disable the option to transfer privileges to a program's 
 * child processes
 * 
 */
//#define SECCOMP_ENABLE


/*--Searching strings for parsing--------------------------------------------*/

#define VMPEAK "VmPeak:"
#define VMSIZE "VmSize:"
#define VMLCK "VmLck:"
#define VMPIN "VmPin:"
#define VMHWM "VmHWM:"
#define VMRSS "VmRSS:"

/* --- */

#define NAME "Name:"
#define STATE "State:"
#define PID "Pid:"

/*--Function headers---------------------------------------------------------*/

/**
 * @brief Configure binary execution security according to the selected flags 
 * in the Interface section 
 * 
 */
void set_security(void);

#endif