/*
Communicates with the kernel driver to hide the current process using DKOM

Compile:
>cl /EHsc TestDriverApp.cpp drvutil.c advapi32.lib

Tested with:
Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 15.00.30729.01 for 80x86
*/

#include <windows.h>

#include <stdio.h>
#include <string.h>

#include "drvutil.h"

#define IOCTL_TESTDRIVER_HIDE_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_ANY_ACCESS)

#define     DRIVER_NAME     "TestDriver"
//#define     FILE_NAME       "C:\\utils\\TestDriver.sys"
#define 	FILE_NAME		"c:\\Users\\liv\\Desktop\\TestDriver.sys"

const char userLink[] = ("\\\\.\\ProcHider");
HANDLE hProcHider = NULL;

int main(int argc, char* argv[]) {
	DWORD BytesReturned = 0;
		
	if ((argc != 2) || (strlen(argv[1]) > 7)) {
		printf("Invalid pid specified\n");
		return 1;
	} else {
		printf("Hiding PID %s\n", argv[1]);
	}    
    
    //
	//	Register the driver as a system service
	//
	if(!RegisterDriver(DRIVER_NAME, FILE_NAME))	{
		printf("[%x] Failed to install %s\n", GetLastError(), DRIVER_NAME);		
		return 0;
	}
    
    //
	//	Start the driver!
	//
	printf("Starting %s.sys\n", DRIVER_NAME);

	if(!StartDriver(DRIVER_NAME)) {
		printf("[%x] Failed to start %s\n", GetLastError(), DRIVER_NAME);
		UnregisterDriver(DRIVER_NAME);			
		return 0;
	}
    
	hProcHider = CreateFile( userLink, GENERIC_ALL, 
		FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL);

	if(NULL == hProcHider) {
		printf("Error opening ProcHider device handle: 0x%08X\n", GetLastError());
		return 0;
	}

	if(DeviceIoControl(hProcHider, IOCTL_TESTDRIVER_HIDE_PROCESS, 
		argv[1],            // input data -> PID to hide
		sizeof(argv[1]),    // size of input data
		NULL,               // output data
		0,                  // size of output data
		&BytesReturned, NULL )) {
		printf("Succesfully called IOCTL_TESTDRIVER_HIDE_PROCESS\n"
			"Check Task Manager and verify if process is hidden\n");
	}
	else {
		printf("Error calling IOCTL_TESTDRIVER_HIDE_PROCESS: 0x%08X\n",GetLastError());
	}


    //
	//	Cleanup when we are finished 
	//
    CloseHandle(hProcHider);
	printf("Unloading %s.sys\n", DRIVER_NAME);
	StopDriver(DRIVER_NAME);
	UnregisterDriver(DRIVER_NAME);


	return 0;
}
