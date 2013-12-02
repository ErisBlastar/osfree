/*
 * Copyright (C) 1997-2001, The University of Queensland
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/* File: BinaryFile.h
 * Desc: This file contains the definition of the abstract class BinaryFile
 *
 * Copyright (C) 1997-2001, The University of Queensland, BT group
 * Copyright (C) 2001, Sun Microsystems, Inc
*/

/* $Revision: 1.26 $
 * This class attempts to provide a relatively machine independent
 * interface for programs that read binary files. For details on
 * usage, see the bintrans tex file (bintrans/tex/bintrans/loader.tex)
 * Created by Mike, 97
 * 30 Jan 98 - Cristina
 *  removed definitions for TRUE and FALSE; moved them to driver.h.
 * 3 Feb 98 - Mike
 *  included unneeded <string> library to avoid linking problems.
 *      Whenever list and string are used, string should be included
 *      before list (at least with the gcc compiler). 
 * 25 Feb 98 - Cristina
 *  removed BOOL type as it's now defined in driver.h.
 * 3 Mar 98 - Cristina
 *  make use of global ADDRESS type instead of local ADDR definition.
 *  when the BinaryFile object is used on its own, a -DLOADER directive 
 *      needs to be specified at compilation time so that the type 
 *      definitions are effected.  If used as part of the uqbt project 
 *      (or any other project for that matter), the driver.h file should
 *      have the right type definitions.
 * 11 Mar 98 - Cristina  
 *  replaced BOOL for bool type (C++'s), same for TRUE and FALSE.
 * 24 Mar 98 - Cristina: replaced driver include to global.h. 
 * 22 May 98 - Mike: Now uses its own global.h
 * 27 May 98 - Mike: Class called BinaryFile
 * 28 Apr 99 - Mike: Added GetSectionInfoByAddr()
 * 26 Sep 99 - Mike: GetSectionIndexByName now public
 * 27 Apr 00 - Mike: Added bNoTypeOK to GetAddressByName and GetSizeByName
 *              (the overridden classes have for some time). Fixes uqbtxx -s
 * 11 Feb 01 - Nathan: Added static getInstanceFor() to perform file magic
 * 22 Feb 01 - Nathan: Added a number of functions for use in runtime loading
 *              (vs translation loading)
 * 16 Mar 01 - Mike: Added LOADFMT_EXE
 * 31 Mar 01 - Mike: Moved NO_ADDRESS to global.h (and is now -1)
 * 16 Apr 01 - Brian: Moved definition of the LH macro to here.
 * 01 Aug 01 - Mike: Changed the definition of GetGlobalPointerInfo()
 * 10 Aug 01 - Mike: Added GetDynamicGlobalMap()
 */

#ifndef __BINARYFILE_H__
#define __BINARYFILE_H__

/*==============================================================================
 * Dependencies.
 *============================================================================*/

#ifdef TEST
#include "palmglobal.h"             // Don't want to drag in too much
#else
#include "global.h"
#endif
#include "SymTab.h"
#include <list>

// Given a pointer p, returns the 16 bits (halfword) in the two bytes
// starting at p.
#define LH(p)  ((int)((Byte *)(p))[0] + ((int)((Byte *)(p))[1] << 8))

// SectionInfo structure. GetSectionInfo returns a pointer to an array of
// these structs. All information about the sections is contained in these
// structures.

typedef struct sectioninfo_tag
{
    char*       pSectionName;       // Name of section
    ADDRESS     uNativeAddr;        // Logical or native load address
    ADDRESS     uHostAddr;          // Host or actual address of data
    ADDRESS     uSectionSize;       // Size of section in bytes
    ADDRESS     uSectionEntrySize;  // Size of one section entry (if applic)
    unsigned    uType;               // Type of section (format dependent)
    unsigned    bCode:1;            // Set if section contains instructions
    unsigned    bData:1;            // Set if section contains data
    unsigned    bBss:1;             // Set if section is BSS (allocated only)
    unsigned    bReadOnly:1;        // Set if this is a read only section
} SectionInfo;

typedef SectionInfo* PSectionInfo;
    
// Required by GetInitialState()
enum REG_ENUM {REG_PC, REG_SP};
typedef struct reg_addr_tag
{
    REG_ENUM    Register;           // A register
    ADDRESS     uValue;             // Its initial value
} RegAddr;

/*
 * callback function, which when given the name of a library, should return
 * a pointer to an opened BinaryFile, or NULL if the name cannot be resolved.
 */
class BinaryFile;
typedef BinaryFile *(*get_library_callback_t)(char *name);

// This enum allows a sort of run time type identification, without using
// compiler specific features
enum LOAD_FMT {LOADFMT_ELF, LOADFMT_PE, LOADFMT_PALM, LOADFMT_PAR, LOADFMT_EXE};

class BinaryFile {

  friend class ArchiveFile;           // So can use the protected Load()
 
  public:

    /*
     * Perform simple magic on the file by the given name in order to
     * determine the appropriate type, and then return an instance of
     * the appropriate subclass.
     */
    static BinaryFile *BinaryFile::getInstanceFor(const char *sName);
    static BinaryFile *Load( const char *sName );
    
// General loader functions
    BinaryFile(bool bArchive = false);  // Constructor
    // Unload the file. Pure virtual
    virtual void    UnLoad() = 0;
    // Open the file for r/w; pure virt
    virtual bool    Open(const char* sName) = 0;
    // Close file opened with Open()
    virtual void    Close() = 0;
    // Get the format (e.g. LOADFMT_ELF)
    virtual LOAD_FMT GetFormat() const = 0;

    // Return whether or not the object is a library file.
    virtual bool isLibrary() const = 0;
    // Return whether the object can be relocated if necessary
    // (ie if it is not tied to a particular base address). If not, the object
    // must be loaded at the address given by getImageBase()
    virtual bool isRelocatable() const { return isLibrary(); }
    // Return a list of library names which the binary file depends on
    virtual list<const char *> getDependencyList() = 0;    
    // Return the virtual address at which the binary expects to be loaded.
    // For position independent / relocatable code this should be NO_ADDDRESS
    virtual ADDRESS getImageBase() = 0;
    // Return the total size of the loaded image
    virtual size_t getImageSize() = 0;

// Section functions
    int         GetNumSections() const;     // Return number of sections
    PSectionInfo GetSectionInfo(int idx) const; // Return section struct
    // Find section info given name, or 0 if not found
    PSectionInfo GetSectionInfoByName(const char* sName);
    // Find the end of a section, given an address in the section
    PSectionInfo GetSectionInfoByAddr(ADDRESS uEntry) const;

// Symbol table functions
    // Lookup the address, return the name, or 0 if not found
    virtual const char* SymbolByAddress(ADDRESS uNative);
    // Lookup the name, return the address
    virtual ADDRESS GetAddressByName(const char* pName,
                                     bool bNoTypeOK = false);
    // Lookup the name, return the size
    virtual int GetSizeByName(const char* pName, bool bTypeOK = false);
    // Get an array of addresses of imported function stubs
    // Set number of these to numImports
    virtual ADDRESS* GetImportStubs(int& numImports);

// Relocation table functions
    virtual bool    IsAddressRelocatable(ADDRESS uNative);
    virtual ADDRESS GetRelocatedAddress(ADDRESS uNative);
    //virtual   WORD    ApplyRelocation(ADDRESS uNative, WORD wWord);
    // Get symbol associated with relocation at address, if any
    virtual const char* GetRelocSym(ADDRESS uNative);

    // Specific to BinaryFile objects that implement a "global pointer"
    // Gets a pair of unsigned integers representing the address of the
    // abstract global pointer (%agp) (in first) and a constant that will
    // be available in the csrparser as GLOBALOFFSET (second). At present,
    // the latter is only used by the Palm machine, to represent the space
    // allocated below the %a5 register (i.e. the difference between %a5 and
    // %agp). This value could possibly be used for other purposes.
    virtual pair<unsigned,unsigned> GetGlobalPointerInfo();

    // Get a map from ADDRESS to const char*. This map contains the native
    // addresses and symbolic names of global data items (if any) which are
    // shared with dynamically linked libraries. Example: __iob (basis for
    // stdout).The ADDRESS is the native address of a pointer to the real
    // dynamic data object.
    // The caller should delete the returned map.
    virtual map<ADDRESS, const char*>* GetDynamicGlobalMap();

//
//  --  --  --  --  --  --  --  --  --  --  --
//

    // Internal information
    // Dump headers, etc
    virtual bool    DisplayDetails(const char* fileName, FILE* f = stdout);

    // Analysis functions
    virtual list<RegAddr>& GetInitialState();
    virtual bool    IsDynamicLinkedProc(ADDRESS uNative);
    virtual list<SectionInfo*>& GetEntryPoints(const char* pEntry = "main") = 0;
    virtual ADDRESS GetMainEntryPoint() = 0;

    /*
     * Return the "real" entry point, ie where execution of the program begins
     */
    virtual ADDRESS GetEntryPoint() = 0; 
    // Find section index given name, or -1 if not found
    int         GetSectionIndexByName(const char* sName);


//
//  --  --  --  --  --  --  --  --  --  --  --
//

  protected:
    // Special load function for archive members
    virtual bool    PostLoad(void* handle) = 0;     // Called after loading archive member
    // Load the file. Pure virtual function
    virtual bool    RealLoad(const char* sName) = 0;

    // Data
    bool        m_bArchive;                 // True if archive member
    int         m_iNumSections;             // Number of sections
    PSectionInfo m_pSections;               // The section info
    ADDRESS     m_uInitPC;                  // Initial program counter
    ADDRESS     m_uInitSP;                  // Initial stack pointer

};

#endif      // #ifndef __BINARYFILE_H__
