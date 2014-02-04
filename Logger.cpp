#include "Logger.h"


Logger * Logger::p_instance = 0;
LoggerDestroyer Logger::destroyer;

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

const char *const Logger::TAG[] = 
{
	"INFO:    ",
	"WARNING: ",
	"ERROR:   "
};

Logger::Logger()
{
	fileName = "log.txt";
	file.open(fileName.c_str());
}

Logger::~Logger()
{
	file.close();
}

void Logger::Log( LOG_TYPE logType, std::string str)
{
	file << TAG[logType] << str << "\n";

	if(logType == LOG_ERROR)
	{
		exit(0);
	}
}
