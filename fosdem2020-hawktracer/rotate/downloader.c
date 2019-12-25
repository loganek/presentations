#include <curl/curl.h>
#include <hawktracer.h>

int32_t download_file(const char* url, const char* destination)
{
    HT_G_TRACE_FUNCTION();

    CURL* curl = curl_easy_init();
    if (!curl)
    {
        return -1;
    }

    FILE* fp = fopen(destination, "wb");
    if (!fp)
    {
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    CURLcode res;
    {
        HT_G_TRACE_OPT_STATIC("http request");
        res = curl_easy_perform(curl);
    }

    long http_code = 0;
    if(res != CURLE_OK)
    {
        http_code = -1;
    }
    else
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    }

    fclose(fp);
    curl_easy_cleanup(curl);

    return http_code;
}
