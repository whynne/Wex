#ifndef H_BEGIN_CODE
#define H_BEGIN_CODE

#if defined WIN32 && defined WEX_EXPORT
	#define WEXAPI __declspec(dllexport) 
#else
	#define WEXAPI __declspec(dllimport)
#endif

#endif