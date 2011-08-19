/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 2007, 2009  Peter Wortmann
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de
 *
 * Portions might be copyrighted by other authors who have contributed
 * to OpenClonk.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * See isc_license.txt for full license and disclaimer.
 *
 * "Clonk" is a registered trademark of Matthes Bender.
 * See clonk_trademark_license.txt for full license.
 */
#ifndef C4INTERACTIVETHREAD_H
#define C4INTERACTIVETHREAD_H

#include "StdScheduler.h"
#include "StdSync.h"
#include "StdWindow.h"

#include <boost/function.hpp>

// Event types
enum C4InteractiveEventType
{
	Ev_None = 0,

	Ev_Function,

	Ev_Log,
	Ev_LogSilent,
	Ev_LogFatal,

	Ev_FileChange,

	Ev_HTTP_Response,

	Ev_IRC_Message,

	Ev_Net_Conn,
	Ev_Net_Disconn,
	Ev_Net_Packet,

	Ev_Last = Ev_Net_Packet
};

class C4InteractiveThreadNotifyProc : public CStdNotifyProc
{
private:
	class C4InteractiveThread *pNotify;
public:
	void SetNotify(class C4InteractiveThread *pnNotify) { pNotify = pnNotify; }
	virtual bool Execute(int iTimeout, pollfd * readyfds);
};

// Collects StdSchedulerProc objects and executes them in a separate thread
// Provides an event queue for the procs to communicate with the main thread
class C4InteractiveThread
{
public:
	C4InteractiveThread();
	~C4InteractiveThread();

	// Event callback interface
	class Callback
	{
	public:
		virtual void OnThreadEvent(C4InteractiveEventType eEvent, void *pEventData) = 0;
		virtual ~Callback() { }
	};

private:

	// the thread itself
	StdSchedulerThread Scheduler;

	// event queue (signals to main thread)
	struct Event
	{
		C4InteractiveEventType Type;
		void *Data;
#ifdef _DEBUG
		int Time;
#endif
		Event *Next;
	};
	Event *pFirstEvent, *pLastEvent;
	CStdCSec EventPushCSec, EventPopCSec;

	// callback objects for events of special types
	Callback *pCallbacks[Ev_Last + 1];

	// proc that is added to the main thread to receive messages from our thread
	C4InteractiveThreadNotifyProc NotifyProc;

public:

	// process management
	bool AddProc(StdSchedulerProc *pProc);
	void RemoveProc(StdSchedulerProc *pProc);

	// event queue
	bool PushEvent(C4InteractiveEventType eEventType, void *pData = NULL);
	void ProcessEvents(); // by main thread

	// special events
	bool ThreadLog(const char *szMessage, ...) GNUC_FORMAT_ATTRIBUTE_O;
	bool ThreadLogFatal(const char *szMessage, ...) GNUC_FORMAT_ATTRIBUTE_O;
	bool ThreadLogS(const char *szMessage, ...) GNUC_FORMAT_ATTRIBUTE_O;

	template<typename Functor>
	bool ThreadPostAsync(Functor function)
	{
		return PushEvent(Ev_Function, new boost::function<void ()>(function));
	}

	// event handlers
	void SetCallback(C4InteractiveEventType eEvent, Callback *pnNetworkCallback)
	{ pCallbacks[eEvent] = pnNetworkCallback; }
	void ClearCallback(C4InteractiveEventType eEvent, Callback *pnNetworkCallback)
	{ if (pCallbacks[eEvent] == pnNetworkCallback) pCallbacks[eEvent] = NULL; }

private:
	bool PopEvent(C4InteractiveEventType *pEventType, void **ppData); // by main thread

};

#endif // C4INTERACTIVETHREAD_H