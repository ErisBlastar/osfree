/*
 * Copyright (C) 1997-2000, The University of Queensland
 * Copyright (C) 2000-2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * FILE:       cfg.cc
 * OVERVIEW:   Implementation of the CFG class.
 *
 * Copyright (C) 1997-2001, The University of Queensland, BT group
 * Copyright (C) 2000-2001, Sun Microsystems, Inc
 *============================================================================*/

/*
 * $Revision: 1.69 $
 * Dec 97 - created by Mike
 * 19 Feb 98  - Cristina
 *  changed RTLLIT for RTL_CIT as this name is defined in the new rtl.h.
 * 25 Feb 98 - Cristina
 *  made RTL iterator arguments in Init() and newBB() constant iterators.
 * 3 Mar 98 - Cristina
 *  replaced ADDR for ADDRESS to conform with other tools.
 * 11 Mar 98 - Cristina  
 *  replaced BOOL for bool type (C++'s), same for TRUE and FALSE.
 * 13 Mar 98 - Cristina 
 *  added Label() and splitBB() procedures. 
 * 18 Mar 98 - Cristina 
 *  fixed Label() problem with freeing up the entry that was introduced 
 *      in the map in order to check for a BB.  When false is returned,
 *      the entry should be freed from the map.
 * 27 Mar 98 - Cristina
 *  added AddInterProcOutEdge().
 * 7 Aug 98 - Mike
 *  Changed Init() to a constructor - decided it's OK to have constructors
    that may fail
 * 13 Aug 98 - Mike
 *  Removed the ADDRESS parameter from newBB - can get it from itFirst
 * 14 Aug 98 - Mike
 *  m_listBB is a list of pointers now, rather than BBs. Needed so that
 *  changes made to this list happen to all BBs
 * 19 Aug 98 - Mike
 *  Out edges are all pointers to BBs now; added m_bIncomplete to BasicBlock
 * 11th Sep 98 - Mike
 *  Add in edges as out edges are added (can do this now that out edges are
 *  pointers to real BBs)
 * 30 Sep 98 - Mike
 *  Erase in edges of "bottom half" of BB when splitting. Also fixed an
 *  obscure bug whereby "splitting" incomplete BBs resulted in the
 *  m_itFirstRtl iterator being assigned a value when it needn't have
 *  (and caused problems later)
 * 08 Oct 98 - Mike
 *  Corrected in-edge logic of splitBB() where a new BB overlaps an
 *  existing but incomplete BB
 * 25 Nov 98 - Mike: Added writeCfgFile()
 * 10 Dec 98 - Mike: writeCfgFile() and writeBBFile()
 * 12 Dec 98 - Mike: AddCoverage()
 * 22 Dec 98 - Mike: itFirstRtl etc are not const now
 * 13 Jan 99 - Mike: Fixed a memory problem with a locally allocated BB
 *                  in writeCfgFile. Thanks, Purify!
 * 14 Jan 99 - Mike: Added a cast to ensure that edge indices come out as
 *  decimal, not hex (in writeBBFile())
 * 22 Jan 99 - Mike: Replaced m_it[First,Last]Rtl with m_pRtls
 * 25 Jan 99 - Mike: Fixed bug in splitBB where lower BB had the wrong RTLs
 *                      deleted
 * 27 Jan 99 - Mike: Use COMPJUMP and COMPCALL now
 * 04 Feb 99 - Mike: Added GetInEdges()
 * 30 Mar 99 - Mike: splitBB now fixes in-edges of descendants of the new BB
 * 30 Mar 99 - Mike: Added sortBBs()
 * 15 Apr 99 - Mike: Restored fix for putting NOPs into all BBs. Needed when
 *                      they are the destination of jumps
 * 27 Apr 99 - Doug: Added liveness sets for analysis.
 * 28 Apr 99 - Doug: Removed AddInEdges (was enclosed in #if 0)
 * 29 Apr 99 - Mike: Added delete_lrtls() to deep delete these objects; also
 *                   added erase_lrtls()
 * 29 Apr 99 - Mike: Label() now doesn't finalise a label, it merely checks
 *                      whether the address is already decoded in some BB.
 *                      Also takes an extra parameter
 *                      Also fixed delete logic in splitBB()
 * 13 May 99 - Doug: Moved BB code into basicblock.cc
 * 14 May 99 - Mike: The AddOutEdge() functions take a bool to say whether
 *                      to set a label there
 * 16 May 99 - Doug: Changed all code that set the RTLs for a BB to use setRTLs
 * 02 Jun 99 - Mike: Removed leading upper case on function names
 * 25 Jun 99 - Mike: splitBB now respects pNewBB->m_bHasLabel
 * 28 Jun 99 - Mike: Added lastLabel and setLabel()
 * 21 Jul 99 - Mike: WellFormCFG checks in-edges against out edges, and vice
 *                      versa
 * 21 Jul 99 - Mike: Fixed a bug in splitBB() that was causing some BBs to get
 *                   an extraneous in-edge
 * 13 Sep 99 - Mike: isLabel returns true now if an incomplete BB there; use
 *                      isInComplete() to tell whether the BB is complete
 * 15 Mar 00 - Cristina: Cfg::setAFP transformed into setAXP. 
 * 18 Apr 00 - Mike: Added checkTailCall()
 * 19 May 00 - Mike: When adding a NEW outedge (with addNewOutEdge), set the
 *                      m_bJumpReqd flag, so a jump will be generated
                        Also moved from BasicBlock class to Cfg class
 * 21 Jun 00 - Mike: Removed checkTailCall (just not a good idea)
 * 29 Jun 00 - Mike: Cleaned up compressCFG(): was deleting needed BBs;
 *                      also set the JumpReqd bit and set label if needed
 * 18 Jul 00 - Mike: Check entryBB for being NULL
 * 07 Sep 00 - Mike: getUseDefLocations takes a Proc* now, so we can register
 *              the use of the return location
 * 28 Sep 00 - Mike: joinBB no longer has a third parameter (bbit)
 * 31 Jul 01 - Brian: New class HRTL replaces RTlist. Renamed LRTL to HRTLList,
 *              getLrtls to getHRTLs, setRTLs to setHRTLs,
 *              and RTL_IT to HRTLList_IT.
 */


/*==============================================================================
 * Dependencies.
 *============================================================================*/

#include "global.h"
#include "ss.h"
#include "cfg.h"
#include "rtl.h"
#include "csr.h"
#include "prog.h"           // For findProc()
#include "proc.h"           // For Proc::setTailCaller()

// Prototype this procedure, to avoid dependency problems
//void Proc::setReturnType(const SemStr& retLoc, const ReturnLocations& retSpec);

void delete_lrtls(HRTLList* pLrtl);
void erase_lrtls(HRTLList* pLrtl, HRTLList_IT begin, HRTLList_IT end);

/**********************************
 * Cfg methods.
 **********************************/

/*==============================================================================
 * FUNCTION:        Cfg::Cfg
 * OVERVIEW:        
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
Cfg::Cfg()
    :   m_bWellFormed(false), m_uExtraCover(0), lastLabel(0)
{}

/*==============================================================================
 * FUNCTION:        Cfg::~Cfg
 * OVERVIEW:        Destructor. Note: destructs the component BBs as well
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
Cfg::~Cfg()
{
    // Delete the BBs
    BB_CIT it;
    for (it = m_listBB.begin(); it != m_listBB.end(); it++)
        if (*it)
        {
            delete *it;
        }
}

/*==============================================================================
 * FUNCTION:        Cfg::setProc
 * OVERVIEW:        Set the pointer to the owning UserProc object
 * PARAMETERS:      proc - pointer to the owning UserProc object
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::setProc(UserProc* proc)
{
    myProc = proc;
}

/*==============================================================================
 * FUNCTION:        Cfg::operator=
 * OVERVIEW:        
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
const Cfg& Cfg::operator=(const Cfg& other)
{
    m_listBB = other.m_listBB;
    m_mapBB = other.m_mapBB;
    m_bWellFormed = other.m_bWellFormed;
    return *this;
}

/*==============================================================================
 * FUNCTION:        checkEntryBB
 * OVERVIEW:        Check the entry BB pointer; if zero, emit error message
 *                    and return true
 * PARAMETERS:      <none>
 * RETURNS:         true if was null
 *============================================================================*/
bool Cfg::checkEntryBB() const
{
    if (entryBB == NULL) {
        ostrstream ost;
        ost << "No entry BB for ";
        if (myProc)
            ost << myProc->getName();
        else
            ost << "unknown proc";
        error(str(ost));
        return true;
    }
    return false;
}

/*==============================================================================
 * FUNCTION:        Cfg::newBB
 * OVERVIEW:        Add a new basic block to this cfg 
 * PARAMETERS:      pRtls: list of pointers to RTLs to initialise the BB with
 *                  bbType: the type of the BB (e.g. TWOWAY)
 *                  iNumOutEdges: number of out edges this BB will eventually
 *                    have
 * RETURNS:         Pointer to the newly created BB, or 0 if there is already
 *                    an incomplete BB with the same address
 *============================================================================*/
PBB Cfg::newBB(HRTLList* pRtls, BBTYPE bbType, int iNumOutEdges)
{
    MAPBB::iterator mi;
    PBB pBB;

    // First find the native address of the first RTL
    // Can't use BasicBlock::GetLowAddr(), since we don't yet have a BB!
    ADDRESS addr = pRtls->front()->getAddress();
    // If this is zero, try the next RTL (only). This may be necessary
    // if e.g. there is a BB with a delayed branch only, with its delay
    // instruction moved in front of it (with 0 address).
    // Note: it is possible to see two RTLs with zero address with
    // Sparc: jmpl %o0, %o1. There will be one for the delay instr (if
    // not a NOP), and one for the side effect of copying %o7 to %o1.
    // Note that orphaned BBs (for which we must compute addr here to
    // to be 0) must not be added to the map, but they have no RTLs with
    // a non zero address.
    if ((addr == 0) && (pRtls->size() > 1))
    {
        HRTLList_IT next = pRtls->begin();
        addr = (*++next)->getAddress();
    }

    // If this addr is non zero, check the map to see if we have a
    // (possibly incomplete) BB here already
    // If it is zero, this is a special BB for handling delayed
    // branches or the like
    bool bDone = false;
    if (addr != 0)
    {
        mi = m_mapBB.find(addr);
        if (mi != m_mapBB.end() && (*mi).second)
        {
            pBB = (*mi).second;
            // It should be incomplete, or the pBB there should be zero
            // (we have called Label but not yet created the BB for it).
            // Else we have duplicated BBs. Note: this can happen with
            // forward jumps into the middle of a loop, so not error
            if (!pBB->m_bIncomplete)
            {
                // This list of RTLs is not needed now
                delete_lrtls(pRtls);
                return 0;
            }
            else
            {
                // Fill in the details, and return it
                pBB->setHRTLs(pRtls);
                pBB->m_nodeType = bbType;
                pBB->m_iNumOutEdges = iNumOutEdges;
                pBB->m_bIncomplete = false;
            }
            bDone = true;
        }
    }
    if (!bDone)
    {
        // Else add a new BB to the back of the current list.
        pBB = new BasicBlock(pRtls, bbType, iNumOutEdges);
        m_listBB.push_back(pBB);

        // Also add the address to the map from native (source) address to
        // pointer to BB, unless it's zero
        if (addr != 0)
        {
            m_mapBB[addr] = pBB;            // Insert the mapping
            mi = m_mapBB.find(addr);
        }
    }

    if (addr != 0 && (mi != m_mapBB.end()))
    {
        // Existing New         +---+ Top of new
        //          +---+       +---+
        //  +---+   |   |       +---+ Fall through
        //  |   |   |   | =>    |   |
        //  |   |   |   |       |   | Existing; rest of new discarded
        //  +---+   +---+       +---+
        //  
        // Check for overlap of the just added BB with the next BB
        // (address wise). If there is an overlap, truncate the HRTLList for
        // the new BB to not overlap, and make this a fall through BB
        // We still want to do this even if the new BB overlaps with an
        // incomplete BB, though in this case, splitBB needs to fill in
        // the details for the "bottom" BB of the split. Also, in this
        // case, we return a pointer to the newly completed BB, so it
        // will get out edges added (if required). In the other case
        // (i.e. we overlap with an exising, completed BB), we want to
        // return 0, since the out edges are already created.
        if (++mi != m_mapBB.end())
        {
            PBB pNextBB = (*mi).second;
            ADDRESS uNext = (*mi).first;
            bool bIncomplete = pNextBB->m_bIncomplete;
            if (uNext <= pRtls->back()->getAddress())
            {
                // Need to truncate the current BB. We use splitBB(), but
                // pass it pNextBB so it doesn't create a new BB for the
                // "bottom" BB of the split pair
                splitBB(pBB, uNext, pNextBB);
                // If the overlapped BB was incomplete, return the
                // "bottom" part of the BB, so adding out edges will work
                // properly.
                if (bIncomplete)
                {
                    return pNextBB;
                }
                // However, if the overlapping BB was already
                // complete, return 0, so out edges won't be added twice
                else return 0;
            }
        }

        // Existing New         +---+ Top of existing
        //  +---+               +---+
        //  |   |   +---+       +---+ Fall through
        //  |   |   |   | =>    |   |
        //  |   |   |   |       |   | New; rest of existing discarded
        //  +---+   +---+       +---+
        // Note: no need to check the other way around, because in this
        // case, we will have called Cfg::Label(), and it will have
        // split the existing BB already.
    }
    return pBB;
}

// Use this function when there are outedges to BBs that are not created
// yet. Usually used via addOutEdge()
/*==============================================================================
 * FUNCTION:        Cfg::newIncompleteBB
 * OVERVIEW:        
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
PBB Cfg::newIncompleteBB(ADDRESS addr)
{
    // Create a new (basically empty) BB
    PBB pBB = new BasicBlock();
    // Add it to the list
    m_listBB.push_back(pBB);
    m_mapBB[addr] = pBB;                // Insert the mapping
    return pBB;
}

/*==============================================================================
 * FUNCTION:        Cfg::addOutEdge
 * OVERVIEW:        Add an out edge to this BB (and the in-edge to the dest BB)
 *                  May also set a label
 * NOTE:            Overloaded with address as 2nd argument (calls this proc
 *                    in the end)
 * NOTE ALSO:       Does not increment m_iNumOutEdges; this is supposed to be
 *                    constant for a BB. (But see BasicBlock::addNewOutEdge())
 * PARAMETERS:      pBB: source BB (to have the out edge added to)
 *                  pDestBB: destination BB (to have the out edge point to)
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::addOutEdge(PBB pBB, PBB pDestBB, bool bSetLabel /* = false */)
{
    // Add the given BB pointer to the list of out edges
    pBB->m_OutEdges.push_back(pDestBB);
    // Note that the number of out edges is set at constructor time,
    // not incremented here.
    // Add the in edge to the destination BB
    pDestBB->m_InEdges.push_back(pBB);
    pDestBB->m_iNumInEdges++;           // Inc the count
    if (bSetLabel) setLabel(pDestBB);   // Indicate "label required"
}

/*==============================================================================
 * FUNCTION:        Cfg::addOutEdge
 * OVERVIEW:        Add an out edge to this BB (and the in-edge to the dest BB)
 *                  May also set a label
 * NOTE:            Calls the above
 * PARAMETERS:      pBB: source BB (to have the out edge added to) 
 *                  addr: source address of destination (the out edge is to
 *                      point to the BB whose lowest address is addr)
 *                  bSetLabel: if true, set a label at the destination address.
 *                      Set true on "true" branches of labels
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::addOutEdge(PBB pBB, ADDRESS addr, bool bSetLabel /* = false */)
{
    // Check to see if the address is in the map, i.e. we already have
    // a BB for this address
    MAPBB::const_iterator it = m_mapBB.find(addr);
    PBB pDestBB;
    if (it != m_mapBB.end() && (*it).second) {
        // Just add this PBB to the list of out edges
        pDestBB = (*it).second;
    }
    else {
        // Else, create a new incomplete BB, add that to the map, and add
        // the new BB as the out edge
        pDestBB = newIncompleteBB(addr);
    }
    addOutEdge(pBB, pDestBB, bSetLabel);
}

/*==============================================================================
 * FUNCTION:        Cfg::isLabel 
 * OVERVIEW:        Return true if the given address is the start of a basic
 *                    block, complete or not
 * PARAMETERS:      uNativeAddr: native address to look up
 * RETURNS:         True if uNativeAddr starts a BB
 *============================================================================*/
// Note: must ignore entries with a null pBB, since these are caused by
// calls to Label that failed, i.e. the instruction is not decoded yet.
bool Cfg::isLabel (ADDRESS uNativeAddr)
{
    MAPBB::iterator mi;
    mi = m_mapBB.find (uNativeAddr);
    return (mi != m_mapBB.end() && (*mi).second);
}

/*==============================================================================
 * FUNCTION:    Cfg::splitBB (private)
 * OVERVIEW:    Split the given basic block at the RTL associated with
 *              uNativeAddr. The first node's type becomes fall-through
 *              and ends at the RTL prior to that associated with uNativeAddr.
 *              The second node's type becomes the type of the original basic
 *              block (pBB), and its out-edges are those of the original
 *              basic block. In edges of the new BB's descendants are changed.
 * PRECONDITION: assumes uNativeAddr is an address within the boundaries
 *              of the given basic block.
 * PARAMETERS:  pBB -  pointer to the BB to be split
 *              uNativeAddr - address of RTL to become the start of the new BB
 *              pNewBB -  if non zero, it remains as the "bottom" part of the
 *              BB, and splitBB only modifies the top part to not overlap.
 *              bDelRtls - if true, deletes the RTLs removed from the existing
 *              BB after the split point. Only used if there is an overlap with
 *              existing instructions
 * RETURNS:     Returns a pointer to the "bottom" (new) part of the split BB.
 *============================================================================*/
PBB Cfg::splitBB (PBB pBB, ADDRESS uNativeAddr, PBB pNewBB /* = 0 */,
    bool bDelRtls /* = false */)
{ 
    HRTLList_IT ri;

    // First find which RTL has the split address; note that this
    // could fail (e.g. label in the middle of an instruction, or
    // some weird delay slot effects)
    for (ri = pBB->m_pRtls->begin(); ri != pBB->m_pRtls->end(); ri++)
    {
        if ((*ri)->getAddress() == uNativeAddr)
            break;
    }
    if (ri == pBB->m_pRtls->end())
    {
        ostrstream ost;
        ost << "could not split BB at " << hex;
        ost << pBB->getLowAddr() << " at splt address " << uNativeAddr;
        error(str(ost));
        return pBB;
    }

    // If necessary, set up a new basic block with information from the
    // original bb
    if (pNewBB == 0)
    {
        pNewBB = new BasicBlock(*pBB);
        // But we don't want the top BB's in edges; our only in-edge should
        // be the out edge from the top BB
        pNewBB->m_iNumInEdges = 0;
        pNewBB->m_InEdges.erase(pNewBB->m_InEdges.begin(),
            pNewBB->m_InEdges.end());
        // The "bottom" BB now starts at the implicit label, so we create
        // a new list that starts at ri. We need a new list, since it is
        // different from the original BB's list. We don't have to "deep
        // copy" the HRTLs themselves, since they will never overlap
        pNewBB->setHRTLs(new HRTLList(ri, pBB->m_pRtls->end()));
        // Put it in the graph
        m_listBB.push_back(pNewBB);
        // Put the implicit label into the map. Need to do this before the
        // addOutEdge() below
        m_mapBB[uNativeAddr] = pNewBB;
        // There must be a label here; else would not be splitting.
        // Give it a new label
        pNewBB->m_iLabelNum = ++lastLabel;
    }
    else if (pNewBB->m_bIncomplete)
    {
        // We have an existing BB and a map entry, but no details except
        // for in-edges and m_bHasLabel.
        // First save the in-edges and m_iLabelNum
        vector<PBB> ins(pNewBB->m_InEdges);
        int label = pNewBB->m_iLabelNum;
        // Copy over the details now, completing the bottom BB
        *pNewBB = *pBB;                 // Assign the BB, copying fields
                                        // This will set m_bIncomplete false
        // Replace the in edges (likely only one)
        pNewBB->m_InEdges = ins;
        pNewBB->m_iNumInEdges = ins.size();
        // Replace the label (must be one, since we are splitting this BB!)
        pNewBB->m_iLabelNum = label;
        // The "bottom" BB now starts at the implicit label
        // We need to create a new list of HRTLs, as per above
        pNewBB->setHRTLs(new HRTLList(ri, pBB->m_pRtls->end()));
    }
    // else pNewBB exists and is complete. We don't want to change the
    // complete BB in any way, except to later add one in-edge

    // Update original ("top") basic block's info and make it a fall-through
    pBB->m_nodeType = FALL;
    // Fix the in-edges of pBB's descendants. They are now pNewBB
    // Note: you can't believe m_iNumOutEdges at the time that this function
    // may get called
    for (unsigned j=0; j < pBB->m_OutEdges.size(); j++)
    {
        PBB pDescendant = pBB->m_OutEdges[j];
        // Search through the in edges for pBB (old ancestor)
        unsigned k;
        for (k=0; k < pDescendant->m_InEdges.size(); k++)
        {
            if (pDescendant->m_InEdges[k] == pBB)
            {
                // Replace with a pointer to the new ancestor
                pDescendant->m_InEdges[k] = pNewBB;
                break;
            }
        }
        // That pointer should have been found!
        assert (k < pDescendant->m_InEdges.size());
    }
    // The old BB needs to have part of its list of HRTLs erased, since
    // the instructions overlap
    if (bDelRtls)
    {
        // Delete the list of pointers, and also the RTLs they point to
        erase_lrtls(pBB->m_pRtls, ri, pBB->m_pRtls->end());
    }
    else
    {
        // Delete the list of pointers, but not the RTLs they point to
        pBB->m_pRtls->erase(ri, pBB->m_pRtls->end());
    }
    // Erase any existing out edges
    pBB->m_OutEdges.erase(pBB->m_OutEdges.begin(), pBB->m_OutEdges.end());
    pBB->m_iNumOutEdges = 1;
    addOutEdge (pBB, uNativeAddr);  
    return pNewBB;
}

/*==============================================================================
 * FUNCTION:        Cfg::getFirstBB
 * OVERVIEW:        Get the first BB of this cfg
 * PARAMETERS:      it: set to an value that must be passed to getNextBB
 * RETURNS:         Pointer to the first BB this cfg, or NULL if none
 *============================================================================*/
PBB Cfg::getFirstBB(BB_IT& it)
{
    if ((it = m_listBB.begin()) == m_listBB.end()) return 0;
    return *it;
}

/*==============================================================================
 * FUNCTION:        Cfg::getFirstBB
 * NOTE:            const version of the above
 * OVERVIEW:        Get the first BB of this cfg
 * PARAMETERS:      it: set to an value that must be passed to getNextBB
 * RETURNS:         Pointer to the first BB this cfg, or NULL if none
 *============================================================================*/
PBB Cfg::getFirstBB(BB_CIT& it) const
{
    if ((it = m_listBB.begin()) == m_listBB.end()) return 0;
    return *it;
}

/*==============================================================================
 * FUNCTION:        Cfg::getNextBB
 * OVERVIEW:        Get the next BB this cfg. Basically increments the given
 *                  iterator and returns it
 * PARAMETERS:      iterator from a call to getFirstBB or getNextBB
 * RETURNS:         pointer to the BB, or NULL if no more
 *============================================================================*/
PBB Cfg::getNextBB(BB_IT& it)
{
    if (++it == m_listBB.end()) return 0;
    return *it;
}

/*==============================================================================
 * FUNCTION:        Cfg::getNextBB
 * OVERVIEW:        Get the next BB this cfg. Basically increments the given
 *                  iterator and returns it
 * NOTE:            Const version of the above
 * PARAMETERS:      iterator from a call to getFirstBB or getNextBB
 * RETURNS:         pointer to the BB, or NULL if no more
 *============================================================================*/
PBB Cfg::getNextBB(BB_CIT& it) const
{
    if (++it == m_listBB.end()) return 0;
    return *it;
}
/*==============================================================================
 * FUNCTION:    Cfg::label
 * OVERVIEW:    Checks whether the given native address is a label (explicit
 *              or non explicit) or not. Returns false for incomplete BBs.
 *              So it returns true iff the address has already been decoded
 *              in some BB. If it was not already a label (i.e. the first
 *              instruction of some BB), the BB is split so that it becomes
 *              a label.
 *              Explicit labels are addresses that have already been tagged
 *              as being labels due to transfers of control to that address,
 *              and are therefore the start of some BB.  Non explicit labels
 *              are those that belong to basic blocks that have already been
 *              constructed (i.e. have previously been parsed) and now need
 *              to be made explicit labels.  In the case of non explicit
 *              labels, the basic block is split into two and types and
 *              edges are adjusted accordingly. If pCurBB is the BB that
 *              gets split, it is changed to point to the address of the
 *              new (lower) part of the split BB.
 *              If there is an incomplete entry in the table for this
 *              address which overlaps with a completed address, the completed
 *              BB is split and the BB for this address is completed.
 * PARAMETERS:  uNativeAddress - native (source) address to check
 *              pCurBB - See above
 * RETURNS:     True if uNativeAddr is a label, i.e. (now) the start of a BB
 *              Note: pCurBB may be modified (as above)
 *============================================================================*/
bool Cfg::label ( ADDRESS uNativeAddr, PBB& pCurBB )
{ 
    MAPBB::iterator mi, newi;

    // check if the native address is in the map already (explicit label)
    mi = m_mapBB.find (uNativeAddr);
    
    if (mi == m_mapBB.end())        // not in the map
    {
        // If not an explicit label, temporarily add the address to the map
        m_mapBB[uNativeAddr] = (PBB) 0;     // no PBB yet

        // get an iterator to the new native address and check if the
        // previous element in the (sorted) map overlaps
        // this new native address; if so, it's a non-explicit label 
        // which needs to be made explicit by splitting the previous BB.
        mi = m_mapBB.find (uNativeAddr);

        newi = mi;
        bool bSplit = false;
        PBB pPrevBB;
        if (newi != m_mapBB.begin()) {
            pPrevBB = (*--mi).second;
            if (!pPrevBB->m_bIncomplete &&
                (pPrevBB->getLowAddr() < uNativeAddr) &&
                (pPrevBB->getHiAddr () >= uNativeAddr))
                    bSplit = true;
        }
        if (bSplit) {
            // Non-explicit label. Split the previous BB
            PBB pNewBB = splitBB (pPrevBB, uNativeAddr);    
            if (pCurBB == pPrevBB) {
                // This means that the BB that we are expecting to use, usually
                // to add out edges, has changed. We must change this pointer
                // so that the right BB gets the out edges. However, if the
                // new BB is not the BB of interest, we mustn't change pCurBB
                pCurBB = pNewBB;
            }
            return true;            // wasn't a label, but already parsed
        }
        else {                      // not a non-explicit label
            // We don't have to erase this map entry. Having a null
            // BasicBlock pointer is coped with in newBB() and addOutEdge();
            // when eventually the BB is created, it will replace this entry.
            // we should be currently processing this BB. The map will
            // be corrected when newBB is called with this address.
            return false;               // was not already parsed
        }
    }
    else            // We already have uNativeAddr in the map
    {
        if ((*mi).second && !(*mi).second->m_bIncomplete) {
            // There is a complete BB here. Return true.
            return true;
        }

        // We are finalising an incomplete BB. Still need to check previous
        // map entry to see if there is a complete BB overlapping
        bool bSplit = false;
        PBB pPrevBB, pBB = (*mi).second;
        if (mi != m_mapBB.begin())
        {
            pPrevBB = (*--mi).second;
            if (!pPrevBB->m_bIncomplete &&
                (pPrevBB->getLowAddr() < uNativeAddr) &&
                (pPrevBB->getHiAddr () >= uNativeAddr))
                    bSplit = true;
        }
        if (bSplit)
        {
            // Pass the third parameter to splitBB, because we already
            // have an (incomplete) BB for the "bottom" BB of the split
            splitBB (pPrevBB, uNativeAddr, pBB);    // non-explicit label
            return true;            // wasn't a label, but already parsed
        }
        // A non overlapping, incomplete entry is in the map.
        return false;
    }
}

// Return true if there is an incomplete BB already at this address
/*==============================================================================
 * FUNCTION:        Cfg::isIncomplete
 * OVERVIEW:        Return true if given address is the start of an incomplete
 *                    basic block
 * PARAMETERS:      uAddr: Address to look up
 * RETURNS:         True if uAddr starts an incomplete BB
 *============================================================================*/
bool Cfg::isIncomplete(ADDRESS uAddr)
{
    MAPBB::iterator mi = m_mapBB.find(uAddr);
    if (mi == m_mapBB.end())
        // No entry at all
        return false;
    // Else, there is a BB there. If it's incomplete, return true
    PBB pBB = (*mi).second;
    return pBB->m_bIncomplete;
}

/*==============================================================================
 * FUNCTION:        Cfg::sortByAddress
 * OVERVIEW:        Sorts the BBs in a cfg by first address. Just makes it more
 *                  convenient to read when BBs are iterated.
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/

#ifdef WIN32
struct tag_greaterAddress : greater<BasicBlock*> {
    bool operator()(const BasicBlock* x, const BasicBlock* y) const
        {
        return BasicBlock::lessAddress((BasicBlock*)y,(BasicBlock*)x);            
        }
} greaterAddress;
#endif

void Cfg::sortByAddress()
{
#ifndef WIN32
    m_listBB.sort(BasicBlock::lessAddress);
#else
    m_listBB.sort(greaterAddress);
#endif
}

/*==============================================================================
 * FUNCTION:        Cfg::sortByFirstDFS
 * OVERVIEW:        Sorts the BBs in a cfg by their first DFS numbers.
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
#ifdef WIN32
struct tag_greaterFirstDFS : greater<BasicBlock*> {
    bool operator()(const BasicBlock* x, const BasicBlock* y) const
        {
        return BasicBlock::lessFirstDFS((BasicBlock*)y,(BasicBlock*)x);            
        }
} greaterFirstDFS;
#endif

void Cfg::sortByFirstDFS()
{
#ifndef WIN32
    m_listBB.sort(BasicBlock::lessFirstDFS);
#else
    m_listBB.sort(greaterFirstDFS);
#endif
}

/*==============================================================================
 * FUNCTION:        Cfg::sortByLastDFS
 * OVERVIEW:        Sorts the BBs in a cfg by their last DFS numbers.
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/

#ifdef WIN32
struct tag_greaterLastDFS : greater<BasicBlock*> {
    bool operator()(const BasicBlock* x, const BasicBlock* y) const
        {
        return BasicBlock::lessLastDFS((BasicBlock*)y,(BasicBlock*)x);            
        }
} greaterLastDFS;
#endif

void Cfg::sortByLastDFS()
{
#ifndef WIN32
    m_listBB.sort(BasicBlock::lessLastDFS);
#else
    m_listBB.sort(greaterLastDFS);
#endif
}

/*==============================================================================
 * FUNCTION:        Cfg::updateVectorBB
 * OVERVIEW:        Updates m_vectorBB to m_listBB
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::updateVectorBB()
{
    m_vectorBB.clear();
    for (list<PBB>::iterator it = m_listBB.begin(); it != m_listBB.end(); it++)
        m_vectorBB.push_back(*it);
}


/*==============================================================================
 * FUNCTION:        Cfg::wellFormCfg
 * OVERVIEW:        Checks that all BBs are complete, and all out edges are
 *                  valid. However, ADDRESSes that are interprocedural out edges
 *                  are not checked or changed.
 * PARAMETERS:      <none>
 * RETURNS:         transformation was successful
 *============================================================================*/
bool Cfg::wellFormCfg()
{
    m_bWellFormed = true;
    for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++)
    {
        // it iterates through all BBs in the list
        // Check that it's complete
        if ((*it)->m_bIncomplete)
        {
            m_bWellFormed = false;
            MAPBB::const_iterator itm;
            for (itm = m_mapBB.begin(); itm != m_mapBB.end(); itm++)
                if ((*itm).second == *it) break;
            ostrstream ost;
            if (itm == m_mapBB.end())
                ost << "WellFormCfg: incomplete BB not even in map!";
            else
            {
                ost << "WellFormCfg: BB with native address ";
                ost << hex << (*itm).first << " is incomplete";
            }
            error(str(ost));
        }
        else
        {
            // Complete. Test the out edges
            assert((int)(*it)->m_OutEdges.size() == (*it)->m_iNumOutEdges);
            for (int i=0; i < (*it)->m_iNumOutEdges; i++) {
                // check if address is interprocedural
//              if ((*it)->m_OutEdgeInterProc[i] == false)
                {
                    // i iterates through the outedges in the BB *it
                    PBB pBB = (*it)->m_OutEdges[i];

                    // Check that the out edge has been written (i.e. nonzero)
                    if (pBB == 0) {
                        m_bWellFormed = false;  // At least one problem
                        ADDRESS addr = (*it)->getLowAddr();
                        ostrstream ost;
                        ost << "WellFormCfg: BB with native address ";
                        ost << hex << addr << " is missing outedge ";
                        ost << i;
                        error(str(ost));
                    }
                    else {
                        // Check that there is a corresponding in edge from the
                        // child to here
                        vector<PBB>::const_iterator ii;
                        for (ii=pBB->m_InEdges.begin();
                                ii != pBB->m_InEdges.end(); ii++)
                            if (*ii == *it) break;
                        if (ii == pBB->m_InEdges.end()) {
                            ostrstream ost;
                            ost << "WellFormCfg: No in edge to BB at " << hex;
                            ost << (*it)->getLowAddr();
                            ost << " from successor BB at ";
                            ost << pBB->getLowAddr();
                            error(str(ost));
                            m_bWellFormed = false;  // At least one problem
                        }
                    }
                }
            }
            // Also check that each in edge has a corresponding out edge to here
            // (could have an extra in-edge, for example)
            assert((int)(*it)->m_InEdges.size() == (*it)->m_iNumInEdges);
            vector<PBB>::const_iterator ii;
            for (ii = (*it)->m_InEdges.begin(); ii != (*it)->m_InEdges.end();
                    ii++) {
                vector<PBB>::const_iterator oo;
                for (oo=(*ii)->m_OutEdges.begin();
                        oo != (*ii)->m_OutEdges.end(); oo++)
                    if (*oo == *it) break;
                if (oo == (*ii)->m_OutEdges.end()) {
                    ostrstream ost;
                    ost << "WellFormCfg: No out edge to BB at " << hex;
                    ost << (*it)->getLowAddr();
                    ost << " from predecessor BB at ";
                    ost << (*ii)->getLowAddr();
                    error(str(ost));
                    m_bWellFormed = false;  // At least one problem
                }
            }
        }
    }
    return m_bWellFormed;
}

/*==============================================================================
 * FUNCTION:        Cfg::mergeBBs
 * OVERVIEW:        
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
bool Cfg::mergeBBs( PBB pb1, PBB pb2)
{
    // Can only merge if pb1 has only one outedge to pb2, and pb2 has
    // only one in-edge, from pb1. This can only be done after the
    // in-edges are done, which can only be done on a well formed CFG.
    if (!m_bWellFormed) return false;
    if (pb1->m_iNumOutEdges != 1) return false;
    if (pb2->m_iNumInEdges != 1) return false;
    if (pb1->m_OutEdges[0] != pb2) return false;
    if (pb2->m_InEdges[0] != pb1) return false;

    // Merge them! We remove pb1 rather than pb2, since this is also
    // what is needed for many optimisations, e.g. jump to jump.
    completeMerge(pb1, pb2, true);
    return true;
}

/*==============================================================================
 * FUNCTION:        Cfg::completeMerge
 * OVERVIEW:        Complete the merge of two BBs by adjusting in and out edges.
 *                  If bDelete is true, delete pb1
 * PARAMETERS:      pb1, pb2: pointers to the two BBs to merge
 *                  bDelete: if true, pb1 is deleted as well
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::completeMerge(PBB pb1, PBB pb2, bool bDelete = false)
{
    // First we replace all of pb1's predecessors' out edges that used to
    // point to pb1 (usually only one of these) with pb2
    for (int i=0; i < pb1->m_iNumInEdges; i++)
    {
        PBB pPred = pb1->m_InEdges[i];
        for (int j=0; j < pPred->m_iNumOutEdges; j++)
        {
            if (pPred->m_OutEdges[j] == pb1)
                pPred->m_OutEdges[j] = pb2;
        }
    }

    // Now we replace pb2's in edges by pb1's inedges
    pb2->m_InEdges = pb1->m_InEdges;
    pb2->m_iNumInEdges = pb1->m_iNumInEdges;

    if (bDelete) {
        // Finally, we delete pb1 from the BB list. Note: remove(pb1)
        // should also work, but it would involve member comparison
        // (not implemented), and also would attempt to remove ALL
        // elements of the list with this value (so it has to
        // search the whole list, instead of an average of half the
        // list as we have here).
        for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++)
        {
            if (*it == pb1)
            {
                m_listBB.erase(it);
                break;
            }
        }
    }
}

/*==============================================================================
 * FUNCTION:        Cfg::joinBB
 * OVERVIEW:        Amalgamate the RTLs for pb1 and pb2, and place the result
 *                    into pb2
 * PARAMETERS:      pb1, pb2: pointers to the BBs to join
 * ASSUMES:         Fallthrough of *pb1 is *pb2
 * RETURNS:         True if successful
 *============================================================================*/
bool Cfg::joinBB(PBB pb1, PBB pb2)
{
    // Ensure that the fallthrough case for pb1 is pb2
    const vector<PBB>& v = pb1->getOutEdges();
    if (v.size() != 2 || v[1] != pb2)
        return false;
    // Prepend the RTLs for pb1 to those of pb2. Since they will be pushed to
    // the front of pb2, push them in reverse order
    HRTLList_RIT it;
    for (it = pb1->m_pRtls->rbegin(); it != pb1->m_pRtls->rend(); it++) {
        pb2->m_pRtls->push_front(*it);
    }
    completeMerge(pb1, pb2);                // Mash them together
    // pb1 no longer needed. Remove it from the list of BBs
    // This will also delete *pb1. It will be a shallow delete, but that's good
    // because we only did shallow copies to *pb2
    BB_IT bbit = find(m_listBB.begin(), m_listBB.end(), pb1);
    m_listBB.erase(bbit);
    return true;
}

#if 0           // Old code; doesn't work. May have good ideas, so keep here
bool Cfg::compressCfg()
{
    if (!m_bWellFormed) return false;
    for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++)
    {
        for (int i=0; i < (*it)->m_iNumOutEdges; i++)
        {
            PBB pSucc = (*it)->m_OutEdges[i];
            // We have a BB (*it) and its successor (*pSucc).
            // Attempt the three optimisations on it.
            // 1) Jumps to jumps
            if (((*it)->m_nodeType == ONEWAY) && 
                (pSucc->m_nodeType == ONEWAY))
            {
                completeMerge(*it, pSucc, true);
            }
            // 2) Jump to next BB
            else if(((*it)->m_nodeType == ONEWAY) &&
                ((*it)->m_OutEdges[0] == pSucc) && pSucc->m_iNumInEdges == 1)
            {
                completeMerge(*it, pSucc, true);
            }
            // 3) Attempt ordinary merge
            else mergeBBs(*it, pSucc);
        }
    }
    return true;
}
#endif

/*==============================================================================
 * FUNCTION:        Cfg::compressCfg
 * OVERVIEW:        Compress the CFG. For now, it only removes BBs that are
 *                    just branches
 * PARAMETERS:      <none>
 * RETURNS:         False if not well formed; true otherwise
 *============================================================================*/
bool Cfg::compressCfg()
{
    // Find A -> J -> B  where J is a BB that is only a jump
    // Then A -> B
    if (!m_bWellFormed) return false;
    for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++)
    {
        for (vector<PBB>::iterator it1 = (*it)->m_OutEdges.begin();
          it1 != (*it)->m_OutEdges.end(); it1++) {
            PBB pSucc = (*it1);         // Pointer to J
            PBB bb = (*it);             // Pointer to A
            if (pSucc->m_InEdges.size()==1 && pSucc->m_OutEdges.size()==1 &&
              pSucc->m_pRtls->size()==1 &&
              pSucc->m_pRtls->front()->getKind()==JUMP_HRTL) {
                // Found an out-edge to an only-jump BB
                /* cout << "outedge to jump detected at " << hex <<
                    bb->getLowAddr() << " to ";
                    cout << pSucc->getLowAddr() << " to " <<
                    pSucc->m_OutEdges.front()->getLowAddr() << dec << endl; */
                // Point this outedge of A to the dest of the jump (B)
                *it1=pSucc->m_OutEdges.front();
                // Now pSucc still points to J; *it1 points to B.
                // Almost certainly, we will need a jump in the low level C
                // that may be generated. Also force a label for B
                bb->m_bJumpReqd = true;
                setLabel(*it1);
                // Find the in-edge from B to J; replace this with an in-edge
                // to A
                vector<PBB>::iterator it2;
                for (it2 = (*it1)->m_InEdges.begin();
                  it2 != (*it1)->m_InEdges.end(); it2++) {
                    if (*it2==pSucc)
                        *it2 = bb;          // Point to A
                }
                // Remove the in-edge from J to A. First find the in-edge
                for (it2 = pSucc->m_InEdges.begin();
                  it2 != pSucc->m_InEdges.end(); it2++) {
                    if (*it2 == bb)
                        break;
                }
                assert(it2 != pSucc->m_InEdges.end());
                pSucc->deleteInEdge(it2);
                // If nothing else uses this BB (J), remove it from the CFG
                if (pSucc->m_iNumInEdges == 0) {
                    for (BB_IT it3 = m_listBB.begin(); it3 != m_listBB.end();
                      it3++) {
                        if (*it3==pSucc) {
                            m_listBB.erase(it3);
                            // And delete the BB
                            delete pSucc;
                            break;
                        }
                    }
                }
            }
        }
    }
    return true;
}

/*==============================================================================
 * FUNCTION:        Cfg::unTraverse
 * OVERVIEW:        Reset all the traversed flags.
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::unTraverse()
{
    for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++)
    {
        (*it)->m_iTraversed = false;
    }
}
    
/*==============================================================================
 * FUNCTION:        Cfg::establishDFTOrder
 * OVERVIEW:        Given a well-formed cfg graph, a partial ordering is
 *                  established between the nodes. The ordering is based on the
 *                  final visit to each node during a depth first traversal such
 *                  that if node n1 was visited for the last time before node n2
 *                  was visited for the last time, n1 will be less than n2. The
 *                  return value indicates if all nodes where ordered. This will
 *                  not be the case for incomplete CFGs (e.g. switch table not
 *                  completely recognised) or where there are nodes unreachable
 *                  from the entry node.
 * PARAMETERS:      <none>
 * RETURNS:         all nodes where ordered
 *============================================================================*/
bool Cfg::establishDFTOrder()
{
    // Must be well formed.
    if (!m_bWellFormed) return false;

    // Reset all the traversed flags
    unTraverse();

    int first = 0;
    int last = 0;
    unsigned numTraversed;

    if (checkEntryBB()) return false;

    numTraversed = entryBB->DFTOrder(first,last);

    return numTraversed == m_listBB.size();
}

/*==============================================================================
 * FUNCTION:        Cfg::establishRevDFTOrder
 * OVERVIEW:        Performs establishDFTOrder on the reverse (flip) of the 
 *          graph, assumes: establishDFTOrder has already been called
 * PARAMETERS:      <none>
 * RETURNS:         all nodes where ordered
 *============================================================================*/
bool Cfg::establishRevDFTOrder()
{
    // Must be well formed.
    if (!m_bWellFormed) return false;

    // sort by last dfs and grab the exit node
    sortByLastDFS();
    
    PBB retNode = m_listBB.front();

    // Reset all the traversed flags
    unTraverse();

    int first = 0;
    int last = 0;
    unsigned numTraversed;

    numTraversed = retNode->RevDFTOrder(first,last);

    return numTraversed == m_listBB.size();
}

/*==============================================================================
 * FUNCTION:        Cfg::isWellFormed
 * OVERVIEW:        
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
bool Cfg::isWellFormed() const
{
    return m_bWellFormed;
}

/*==============================================================================
 * FUNCTION:        Cfg::isOrphan
 * OVERVIEW:        
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
bool Cfg::isOrphan(ADDRESS uAddr)
{
    MAPBB::iterator mi = m_mapBB.find(uAddr);
    if (mi == m_mapBB.end())
        // No entry at all
        return false;
    // Return true if the first RTL at this address has an address set to 0
    PBB pBB = (*mi).second;
    // If it's incomplete, it can't be an orphan
    if (pBB->m_bIncomplete) return false;
    return pBB->m_pRtls->front()->getAddress() == 0;
}

// Return an index for the given pointer to a basic block.
// Linear search: O(N) complexity
// Returns -1 for unknown PBB
/*==============================================================================
 * FUNCTION:        Cfg::pbbToIndex 
 * OVERVIEW:        
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
int Cfg::pbbToIndex (PBB pBB) const
{
    BB_CIT it = m_listBB.begin();
    int i = 0;
    while (it != m_listBB.end())
    {
        if (*it++ == pBB) return i;
        i++;
    }
    return -1;
}

/*==============================================================================
 * FUNCTION:        Cfg::getCoverage
 * OVERVIEW:        
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
unsigned Cfg::getCoverage() const
{
    // Start with the extra coverage from nops, switch tables, and the like
    unsigned uTotal = m_uExtraCover;
    for (BB_CIT it=m_listBB.begin(); it != m_listBB.end(); it++)
    {
        uTotal += (*it)->getCoverage();
    }
    return uTotal;
}

/*==============================================================================
 * FUNCTION:        Cfg::subAXP
 * OVERVIEW:        (see comment for Proc::subAXP)
 * PARAMETERS:      subMap - a map from register to expressions
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::subAXP(map<SemStr,SemStr>& subMap)
{
    unTraverse();
    if (checkEntryBB()) return;
    entryBB->subAXP(subMap);
}

/*==============================================================================
 * FUNCTION:        Cfg::resetDFASets
 * OVERVIEW:        Resets the DFA sets of all the BBs.
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::resetDFASets()
{
    for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++)
        (*it)->resetDFASets();
}

/*==============================================================================
 * FUNCTION:        Cfg::buildUseDefSets
 * OVERVIEW:        Build the set of locations that are used and
 *                  defined in each BB of this CFG.
 * PARAMETERS:      filter - a filter to restrict which locations are
 *                    considered
 *                  proc - Proc object associated with this Cfg
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::buildUseDefSets(LocationMap& locMap, LocationFilter* filter, Proc*
    proc)
{
    // Build the use and def sets for each BB
    for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++)
        (*it)->buildUseDefSets(locMap,filter,proc);
}

/*==============================================================================
 * FUNCTION:        Cfg::buildLiveInSets
 * OVERVIEW:        Build the sets of definitions that are live into 
 *                  each basic block. This method is only called when
 *                  analysing this procedure as a callee. The purpose
 *                  of the parameter is to prevent uses of a return
 *                  location as a use-without-definition.
 * PARAMETERS:      callDefines - this is the set of locations that will
 *                    be considered as defined by a call.
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::buildLiveInSets(const BITSET& callDefines)
{
    buildLiveInOutSets(BITSET(), &callDefines);
}

/*==============================================================================
 * FUNCTION:        Cfg::buildLiveOutSets
 * OVERVIEW:        Build the sets of definitions that are live out of
 *                  each basic block. This method is only called when
 *                  analysing this procedure as a caller.
 * PARAMETERS:      params: bitset representing the parameters of this proc
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::buildLiveOutSets(const BITSET& params)
{
    buildLiveInOutSets(params, NULL);
}

/*==============================================================================
 * FUNCTION:        Cfg::buildLiveInOutSets
 * OVERVIEW:        Build the sets of definitions that are live into and out of
 *                  each basic block.
 *                  This implements Algorithm 10.3 given on
 *                  page 631 of "Compilers: Principles, Techniques and Tools"
 *                  (i.e the 'dragon book').
 * PARAMETERS:      params - set of parameter locations
 *                  defines - this is the set of locations that will
 *                    be considered as defined by a call. If it is
 *                    NULL, we assume that this procedure is being
 *                    analysed as a caller and as such calls kill all
 *                    definitions
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::buildLiveInOutSets(const BITSET& params, const BITSET* defines
    /*= NULL*/)
{
    // For efficiency, we require the BBs to be sorted by the last DFS number.
    this->sortByLastDFS();

    // Initialise the liveOut set of each BB to equal its defined set. The
    // liveIn sets are initialised to the universal set.
    for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++) {
        (*it)->liveOut.set();
        (*it)->liveIn.set();
    }

    // The first basic block is set to have no live ins, apart from parameters
    if (checkEntryBB()) return;
    entryBB->liveIn = params;
    entryBB->liveOut = entryBB->defSet;

    bool change = true;

    while (change) {
        change = false;

        // Process the BBs in reverse order (i.e. coming down the graph,
        // visiting parents before children)
        for (list<PBB>::reverse_iterator it = m_listBB.rbegin();
            it != m_listBB.rend(); it++) {

            PBB currBB = (*it);

            // Set the liveIn set to be the intersection of all the
            // predecessor's liveOut sets and add any new members of
            // liveIn to liveOut.
            change = currBB->buildLiveInOutSets(defines);
        }
    }
}

/*==============================================================================
 * FUNCTION:        Cfg::buildRecursiveUseUndefSets
 * OVERVIEW:        For each BB, b, in this procedure, build the set of
 *                  locations used before definition in the subgraph
 *                  headed by b.
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::buildRecursiveUseUndefSets()
{
    unTraverse();
    if (checkEntryBB()) return;
    entryBB->buildRecursiveUseUndefSet();
}

/*==============================================================================
 * FUNCTION:        Cfg::getLiveIns
 * OVERVIEW:        Get the locations that are live into this procedure.
 * PARAMETERS:      liveIn - set to the locations that are live into this proc
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::getLiveIns(BITSET& liveIn)
{
    for (BB_IT it = m_listBB.begin(); it != m_listBB.end(); it++) {

        PBB bb = (*it);

        // Add all the locations used without definition in the current BB
        // to the set of proc live ins
        liveIn |= bb->useUndefSet & ~(bb->liveIn);
    }
}

/*==============================================================================
 * FUNCTION:        Cfg::addCall
 * OVERVIEW:        Add a call to the set of calls within this procedure.
 * PARAMETERS:      call - a call instruction
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::addCall(HLCall* call)
{
    callSites.insert(call);
}

/*==============================================================================
 * FUNCTION:        Cfg::getCalls
 * OVERVIEW:        Get the set of calls within this procedure.
 * PARAMETERS:      <none>
 * RETURNS:         the set of calls within this procedure
 *============================================================================*/
SET_CALLS& Cfg::getCalls()
{
    return callSites;
}

/*==============================================================================
 * FUNCTION:        Cfg::searchAndReplace
 * OVERVIEW:        Replace all instances of search with replace.
 * PARAMETERS:      search - a location to search for
 *                  replace - the expression with which to replace it
 *                  typeSens - if true, the search is sensitive to type
 * NOTES:           It is difficult to make search a const parameter, even
 *                    it should be
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::searchAndReplace(SemStr& search, const SemStr& replace,
    bool typeSens /* = false */)
{
    for (BB_IT bb_it = m_listBB.begin(); bb_it != m_listBB.end(); bb_it++) {

        list<HRTL*>& rtls = *((*bb_it)->getHRTLs());
        for (HRTLList::iterator rtl_it = rtls.begin(); rtl_it != rtls.end();
            rtl_it++) {

            HRTL& rtl = **rtl_it;
            rtl.searchAndReplace(search,replace, typeSens);
        }

    }
}

/*==============================================================================
 * FUNCTION:        Cfg::printDFAInfo
 * OVERVIEW:        Print any data flow analysis info gathered so far such as
 *                  the live-in and live-out sets for each BB.
 * PARAMETERS:      os - the output stream to use
 *                  locMap - maps integers to locations
 * RETURNS:         the given output stream
 *============================================================================*/
ostream& Cfg::printDFAInfo(ostream& os, LocationMap& locMap)
{
    for (list<PBB>::reverse_iterator it = m_listBB.rbegin();
        it != m_listBB.rend(); it++){

        (*it)->printDFAInfo(os,locMap);
        os << endl;
    }
    return os;
}

/*==============================================================================
 * FUNCTION:        Cfg::computeDominators
 * OVERVIEW:        computes the dominators of each BB 
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::computeDominators() {
    sortByLastDFS();

    int index=0;
    for (list<PBB>::iterator it = m_listBB.begin(); it != m_listBB.end(); it++) 
    {       
        (*it)->m_index = index++;
        (*it)->dominators.set();
    }
    
    if (checkEntryBB()) return;
    entryBB->dominators.reset();
    entryBB->dominators.set(entryBB->m_index);

    bool change = true;

    while(change) 
    {   
        change=false;
        for (list<PBB>::iterator it1 = m_listBB.begin(); it1 != m_listBB.end(); it1++)
            if ((*it1) != entryBB)
            {
                BITSET intersect,newd;
                intersect.set();
                newd.reset();
                newd.set((*it1)->m_index);
                for (vector<PBB>::iterator in = (*it1)->m_InEdges.begin(); in != (*it1)->m_InEdges.end(); in++)
                    intersect &= (*in)->dominators;
                newd |= intersect;
                if (newd != (*it1)->dominators) {               
                    change=true;
                    (*it1)->dominators = newd;
                }
            }
    }

    for (list<PBB>::iterator it2 = m_listBB.begin(); it2 != m_listBB.end(); it2++) 
    {
        (*it2)->m_dominatedBy.clear();
        for (list<PBB>::iterator it3 = m_listBB.begin(); it3 != m_listBB.end(); it3++)
            if ((*it2)->dominators.test((*it3)->m_index))
                (*it2)->m_dominatedBy.push_back(*it3);
    }
}

/*==============================================================================
 * FUNCTION:        Cfg::computePostDominators
 * OVERVIEW:        computes the dominators of each BB 
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::computePostDominators() {
    sortByLastDFS();

    PBB retnode = NULL;

    int index=0;
    for (list<PBB>::iterator it = m_listBB.begin(); it != m_listBB.end(); it++) 
    {       
        (*it)->m_index = index++;
        (*it)->postdominators.set();
        if ((*it)->m_OutEdges.empty()) 
        {
            if (retnode!=NULL)
                cout << "eek, more than one ret node detected" << endl;
            retnode = *it;
        }
    }
    
    retnode->postdominators.reset();
    retnode->postdominators.set(retnode->m_index);

    bool change = true;

    while(change) 
    {   
        change=false;
        for (list<PBB>::iterator it1 = m_listBB.begin(); it1 != m_listBB.end(); it1++)
            if ((*it1) != retnode)
            {
                BITSET intersect,newd;
                intersect.set();
                newd.reset();
                newd.set((*it1)->m_index);
                for (vector<PBB>::iterator in = (*it1)->m_OutEdges.begin(); in != (*it1)->m_OutEdges.end(); in++)
                    intersect &= (*in)->postdominators;
                newd |= intersect;
                if (newd != (*it1)->postdominators) {               
                    change=true;
                    (*it1)->postdominators = newd;
                }
            }
    }

    for (list<PBB>::iterator it2 = m_listBB.begin(); it2 != m_listBB.end(); it2++) 
    {
        (*it2)->m_postdominatedBy.clear();
        for (list<PBB>::iterator it3 = m_listBB.begin(); it3 != m_listBB.end(); it3++)
            if ((*it2)->postdominators.test((*it3)->m_index))
                (*it2)->m_postdominatedBy.push_back(*it3);
    }
}
         

/*==============================================================================
 * FUNCTION:        Cfg::structure
 * OVERVIEW:        Structures the control flow graph into loops and conditionals
 * PARAMETERS:      <none>
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::structure() 
{   
    computeDominators();
    computePostDominators();
    for (list<PBB>::iterator bb1 = m_listBB.begin(); bb1 != m_listBB.end(); bb1++) 
    {
        (*bb1)->m_header=false;
        (*bb1)->m_latch=false;
        (*bb1)->m_follow=false;
        (*bb1)->m_listLatchNodes.clear();
    }
    for (list<PBB>::iterator bb = m_listBB.begin(); bb != m_listBB.end(); bb++) 
    {       
        for (vector<PBB>::iterator dst = (*bb)->m_OutEdges.begin(); dst != (*bb)->m_OutEdges.end(); dst++) 
            if ((*bb)->m_last < (*dst)->m_last)
            {
//              cout << "back edge " << (*bb)->m_last << " -> " << (*dst)->m_last << endl;
                (*dst)->m_header = true;
                (*dst)->m_listLatchNodes.push_back(*bb);
                (*bb)->m_latch=true;
                (*bb)->m_headerNode=*dst;
                if ((*bb)->m_nodeType==TWOWAY)
                    if ((*bb)->m_OutEdges.front()==*dst)
                    {
                        (*bb)->m_OutEdges.back()->m_follow=true;
                        (*bb)->m_followNode=(*bb)->m_OutEdges.back();
                    } 
                    else
                    {
                        (*bb)->m_OutEdges.front()->m_follow=true;
                        (*bb)->m_followNode=(*bb)->m_OutEdges.front();
                    }
                else if ((*dst)->m_nodeType==TWOWAY) {
                    // whichever branch of the header doesn't dominate the latch is the follow
                    if ((*bb)->dominators.test((*dst)->m_OutEdges.front()->m_index))
                    {
                        (*dst)->m_OutEdges.back()->m_follow=true;
                        (*dst)->m_followNode=(*dst)->m_OutEdges.back();
                    }
                    else
                    {
                        (*dst)->m_OutEdges.front()->m_follow=true;
                        (*dst)->m_followNode=(*dst)->m_OutEdges.front();
                    }
                }               
            }
    }

    

    for (list<PBB>::iterator bb3 = m_listBB.begin(); bb3 != m_listBB.end(); bb3++) 
        if ((*bb3)->m_nodeType==TWOWAY && ((!(*bb3)->m_header && !(*bb3)->m_latch) || 
                           ((*bb3)->m_header && (*bb3)->m_listLatchNodes.front()->m_nodeType==TWOWAY))) 
        {
            PBB biggest = NULL;
            for (vector<PBB>::iterator pd1 = (*bb3)->m_postdominatedBy.begin(); 
                           pd1 != (*bb3)->m_postdominatedBy.end(); pd1++)
                if ((*pd1)->m_last<(*bb3)->m_last && (biggest==NULL || (*pd1)->m_last>biggest->m_last))
                {
                    if ((*pd1)->dominators.test((*bb3)->m_index))
                        biggest=*pd1;
                }
            if (biggest!=NULL)
            {
                biggest->m_follow=true;
                (*bb3)->m_followNode=biggest;
            }
        }

/*  for (list<PBB>::iterator bb1 = m_listBB.begin(); bb1 != m_listBB.end(); bb1++) 
        if ((*bb1)->m_header && (*bb1)->m_nodeType==TWOWAY && (*bb1)->m_listLatchNodes.front()->m_nodeType==TWOWAY) 
        {
            // hieristic one, an if as the first statement in a post tested loop
            PBB ifstat = new BasicBlock();
            for (vector<PBB>::iterator out1 = (*bb1)->m_OutEdges.begin(); out1 != (*bb1)->m_OutEdges.end(); out1++) 
            {
                ifstat->m_OutEdges.push_back(*out1);
                for (vector<PBB>::iterator in1 = (*out1)->m_InEdges.begin(); in1 != (*out1)->m_InEdges.end(); in1++) 
                    if ((*in1)==(*bb1)) 
                        (*in1)=ifstat;                      
            }
            (*bb1)->m_OutEdges.clear();
            (*bb1)->m_OutEdges.push_back(ifstat);
            ifstat->m_InEdges.push_back(*bb1);
            ifstat->m_pRtls=(*bb1)->m_pRtls;
            (*bb1)->m_pRtls=new HRTLList;
            (*bb1)->m_pRtls->push_back(new HLJump(3434,ifstat->getLowAddr()));
            ifstat->m_nodeType=TWOWAY;
            (*bb1)->m_nodeType=ONEWAY;
            ifstat->m_header=ifstat->m_latch=0;
            m_listBB.push_back(ifstat);
        }
*/

    establishDFTOrder();
//  establishRevDFTOrder();

    // Output a dotty graph to stdout, if enabled
#if 0
    cout << "digraph blah {" << endl;
    cout << "\"main\" [shape=box,style=filled,color=yellow];" << endl;
    for (list<PBB>::iterator bb2 = m_listBB.begin(); bb2 != m_listBB.end();
      bb2++) {
        cout << dec << (*bb2)->m_first << " [shape=ellipse,label=\"";
        if ((*bb2)->m_pRtls != NULL)
            cout << hex << (*bb2)->getLowAddr() << dec;
        else
            cout << "synth";
        cout << ": ";
        if ((*bb2)->m_header) {
            cout << " header to (";
            for (list<PBB>::iterator l1 = (*bb2)->m_listLatchNodes.begin();
              l1 != (*bb2)->m_listLatchNodes.end(); l1++)
                cout << hex << (*l1)->getLowAddr() << dec << ",";
            cout << ")";
        }
        if ((*bb2)->m_latch)
            cout << " latch";
        if ((*bb2)->m_follow)
            cout << " follow";

        cout << " dfslast: " << dec << (*bb2)->m_last;
        cout << " postdom: ";
        for (vector<PBB>::iterator pd1 = (*bb2)->m_postdominatedBy.begin(); pd1 != (*bb2)->m_postdominatedBy.end(); pd1++)
        {
            cout << hex << (*pd1)->getLowAddr() << " ";
        }
        cout << "\"];" << endl;
    }
    cout << "\"main\" -> " << entryBB->m_first << ";" << endl;
    for (list<PBB>::iterator bb3 = m_listBB.begin(); bb3 != m_listBB.end();
      bb3++) {
        for (vector<PBB>::iterator dst1 = (*bb3)->m_OutEdges.begin();
          dst1 != (*bb3)->m_OutEdges.end(); dst1++) {
            cout << dec << (*bb3)->m_first << " -> " << (*dst1)->m_first <<
                ";" << endl;
        }
    }
    cout << "}" << endl;
#endif
}


/*==============================================================================
 * FUNCTION:    delete_lrtls
 * OVERVIEW:    "deep" delete for a list of pointers to HRTLs
 * PARAMETERS:  pLrtl - the list
 * RETURNS:     <none>
 *============================================================================*/
void delete_lrtls(HRTLList* pLrtl)
{
    HRTLList_IT it;
    for (it = pLrtl->begin(); it != pLrtl->end(); it++)
    {
        delete (*it);
        
    }
}

/*==============================================================================
 * FUNCTION:    erase_lrtls
 * OVERVIEW:    "deep" erase for a list of pointers to HRTLs
 * PARAMETERS:  pLrtls - the list
 *              begin - iterator to first (inclusive) item to delete
 *              end - iterator to last (exclusive) item to delete
 * RETURNS:     <none>
 *============================================================================*/
void erase_lrtls(HRTLList* pLrtl, HRTLList_IT begin, HRTLList_IT end)
{
    HRTLList_IT it;
    for (it = begin; it != end; it++)
    {
        delete (*it);
    }
    pLrtl->erase(begin, end);
}

/*==============================================================================
 * FUNCTION:        Cfg::setLabel
 * OVERVIEW:        Sets a flag indicating that this BB has a label, in the
 *                  sense that a label is required in the translated source code
 * PARAMETERS:      pBB: Pointer to the BB whose label will be set
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::setLabel(PBB pBB) {
    if (pBB->m_iLabelNum == 0)
        pBB->m_iLabelNum = ++lastLabel;
}

/*==============================================================================
 * FUNCTION:        Cfg::addNewOutEdge
 * OVERVIEW:        Append a new out-edge from the given BB to the other
 *                    given BB
 *                  Needed for example when converting a one-way BB to a two-
 *                  way BB
 * NOTE:            Use BasicBlock::setOutEdge() for the common case where
 *                    an existing out edge is merely changed
 * NOTE ALSO:       Use Cfg::addOutEdge for ordinary BB creation; this is for
 *                    unusual cfg manipulation (e.g. analysis.cc)
 * PARAMETERS:      pFromBB: pointer to the BB getting the new out edge
 *                  pNewOutEdge: pointer to BB that will be the new successor
 * SIDE EFFECTS:    Increments m_iNumOutEdges
 * RETURNS:         <nothing>
 *============================================================================*/
void Cfg::addNewOutEdge(PBB pFromBB, PBB pNewOutEdge)
{
    pFromBB->m_OutEdges.push_back(pNewOutEdge);
    pFromBB->m_iNumOutEdges++;
    // Since this is a new out-edge, set the "jump required" flag
    pFromBB->m_bJumpReqd = true;
    // Make sure that there is a label there
    setLabel(pNewOutEdge);
}


