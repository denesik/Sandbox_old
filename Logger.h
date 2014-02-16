#ifndef Logger_h__
#define Logger_h__

#include <fstream>

//#define LOG_DEBUG_INFO
//#define LOG_DEBUG_WARNING
#define LOG_DEBUG_ERROR
#define LOG_DEBUG_FATAL

#define LOG_TIME_INFO

#ifdef LOG_DEBUG_INFO
	#define LOG_DEBUG__	
#endif

#ifdef LOG_DEBUG_WARNING
#define LOG_DEBUG__	
#endif

#ifdef LOG_DEBUG_ERROR
#define LOG_DEBUG__	
#endif

#ifdef LOG_DEBUG_FATAL
#define LOG_DEBUG__	
#endif

enum LOG_TYPE
{ 
	LOG_TYPE_INFO = 0, 
	LOG_TYPE_WARNING = 1, 
	LOG_TYPE_ERROR = 2,
	LOG_TYPE_FATAL = 3
};

#ifdef LOG_DEBUG_INFO
#define LOG_INFO(...)		Logger::getInstance().LogMessage(__FILE__, __LINE__, LOG_TYPE_INFO, ## __VA_ARGS__)
#else
#define LOG_INFO(...)		Logger::getInstance().LogMessage(LOG_TYPE_INFO, ## __VA_ARGS__)
#endif

#ifdef LOG_DEBUG_WARNING
#define LOG_WARNING(...)	Logger::getInstance().LogMessage(__FILE__, __LINE__, LOG_TYPE_WARNING, ## __VA_ARGS__)
#else
#define LOG_WARNING(...)	Logger::getInstance().LogMessage(LOG_TYPE_WARNING, ## __VA_ARGS__)
#endif

#ifdef LOG_DEBUG_ERROR
#define LOG_ERROR(...)		Logger::getInstance().LogMessage(__FILE__, __LINE__, LOG_TYPE_ERROR, ## __VA_ARGS__)
#else
#define LOG_ERROR(...)		Logger::getInstance().LogMessage(LOG_TYPE_ERROR, ## __VA_ARGS__)
#endif

#ifdef LOG_DEBUG_FATAL
#define LOG_FATAL(...)		Logger::getInstance().LogMessage(__FILE__, __LINE__, LOG_TYPE_FATAL, ## __VA_ARGS__)
#else
#define LOG_FATAL(...)		Logger::getInstance().LogMessage(LOG_TYPE_FATAL, ## __VA_ARGS__)
#endif


#define OPENGL_CHECK_ERRORS								\
	while( unsigned int openGLError = glGetError() )	\
	{													\
		LOG_ERROR("OpenGL Error 0x%X", openGLError);	\
	};


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

	std::ofstream fileLog;

protected: 
	Logger();
	Logger(const Logger&);
	Logger &operator = (Logger&);
	~Logger();

	friend class LoggerDestroyer;

public:
	static Logger &getInstance();  

#ifdef LOG_DEBUG__
public:
	void LogMessage(const char* fileName, const int numberString, LOG_TYPE logType, const char* mess, ...);
private:
	void LogMessage(const char* fileName, const int numberString, const char *mess, LOG_TYPE logType);
#endif

public:
	void LogMessage(LOG_TYPE logType, const char* mess, ...);
private:
	void LogMessage(const char *mess, LOG_TYPE logType);

};


#endif // Logger_h__