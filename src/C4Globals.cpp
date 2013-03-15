/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 2009  Nicolas Hake
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

/* Global interdependent objects */

#include <C4Include.h>
#include "C4Application.h"
#include "C4Aul.h"
#include "C4Console.h"
#include <C4DefList.h>
#include "C4FullScreen.h"
#include "C4Game.h"
#include "C4GameObjects.h"
#include "C4MouseControl.h"
#include "C4Network2.h"
#include "C4PropList.h"
#include "C4StringTable.h"

#ifdef _DEBUG
C4Set<C4PropList *> C4PropList::PropLists;
#endif
C4Set<C4PropListNumbered *> C4PropListNumbered::PropLists;
int32_t C4PropListNumbered::EnumerationIndex = 0;
C4StringTable  Strings;
C4AulScriptEngine ScriptEngine;
C4Application  Application;
C4Console      Console;
C4FullScreen   FullScreen;
C4MouseControl MouseControl;
C4GameObjects  Objects;
C4DefList      Definitions;
C4Game         Game;
C4Network2     Network;