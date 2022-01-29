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
		fclose(m_file);
		printf("Log::~Log()\n");
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

//-------------------------------------------------------------------
}