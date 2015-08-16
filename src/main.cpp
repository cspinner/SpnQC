
#include "spnQC.h"
#include <sys/time.h>
#include <signal.h>

using namespace std;

int main (void)
{
	spnStatusSet(spnInit());

	if(spnStatusGet() == SUCCESS)
	{
		 struct sigaction sa = { 0 };
		 struct itimerval timer = { 0 };

		 // Set the minor frame timer
	 	 sa.sa_handler = &spnHandleSignal;
	 	 sigaction (SIGALRM, &sa, NULL);
		 timer.it_value.tv_sec = 0;
		 timer.it_value.tv_usec = MINOR_FRAME_TIME_USEC;
		 timer.it_interval.tv_sec = 0;
		 timer.it_interval.tv_usec = MINOR_FRAME_TIME_USEC;
		 setitimer (ITIMER_REAL, &timer, NULL);

		 // Register the handler to catch exits
		 signal(SIGINT, &spnHandleSignal);
		 signal(SIGQUIT, &spnHandleSignal);
		 signal(SIGTSTP, &spnHandleSignal);

		 // Transition to Run
		 spnModeSet(MODE_RUN_E);

		 while(spnStatusGet() == SUCCESS);
	}

	setitimer (ITIMER_REAL, NULL, NULL);
    return 0;

}

