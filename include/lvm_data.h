/*
 *
 *   Copyright (c) International Business Machines  Corp., 2000
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or 
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software 
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Module: lvm_data.h
 */ 

/*
 * Change History:
 * 
 */

/*
 * Description:  This module defines the disk structures used by LVM,
 *               including that of the Master Boot Record (MBR) and
 *               Extended Boot Records (EBR).
 *
 * Notes: LVM Drive Letter Assignment Tables (DLA_Tables) appear on the
 *        last sector of each track containing a valid MBR or EBR.  Since
 *        partitions must be track aligned, any track containing an MBR or
 *        EBR will be almost all empty sectors.  We will grab the last
 *        of these empty sectors for our DLT_Tables.
 *
 */

/* XLATOFF */
#ifndef LVM_DATA_H_INCLUDED

#define LVM_DATA_H_INCLUDED  1
/* XLATON */

/* XLATOFF */
//#include "LVM_Gbls.h"
/* XLATON */

//#include "LVM_Cons.h"

/* XLATOFF */
//#include "LVM_Type.h"
/* XLATON */

#include <types.h>

/* Define the size of a Partition Name.  Partition Names are user defined names given to a partition. */
#define PARTITION_NAME_SIZE  20

/* Define the size of a volume name.  Volume Names are user defined names given to a volume. */
#define VOLUME_NAME_SIZE  20

/* Define the size of a disk name.  Disk Names are user defined names given to physical disk drives in the system. */
#define DISK_NAME_SIZE    20

/* The name of the filesystem in use on a partition.  This name may be up to 12 ( + NULL terminator) characters long. */
#define FILESYSTEM_NAME_SIZE 20

/* The comment field is reserved but is not currently used.  This is for future expansion and use. */
#define COMMENT_SIZE 81

/* Define the minimum number of sectors to reserve on the disk for Boot Manager. */
#define BOOT_MANAGER_SIZE   2048

/*--------------------------------------------------
 * Type definitions
 --------------------------------------------------*/

/* The following definitions define the drive letter assignment table used by LVM.
   For each partition table on the disk, there will be a drive letter assignment table in the last sector
   of the track containing the partition table. */

/* NOTE: DLA stands for Drive Letter Assignment. */

#define DLA_TABLE_SIGNATURE1  0x424D5202L
#define DLA_TABLE_SIGNATURE2  0x44464D50L


typedef struct _DLA_Entry { /* DE */
                             u32  Volume_Serial_Number;                        /* The serial number of the volume that t
his partition belongs to. */
                             u32  Partition_Serial_Number;                     /* The serial number of this partition. */
                             u32  Partition_Size;                              /* The size of the partition, in sectors. */
                             u32  Partition_Start;                             /* The starting sector of the partition. */
                             u8   On_Boot_Manager_Menu;                        /* Set to TRUE if this volume/partition is on the Boot Manager Menu. */
                             u8   Installable;                                 /* Set to TRUE if this volume is the one to install the operating system on. */
                             i8   Drive_Letter;                                /* The drive letter assigned to the partition. */
                             u8   Reserved;
                             i8   Volume_Name[VOLUME_NAME_SIZE];               /* The name assigned to the volume by the user. */
                             i8   Partition_Name[PARTITION_NAME_SIZE];         /* The name assigned to the partition. */
                          } DLA_Entry;
typedef struct _DLA_Table_Sector 
{ /* DTS */
   u32   DLA_Signature1;          /* The magic signature (part 1) of a Drive Letter Assignment Table. */
   u32   DLA_Signature2;          /* The magic signature (part 2) of a Drive Letter Assignment Table. */
   u32   DLA_CRC;                 /* The 32 bit CRC for this sector.  Calculated assuming that this */
                                  /* field and all unused space in the sector is 0. */
   u32   Disk_Serial_Number;      /* The serial number assigned to this disk. */
   u32   Boot_Disk_Serial_Number; /* The serial number of the disk used to boot the system.  
                                     This is for conflict resolution when multiple volumes
                                     want the same drive letter.  Since LVM.EXE will not let 
                                     this situation happen, the only way to get this situation
                                     is for the disk to have been altered by something other 
                                     than LVM.EXE, or if a disk drive has been moved from one
                                     machine to another.  If the drive has been moved, then 
                                     it should have a different Boot_Disk_Serial_Number.  Thus,
                                     we can tell which disk drive is the "foreign" drive and 
                                     therefore reject its claim for the drive letter in question.
                                     If we find that all of the claimaints have the same 
                                     Boot_Disk_Serial_Number, then we must assign drive letters on                                                                                  
                                     a first come, first serve basis.   */
   u32   Install_Flags;           /* Used by the Install program. */
   u32   Cylinders;
   u32   Heads_Per_Cylinder;
   u32   Sectors_Per_Track;
   i8    Disk_Name[DISK_NAME_SIZE];/* The name assigned to the disk containing this sector. */
   u8    Reboot;                   /* For use by Install.  Used to keep track of reboots initiated by install. */
   u8    Reserved[3];              /* Alignment. */
   DLA_Entry    DLA_Array[4];             /* These are the four entries which correspond to the entries in the partition table. */
} DLA_Table_Sector;


/* The following definitions define the LVM signature sector which will appear as the last sector in an LVM partition. */


#define  LVM_PRIMARY_SIGNATURE   0x4A435332L
#define  LVM_SECONDARY_SIGNATURE 0x4252444BL


#define  CURRENT_LVM_MAJOR_VERSION_NUMBER   2        /* Define as appropriate. */
#define  CURRENT_LVM_MINOR_VERSION_NUMBER   0        /* Define as appropriate. */


/* The following definitions limit the number of LVM features that can be applied to a volume, as well as defining a "NULL" feature for use in feature table entries that are not being used. */
#define  MAX_FEATURES_PER_VOLUME  10     /* The maximum number of LVM features that can be applied to a volume. */
#define  NULL_FEATURE              0     /* No feature.  Used in all unused entries of the feature array in the LVM Signature sector. */


/* The following structure is used to hold the location of the feature specific data for LVM features. */
typedef struct _LVM_Feature_Data { /* LFD */
                                   u32     Feature_ID;                            /* The ID of the feature. */
                                   u32     Location_Of_Primary_Feature_Data;      /* The PSN of the starting sector of the private data for this feature. */
                                   u32     Location_Of_Secondary_Feature_Data;    /* The PSN of the starting sector of the backup copy of the private data for this feature. */
                                   u32     Feature_Data_Size;                     /* The number of sectors used by this feature for its private data. */
                                   u16     Feature_Major_Version_Number;          /* The integer portion of the version number of this feature. */
                                   u16     Feature_Minor_Version_Number;          /* The decimal portion of the version number of this feature. */
                                   u8      Feature_Active;                        /* TRUE if this feature is active on this partition/volume, FALSE otherwise. */
                                   u8      Reserved[3];                           /* Alignment. */
                                 } LVM_Feature_Data;


/* The following structure defines the LVM Signature Sector.  This is the last sector of every partition which is part of an LVM volume.  It gives vital
   information about the version of LVM used to create the LVM volume that it is a part of, as well as which LVM features (BBR, drive linking, etc.) are
   active on the volume that this partition is a part of.                                                                      
                             */
typedef struct _LVM_Signature_Sector { /* LSS */
                                        u32        LVM_Signature1;                             /* The first part of the magic LVM signature. */
                                        u32        LVM_Signature2;                             /* The second part of the magic LVM signature. */
                                        u32        Signature_Sector_CRC;                       /* 32 bit CRC for this sector.  Calculated using 0 for this field. */
                                        u32        Partition_Serial_Number;                    /* The LVM assigned serial number for this partition.  */
                                        u32        Partition_Start;                            /* LBA of the first sector of this partition. */
                                        u32        Partition_End;                              /* LBA of the last sector of this partition. */
                                        u32        Partition_Sector_Count;                     /* The number of sectors in this partition. */
                                        u32        LVM_Reserved_Sector_Count;                  /* The number of sectors reserved for use by LVM. */
                                        u32        Partition_Size_To_Report_To_User;           /* The size of the partition as the user sees it - i.e. (the actual size of the partition - LVM reserved sectors) rounded to a track boundary. */
                                        u32        Boot_Disk_Serial_Number;                    /* The serial number of the boot disk for the system.  If the system contains Boot Manager, then this is the serial number of the disk containing the active copy of Boot Manager. */
                                        u32        Volume_Serial_Number;                       /* The serial number of the volume that this partition belongs to. */
                                        u32        Fake_EBR_Location;                          /* The location, on disk, of a Fake EBR, if one has been allocated. */
                                        u16        LVM_Major_Version_Number;                   /* Major version number of the LVM that created this partition. */
                                        u16        LVM_Minor_Version_Number;                   /* Minor version number of the LVM that created this partition. */
                                        i8         Partition_Name[PARTITION_NAME_SIZE];        /* User defined partition name. */
                                        i8         Volume_Name[VOLUME_NAME_SIZE];              /* The name of the volume that this partition belongs to. */
                                        LVM_Feature_Data  LVM_Feature_Array[MAX_FEATURES_PER_VOLUME]; /* The feature array.  This indicates which LVM features, if any, are active on this volume
                                                                                                         and what order they should be applied in.                                                  */
                                        i8         Drive_Letter;                               /* The drive letter assigned to the volume that this partition is part of. */
                                        u8         Fake_EBR_Allocated;                         /* If TRUE, then a fake EBR has been allocated. */
                                        i8         Comment[COMMENT_SIZE];                      /* User comment. */
                                        i8         Disk_Name[DISK_NAME_SIZE];                  /* Added to allow BBR to report the name of a disk when bad sectors are encountered on that disk. */
                                        u32        Sequence_Number;                            /* This indicates the order that partitions within a volume are used.  This number is 1 based.  A 0 here indicates that the volume was made by LVM Ver. 1. */
                                        u32        Next_Aggregate_Number;                      /* Used during volume creation and expansion when creating unique names for aggregates. */
                                        /* The remainder of the sector is reserved for future use and should be all zero or else the CRC will not come out correctly. */
                                     } LVM_Signature_Sector;


/* The following definitions define the format of a partition table and the Master Boot Record (MBR). */
typedef struct _Partition_Record { /* PR */
                                    u8       Boot_Indicator;    /* 80h = active partition. */
                                    u8       Starting_Head;
                                    u8       Starting_Sector;   /* Bits 0-5 are the sector.  Bits 6 and 7 are the high order bits of the starting cylinder. */
                                    u8       Starting_Cylinder; /* The cylinder number is a 10 bit value.  The high order bits of the 10 bit value come from bits 6 & 7 of the Starting_Sector field. */
                                    u8       Format_Indicator;  /* An indicator of the format/operation system on this partition. */
                                    u8       Ending_Head;
                                    u8       Ending_Sector;
                                    u8       Ending_Cylinder;
                                    u32      Sector_Offset;     /* The number of sectors on the disk which are prior to the start of this partition. */
                                    u32      Sector_Count;      /* The number of sectors in this partition. */
                                 } Partition_Record;

typedef struct _Master_Boot_Record { /* MBR */
                                      u8                Reserved[446];
                                      Partition_Record    Partition_Table[4];
                                      u16               Signature;            /* AA55h in this field indicates that this is a valid partition table/MBR. */
                                   } Master_Boot_Record;

typedef Master_Boot_Record  Extended_Boot_Record;

/* The following definition covers the Boot Manager Alias Table in the EBR.

   The Alias Table in the EBR has 2 entries in it, although only the first one is actually used.  */
#define ALIAS_NAME_SIZE  8
typedef struct _AliasTableEntry { /* ATE */
                                  u8     On_Boot_Manager_Menu;
                                  i8     Name[ALIAS_NAME_SIZE];
                                } AliasTableEntry;

#define ALIAS_TABLE_OFFSET  0x18A

/* XLATOFF */
/* The following text is used for the Boot Manager Alias for items that were placed on the Boot Manager Menu by FDISK and
   which have since been migrated to the new LVM format.  This text is put into the Name field of an AliasTableEntry so
   that, if FDISK ( or another program which understands the old Boot Manager Menu format) is run, it will display
   something for those partitions/volumes which are on the Boot Manager Menu.

   NOTE: This text must be exactly ALIAS_NAME_SIZE characters in length!                                                     */

#define ALIAS_TABLE_ENTRY_MIGRATION_TEXT       "--> LVM "
#define ALIAS_TABLE_ENTRY_MIGRATION_TEXT2      "--> LVM*"

/* XLATON */

/* The following is the signature used for an Master Boot Record, an Extended Boot Record, and a Boot Sector. */
#define MBR_EBR_SIGNATURE  0xAA55


/* The following list of definitions defines the values of interest for the Format_Indicator in a Partition_Record. */
#define EBR_INDICATOR                          0x5
#define WINDOZE_EBR_INDICATOR                  0xF
#define UNUSED_INDICATOR                       0x0
#define IFS_INDICATOR                          0x7
#define FAT12_INDICATOR                        0x1
#define FAT16_SMALL_PARTITION_INDICATOR        0x4
#define FAT16_LARGE_PARTITION_INDICATOR        0x6
#define BOOT_MANAGER_HIDDEN_PARTITION_FLAG     0x10
#define LVM_PARTITION_INDICATOR                0x35
#define BOOT_MANAGER_INDICATOR                 0x0A


/* The following is the signature used in the Boot Sector for Boot Manager. */
#define BOOT_MANAGER_SIGNATURE       "APJ&WN"


/* The following is used for determining the synthetic geometry reported for Volumes employing drive linking. */
#define SYNTHETIC_SECTORS_PER_TRACK  63


/* XLATOFF */
#endif
/* XLATON */
