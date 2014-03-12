#ifndef FPSCounter_h__
#define FPSCounter_h__

#include <list>

class FPSCounter
{
private:
	std::list<double> fpsStack;
	double currentTime;
	double lastTime;

	double fpsTime;

public:
	FPSCounter(void);
	~FPSCounter(void);

	void Update();

	unsigned int GetCount();

};


#endif // FPSCounter_h__