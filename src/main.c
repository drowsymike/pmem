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
  puts("Usage: pmem [OPTION...]");
  puts("Monitoring process memory consumption");
  puts("");
  puts("  -h, -H, --help         Show help options (flag)");
  puts("  -v, -V, --version      Show current version (flag)");
  puts("  -p, --procid           Choose the concrete PID for monitoring (argument)");
  puts("  -l, --live             Enable alive continuous monitoring mode (flag)");
  puts("  -e. --extout           Enable detailed output of process information (in the case of live mode, every 10 lines of tracking are output) (flag)");
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

  return 0;
}

/**
 *  @brief Fuinction for checking the process' status for the moment
 * 
 */
int8_t process_monitoring(char* process_file, bool live_flag, bool extended_output) 
{
  FILE *fp;
  if ((fp = fopen(process_file, "r")) == NULL) 
  {
    exit(1);
  }

  /* --- */

  if (live_flag == true)
  {
    //note: use nanosleep here
  } else 
  {
    parse_process_state(fp);
  }

  return 0;
}

/**
 *  @brief Function for parsing the process' argument (and searching this)
 * 
 */
void parse_process_state(FILE *fp)
{
  char buffer[256];
  int line_number = 1;
  //int found = 0;

  char proc_name[256] = {0};
  unsigned long vm_hwm = 0;
  unsigned long vm_peak = 0;

  while (fgets(buffer, sizeof(buffer), fp) != NULL) 
  {
    if (strstr(buffer, NAME) != NULL) 
    {
      //printf("Name found at the string: %d\n", line_number);
      sscanf(buffer, "%*s %255s", proc_name);
    } else if (strstr(buffer, VMHWM) != NULL) 
    {
      //printf("VmHwm found at the string: %d\n", line_number);
      sscanf(buffer, "%*s %lu", &vm_hwm);
    } else if (strstr(buffer, VMPEAK) != NULL) 
    {
      //printf("VmPeak found at the string: %d\n", line_number);
      sscanf(buffer, "%*s %lu", &vm_peak);
    }
    line_number++;
  }

  if (vm_hwm == 0) 
  {
    puts("Warning: VmHwm parameter not found");
  }

  if (vm_peak == 0) 
  {
    puts("Warning: VmPeak parameter not found");
  }

  printf("Process: %s | VmPeak: %lu KB | VmHWM: %lu KB\n", proc_name, vm_peak, vm_hwm);
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
    {VERSION_ARG, no_argument, 0, VERSION_FLAG},
    {HELP_ARG, no_argument, 0, HELP_FLAG},
    {PROCESS_PID_ARG, required_argument, 0, PROCESS_PID_FLAG},
    {LIVE_ARG, no_argument, 0, LIVE_FLAG},
    {EXTENDED_OUTPUT_ARG, no_argument, 0, EXTENDED_OUTPUT_FLAG},
    {0, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "vhp:le", long_options, NULL)) != -1) 
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
      case EXTENDED_OUTPUT_FLAG:
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

  char process_dir[64]; 
  char process_state_file[128];
  snprintf(process_dir, sizeof(process_dir), "%s%s", PROCESS_PATHNAME_FOREWORD, pid);
  snprintf(process_state_file, sizeof(process_state_file), "%s%s%s", PROCESS_PATHNAME_FOREWORD, pid, PROCESS_PATHNAME_AFTERWORD);

  //printf("%s\n", process_dir);
  //printf("%s\n", process_state_file);

  /* --- */

  set_security();
  check_does_process_exist(process_dir);
  process_monitoring(process_state_file, false, false);

  /* --- */

  return 0;
}