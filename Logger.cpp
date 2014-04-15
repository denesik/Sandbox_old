#include "Logger.h"
#include <stdarg.h>
#include <time.h>
#include <iomanip>

Logger * Logger::p_instance = 0;
LoggerDestroyer Logger::destroyer;

static const char *const TAG[] = 
{
	"INFO.    ",
	"WARNING. ",
	"ERROR.   ",
	"FATAL.   "
};


LoggerDestroyer::~LoggerDestroyer() 
{   
	delete p_instance; 
}

void LoggerDestroyer::initialize( Logger* p ) 
{
	p_instance = p; 
}


Logger& Logger::getInstance() 
{
	if(!p_instance)     
	{
		p_instance = new Logger();
		destroyer.initialize( p_instance);     
	}

	return *p_instance;
}

Logger::Logger()
{
	fileLog.open("log.txt");
}

Logger::~Logger()
{
	fileLog.close();
}

#ifdef LOG_DEBUG__
void Logger::LogMessage(const char* fileName, const int numberString, LOG_TYPE logType, const char* mess, ... )
{
	static char buffer[4096];
	va_list list;

	va_start(list, mess);
	vsnprintf_s(buffer, 4096, mess, list);
	va_end(list);

	LogMessage(fileName, numberString, buffer, logType);
}

void Logger::LogMessage( const char* fileName, const int numberString, const char *mess, LOG_TYPE logType )
{
#ifdef LOG_TIME_INFO
	struct tm newtime;
	time_t time_current;
	time(&time_current);
	localtime_s(&newtime, &time_current);
#endif

	fileLog	<< TAG[logType] 

#ifdef LOG_TIME_INFO
		// Выводим время
		<< "Time: "
		<< std::setw(2) << std::setfill('0') << newtime.tm_hour << ":" 
		<< std::setw(2) << std::setfill('0') << newtime.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << newtime.tm_sec << " "
#endif // LOG_TIME_INFO

		// Выводим имя файла и номер строки
		<< std::endl << "File name: " << fileName << ". Number string: " << std::setw(3) << std::setfill(' ') << numberString << std::endl
		// Выводим сообщение
		<< mess << std::endl;
	fileLog.flush();
	if(logType == LOG_TYPE_FATAL)
	{
		exit(0);
	}
}
#endif



void Logger::LogMessage( LOG_TYPE logType, const char* mess, ... )
{
	static char buffer[4096];
	va_list list;

	va_start(list, mess);
	vsnprintf_s(buffer, 4096, mess, list);
	va_end(list);

	LogMessage(buffer, logType);
}

void Logger::LogMessage( const char *mess, LOG_TYPE logType )
{
#ifdef LOG_TIME_INFO
	struct tm newtime;
	time_t time_current;
	time(&time_current);
	localtime_s(&newtime, &time_current);
#endif

	fileLog	<< TAG[logType] 

#ifdef LOG_TIME_INFO 
		// Выводим время
		<< "Time: "
		<< std::setw(2) << std::setfill('0') << newtime.tm_hour << ":" 
		<< std::setw(2) << std::setfill('0') << newtime.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << newtime.tm_sec << " "
#endif // LOG_TIME_INFO

		// Выводим сообщение
		<< mess << std::endl;
	fileLog.flush();

	if(logType == LOG_TYPE_FATAL)
	{
		exit(0);
	}
}

