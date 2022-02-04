#include "stdafx.h"
#include "Log.h"

#define FILENAME "log.txt"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	Log* Log::m_instance = nullptr;

	Log::Log(void)
	{
		if (!m_instance)
		{
			m_instance = this;
			m_file = nullptr;
			m_init();
			Debug("Log::Log()");
		}
		else
		{
			printf("Error: The log file has already created!\n");
		}
	}

	Log::~Log(void)
	{
		if (!m_file)
			return;

		m_close();
		fflush(m_file);
		fclose(m_file);

		printf("Log::~Log()\n");
	}

	void Log::Print(const char* message, ...)
	{
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1;

		buffer = new (std::nothrow) char[len];
		assert(buffer != nullptr);

		vsprintf_s(buffer, len, message, args);
		m_print("", buffer);

		delete[] buffer;
		buffer = nullptr;
		va_end(args);
	}

	void Log::Debug(const char* message, ...)
	{
#ifdef _DEBUG
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1;

		buffer = new (std::nothrow) char[len];
		assert(buffer != nullptr);

		vsprintf_s(buffer, len, message, args);
		m_print("*DEBUG", buffer);

		delete[] buffer;
		buffer = nullptr;
		va_end(args);
#endif
	}

	void Log::Error(const char* message, ...)
	{
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1;

		buffer = new (std::nothrow) char[len];
		assert(buffer != nullptr);

		vsprintf_s(buffer, len, message, args);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		FlushConsoleInputBuffer(hConsole);
		SetConsoleTextAttribute(hConsole, 12);
		m_print("*ERROR", buffer);
		SetConsoleTextAttribute(hConsole, 7);

		delete[] buffer;
		buffer = nullptr;
		va_end(args);
	}

	void Log::m_init(void)
	{
		if (fopen_s(&m_file, FILENAME, "w") == 0)
		{
			char date[9];
			char time[9];

			_strdate_s(date, 9);
			_strtime_s(time, 9);

			fprintf(m_file, "[%s :: %s]: The log file is successfully created\n", date, time);
			fprintf(m_file, "----------------------------------------------\n");
		}
		else
		{
			printf("ERROR: can't create the log file");
			m_file = nullptr;
		}
	}

	void Log::m_close(void)
	{
		if (!m_file)
			return;

		char date[9];
		char time[9];

		_strdate_s(date, 9);
		_strtime_s(time, 9);

		fprintf(m_file, "[%s :: %s]: The end of the log file\n", date, time);
		fprintf(m_file, "----------------------------------------------\n");
	}

	void Log::m_print(const char* levtext, const char* text)
	{
		time_t tickCount = GetTickCount();
		char time[9];

		_strtime_s(time, 9);
		printf("[%s :: %Id]: %s: %s\n", time, tickCount, levtext, text);
		
		if (m_file)
		{
			fprintf(m_file, "[%s :: %lld]: %s: %s\n", time, tickCount, levtext, text);
			fflush(m_file);
			
		}
	}

//-------------------------------------------------------------------
}