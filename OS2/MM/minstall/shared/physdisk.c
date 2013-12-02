
// � � ������ = �  �  � = ������ � �
// �                               �
//    ���������   ���  ����������          �  �����������������������������
// � ����������� ����� ����   ���� �           AiR-BOOT - Installer/2    �
// � ����������� ����� ����   ���� �      � �������������������������������
// � ����������� ����� ����������  �       Section: AiR-BOOTUP Package   �
// � ����   ���� ����� ���� ������ �     � Created: 24/10/02             �
// � ����   ����  ���  ����   ���� �     � Last Modified:                �
//                  ���                  � Number Of Modifications: 000  �
// �              ����             �     � INCs required: *none*         �
//      ������� ����                     � Written By: Martin Kiewitz    �
// �     ڿڿ�����������           �     � (c) Copyright by              �
// �     �ٳ������������           �     �      AiR ON-Line Software '02 �
// �    �������    ����            �     � All rights reserved.
// �              �������������    �    �������������������������������  �
// �             ����ݳ �ڿ���     �
// �            ����� �ĳ��ٳ�     �
//             ����������������     
// �             ��                �
// � Ĵ-=�i� ��-Li�� S��W���=-��� �

#define INCL_NOPMAPI
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#include <os2.h>
#include <malloc.h>

#include <global.h>

USHORT PHYS_EnumeratePhysicalDisks (void) {
   USHORT NumDrives = 0;

   if (DosPhysicalDisk(INFO_COUNT_PARTITIONABLE_DISKS, &NumDrives, sizeof(NumDrives),NULL, 0) != 0)
      return 0;
   return NumDrives;
 }

USHORT PHYS_GetIOCTLHandle (PSZ DriveID) {
   USHORT IOCTLHandle = 0;

   if (DosPhysicalDisk(INFO_GETIOCTLHANDLE, &IOCTLHandle, sizeof(IOCTLHandle),DriveID, 3) != 0)
      return 0;
   return IOCTLHandle;
 }

VOID PHYS_FreeIOCTLHandle (USHORT IOCTLHandle) {
   DosPhysicalDisk(INFO_FREEIOCTLHANDLE, NULL, 0, &IOCTLHandle, sizeof(IOCTLHandle));
   return;
 }

BOOL PHYS_ReadPhysicalSector (USHORT IOCTLHandle, UCHAR Cylinder, UCHAR Head, UCHAR SectorCount, PBYTE BufferPtr) {
   ULONG       TrackLayoutLen  = sizeof(TRACKLAYOUT)+sizeof(ULONG)*(SectorCount-1);
   TRACKLAYOUT *TrackLayoutPtr = malloc(TrackLayoutLen);
   ULONG       cbParms = sizeof(TrackLayoutPtr);
   ULONG       cbData  = 512;
   INT         i;

   TrackLayoutPtr->bCommand      = 0x01;
   TrackLayoutPtr->usHead        = Head;
   TrackLayoutPtr->usCylinder    = Cylinder;
   TrackLayoutPtr->usFirstSector = 0;
   TrackLayoutPtr->cSectors      = SectorCount;

   for (i=0; i<SectorCount; i++) {
      TrackLayoutPtr->TrackTable[i].usSectorNumber = i+1;
      TrackLayoutPtr->TrackTable[i].usSectorSize   = 512;
    }

   i = 1;
   if (DosDevIOCtl(IOCTLHandle, IOCTL_PHYSICALDISK, PDSK_READPHYSTRACK,
       TrackLayoutPtr, cbParms, &cbParms, BufferPtr, cbData, &cbData))
      i = 0;
   free (TrackLayoutPtr);
   return i;
 }

BOOL PHYS_WritePhysicalSector (USHORT IOCTLHandle, UCHAR Cylinder, UCHAR Head, UCHAR SectorCount, PBYTE BufferPtr) {
   ULONG       TrackLayoutLen  = sizeof(TRACKLAYOUT)+sizeof(ULONG)*(SectorCount-1);
   TRACKLAYOUT *TrackLayoutPtr = malloc(TrackLayoutLen);
   ULONG       cbParms = sizeof(TrackLayoutPtr);
   ULONG       cbData  = 512;
   INT         i;

   TrackLayoutPtr->bCommand      = 0x01;
   TrackLayoutPtr->usHead        = Head;
   TrackLayoutPtr->usCylinder    = Cylinder;
   TrackLayoutPtr->usFirstSector = 0;
   TrackLayoutPtr->cSectors      = SectorCount;

   for (i=0; i<SectorCount; i++) {
      TrackLayoutPtr->TrackTable[i].usSectorNumber = i+1;
      TrackLayoutPtr->TrackTable[i].usSectorSize   = 512;
    }

   i = 1;
   if (DosDevIOCtl(IOCTLHandle, IOCTL_PHYSICALDISK, PDSK_WRITEPHYSTRACK,
       TrackLayoutPtr, cbParms, &cbParms, BufferPtr, cbData, &cbData))
      i = 0;
   free (TrackLayoutPtr);
   return i;
 }

USHORT PHYS_GetGeometrySectorsPerTrack (USHORT IOCTLHandle) {
   DEVICEPARAMETERBLOCK DeviceParmBlock;
   ULONG                ulDataLength;
   
   if (!DosDevIOCtl(IOCTLHandle, IOCTL_PHYSICALDISK, PDSK_GETPHYSDEVICEPARAMS,
       NULL, 0, NULL, &DeviceParmBlock, sizeof(DeviceParmBlock), &ulDataLength))
      return DeviceParmBlock.cSectorsPerTrack;
   return 0;
 }

USHORT PHYS_GetGeometryHeadsPerCylinder (USHORT IOCTLHandle) {
   DEVICEPARAMETERBLOCK DeviceParmBlock;
   ULONG                ulDataLength;
   
   if (!DosDevIOCtl(IOCTLHandle, IOCTL_PHYSICALDISK, PDSK_GETPHYSDEVICEPARAMS,
       NULL, 0, NULL, &DeviceParmBlock, sizeof(DeviceParmBlock), &ulDataLength))
      return DeviceParmBlock.cHeads;
   return 0;
 }
