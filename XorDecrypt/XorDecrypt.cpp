// XorDecrypt.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

struct Info {
	BYTE size;
	BYTE instructions[16];
	BYTE trash;
} Info;

int main()
{
	struct Info a;
	DWORD dwBytesRead, dwBytesWrite;

	HANDLE file = CreateFileA("crypted_instructions.bin",GENERIC_READ,TRUE,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE fileResult = CreateFileA("decrypted.bin", GENERIC_WRITE, TRUE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE || fileResult == INVALID_HANDLE_VALUE) {
		ExitProcess(-1);
	}

	do {
		a = {};

		BOOL r = ReadFile(file, &a.size, 1, &dwBytesRead, NULL);
		if (!r)
			printf("error: 0x%08X\r\n", GetLastError());
		a.size ^= 0x63;

		ReadFile(file, &a.instructions, a.size, &dwBytesRead, NULL);
		for (unsigned int i = 0; i < a.size;i++) {
			a.instructions[i] ^= 0x63;
		}

		ReadFile(file, &a.trash, 1, &dwBytesRead, NULL);
		WriteFile(fileResult, &a.instructions, a.size, &dwBytesWrite, NULL);
	} while (dwBytesRead != 0);

	FlushFileBuffers(fileResult);
	CloseHandle(file);
	CloseHandle(fileResult);

	ExitProcess(0);
    return 0;
}

