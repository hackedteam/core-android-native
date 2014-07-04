/*
 * Simple string obfuscation - Quequero 2013
 *
 * Obfuscated string format:
 * 1-byte key. 1-byte module. 1-byte length, <string>
 */

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// <search string> <replace string> <string to search>
unsigned char *str_replace(unsigned char *search , unsigned char *replace , unsigned char *subject) {
  unsigned char *p = NULL , *old = NULL , *new_subject = NULL ;
  int c = 0 , search_size;

  search_size = strlen(search);

  for (p = strstr(subject , search) ; p != NULL ; p = strstr(p + search_size , search)) {
    c++;
  }

  c = (strlen(replace) - search_size )*c + strlen(subject);
  new_subject = malloc(c);
  strcpy(new_subject , "");
  old = subject;

  for (p = strstr(subject , search) ; p != NULL ; p = strstr(p + search_size , search)) {
    strncpy(new_subject + strlen(new_subject) , old , p - old);
    strcpy(new_subject + strlen(new_subject) , replace);
    old = p + search_size;
  }

  strcpy(new_subject + strlen(new_subject) , old);
  return new_subject;
}

int obfuscate(unsigned char *s, unsigned char *d) {
  unsigned int seed;
  unsigned char key, mod;
  unsigned char *rep = NULL;
  unsigned char nl[] = "\n";
  unsigned char tb[] = "\t";
  unsigned char cr[] = "\r";
  int i, j, len;

  srandom(time(0));
  seed = random();

  key = (unsigned char)(seed & 0x000000ff);
  mod = (unsigned char)((seed & 0x0000ff00) >> 8);

  rep = str_replace("\\n", nl, s);
  
  /*rep = str_replace("\\t", tb, rep);
  
  rep = str_replace("\\r", cr, rep);
  printf("LENGTH rep3: %d\n", strlen(rep));*/

  len = strlen(rep);

  

  if (len > 255) {
    printf("String too long\n");
    return;
  }

  d[0] = key;
  d[1] = mod;
  d[2] = (unsigned char)len ^ key ^ mod;

  for (i = 0, j = 3; i < len; i++, j++) {
    d[j] = rep[i] ^ key;
    d[j] += mod;
    d[j] ^= mod;
  }

  return len;
}

unsigned char* deobfuscate(unsigned char *s) {
  unsigned char key, mod, len;
  int i, j;
  static unsigned char d[256]; // E' zozza ma cosi' non serve la free()

  key = s[0];
  mod = s[1];
  len = s[2] ^ key ^ mod;

  // zero terminate the string
  memset(d, 0x00, len + 1);

  for (i = 0, j = 3; i < len; i++, j++) {
    d[i] = s[j] ^ mod;
    d[i] -= mod;
    d[i] ^= key;
  }

  d[len] = 0;
  return d;
}

int main(int argc, char *argv[]) {
  unsigned char *buf, *test;
  int i, obf_len;

  
  if (argc < 2) {
    printf("Usage: %s <string>\n", argv[0]);
    return 0;
  }

  if (strlen(argv[1]) > 255) {
    printf("String too long\n");
    return 0;
    }


  obf_len = strlen(argv[1]) + 3;

  buf = (unsigned char *)malloc(obf_len);
  memset(buf, 0x00, obf_len);

  obf_len = obfuscate(argv[1], buf);
  obf_len += 3;

  printf("unsigned char obf_string[] = \"");

  for (i = 0; i < obf_len; i++) {
    printf("\\x");
    printf("%02x", buf[i]);
  }

  printf("\"; // \"%s\"\n", argv[1]);

  test = deobfuscate(buf);

  printf("Deobfuscated string: \"%s\"\n", test);

  free(buf);
  return 0;
}
