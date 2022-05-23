//
//////////////////////////////////////////////////////////////////
//
//  Windows Mbox Viewer is a free tool to view, search and print mbox mail archives.
//
// Source code and executable can be downloaded from
//  https://sourceforge.net/projects/mbox-viewer/  and
//  https://github.com/eneam/mboxviewer
//
//  Copyright(C) 2019  Enea Mansutti, Zbigniew Minciel
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the version 3 of GNU Affero General Public License
//  as published by the Free Software Foundation; 
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the
//  Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
//  Boston, MA  02110 - 1301, USA.
//
//////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "eh.h"
#include "FileUtils.h"
#include "SimpleString.h"
#include <dbghelp.h>
#include "StackWalker.h"
#include "ExceptionUtil.h"


void __cdecl trans_func(unsigned int u, EXCEPTION_POINTERS* ep)
{
	char const*  szCause = seDescription(u);
	BOOL ret = DumpStack(nullptr, "TranslationSystemE_StackTrace.txt", (TCHAR*)szCause, u, ep->ContextRecord);

	throw SE_Exception(u);
}

#ifdef USE_STACK_WALKER

SimpleString *MyStackWalker::stackWalkerBuffer = 0;


MyStackWalker::MyStackWalker(int options, LPCSTR szSymPath, DWORD  dwProcessId, HANDLE hProcess) :
	StackWalker(options, szSymPath, dwProcessId, hProcess)
{
	if (stackWalkerBuffer == 0)
	{
		stackWalkerBuffer = new SimpleString(20000);
	}

	if (stackWalkerBuffer)
	{
		MyStackWalker::stackWalkerBuffer->Clear();
	}
};

MyStackWalker::~MyStackWalker()
{
	if (stackWalkerBuffer)
	{
		SimpleString *swb = MyStackWalker::stackWalkerBuffer;
		MyStackWalker::stackWalkerBuffer = 0;
		delete swb;
	}
};

SimpleString *MyStackWalker::GetBuffer()
{
	return stackWalkerBuffer;
}

void MyStackWalker::ClearBuffer()
{
	if (stackWalkerBuffer)
		stackWalkerBuffer->SetCount(0);
}

void MyStackWalker::OnOutput(LPCSTR szText)
{
	if (stackWalkerBuffer)
		stackWalkerBuffer->Append((char*)szText);
};

#endif

BOOL GetProgramDir(CString &progDir)
{
	progDir.Empty();

	char *pProgPath = 0;
	errno_t  er = _get_pgmptr(&pProgPath);
	if (pProgPath && (er == 0))
	{
		BOOL ret = FileUtils::CPathGetPath(pProgPath, progDir);
		return ret;
	}
	else
		return FALSE;
}

BOOL CreateDumpFilePath(char *fileName, CString &filePath)
{
	filePath.Empty();

	char *pProgPath = 0;
	errno_t  er = _get_pgmptr(&pProgPath);
	if (pProgPath && (er == 0))
	{
		CString fileDir;
		//BOOL ret = FileUtils::CPathGetPath(pProgPath, fileDir);

		char *noExtension_ProgPath = new char[strlen(pProgPath) + 1];
		strcpy(noExtension_ProgPath, pProgPath);
		PathRemoveExtension(noExtension_ProgPath);
		filePath.Append(noExtension_ProgPath);
		delete noExtension_ProgPath;

		filePath.Append("_");
		filePath.Append(fileName);
	}
	if (filePath.IsEmpty())
	{
		// Should never be here but -:)
		filePath.Append("C:\\");
		filePath.Append(fileName);
	}
	return TRUE;
}

#ifdef USE_STACK_WALKER

BOOL DumpStack(MyStackWalker *sw, char *fileName, const char *seText, UINT seNumb, PCONTEXT ContextRecord, int mailPosition)
{
	BOOL ret = TRUE;

	int optionsAll = 
		  StackWalker::RetrieveSymbol      // Try to get the symbol-name
		| StackWalker::RetrieveLine        // Try to get the line for this symbol
		| StackWalker::RetrieveModuleInfo  // Try to retrieve the module-infos
		| StackWalker::RetrieveFileVersion // Also retrieve the version for the DLL/EXE
		| StackWalker::SymBuildPath        // Generate a "good" symbol-search-path
		| StackWalker::SymUseSymSrv        // Also use the public Microsoft-Symbol-Server
		; 

	int options =
		StackWalker::RetrieveSymbol      // Try to get the symbol-name
		| StackWalker::RetrieveLine        // Try to get the line for this symbol
		//| StackWalker::RetrieveModuleInfo  // Try to retrieve the module-infos
		//| StackWalker::RetrieveFileVersion // Also retrieve the version for the DLL/EXE
		| StackWalker::SymBuildPath        // Generate a "good" symbol-search-path
		//| StackWalker::SymUseSymSrv        // Also use the public Microsoft-Symbol-Server
		;

	if (sw == nullptr)
	{
		sw = new MyStackWalker(options);
	}
	
	if (sw == nullptr)
		return FALSE;

	sw->ShowCallstack(GetCurrentThread(), ContextRecord);

	SimpleString *buffer = MyStackWalker::GetBuffer();
	if (buffer)
	{
		CString filePath;
		BOOL retC = CreateDumpFilePath(fileName, filePath);

		CFile fp;
		CFileException ExError;
		if (!fp.Open(filePath, CFile::modeWrite | CFile::modeCreate, &ExError))
		{
#if 0
			CString txt = _T("Could not create Stack Dump File\"") + procFullPath;
			txt += _T("\" file.\n");
			HWND h = NULL; // we don't have any window yet
			int answer = ::MessageBox(h, txt, _T("Error"), MB_APPLMODAL | MB_ICONERROR | MB_OK);
#endif
			ret = FALSE;
		}
		else
		{
			CString tmp = CTime::GetCurrentTime().Format(_T("%A, %B %d, %Y %H:%M:%S\n"));
			fp.Write(tmp, tmp.GetLength());

			tmp.Format(_T("Exception: Code %8.8x Description: "), seNumb);
			fp.Write(tmp, tmp.GetLength());

			fp.Write(seText, strlen(seText));

			tmp.Format(_T("\nMail Position: %d\n"), mailPosition);
			fp.Write(tmp, tmp.GetLength());

			fp.Write("\n\n", 2);

			fp.Write(buffer->Data(), buffer->Count());
			fp.Close();
			ret = TRUE;
		}
		buffer->Clear();
	}
	else
		ret = FALSE;

	return ret;
}


BOOL DumpStackEx(char *fileName, CException* e)
{
	TCHAR szCause[256];
	UINT seNumber = 0;
	e->GetErrorMessage(szCause, 255, &seNumber);

	BOOL ret = DumpStack(nullptr, fileName, szCause, seNumber);
	return ret;
}
#else

BOOL DumpStack(MyStackWalker *sw, char *fileName, const char *seText, UINT seNumb, PCONTEXT ContextRecord, int mailPosition)
{
	return TRUE;
}

BOOL DumpStackEx(char *fileName, CException* e)
{
	return TRUE;
}

#endif

const char* seDescription(const int code)
{
	switch (code) {
	case EXCEPTION_ACCESS_VIOLATION:         return "ACCESS VIOLATION";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "ARRAY BOUNDS EXCEEDED";
	case EXCEPTION_BREAKPOINT:               return "BREAKPOINT";
	case EXCEPTION_DATATYPE_MISALIGNMENT:    return "DATATYPE MISALIGNMENT";
	case EXCEPTION_FLT_DENORMAL_OPERAND:     return "FLT DENORMAL OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "FLT DIVIDE BY ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT:       return "FLT INEXACT RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION:    return "FLT INVALID OPERATION";
	case EXCEPTION_FLT_OVERFLOW:             return "FLT OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK:          return "FLT STACK CHECK";
	case EXCEPTION_FLT_UNDERFLOW:            return "FLT UNDERFLOW";
	case EXCEPTION_ILLEGAL_INSTRUCTION:      return "ILLEGAL INSTRUCTION";
	case EXCEPTION_IN_PAGE_ERROR:            return "IN PAGE ERROR";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "INT DIVIDE BY ZERO";
	case EXCEPTION_INT_OVERFLOW:             return "INT OVERFLOW";
	case EXCEPTION_INVALID_DISPOSITION:      return "INVALID DISPOSITION";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "NONCONTINUABLE EXCEPTION";
	case EXCEPTION_PRIV_INSTRUCTION:         return "PRIV INSTRUCTION";
	case EXCEPTION_SINGLE_STEP:              return "SINGLE STEP";
	case EXCEPTION_STACK_OVERFLOW:           return "STACK OVERFLOW";
	default: return "UNKNOWN";
	}
}

BOOL DumpMailData(char *fileName, const char *seText, UINT seNumb, int mailPosition, char *data, int datalen)
{
	BOOL ret = TRUE;

	CString filePath;
	BOOL retC = CreateDumpFilePath(fileName, filePath);

	CFile fp;
	CFileException ExError;
	if (!fp.Open(filePath, CFile::modeWrite | CFile::modeCreate, &ExError))
	{
		CString exErrorStr = FileUtils::GetFileExceptionErrorAsString(ExError);

		CString txt = _T("Could not create \"") + filePath;
		txt += _T("\" file.\n");
		txt += exErrorStr;

		TRACE(_T("%s\n"), txt);
		ret = FALSE;
	}
	else
	{
		CString tmp = CTime::GetCurrentTime().Format(_T("%A, %B %d, %Y %H:%M:%S\n"));
		fp.Write(tmp, tmp.GetLength());

		tmp.Format(_T("Exception: Code %8.8x Description: "), seNumb);
		fp.Write(tmp, tmp.GetLength());

		fp.Write(seText, uistrlen(seText));

		tmp.Format(_T("\nMail Position: %d\n"), mailPosition);
		fp.Write(tmp, tmp.GetLength());

		fp.Write("\n\n", 2);

		fp.Write(data, datalen);
		fp.Write("\n", 1);

		fp.Close();
		ret = TRUE;
	}
	return ret;
}