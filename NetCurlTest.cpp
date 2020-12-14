
#include <curl\curl.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>

using namespace std;

string Requests[] = {
	"https://fapi.binance.com/fapi/v1/ticker/bookTicker",
		"https://fapi.binance.com/fapi/v1/time",
		"https://fapi.binance.com/fapi/v1/depth?symbol=EGLDUSDT&limit=500",
		"https://fapi.binance.com/fapi/v1/depth?symbol=SOLUSDT&limit=500",
		"https://fapi.binance.com/fapi/v1/klines?symbol=KSMUSDT&interval=5m",
		"https://fapi.binance.com/fapi/v1/klines?symbol=SOLUSDT&interval=5m"
};

CURL* curl;

static bool Terminated = false;

BOOL WINAPI consoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT) {
		printf("Ctrl-C handled\n"); // do cleanup
		Terminated = true;
	}
	return TRUE;
}

size_t  curl_cb(void* content, size_t size, size_t nmemb, std::string* buffer)
{
	buffer->append((char*)content, size * nmemb);
	return size * nmemb;
}

long curl_api(const string& url, string& result) {
	long http_code = 0;
	vector <string> v;
	string action = "GET";
	string post_data = "";
	CURLcode res;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip");

	struct curl_slist* list = NULL;
	list = curl_slist_append(list, "Connection: Keep-Alive");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);


	res = curl_easy_perform(curl);

	/* Check for errors */
	if (res != CURLE_OK) {
		string err_str = curl_easy_strerror(res);
		printf("\r\nError %s <%d> has occurred.   <%s> \n ", err_str.c_str(), res, url.c_str());

	}
	else {
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	}
	return http_code;
}

tm NowTIme()
{
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	return timeinfo;
}

void doNextRequest(const string& url) {
	long hcode;
	string resp;
	hcode = curl_api(url, resp);

	if (hcode)
	{
		struct tm timeinfo = NowTIme();
		if (hcode == 200) {
			printf("%02d:%02d:%02d: [%d]  <%s>                                  \r",
				timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
				hcode, url.c_str());
		}
		else {
			printf("\r\n%02d:%02d:%02d: [%d]  <%s> %s                                 \r\n",
				timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
				hcode, url.c_str(), resp.c_str());
		}
	}
}


int main()
{
	if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
		cout << "SetConsoleCtrlHandler!\n";
		return 1;
	}
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	FILE* filep;
	fopen_s(&filep, "dump.log", "w+");

	curl_easy_setopt(curl, CURLOPT_STDERR, filep);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); // dig into the error

	while (!Terminated) {
		for (string s : Requests)
		{
			doNextRequest(s);
			//	Sleep(100);
		}
		Sleep(1500);
	}

	cout << "cleanup ..\n";
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	cout << "DONE!\n";
	_fcloseall();
	return 0;
}

