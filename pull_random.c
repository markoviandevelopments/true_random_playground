#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include NUM_BITS 100

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

int main(void) {
    CURL *curl_handle;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, "http://108.254.1.184:8003/bits?count=100");
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl_handle);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Find the start of the bits array
            char *array_start = strchr(chunk.memory, '[');
            if (array_start) {
                array_start++; // Skip '['
                int bits[NUM_BITS];
                for (int i = 0; i < NUM_BITS; i++) {
                    bits[i] = array_start[2 * i] - '0';
                }
                // Now bits[] contains the decoded buffer; print it as an example
                printf("Decoded bits: ");
                for (int i = 0; i < NUM_BITS; i++) {
                    printf("%d ", bits[i]);
                }
                printf("\n");
            } else {
                printf("Failed to find bits array in response.\n");
            }
        }
        curl_easy_cleanup(curl_handle);
    }
    free(chunk.memory);
    curl_global_cleanup();
    return 0;
}
