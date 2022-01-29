#pragma once

namespace D3D11Framework
{
//-------------------------------------------------------------------

	class Log
	{
	public:
		Log(void);
		~Log(void);

		void Print(const char* message, ...);
		void Debug(const char* message, ...);
		void Error(const char* message, ...);

		static Log* Get() { return m_instance; }

	private:
		static Log* m_instance;

		void m_init(void);
		void m_close(void);
		void m_print(const char* levtext, const char* text);

		FILE* m_file;
	};

//-------------------------------------------------------------------
}