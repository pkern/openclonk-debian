/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000  Matthes Bender
 * Copyright (c) 2001, 2004-2005, 2007  Sven Eberhardt
 * Copyright (c) 2008, 2010  Günther Brammer
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

/* Loads all standard graphics from Graphics.ocg */

#ifndef INC_C4GraphicsResource
#define INC_C4GraphicsResource

#include <C4Group.h>
#include <C4GroupSet.h>
#include <C4Surface.h>
#include <C4FacetEx.h>
#include <C4Gui.h>

class C4GraphicsResource
{
private:
	bool fInitialized;
public:
	C4GraphicsResource();
	~C4GraphicsResource();
protected:
	C4Surface sfcControl, sfcClonkSkins;
	C4Surface sfcCaption, sfcButton, sfcButtonD;
	C4Surface sfcScroll, sfcContext;
	int32_t idSfcCaption, idSfcButton, idSfcButtonD, idSfcScroll, idSfcContext, idSfcClonkSkins;
	int32_t idSfcControl; // id of source group of control surface
	// ID of last group in main group set that was already registered into the Files-set
	// used to avoid doubled entries by subsequent calls to RegisterMainGroups
	int32_t idRegisteredMainGroupSetFiles;
public:
	C4GroupSet Files;
	float ProgressStart, ProgressIncrement;
	C4FacetID fctPlayer;
	C4FacetID fctFlag;
	C4FacetID fctCrew;
	C4FacetID fctWealth;
	C4FacetID fctRank;
	int32_t iNumRanks;
	C4FacetID fctFire;
	C4FacetID fctBackground;
	C4FacetID fctCaptain;
	C4FacetID fctMouseCursor;
	C4FacetID fctSelectMark;
	C4FacetID fctOptions;
	C4FacetID fctMenu;
	C4FacetID fctUpperBoard;
	C4FacetID fctLogo;
	C4FacetID fctConstruction;
	C4FacetID fctEnergy;
	C4FacetID fctArrow;
	C4FacetID fctExit;
	C4FacetID fctHand;
	C4FacetID fctGamepad;
	C4FacetID fctBuild;
	C4Facet fctClonkSkin;
	C4Facet fctCursor;
	C4Facet fctDropTarget;
	C4Facet fctKeyboard;
	C4Facet fctMouse;
	C4Facet fctCommand;
	C4Facet fctKey;
	C4Facet fctOKCancel;
	C4FacetID fctCrewClr; // ColorByOwner-surface of fctCrew
	C4FacetID fctFlagClr; // ColorByOwner-surface of fctFlag
	C4FacetID fctPlayerClr; // ColorByOwner-surface of fctPlayer

	C4GUI::DynBarFacet barCaption, barButton, barButtonD;
	C4FacetID fctButtonHighlight;
	C4FacetID fctButtonHighlightRound;
	C4FacetID fctIcons, fctIconsEx;
	C4FacetID fctSubmenu;
	C4FacetID fctCheckbox;
	C4FacetID fctBigArrows;
	C4FacetID fctProgressBar;
	C4GUI::ScrollBarFacets sfctScroll;
	C4Facet fctContext;
	
	// fonts
	CStdFont &CaptionFont; // small, bold font
	CStdFont &TitleFont;   // large, bold font
	CStdFont &TextFont;    // font for normal text
	CStdFont &MiniFont;    // tiny font (logfont)
	CStdFont &TooltipFont; // same as BookFont
	CStdFont FontTiny;     // used for logs
	CStdFont FontRegular;  // normal font - just refed from graphics system
	CStdFont FontCaption;  // used for title bars
	CStdFont FontTitle;    // huge font for titles
	CStdFont FontTooltip;  // normal, non-shadowed font (same as BookFont)
public:
	int32_t GetColorIndex(int32_t iColor, bool fLast=false);
	CStdFont &GetFontByHeight(int32_t iHgt, float *pfZoom=NULL); // get optimal font for given control size
	void Default();
	void Clear();
	bool InitFonts();
	void ClearFonts(); // clear fonts ()
	bool Init();

	bool IsInitialized() { return fInitialized; } // return whether any gfx are loaded (so dlgs can be shown)

	bool RegisterGlobalGraphics(); // register global Graphics.ocg into own group set
	bool RegisterMainGroups();     // register new groups of Game.GroupSet into own group set
	void CloseFiles();             // free group set

	bool ReloadResolutionDependantFiles(); // reload any files that depend on the current resolution

protected:
	bool LoadFile(C4FacetID& fct, const char *szName, C4GroupSet &rGfxSet, int32_t iWdt = C4FCT_Full, int32_t iHgt = C4FCT_Full, bool fNoWarnIfNotFound=false);
	bool LoadFile(C4Surface& sfc, const char *szName, C4GroupSet &rGfxSet, int32_t &ridCurrSfc);
	bool FindLoadRes(C4Group *pSecondFile, const char *szName);
	bool LoadCursorGfx();

	friend class C4StartupGraphics;
};

extern C4GraphicsResource GraphicsResource;
#define GfxR (&(::GraphicsResource))
#endif