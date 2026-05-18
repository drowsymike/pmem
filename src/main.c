#include "main.h"
#include "usage.h"

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

/**
 * @brief Function for printing utility version
 * 
 */
void print_version() {
  printf("%s %s\n", PROGRAM_NAME, PRIGRAM_VERSION);
  puts("Copyright (c) 2026 drowsymike");
  puts("License: ISC <https://opensource.org>");
  puts("");
  puts("This is free software: you are free to change and redistribute it.");
  puts("There is NO WARRANTY, to the extent permitted by law.");
}

/**
 * @brief Function for printing the help for utility
 * 
 */
void print_help(void)
{
  puts("NO WAY HAHAHAHHAHAHA");
}

/**
 * @brief Function for checking the existing of process
 * 
 */
int8_t check_does_process_exist(char* process_dir) 
{
  DIR* dir = opendir(process_dir); 

  if (dir == NULL) 
  {
    printf("Error: Process with following PID does not exist\n");
    exit(ENOENT);
  }

  printf("Process exists");

  return 0;
}

/*---------------------------------------------------------------------------*/

/**
 * @brief main function
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
  if (argc < 0 || argc > 1024) {
    _exit(1);
  }

  /* --- */

  int opt;
  char* pid = NULL;

  static struct option long_options[] = 
	{
    {"version", no_argument, 0, VERSION_FLAG},
    {"help", no_argument, 0, HELP_FLAG},
    {"procid", required_argument, 0, PROCESS_PID_FLAG},
    {"live", no_argument, 0, LIVE_FLAG},
    {0, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "vhp:l", long_options, NULL)) != -1) 
  {
    switch (opt) 
    {
      case VERSION_FLAG: 
        print_version();
        return 0;
      case HELP_FLAG: 
        print_help();
        return 0;
      case PROCESS_PID_FLAG:
        pid = optarg;
        break;
      case LIVE_FLAG:
        puts("nah");
        break;
    }
  }

  /* --- */

  if (pid == NULL) 
  {
    printf("Error: Enter the PID for the process you want to monitor\n");
    exit(EINVAL);
  }

  /* --- */

  char process[15] = PROCESS_PATHNAME_FOREWORD;
  strcat(process, pid);

  /* --- */

  set_security();
  check_does_process_exist(process);

  /* --- */

  return 0;
}