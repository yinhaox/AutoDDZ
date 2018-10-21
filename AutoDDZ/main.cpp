#include <iostream>
#include <string>
#include "AnonymousPipes.h"
using namespace std;

int main() {
	//char path[3][360] = { "C:\\Users\\yinha\\source\\repos\\AutoDDZ\\AutoDDZ\\DDZAI20150406E.exe","C:\\Users\\yinha\\source\\repos\\AutoDDZ\\AutoDDZ\\DDZAI20161023A.exe","" };
	//"DOUDIZHUVER 1.0\n";
	char msg[BUFSIZE];
	char path[] = "C:\\Users\\yinha\\source\\repos\\AutoDDZ\\AutoDDZ\\DDZAI20150406E.exe";
	AnonymousPipes pipe(path);
	sprintf_s(msg, "%s\n", "DOUDIZHUVER 1.0");
	pipe.WriteToPipe(msg);
	pipe.ReadFromPipe();
	return 0;
}