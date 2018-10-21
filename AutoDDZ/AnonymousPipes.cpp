#include "AnonymousPipes.h"

void AnonymousPipes::CreateChildProcess(const char * path)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
	int len = strlen(path);
	TCHAR *szCmdline = new char[len+1];
	memcpy(szCmdline, path, len + 1);
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 
	
	bSuccess = CreateProcess(
		NULL,          // application name
		szCmdline,          // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	 // If an error occurs, exit the application. 
	if (!bSuccess)
		printf("CreateProcess");
	else
	{
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
	}
}

AnonymousPipes::AnonymousPipes(const char *path)
{
	SECURITY_ATTRIBUTES saAttr;

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 
	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
		printf("StdoutRd CreatePipe");

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		printf("Stdout SetHandleInformation");

	// Create a pipe for the child process's STDIN. 
	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
		printf("Stdin CreatePipe");

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
		printf("Stdin SetHandleInformation");

	// Create the child process. 
	CreateChildProcess(path);
}

AnonymousPipes::~AnonymousPipes()
{
	CloseHandle(g_hChildStd_IN_Wr);
}

void AnonymousPipes::WriteToPipe(const char *msg)
// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{
	DWORD dwWritten;
	//strlen(msg)+1， 得把'/0'算进去
	if (!WriteFile(g_hChildStd_IN_Wr, msg, strlen(msg) + 1, &dwWritten, NULL)) {
		printf("Error Write");
	}
}

void AnonymousPipes::ReadFromPipe(void)
// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data.
{
	DWORD dwRead;
	char msg[BUFSIZE] = { 0 };	//初始化0必须有，不然就会烫烫烫烫烫
	if (!ReadFile(g_hChildStd_OUT_Rd, msg, BUFSIZE, &dwRead, NULL)) {
		printf("Error Read");
		return;
	}
	printf(msg);
}
