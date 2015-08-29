
#include "spnQC.h"
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

using namespace std;

int32_t main (void)
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

	// Register the handler to catch exits
	signal(SIGINT, &spnHandleSignal);
	signal(SIGQUIT, &spnHandleSignal);
	signal(SIGTSTP, &spnHandleSignal);

	if(spnInit() == EXIT_SUCCESS)
	{
		// Start the minor frame timer
		setitimer (ITIMER_REAL, &timer, NULL);

		while(1);

		setitimer (ITIMER_REAL, NULL, NULL);

		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}

}

