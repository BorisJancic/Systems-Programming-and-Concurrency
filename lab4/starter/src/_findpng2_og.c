#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "http_search.h"
#include "url_vector.h"

#define URL_HEAD "http://ece252-1.uwaterloo.ca/lab4"
#define	URL_PNG "http://ece252-3.uwaterloo.ca:2540/image?q=gAAAAABdHkoqOKR-cFRCkiCUBEMAAAWfDvBFlRisL9ysLWHYHbcQbn1b28PV_uHBZ0gJf5bvzrnf1HNXxB6KRlAVETwTIqBH2Q=="
#define URL_ERR "http://ece252-2.uwaterloo.ca:2542/image?q=gAAAAABdHlHZmbxmnenlxhndlipqfntjijjammhualsfjfkbgruhpprimngcjssinigxalforazyxxmsbuhxcsqwstfveoonvi=="

void* thread_function(void* arg);
bool search_log_and_push_log(char* p_URL);
bool is_png(char* p_URL);

pthread_mutex_t frontier_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t png_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t wake_condition = PTHREAD_COND_INITIALIZER;

URL_VECTOR log_vec = { .size = 0 };
URL_VECTOR frontier_vec = { .size = 0 };
URL_VECTOR png_vec = { .size = 0 };

int slept_arr[20];
pthread_t threads[20];

unsigned slept_threads = 0;
unsigned thread_count = 0;
unsigned killed_threads = 0;

void print_slept_thread(void){
	printf("Slept threads: ");
	for(int i = 0; i < thread_count; i++){
		printf("%d - ", slept_arr[i]);
	}
	printf("\n");
}

int main(int argc, char** argv) {
	thread_count = 10;
//	pthread_t threads[thread_count];
	for(int i = 0; i < thread_count; i++){
		slept_arr[i] = 0;
	}
	for (unsigned i = 0; i < MAX_SIZE; i++) {
		log_vec.urls[i] = NULL;
		frontier_vec.urls[i] = NULL;
	}

	// push head url
	char* p_head = (char*)malloc((strlen(URL_HEAD) + 1)*sizeof(char));
	strcpy(p_head, URL_HEAD);
	push(&frontier_vec, p_head); 
	
	// create and wait for threads
	for (unsigned i = 0; i < thread_count; i++) {
		pthread_create(&threads[i], NULL, thread_function, NULL);
	}
	for (unsigned i = 0; i < thread_count; i++) {
		pthread_join(threads[i], NULL);
	}
	// print our results
	// printf("Log: %d\n", log_vec.size);
	// for (unsigned i = 0; i < log_vec.size; i++)
	//	printf("	%s\n", log_vec.urls[i]);
	printf("PNGs: %d\n", png_vec.size);
	//for (unsigned i = 0; i < png_vec.size; i++)
	//	printf("	%s\n", png_vec.urls[i]);
	//printf("Frontier: %d\n", frontier_vec.size);
	//for (unsigned i = 0; i < frontier_vec.size; i++)
	//	printf("	%s\n", frontier_vec.urls[i]);

	// cleanup
	for (unsigned i = 0; i < log_vec.size; i++) {
		// printf("log vector: %u - %p\n", i, log_vec.urls[i]);
		if (log_vec.urls[i]) free(log_vec.urls[i]);
		log_vec.urls[i] = NULL;
	}
	for (unsigned i = 0; i < png_vec.size; i++) {
		// printf("png vector: %u - %p\n", i, png_vec.urls[i]);
		if (png_vec.urls[i]) free(png_vec.urls[i]);
		png_vec.urls[i] = NULL;
	}
	for (unsigned i = 0; i < frontier_vec.size; i++) {
		// printf("frontier vector: %u - %p\n", i, frontier_vec.urls[i]);
		if (frontier_vec.urls[i]) free(frontier_vec.urls[i]);
		frontier_vec.urls[i] = NULL;	
	}

	return 0;
}

void* thread_function(void* arg) {
	long int thread_num = 0;
	pthread_t tid = pthread_self();
	for(int i = 0; i < thread_count; i++){
		if(threads[i] == tid){
			thread_num = i;
		}
	}
	while (true) {
		char* p_URL;
		bool pushed_to_log = false;
		pthread_mutex_lock(&frontier_mutex);
			if (frontier_vec.size == 0) {
				if (slept_threads == thread_count - 1) {
__thread_exit:
					++killed_threads;
					pthread_mutex_unlock(&frontier_mutex);
					pthread_cond_broadcast(&wake_condition);
					pthread_exit(NULL);
				} else {
					++slept_threads;
					// print_slept_thread();
					slept_arr[thread_num] = 1;
					printf("Slept Threads: %d\n", slept_threads);
					pthread_cond_wait(&wake_condition, &frontier_mutex);
					slept_arr[thread_num] = 0;
					if ((slept_threads == thread_count - 1) && frontier_vec.size == 0) {
						goto __thread_exit;
					}
					--slept_threads;
					pthread_mutex_unlock(&frontier_mutex);
					printf("Slept Threads: %d\n", slept_threads);
					continue;
				}
			}
			p_URL = pop(&frontier_vec);
		pthread_mutex_unlock(&frontier_mutex);
		if (!p_URL) continue;
		
		pushed_to_log = search_log_and_push_log(p_URL);
		if (pushed_to_log) {
			search_web_and_push_frontier(p_URL, &png_vec, &frontier_vec,
										 png_mutex, frontier_mutex);
			pthread_cond_broadcast(&wake_condition);	
		}
		if (!pushed_to_log) free(p_URL);
	}

	pthread_exit(NULL);
}	
bool search_log_and_push_log(char* p_URL) {
	bool found = false;
	bool pushed_to_log = false;
    pthread_mutex_lock(&log_mutex);
		for (unsigned i = 0; i < log_vec.size; i++) {
			if (!strcmp(log_vec.urls[i], p_URL)) {
				found = true;
				continue;
			}
		}
		if (!found) {
			pushed_to_log = push(&log_vec, p_URL);
		}
	pthread_mutex_unlock(&log_mutex);
	
	return pushed_to_log;
}


/*if (slept_threads == thread_count - 0) {
						printf("        pthread_exit(NULL)\n");
						++killed_threads;
						pthread_mutex_unlock(&frontier_mutex);
						pthread_cond_broadcast(&wake_condition);
						pthread_exit(NULL);
					}*/


