#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <sys/resource.h>
#include <string.h>

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

/*--Function headers---------------------------------------------------------*/

/**
 * @brief Configure binary execution security according to the selected flags 
 * in the Interface section 
 * 
 */
void set_security(void);

/*---------------------------------------------------------------------------*/

/**
 * @brief Configure binary execution security according to the selected flags 
 * in the Interface section 
 * 
 */
void set_security(void) 
{
    //be careful with this code. it isn't scalable because of the nesting of 
    //conditions
    #ifdef DUMP_ENABLE
        struct rlimit rlim = {0, 0};
        setrlimit(RLIMIT_CORE, &rlim);
        #if !defined(SANDBOX_STACK_LIMIT) || (SANDBOX_STACK_LIMIT == 0)
            rlim.rlim_cur = rlim.rlim_max = SANDBOX_STACK_LIMIT * 1024;
            setrlimit(RLIMIT_STACK, &rlim);
        #endif
    #endif
    
    #ifdef PTRACE_DEBUG_DISABLE
        prctl(PR_SET_DUMPABLE, 0);
    #endif
    
    #ifdef CLEANING_ENVIRONMENT_VARIABLES
        unsetenv("LD_PRELOAD");
        unsetenv("LD_LIBRARY_PATH");
    #endif

    #ifdef SECCOMP_ENABLE
        prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
        prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT);
    #endif
}

/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    if (argc < 0 || argc > 1024) {
        _exit(1);
    }

    set_security();
    
    return 0;
}