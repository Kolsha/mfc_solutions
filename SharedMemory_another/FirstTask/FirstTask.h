#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CFirstTaskApp:
// Сведения о реализации этого класса: FirstTask.cpp
//

class CFirstTaskApp : public CWinApp
{
public:
	CFirstTaskApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CFirstTaskApp theApp;
