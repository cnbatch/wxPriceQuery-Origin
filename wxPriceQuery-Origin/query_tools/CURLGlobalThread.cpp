#include "CURLGlobalThread.hpp"
#include <curl/curl.h>

namespace query_tools
{
	CURLGlobalThread::CURLGlobalThread()
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
	}

	CURLGlobalThread::~CURLGlobalThread()
	{
		curl_global_cleanup();
	}
}
