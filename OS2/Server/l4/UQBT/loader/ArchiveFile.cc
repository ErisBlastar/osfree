/* File: ArchiveFile.cc
 * Desc: This file contains the implementation of the ArchiveFile class
 *
 * Copyright (C) 1998, The University of Queensland, BT group
*/

#include "global.h"

ArchiveFile::ArchiveFile()		// Constructor
{
}

ArchiveFile::~ArchiveFile()		// Destructor
{
}

int ArchiveFile::GetNumMembers() const
{
	return m_FileMap.size();
}

const char* ArchiveFile::GetMemberFileName(int i) const
{
	return m_FileNames[i];
}

BinaryFile* ArchiveFile::GetMemberByProcName(const string& sSym)
{
	// Get the index
	int idx = m_SymMap[sSym];
	// Look it up
	return GetMember(idx);
}

BinaryFile* ArchiveFile::GetMemberByFileName(const string& sFile)
{
	// Get the index
	int idx = m_FileMap[sFile];
	// Look it up
	return GetMember(idx);
}

bool ArchiveFile::PostLoadMember(BinaryFile* pBF, void* handle)
{
	return pBF->PostLoad(handle);
}

