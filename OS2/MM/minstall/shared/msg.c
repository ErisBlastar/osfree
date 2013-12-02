
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <global.h>
#include <msg.h>

PCHAR MSG_Filename                    = 0;
CHAR  MSG_Insert[5][MSG_INSERTMAXLEN] = {{0}};
PCHAR MSG_InsertTable[9]              = {(PCHAR)&MSG_Insert[0], (PCHAR)&MSG_Insert[1],
                                         (PCHAR)&MSG_Insert[2], (PCHAR)&MSG_Insert[3],
                                         (PCHAR)&MSG_Insert[4], 0, 0, 0, 0};

BOOL MSG_Init (PSZ MsgFile) {
   CHAR   TempBuffer[1024];

   MSG_Filename = MsgFile;
   if (MSG_Get(TempBuffer, 1024, 0))
      return TRUE;
   printf ("%s not found, execution aborted.\n", MSG_Filename);
   return FALSE;
 }

VOID MSG_Print (MSGID MessageID) {
   CHAR   TempBuffer[2048];
   ULONG  MessageLen = 0;

   if (DosGetMessage(MSG_InsertTable, 5, TempBuffer, 2048, (ULONG)MessageID, MSG_Filename, &MessageLen))
      return;
   TempBuffer[MessageLen] = 0;
   printf (TempBuffer);
 }

VOID MSG_fPrint (HFILE FileHandle, MSGID MessageID) {
   CHAR   TempBuffer[2048];
   ULONG  MessageLen = 0;
   ULONG  Written;

   if (DosGetMessage(MSG_InsertTable, 5, TempBuffer, 2048, (ULONG)MessageID, MSG_Filename, &MessageLen))
      return;
   TempBuffer[MessageLen] = 0;
   DosWrite (FileHandle, &TempBuffer, MessageLen, &Written);
 }

BOOL MSG_Get (PCHAR DestPtr, ULONG DestMaxLength, MSGID MessageID) {
   ULONG  MessageLen = 0;

   if (DosGetMessage(MSG_InsertTable, 5, DestPtr, DestMaxLength, (ULONG)MessageID, MSG_Filename, &MessageLen))
      return FALSE;
   DestPtr[MessageLen] = 0;
   return TRUE;
 }

BOOL MSG_FillInsert (ULONG InsertNo, MSGID MessageID) {
   CHAR   TempBuffer[MSG_INSERTMAXLEN];
   ULONG  MessageLen = 0;

   if ((InsertNo==0) || (InsertNo>5)) return FALSE;
   InsertNo--;
   if (DosGetMessage(MSG_InsertTable, 5, TempBuffer, MSG_INSERTMAXLEN, (ULONG)MessageID, MSG_Filename, &MessageLen)) {
      MSG_Insert[InsertNo][0] = 0;
      return FALSE;
    }
   memcpy (&MSG_Insert[InsertNo], TempBuffer, MessageLen);
   MSG_Insert[InsertNo][MessageLen] = 0;
   return TRUE;
 }

BOOL MSG_SetInsertViaPSZ (ULONG InsertNo, PSZ String) {
   ULONG  StringLen = strlen(String);

   if ((InsertNo==0) || (InsertNo>5)) return FALSE;
   InsertNo--;
   if (StringLen>=MSG_INSERTMAXLEN) StringLen = MSG_INSERTMAXLEN-1;
   memcpy (MSG_Insert[InsertNo], String, StringLen);
   MSG_Insert[InsertNo][StringLen] = 0;
   return TRUE;
 }

BOOL MSG_SetInsertViaString (ULONG InsertNo, PCHAR StringPtr, ULONG StringLen) {
   if ((InsertNo==0) || (InsertNo>5)) return FALSE;
   InsertNo--;
   if (StringLen>=MSG_INSERTMAXLEN) StringLen = MSG_INSERTMAXLEN-1;
   memcpy (MSG_Insert[InsertNo], StringPtr, StringLen);
   MSG_Insert[InsertNo][StringLen] = 0;
   return TRUE;
 }

BOOL MSG_SetInsertFileLocation (ULONG InsertNo, PSZ FileName, ULONG LineNo) {
   ULONG  StringLen = strlen(FileName);

   if ((InsertNo==0) || (InsertNo>5)) return FALSE;
   InsertNo--;
   if (StringLen>=(MSG_INSERTMAXLEN-6)) StringLen = MSG_INSERTMAXLEN-7;
   memcpy (MSG_Insert[InsertNo], FileName, StringLen);
   MSG_Insert[InsertNo][StringLen] = ':';
   ultoa (LineNo, &MSG_Insert[InsertNo][StringLen+1], 10);
   return TRUE;
 }
