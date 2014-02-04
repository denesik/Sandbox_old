#ifndef Logger_h__
#define Logger_h__

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

enum LOG_TYPE{ LOG_INFO = 0, LOG_WARNING = 1, LOG_ERROR = 2 };

#define LOG(a, s) Logger::getInstance().Log(a, s)

class Logger;

class LoggerDestroyer
{
private:
	Logger* p_instance;
public:    
	~LoggerDestroyer();
	void initialize( Logger* p );
};

class Logger
{
private:
	static Logger* p_instance;
	static LoggerDestroyer destroyer;

	std::ofstream file;
	std::string fileName;

	static const char *const TAG[3];

protected: 
	Logger();
	Logger(const Logger&);
	Logger &operator = (Logger&);
	~Logger();

	friend class LoggerDestroyer;

public:
	static Logger &getInstance();  


	void Log(LOG_TYPE logType, std::string);

};


#endif // Logger_h__