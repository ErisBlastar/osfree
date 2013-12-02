
// � � ������ = �  �  � = ������ � �
// �                               �
//    ���������   ���  ����������          �  �����������������������������
// � ����������� ����� ����   ���� �               MSG-Help Calls        �
// � ����������� ����� ����   ���� �      � �������������������������������
// � ����������� ����� ����������  �       Section: MMOS/2 for eCS       �
// � ����   ���� ����� ���� ������ �     � Created: 28/10/02             �
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
#define INCL_BASE
#define INCL_DOSMODULEMGR
#include <os2.h>
#include <malloc.h>

#include <global.h>

#define CATEGORY_DOSSYS 0xD5
#define FUNCTION_REBOOT 0xAB

VOID SHUTDOWN_Reboot (void) {
   HFILE  DosHandle;
   ULONG  DosOpenAction;
   APIRET rc = -1;

   if (!DosOpen("DOS$", &DosHandle, &DosOpenAction, 0, FILE_NORMAL, FILE_OPEN, OPEN_ACCESS_READWRITE|OPEN_SHARE_DENYNONE, NULL)) {
      rc = DosDevIOCtl(DosHandle, CATEGORY_DOSSYS, FUNCTION_REBOOT, NULL, 0, NULL, NULL, 0, NULL);
      DosSleep (60000);
    }
   DosClose(DosHandle);
 }
