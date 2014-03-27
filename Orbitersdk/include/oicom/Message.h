#ifndef _H_ORBITER_INTERCOM_PLUGIN_MESSAGE_H_
#define _H_ORBITER_INTERCOM_PLUGIN_MESSAGE_H_

/*
      This is the standard message class. 
	  In OICOM (orbiter intercom) every message
	  has a string part, as well as a data part. 
	  The string part of the message could be used to describe
	  the raw byte data. The string part of the message must be 
	  present in all instances, otherwise the message will not be queued
	  in the message queue of the destination! The data part can be NULL
*/

// the size of the string message in bytes
#define MESSAGE_STRMSG_SIZE        500

struct OrbiterPluginMessage
{
public:
	// The string part of the message, could be a description for the data
	char strMsg[MESSAGE_STRMSG_SIZE];
	// the data and size
	void * data;
	unsigned int dataSize;
};

#endif