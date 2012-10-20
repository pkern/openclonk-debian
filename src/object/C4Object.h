/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000  Matthes Bender
 * Copyright (c) 2001-2005, 2007  Sven Eberhardt
 * Copyright (c) 2004-2005  Peter Wortmann
 * Copyright (c) 2006-2010  Günther Brammer
 * Copyright (c) 2010  Armin Burgmeier
 * Copyright (c) 2010  Nicolas Hake
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

/* That which fills the world with life */

#ifndef INC_C4Object
#define INC_C4Object

#include "C4Facet.h"
#include "C4Id.h"
#include "C4Def.h"
#include "C4Sector.h"
#include "C4Value.h"
#include "C4Particles.h"
#include "C4PropList.h"
#include "C4ObjectPtr.h"
#include "StdMesh.h"

/* Object status */

#define C4OS_DELETED  0
#define C4OS_NORMAL   1
#define C4OS_INACTIVE 2

/* Action.Dir is the direction the object is actually facing. */

#define DIR_None  0
#define DIR_Left  0
#define DIR_Right 1

/* Action.ComDir tells an active object which way it ought to be going.
   If you set the ComDir to COMD_Stop, the object won't sit still immediately
   but will try to slow down according to it's current Action. ComDir values
   circle clockwise from COMD_Up 1 through COMD_UpLeft 8. */

#define COMD_None       -1
#define COMD_Stop       0
#define COMD_Up         1
#define COMD_UpRight    2
#define COMD_Right      3
#define COMD_DownRight  4
#define COMD_Down       5
#define COMD_DownLeft   6
#define COMD_Left       7
#define COMD_UpLeft     8

/* Visibility values tell conditions for an object's visibility */

#define VIS_All     0
#define VIS_None    1
#define VIS_Owner   2
#define VIS_Allies  4
#define VIS_Enemies 8
#define VIS_Select  16
#define VIS_God     32
#define VIS_LayerToggle 64
#define VIS_OverlayOnly 128

class C4SolidMask;
class C4Command;
class C4MaterialList;
class C4Player;

// Helper struct to serialize an object's mesh instance with other object's mesh instances attached
class C4MeshDenumerator: public StdMeshInstance::AttachedMesh::Denumerator
{
private:
	C4Def* Def; // Set if a definition mesh was attached
	C4ObjectPtr Object; // Set if an instance mesh was attached

public:
	C4MeshDenumerator(): Def(NULL), Object(NULL) {}
	C4MeshDenumerator(C4Def* def): Def(def), Object(NULL) {}
	C4MeshDenumerator(C4Object* object): Def(NULL), Object(object) {}

	virtual void CompileFunc(StdCompiler* pComp, StdMeshInstance::AttachedMesh* attach);
	virtual void DenumeratePointers(StdMeshInstance::AttachedMesh* attach);
};

class C4Action
{
public:
	C4Action();
	~C4Action();
public:
	//C4PropList * pActionDef;
	int32_t Dir;
	int32_t DrawDir; // NoSave // - needs to be calculated for old-style objects.txt anyway
	int32_t ComDir;
	int32_t Time;
	int32_t Data;
	int32_t Phase,PhaseDelay;
	int32_t t_attach; // SyncClearance-NoSave //
	C4ObjectPtr Target,Target2;
	C4Facet Facet; // NoSave //
	int32_t FacetX,FacetY; // NoSave //
	StdMeshInstance::AnimationNode* Animation; // NoSave //
public:
	void Default();
	void CompileFunc(StdCompiler *pComp);

	// BRIDGE procedure: data mask
	void SetBridgeData(int32_t iBridgeTime, bool fMoveClonk, bool fWall, int32_t iBridgeMaterial);
	void GetBridgeData(int32_t &riBridgeTime, bool &rfMoveClonk, bool &rfWall, int32_t &riBridgeMaterial);
};

class C4Object: public C4PropListNumbered
{
private:
	void UpdateInMat();
public:
	C4Object();
	~C4Object();
	C4ID id;
	int32_t RemovalDelay; // NoSave //
	int32_t Owner;
	int32_t Controller;
	int32_t LastEnergyLossCausePlayer; // last player that caused an energy loss to this Clonk (used to trace kills when player tumbles off a cliff, etc.)
	int32_t Category;
	int32_t old_x, old_y; C4LArea Area; // position as currently seen by Game.Objecets.Sectors. UpdatePos to sync.
	int32_t r;
	int32_t NoCollectDelay;
	int32_t Mass, OwnMass;
	int32_t Damage;
	int32_t Energy;
	int32_t Breath;
	int32_t InMat; // SyncClearance-NoSave //
	uint32_t Color;
	int32_t Audible, AudiblePan; // NoSave //
	int32_t PlrViewRange;
	C4Real fix_x,fix_y,fix_r; // SyncClearance-Fix //
	C4Real xdir,ydir,rdir;
	int32_t iLastAttachMovementFrame; // last frame in which Attach-movement by a SolidMask was done
	bool Mobile;
	bool Unsorted; // NoSave //
	bool Initializing; // NoSave //
	bool InLiquid;
	bool EntranceStatus;
	uint32_t t_contact; // SyncClearance-NoSave //
	uint32_t OCF;
	unsigned int Marker; // state var used by Objects::CrossCheck and C4FindObject - NoSave
	C4ObjectPtr Layer;
	C4DrawTransform *pDrawTransform; // assigned drawing transformation

	// Menu
	class C4ObjectMenu *Menu; // SyncClearance-NoSave //

	C4Facet TopFace; // NoSave //
	C4Def *Def;
	C4ObjectPtr Contained;
	C4ObjectInfo *Info;

	C4Action Action;
	C4Shape Shape;
	bool fOwnVertices; // if set, vertices aren't restored from def but from end of own vtx list
	C4TargetRect SolidMask;
	C4IDList Component;
	C4Rect PictureRect;
	C4NotifyingObjectList Contents;
	C4MaterialList *MaterialContents; // SyncClearance-NoSave //
	C4DefGraphics *pGraphics; // currently set object graphics
	StdMeshInstance* pMeshInstance; // Instance for mesh-type objects
	C4Effect *pEffects; // linked list of effects
	C4ParticleList FrontParticles, BackParticles; // lists of object local particles

	uint32_t ColorMod; // color by which the object-drawing is modulated
	uint32_t BlitMode; // extra blitting flags (like additive, ClrMod2, etc.)
	bool CrewDisabled;  // CrewMember-functionality currently disabled

	// Commands
	C4Command *Command;

	StdCopyStrBuf nInfo;

	class C4GraphicsOverlay *pGfxOverlay;  // singly linked list of overlay graphics
protected:
	bool OnFire;
	int32_t Con;
	int32_t Plane;
	bool Alive;
	C4SolidMask *pSolidMaskData; // NoSave //
public:
	void Resort();
	void SetPlane(int32_t z) { if (z) Plane = z; Resort(); }
	int32_t GetPlane() { return Plane; }
	int32_t GetAudible();
	void SetCommand(int32_t iCommand, C4Object *pTarget, C4Value iTx, int32_t iTy=0, C4Object *pTarget2=NULL, bool fControl=false, C4Value iData=C4VNull, int32_t iRetries=0, C4String *szText=NULL);
	void SetCommand(int32_t iCommand, C4Object *pTarget=NULL, int32_t iTx=0, int32_t iTy=0, C4Object *pTarget2=NULL, bool fControl=false, C4Value iData=C4VNull, int32_t iRetries=0, C4String *szText=NULL)
	{ SetCommand(iCommand, pTarget, C4VInt(iTx), iTy, pTarget2, fControl, iData, iRetries, szText); }
	bool AddCommand(int32_t iCommand, C4Object *pTarget, C4Value iTx, int32_t iTy=0, int32_t iUpdateInterval=0, C4Object *pTarget2=NULL, bool fInitEvaluation=true, C4Value iData=C4VNull, bool fAppend=false, int32_t iRetries=0, C4String *szText=NULL, int32_t iBaseMode=0);
	bool AddCommand(int32_t iCommand, C4Object *pTarget=NULL, int32_t iTx=0, int32_t iTy=0, int32_t iUpdateInterval=0, C4Object *pTarget2=NULL, bool fInitEvaluation=true, C4Value iData=C4VNull, bool fAppend=false, int32_t iRetries=0, C4String *szText=NULL, int32_t iBaseMode=0)
	{ return AddCommand(iCommand, pTarget, C4VInt(iTx), iTy, iUpdateInterval, pTarget2, fInitEvaluation, iData, fAppend, iRetries, szText, iBaseMode); }
	C4Command *FindCommand(int32_t iCommandType); // find a command of the given type
	void ClearCommand(C4Command *pUntil);
	void ClearCommands();
	void DrawSelectMark(C4TargetFacet &cgo);
	void UpdateActionFace();
	void SyncClearance();
	void SetSolidMask(int32_t iX, int32_t iY, int32_t iWdt, int32_t iHgt, int32_t iTX, int32_t iTY);
	bool CheckSolidMaskRect(); // clip bounds of SolidMask in graphics - return whether the solidmask still exists
	C4Object *ComposeContents(C4ID id);
	bool MenuCommand(const char *szCommand);

	bool ContainedControl(BYTE byCom);

	void Clear();
	void ClearInfo(C4ObjectInfo *pInfo);
	bool AssignInfo();
	bool ValidateOwner();
	bool AssignPlrViewRange();
	void DrawPicture(C4Facet &cgo, bool fSelected=false, C4RegionList *pRegions=NULL,C4DrawTransform* transform=NULL);
	void Picture2Facet(C4FacetSurface &cgo); // set picture to facet, or create facet in current size and draw if specific states are being needed
	void Default();
	bool Init(C4PropList *ndef, C4Object *pCreator,
	          int32_t owner, C4ObjectInfo *info,
	          int32_t nx, int32_t ny, int32_t nr,
	          C4Real nxdir, C4Real nydir, C4Real nrdir, int32_t iController);
	void CompileFunc(StdCompiler *pComp, C4ValueNumbers *);
	virtual void Denumerate(C4ValueNumbers *);
	void DrawLine(C4TargetFacet &cgo);
	bool SetPhase(int32_t iPhase);
	void AssignRemoval(bool fExitContents=false);
	enum DrawMode { ODM_Normal=0, ODM_Overlay=1, ODM_BaseOnly=2 };
	void Draw(C4TargetFacet &cgo, int32_t iByPlayer = -1, DrawMode eDrawMode=ODM_Normal, float offX=0, float offY=0);
	void DrawTopFace(C4TargetFacet &cgo, int32_t iByPlayer = -1, DrawMode eDrawMode=ODM_Normal, float offX=0, float offY=0);
	void DrawActionFace(C4TargetFacet &cgo, float offX, float offY);
	void DrawFace(C4TargetFacet &cgo, float offX, float offY, int32_t iPhaseX=0, int32_t iPhaseY=0);
	void DrawFaceImpl(C4TargetFacet &cgo, bool action, float fx, float fy, float fwdt, float fhgt, float tx, float ty, float twdt, float thgt, C4DrawTransform* transform);
	void Execute();
	void ClearPointers(C4Object *ptr);
	bool ExecMovement();
	void ExecAction();
	bool ExecLife();
	bool ExecuteCommand();
	void AssignDeath(bool fForced); // assigns death - if forced, it's killed even if an effect stopped this
	void ContactAction();
	void NoAttachAction();
	void DoMovement();
	void Stabilize();
	void SetOCF();
	void UpdateOCF(); // Update fluctuant OCF
	void UpdateShape(bool bUpdateVertices=true);
	void UpdatePos(); // pos/shape changed
	void UpdateSolidMask(bool fRestoreAttachedObjects);
	void UpdateMass();
	void ComponentConCutoff();
	void ComponentConGain();
	bool ChangeDef(C4ID idNew);
	void UpdateFace(bool bUpdateShape, bool fTemp=false);
	void UpdateGraphics(bool fGraphicsChanged, bool fTemp=false); // recreates solidmasks (if fGraphicsChanged), validates Color
	void UpdateFlipDir(); // applies new flipdir to draw transform matrix; creates/deletes it if necessary
	bool At(int32_t ctx, int32_t cty);
	bool At(int32_t ctx, int32_t cty, DWORD &ocf);
	void GetOCFForPos(int32_t ctx, int32_t cty, DWORD &ocf);
	bool CloseMenu(bool fForce);
	bool ActivateMenu(int32_t iMenu, int32_t iMenuSelect=0, int32_t iMenuData=0, int32_t iMenuPosition=0, C4Object *pTarget=NULL);
	int32_t ContactCheck(int32_t atx, int32_t aty, uint32_t *border_hack_contacts=0);
	bool Contact(int32_t cnat);
	void TargetBounds(C4Real &ctco, int32_t limit_low, int32_t limit_hi, int32_t cnat_low, int32_t cnat_hi);
	enum { SAC_StartCall = 1, SAC_EndCall = 2, SAC_AbortCall = 4 };
	C4PropList* GetAction() const;
	bool SetAction(C4PropList * Act, C4Object *pTarget=NULL, C4Object *pTarget2=NULL, int32_t iCalls = SAC_StartCall | SAC_AbortCall, bool fForce = false);
	bool SetActionByName(C4String * ActName, C4Object *pTarget=NULL, C4Object *pTarget2=NULL, int32_t iCalls = SAC_StartCall | SAC_AbortCall, bool fForce = false);
	bool SetActionByName(const char * szActName, C4Object *pTarget=NULL, C4Object *pTarget2=NULL, int32_t iCalls = SAC_StartCall | SAC_AbortCall, bool fForce = false);
	void SetDir(int32_t tdir);
	void SetCategory(int32_t Category) { this->Category = Category; Resort(); SetOCF(); }
	int32_t GetProcedure();
	bool Enter(C4Object *pTarget, bool fCalls=true, bool fCopyMotion=true, bool *pfRejectCollect=NULL);
	bool Exit(int32_t iX=0, int32_t iY=0, int32_t iR=0, C4Real iXDir=Fix0, C4Real iYDir=Fix0, C4Real iRDir=Fix0, bool fCalls=true);
	void CopyMotion(C4Object *from);
	void ForcePosition(C4Real tx, C4Real ty);
	void MovePosition(int32_t dx, int32_t dy);
	void MovePosition(C4Real dx, C4Real dy);
	void DoMotion(int32_t mx, int32_t my);
	bool ActivateEntrance(int32_t by_plr, C4Object *by_obj);
	bool Incinerate(int32_t iCausedBy, bool fBlasted=false, C4Object *pIncineratingObject=NULL);
	void DoDamage(int32_t iLevel, int32_t iCausedByPlr, int32_t iCause);
	void DoEnergy(int32_t iChange, bool fExact, int32_t iCause, int32_t iCausedByPlr);
	void UpdatLastEnergyLossCause(int32_t iNewCausePlr);
	void DoBreath(int32_t iChange);
	void DoCon(int32_t iChange);
	int32_t GetCon() { return Con; }
	void DoExperience(int32_t change);
	bool Promote(int32_t torank, bool exception, bool fForceRankName);
	bool Push(C4Real txdir, C4Real dforce, bool fStraighten);
	bool Lift(C4Real tydir, C4Real dforce);
	void Fling(C4Real txdir, C4Real tydir, bool fAddSpeed); // set/add given speed to current, setting jump/tumble-actions
	C4Object* CreateContents(C4PropList *);
	bool CreateContentsByList(C4IDList &idlist);
	BYTE GetArea(int32_t &aX, int32_t &aY, int32_t &aWdt, int32_t &aHgt);
	inline int32_t addtop() { return Max<int32_t>(18-Shape.Hgt,0); } // Minimum top action size for build check
	inline int32_t Left() { return GetX()+Shape.x; } // left border of shape
	inline int32_t Top() { return GetY()+Shape.y-addtop(); } // top border of shape (+build-top)
	inline int32_t Width() { return Shape.Wdt; } // width of shape
	inline int32_t Height() { return Shape.Hgt+addtop(); } // height of shape (+build-top)
	inline int32_t GetX() { return fixtoi(fix_x); }
	inline int32_t GetY() { return fixtoi(fix_y); }
	inline C4Real GetFixedX() { return fix_x; }
	inline C4Real GetFixedY() { return fix_y; }
	BYTE GetEntranceArea(int32_t &aX, int32_t &aY, int32_t &aWdt, int32_t &aHgt);
	BYTE GetMomentum(C4Real &rxdir, C4Real &rydir);
	C4Real GetSpeed();
	StdStrBuf GetDataString();
	void SetName (const char *NewName = 0);
	int32_t GetValue(C4Object *pInBase, int32_t iForPlayer);
	bool SetOwner(int32_t iOwner);
	bool SetPlrViewRange(int32_t iToRange);
	void SetOnFire(bool OnFire) { this->OnFire = OnFire; SetOCF(); }
	bool GetOnFire() { return OnFire; }
	void SetAlive(bool Alive) { this->Alive = Alive; SetOCF(); }
	bool GetAlive() { return Alive; }
	void PlrFoWActualize();
	void SetAudibilityAt(C4TargetFacet &cgo, int32_t iX, int32_t iY);
	bool IsVisible(int32_t iForPlr, bool fAsOverlay);  // return whether an object is visible for the given player
	void SetRotation(int32_t nr);
	void PrepareDrawing();  // set blit modulation and/or additive blitting
	void FinishedDrawing(); // reset any modulation
	void DrawSolidMask(C4TargetFacet &cgo);     // draw topface image only
	bool Collect(C4Object *pObj);           // add object to contents if it can be carried - no OCF and range checks are done!
	bool GrabInfo(C4Object *pFrom);         // grab info object from other object
	bool ShiftContents(bool fShiftBack, bool fDoCalls); // rotate through contents
	void DirectComContents(C4Object *pTarget, bool fDoCalls);   // direct com: scroll contents to given ID
	void GetParallaxity(int32_t *parX, int32_t *parY);
	bool GetDrawPosition(const C4TargetFacet & cgo, float & resultx, float & resulty, float & resultzoom); // converts the object's position into screen coordinates
	bool GetDrawPosition(const C4TargetFacet & cgo, float x, float y, float zoom, float & resultx, float & resulty, float & resultzoom); // converts object coordinates into screen coordinates
	bool IsInLiquidCheck();                        // returns whether the Clonk is within liquid material
	void UpdateInLiquid(); // makes splash when a liquid is entered
	void GrabContents(C4Object *pFrom); // grab all contents that don't reject it
	bool GetDragImage(C4Object **drag_object, C4ID *drag_id); // return true if object is draggable; assign drag_object/drag_id to gfx to be used for dragging

protected:
	void SideBounds(C4Real &ctcox);       // apply bounds at side; regarding bourder bound and pLayer
	void VerticalBounds(C4Real &ctcoy);   // apply bounds at top and bottom; regarding border bound and pLayer

public:
	void BoundsCheck(C4Real &ctcox, C4Real &ctcoy) // do bound checks, correcting target positions as necessary and doing contact-calls
	{ SideBounds(ctcox); VerticalBounds(ctcoy); }

	bool DoSelect(); // cursor callback if not disabled
	void UnSelect(); // unselect callback
	void GetViewPos(float &riX, float &riY, float tx, float ty, const C4Facet &fctViewport)       // get position this object is seen at (for given scroll)
	{ if (Category & C4D_Parallax) GetViewPosPar(riX, riY, tx, ty, fctViewport); else { riX=float(GetX()); riY=float(GetY()); } }
	void GetViewPosPar(float &riX, float &riY, float tx, float ty, const C4Facet &fctViewport);   // get position this object is seen at, calculating parallaxity
	bool PutAwayUnusedObject(C4Object *pToMakeRoomForObject); // either directly put the least-needed object away, or add a command to do it - return whether successful

	C4DefGraphics *GetGraphics() { return pGraphics; } // return current object graphics
	bool SetGraphics(const char *szGraphicsName=NULL, C4Def *pSourceDef=NULL);      // set used graphics for object; if szGraphicsName or *szGraphicsName are NULL, the default graphics of the given def are used; pSourceDef defaults to own def
	bool SetGraphics(C4DefGraphics *pNewGfx, bool fUpdateData);      // set used graphics for object

	class C4GraphicsOverlay *GetGraphicsOverlay(int32_t iForID, bool fCreate);  // get specified gfx overlay; create if not existant and specified
	bool RemoveGraphicsOverlay(int32_t iOverlayID);                             // remove specified overlay from the overlay list; return if found
	bool HasGraphicsOverlayRecursion(const C4Object *pCheckObj) const; // returns whether, at any overlay recursion depth, the given object appears as an MODE_Object-overlay
	void UpdateScriptPointers(); // update ptrs to C4AulScript *

	bool StatusActivate();   // put into active list
	bool StatusDeactivate(bool fClearPointers); // put into inactive list

	void ClearContentsAndContained(bool fDoCalls=true); // exit from container and eject contents (doing calbacks)

	bool AdjustWalkRotation(int32_t iRangeX, int32_t iRangeY, int32_t iSpeed);

	StdStrBuf GetInfoString(); // return def desc plus effects

	bool CanConcatPictureWith(C4Object *pOtherObject); // return whether this object should be grouped with the other in activation lists, contents list, etc.

	bool IsMoveableBySolidMask(int ComparisonPlane)
	{
		//C4PropList* pActionDef = GetAction();
		return (Status == C4OS_NORMAL)
		       && !(Category & C4D_StaticBack)
		       && (ComparisonPlane < GetPlane())
		       && !Contained
		       ;
	}

	StdStrBuf GetNeededMatStr();

	// This function is used for:
	// -Objects to be removed when a player is removed
	// -Objects that are not to be saved in "SaveScenario"-mode
	bool IsPlayerObject(int32_t iPlayerNumber=NO_OWNER);// true for any object that belongs to any player (NO_OWNER) or a specified player

	// This function is used for:
	// -Objects that are not to be saved in "SaveScenario"-mode
	bool IsUserPlayerObject();// true for any object that belongs to any player (NO_OWNER) or a specified player

	// overloaded from C4PropList
	virtual C4Object * GetObject() { return this; }
	virtual void SetPropertyByS(C4String * k, const C4Value & to);
	virtual void ResetProperty(C4String * k);
	virtual bool GetPropertyByS(C4String *k, C4Value *pResult) const;
	virtual C4ValueArray * GetProperties() const;
};

#endif