//
//  main.c
//  BuddyBox
//
//  Created by Nicholas Robinson on 12/23/12.
//  Copyright (c) 2012 Nicholas Robinson. All rights reserved.
//

#include "BuddyBoxThread.h"

#include <signal.h>
#include <stdlib.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>

//#define PA_USE_DS 

static const unsigned int DEFAULT_SAMPLE_RATE = 124000;

static unsigned int running = 1;

void intHandler(int sig) {
    running = 0;
}

void generateOutput(PASBuddyBox *pasBB)
{
    unsigned int i, outputChannelCount;
    
    outputChannelCount = getBuddyBoxThreadInputChannelCount(pasBB);
    setBuddyBoxThreadOutputChannelCount(pasBB, outputChannelCount);
    for (i = 0; i < outputChannelCount; i++)
        setBuddyBoxThreadOutputChannelValue(pasBB, i, getBuddyBoxThreadInputChannelValue(pasBB, i));
}

void displayInput(PASBuddyBox *pasBB)
{
    unsigned int i, inputChannelCount;
    
    inputChannelCount = getBuddyBoxThreadInputChannelCount(pasBB);
    for (i = 0; i < inputChannelCount; i++)
        printf("%f\t", getBuddyBoxThreadInputChannelValue(pasBB, i));
    printf("\n");
}

int main(int argc, const char * argv[])
{
	/*
	comment 
	*/
    //signal(SIGKILL, intHandler);
    signal(SIGINT, intHandler);

    PASBuddyBox pasBB;
    
    pasBB.sampleRate = (argc > 1) ? (unsigned int) strtol(argv[1], NULL, 0) : DEFAULT_SAMPLE_RATE;
    
    initializeBuddyBoxThread(&pasBB);

	printf("Pa_GetDeviceCount = %d", Pa_GetDeviceCount());

    while(running)
    {   
        startBuddyBoxThread(&pasBB);
        
        while(running && isBuddyBoxThreadRunning(&pasBB))
        {
            generateOutput(&pasBB);
            
            if (isBuddyBoxThreadCalibrated(&pasBB))
                displayInput(&pasBB);
            
            Sleep(100000);
        }
        
        stopBuddyBoxThread(&pasBB);
        
        joinBuddyBoxThread(&pasBB);
    }
    
    cleanupBuddyBoxThread(&pasBB);
    
    printf("Program Halted...\n");
}

