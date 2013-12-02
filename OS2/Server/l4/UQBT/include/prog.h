/*
 * Copyright (C) 1998-2001, The University of Queensland
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:        prog.h
 * OVERVIEW:    interface for the program object.
 *
 * Copyright (C) 1998-2001, The University of Queensland, BT group
 * Copyright (C) 2001, Sun Microsystems, Inc
 *============================================================================*/
/*
 * $Revision: 1.23 $
 * Created by Mike
 * 24 Mar 98 - Cristina
 *  Changed m_procs to be a list of procedure objects rather than pointers
 *      to procedure objects.  Similar for AddProc() and GetProc().
 *      added map of labels.
 * 26 Mar 98 - Cristina
 *  Changed AddProc() to NewProc() so that we have a reference to the  
 *      procedure and are able to change it during parsing.
 * 28 Jul 98 - Mike
 *  Now depends on PROC, which depends on CFG and RTL
 * 2 Sep 98 - Mike
 *  Added findProc(); m_procs now list of pointers to procs
 * 08 Apr 99 - Mike: Changes for HEADERS
 * 02 Jun 99 - Mike: Removed leading upper case on function names
 * 06 Jun 00 - Mike: Added members csrSrc, pBF
 * 06 Jul 00 - Mike: Added getFirstProc / getNextProc functions
 * 31 Jul 01 - Brian: Added new file hrtl.h for HRTL-related declarations.
 * 30 Aug 01 - Mike: mapLibParam has list (was vector)
 */

#ifndef _PROG_H_
#define _PROG_H_

#include "hrtl.h"                        // For RTLInstDict
#include "csr.h"                        // For the csrSrc object
#include "coverage.h"                   // For the Coverage class

class Prog {
public:
            Prog();                     // Default constructor
            Prog(const char* name);     // Constructor with name
    void    setName(const string& name);// Set the name of this program
    Proc*   newProc(const char* name, ADDRESS uNative, bool bLib = false);
                                        // Returns a pointer to a new proc
    void    remProc(UserProc* proc);    // Remove the given UserProc
const char* getName() const;            // Get the name of this program
    int     getNumProcs();              // # of procedures stored in prog
    Proc*   getProc(int i) const;       // returns pointer to indexed proc
    Proc*   findProc(ADDRESS uAddr) const;// Find the Proc with given address
    bool    isProcLabel (ADDRESS addr); // Checks if addr is a label or not
    // Create a dot file for all CFGs
    bool    createDotFile(const char*, bool bMainOnly = false) const;
    // Call the following when a proc is discovered
    Proc*   visitProc(ADDRESS uAddr);
    void    setArgv0(const char* p);    // Set the argv[0] pointer
    const string& getProgPath();        // Get path to the translator executable
    void    readLibParams();            // Read the common.hs file
    string  getNameNoPath() const;      // Get the program name with no path
    // This pair of functions allows the user to iterate through all the procs
    // The procs will appear in order of native address
    Proc*   getFirstProc(PROGMAP::const_iterator& it);
    Proc*   getNextProc(PROGMAP::const_iterator& it);
    // Get the lower and upper limits of the text segment
    void    getTextLimits();

    // Lookup the given native address in the code section, returning
    // a host pointer corresponding to the same address
    const void* getCodeInfo(ADDRESS uAddr, const char*& last, int& delta);

    // Locate the starting address of "main", returning a native address
    ADDRESS getMainEntryPoint( void );

    // Public map from function name (string) to signature (list of
    // SemStr's (the actual string is blank; just have the type. One day
    // we may name library parameters))
    map<string, list<SemStr> > mapLibParam;

    // Public dictionary of instruction patterns, and other information
    // summarised from the SSL file (e.g. source machine's endianness)
    RTLInstDict RTLDict;

    // Public object that collects all the Call Sequence Analysis information
    // for the source processor
    CSR csrSrc;

    // Pointer to the BinaryFile object for the program, which contains the
    // program image. Created in main()
    BinaryFile* pBF;

    // Public object that keeps track of the coverage of the source program's
    // text segment
    Coverage cover;

    // Public booleans that are set if and when a register jump or call is
    // found, respectively
    bool        bRegisterJump;
    bool        bRegisterCall;

    // Public addresses being the lowest used native address (inclusive), and
    // the highest used address (not inclusive) in the text segment
    ADDRESS     limitTextLow;
    ADDRESS     limitTextHigh;
    // Also the difference between the host and native addresses (host - native)
    // At this stage, we are assuming that the difference is the same for all
    // text sections of the BinaryFile image
    int         textDelta;

protected:
    string      m_name;                 // name of the executable
    list<Proc*> m_procs;                // list of procedures
    PROGMAP     m_procLabels;           // map from address to Proc*
    int         m_iNumberedProc;        // Next numbered proc will use this
    string      m_progPath;             // String with the path to this exec
}; 


#endif
