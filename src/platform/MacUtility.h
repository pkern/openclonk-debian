/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000  Matthes Bender
 * Copyright (c) 2004, 2007  G√É¬ºnther Brammer
 * Copyright (c) 2008  Peter Wortmann
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

#ifdef __APPLE__
namespace MacUtility
{
	bool isGerman();
	bool sendFileToTrash(const char* szFilename);
	void restart(char*[]);
	int keyRepeatDelay(int defaultValue);
	int keyRepeatInterval(int defaultValue);

	void ensureWindowInFront();
}
#endif