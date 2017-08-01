//
//  RC-control
//  RC-controls system
//
//  Created by Peter Ershoff on 31/07/17.
//  Copyright (c) 2012 Peter Ershoff. All rights reserved.
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

class	RC-control
{
	private:
		PASBuddyBox			pasBB;
		
	public:
		unsigned int 		ChannelCount;
		
		MinCannelValue		float;		
		MidCannelValue		float;		
		MaxCannelValue		float;		
		
		ChannelValues		*float;

		RC-control(
					unsigned int channelCount=4,
					float min=0,
					float mid=0.5,
					float max=1.0,
					unsigned int sampleRate=DEFAULT_SAMPLE_RATE
					)
		{
			MinCannelValue=min;MidCannelValue=mid;MaxCannelValue=max;
			pasBB.sampleRate = sampleRate;
    		initializeBuddyBoxThread(&pasBB);			
			ChannelValues	= 0;
			ChannelValues	= new float [channelCount];
		};
		
		~RC-control()
		{
			if(ChannelValues != 0) delete[]	ChannelValues;
			cleanupBuddyBoxThread(&pasBB);
		};

		void	setChannelValue(int channelNumber, float channelValue)
			{ 
				if(channelValue>=MinCannelValue && channelValue<=MaxCannelValue)
					setBuddyBoxThreadOutputChannelValue(pasBB, channelNumber, ChannelValues[channelNumber]=channelValue)
			};
		void	incChannelValue(int channelNumber, float channelValueInc)
			{ 
			 float newChannelValue;
			 
			 newChannelValue = ChannelValues[channelNumber]+channelValueInc;
			 
				if(newChannelValue>=MinCannelValue && newChannelValue<=MaxCannelValue)
					setBuddyBoxThreadOutputChannelValue(pasBB, channelNumber, newChannelValue)
			};
		void	decChannelValue(int channelNumber, float channelValueDec)	incChannelValue(channelNumber, -channelValueDec);

		float	getChannelValue(int channelNumber) 							return	ChannelValues[channelNumber];
		
		void generateOutput
		{
			unsigned int i;
    
			setBuddyBoxThreadOutputChannelCount(&pasBB, ChannelCount);
			
			for (i = 0; i < ChannelCount; i++)
				setBuddyBoxThreadOutputChannelValue(pasBB, i, ChannelValues[i]);
		}
};

enum	SticksPositions
{
	ThrottleLow-YawMid-PitchMid-RollMid,
	ArmStickLow,
	ArmStickHigh,
	ThrottleLow-YawLow-PitchLow-RollHigh,
	ThrottleLow-YawHigh-PitchLow-RollHLow	
};

class	RC-control-copter : public RC-control
{
	private:
		
	public:
		int						PitchChannelNumber;
		int						RollChannelNumber;
		int						ThrottleChannelNumber;
		int						YawChannelNumber;
		int						ModeChannelNumber;
		int						ArmChannelNumber;
		enum	SticksPositions	ArmAction;
		enum	SticksPositions	DisarmAction;

		RC-control-copter(
					unsigned int			channelCount=6,
					float 					min=0,
					float 					mid=0.5,
					float 					max=1.0,
					int						PitchChannelNumber_in		=	0,
					int						RollChannelNumber_in		=	1,
					int						ThrottleChannelNumber_in	=	2,
					int						YawChannelNumber_in			=	3,
					int						ModeChannelNumber_in		=	4,
					int						ArmChannelNumber_in			=	5,
					enum	SticksPositions	ArmAction_in				=	ArmStickHigh,
					enum	SticksPositions	DisarmAction_in				=	ArmStickLow,
					unsigned int			sampleRate					=	DEFAULT_SAMPLE_RATE
					)
		{
			RC-control(channelCount,min,mid,max,sampleRate);
		};
		~RC-control-copter() {		~RC-control();	};
		
		void	DoArm();
		void	DoDisarm();
};
