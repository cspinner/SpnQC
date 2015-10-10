ACPP=g++
AC=gcc


VPATH=../src :\
 ../src/APP :\
 ../src/APP/SUPPORT :\
 ../src/HAL/SIM :\
 ../src/OSAL/WIN

INCLUDES=-I..\src \
-I../src/APP \
-I../src/APP/SUPPORT \
-I../src/HAL \
-I../src/OSAL

AFLAGS=$(INCLUDES)

OBJS= main.o \
spnCommand.o       \
spnConfig.o        \
spnFilter.o        \
spnHandlers.o      \
spnInit.o          \
spnMode.o          \
spnMotors.o        \
spnPID.o           \
spnScheduler.o     \
spnSensorManager.o \
spnServer.o        \
spnTransceiver.o   \
spnUserInput.o     \
spnUserOutput.o    \
spnUtils.o         \
MadgwickAHRS.o     \
OSAL_SOCKETS.o \
OSAL_TIME.o \
OSAL_OUTPUT.o \
OSAL_INPUT.o \
OSAL_HANDLE.o \
OSAL.o

all: app

app: $(OBJS)
	$(OBJS)

clean:
	del *.o

%.o : %.cpp 
	$(ACPP) -c $(AFLAGS) $<
	
%.o : %.c
	$(AC) -c $(AFLAGS) $<