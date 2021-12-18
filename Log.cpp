#include "stdafx.h"
#include "Log.h"


#define LOGNAME "log.txt"

namespace D3D11Framework
{
//------------------------------------------------------------------

	Log* Log::m_instance = nullptr;

	Log::Log(void)
	{
		if (!m_instance)
		{
			m_instance = this;
			m_file = nullptr;
			m_init();
			Debug("Log::Log(): constructor");
		}
		else
		{
			Error("Log::Log(): the Log instance has already created");
		}
	}

	Log::~Log(void)
	{
		m_close();
		m_instance = nullptr;
		printf("Log::~Log(): desctructor\n");
	}

	void Log::m_init(void)
	{
		if (fopen_s(&m_file, LOGNAME, "w") == 0)
		{
			char timer[9];
			char date[9];

			_strtime_s(timer, 9);
			_strdate_s(date, 9);

			printf("The log file is successfully created: %s %s\n", date, timer);
			fprintf(m_file, "The log file is successfully created: %s %s\n", date, timer);
			fprintf(m_file, "-------------------------------------------\n\n");
		}
		else
		{
			m_file = nullptr;
			printf("Log::m_init(): an error occured during creation of the log file\n");
		}
	}

	void Log::m_close(void)
	{
		if (!m_file)
			return;

		char timer[9];
		char date[9];

		_strtime_s(timer, 9);
		_strdate_s(date, 9);
		
		fprintf(m_file, "--------------------------------\n");
		fprintf(m_file, "The end of the log file: %s %s\n", date, timer);

		fflush(m_file);
		fclose(m_file);
	}

	void Log::Print(const char* message, ...)
	{
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1; // +1 because of a null-character

		buffer = new(std::nothrow) char[len];

		if (buffer)
		{
			vsprintf_s(buffer, len, message, args);
			m_print("", buffer);

			delete[] buffer;
			buffer = nullptr;
		}

		va_end(args);
	}


	void Log::Debug(const char* message, ...)
	{
#ifdef _DEBUG
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1; // +1 because of a null-character

		buffer = new(std::nothrow) char[len];

		if (buffer)
		{
			vsprintf_s(buffer, len, message, args);
			m_print("*DEBUG: ", buffer);

			delete[] buffer;
			buffer = nullptr;
		}

		va_end(args);
#endif
	}


	void Log::Error(const char* message, ...)
	{
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1;	// + 1 because of a null-character

		buffer = new(std::nothrow) char[len];

		if (buffer)
		{
			vsprintf_s(buffer, len, message, args);
			m_print("*ERROR: ", buffer);

			delete[] buffer;
			buffer = nullptr;
		}
		
		va_end(args);
	}

	void Log::m_print(const char* levtext, const char* text)
	{
		clock_t cl = clock();
		char timer[9];

		_strtime_s(timer, 9);

		if (toupper(levtext[1]) == 'E')
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);
			printf("%s::%ld: %s: %s\n", timer, cl, levtext, text);
			SetConsoleTextAttribute(hConsole, 7);
		}
		else
		{
			printf("%s::%ld: %s: %s\n", timer, cl, levtext, text);
		}
		
		

		if (m_file)
		{
			fprintf(m_file, "%s::%ld: %s%s\n", timer, cl, levtext, text);
			fflush(m_file);
		}
	}



//------------------------------------------------------------------
}