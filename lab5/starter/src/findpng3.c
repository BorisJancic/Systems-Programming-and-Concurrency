#include <curl/multi.h>
#include <stdio.h>
#include <stdlib.h>


#include "url_vector.h"

static void init(CURLM *cm, int i);

URL_VECTOR log_vec = { .size = 0 };
URL_VECTOR frontier_vec = { .size = 0 };
URL_VECTOR png_vec = { .size = 0 };


int main(int argc, char** argv) {
	int t = 1;

	CURLM* p_multi_handle;
	CURL* p_easy_handle;

	curl_global_init(CURL_GLOBAL_ALL);
	cm = curl_multi_init();

	
	CURLM handle_arr[t];
	push(&frontier_vec, 

	for (int i = 0; i < t; i++) {
		init(&handle_arr[i]);
	}
/*
 * for i in t:
 *	creaete curl handle and send request
 *
 * while (frontier not empty and no live handles)
 *		while new message:
 *			if frontier not empty pop(frontier) send new request
 *		
 *		get response
 *		if png: push png
 *		else:
 *			for i in url
 *				push to frontier
 */
    curl_multi_cleanup(cm);
	curl_global_cleanup();
	
	return 0;
}

static void init(CURLM *cm) {
  CURL *eh = curl_easy_init();
  curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, cb);
  curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
  curl_easy_setopt(eh, CURLOPT_URL, "");
  curl_easy_setopt(eh, CURLOPT_PRIVATE, "");
  curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
  curl_multi_add_handle(cm, eh);
}


