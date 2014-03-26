// AsyncIO.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <windows.h>

void CALLBACK FileWriteCompletionCallback( DWORD errCode, DWORD bytes, LPOVERLAPPED overlapped )
{
	wprintf_s( L"FILE %s Write completed. (error code : %u) \n", (wchar_t*)overlapped->hEvent, errCode );
	wprintf_s( L"Transferred bytes : %u \n", bytes );
}

const wchar_t* fileName = L"newfile.txt";
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hFile = CreateFile( fileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0 );
	// Async I/O ���� ���� ����

	if ( INVALID_HANDLE_VALUE == hFile )
	{
		return -1;
	}

	OVERLAPPED overlapped;
	memset( &overlapped, 0, sizeof( overlapped ) );
	overlapped.hEvent = (HANDLE)fileName; // �߰��� ������ �����ϴ� ���

	char* writeData = (char*)malloc( 1024 * 1024 * 100 );
	WriteFileEx( hFile, writeData, 1024 * 1024 * 100, &overlapped, FileWriteCompletionCallback );

	// ���⿡�� �ٷ� �ٸ� ���� ������
	for ( int i = 0; i < 1000; ++i )
	{
		wprintf_s( L"doing something.... \n" );
	}

	// Alertable ���·� ����� �ָ�, I/O �۾��� �Ϸ� �� �� callback �Լ��� Ŀ�ο��� ȣ���� ��
	SleepEx( INFINITE, TRUE );

	CloseHandle( hFile );
	
	getchar();
	return 0;
}

