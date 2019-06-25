#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

void LoadDll(HANDLE process, const wchar_t* dllPath)
{
	// write the dll name to memory
	int namelen = wcslen(dllPath) + 1;
	LPVOID remoteString = VirtualAllocEx(process, NULL, namelen * 2, MEM_COMMIT, PAGE_EXECUTE);
	WriteProcessMemory(process, remoteString, dllPath, namelen * 2, NULL);

	// get the address of LoadLibraryW()
	HMODULE k32 = GetModuleHandleA("kernel32.dll");
	LPVOID funcAdr = GetProcAddress(k32, "LoadLibraryW");

	// create the thread
	HANDLE thread =
		CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)funcAdr, remoteString, NULL, NULL);

	// let the thread finish and clean up
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
}

DWORD getPIDByName(std::wstring name)
{
	DWORD PID = -1;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			std::wstring binaryPath = entry.szExeFile;
			if (binaryPath.find(name) != std::wstring::npos)
			{
				PID = entry.th32ProcessID;
				break;
			}
		}
	}

	CloseHandle(snapshot);
	return PID;
}


int main()
{
	DWORD pid = getPIDByName(L"ac_client.exe");
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (process == NULL) {
		std::cout << "Open process failed\n";
	}
	else {
		std::cout << "Sucess, pid=" << pid << '\n';
		LoadDll(process, L"D:\\Fichiers\\GameHacking\\assault_cube_hack\\ACDLL\\Debug\\ACDLL.dll");
	}

	CloseHandle(process);
    
}
