#pragma once
/*
https://docs.microsoft.com/zh-cn/windows/desktop/ProcThread/creating-a-child-process-with-redirected-input-and-output
*/

#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>

constexpr auto BUFSIZE = 4096 ;

class AnonymousPipes {
private:
	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL;
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;
	void CreateChildProcess(const char *path);
public:
	AnonymousPipes(const char *path);
	~AnonymousPipes();
	void WriteToPipe(const char *msg);
	void ReadFromPipe(void);
};
