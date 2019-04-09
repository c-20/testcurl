#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <curl/curl.h>

typedef struct _string {
  char *v;
  size_t len;
} string;

void initstring(string *s) {
  s->len = 0;
  s->v = (char *)malloc(s->len * sizeof(char) + 1);
  if (s->v == NULL) {
    fprintf(stderr, "initstring alloc fail\n");
    exit(EXIT_FAILURE);
  }
  s->v[0] = '\0';
}

static size_t urlhead(void *ptr, size_t size, size_t nmemb, void *str) {
  string *s = (string *)str;
  size_t numbytes = size * nmemb;
  size_t newlen = s->len + numbytes;
  s->v = realloc(s->v, newlen + 1);
  if (s->v == NULL) {
    fprintf(stderr, "urlhead realloc fail\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->v + s->len, ptr, numbytes);
  s->v[newlen] = '\0';
  printf("HEAD[%d]:", numbytes);
  printf("%s", &s->v[s->len]); // includes linebreak
  s->len = newlen;
  return numbytes;
}

static size_t urldata(void *ptr, size_t size, size_t nmemb, void *str) {
  string *s = (string *)str;
  size_t numbytes = size * nmemb;
  size_t newlen = s->len + numbytes;
  s->v = realloc(s->v, newlen + 1);
  if (s->v == NULL) {
    fprintf(stderr, "urldata realloc fail\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->v + s->len, ptr, numbytes);
  s->v[newlen] = '\0';
  s->len = newlen;
  printf("DATA[%d]", numbytes);
  return numbytes;
}

#define readdate main
int readdate(int argc, char **argv) {
  char *sourceurl = "https://xkcd.com";
  CURL *curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_ALL); // init winsock
  curl = curl_easy_init();
  if (curl) {
    string urlheadstr, urldatastr;
    initstring(&urlheadstr);
    initstring(&urldatastr);
//    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
//    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
//    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, sourceurl);
//    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, urlhead);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &urlheadstr);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, urldata);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &urldatastr);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  //  curl_easy_setopt(curl, CURLOPT_READDATA, readfile);
  //  curl_off_t filesize = (curl_off_t)file_info.st_size;
  //  curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, filesize);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    // note: date is in the header ....
    /*
    char *match = "Date:";
    char *mc = match;
    char matching = 'n';
    int i = -1;
    char *ch = urldatastr.v;
    while (++i < urldatastr.len) {
      if (matching == 'y') {
        printf("%c", ch[i]);
      } else { // not yet matching ...
        if (ch[i] == mc[0]) {
          mc++;
          if (mc[0] == '\0') {
            matching = 'y';
          }
        } else {
          mc = match;
        }
      }
    }
    */
  }
//  fclose(readfile);

  curl_global_cleanup();
  return 0;
}
