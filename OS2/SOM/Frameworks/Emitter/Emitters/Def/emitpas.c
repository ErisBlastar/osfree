/* @(#) somc/gen_emit.efs 2.7 7/14/95 12:05:40 [7/30/96 14:44:56] */

/*
 *   COMPONENT_NAME: some
 *
 *   ORIGINS: 27
 *
 *
 *   10H9767, 10H9769  (C) COPYRIGHT International Business Machines Corp. 1992,1994
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */

/*
 *         File:    emitpas.c
 *       Author:    SOMObjects Emitter Framework
 *     Contents:    Generic framework emitter for Pascal.
 *         Date:    Tue Sep 21 20:16:44 2004.
 */

#pragma aux somtopenEmitFile "somtopenEmitFile"
#pragma aux somtfclose "somtfclose"
#pragma aux emit "emit"

#include <scentry.h>
#include <emitlib.h>
#include "pas.h"

//#define DEBUG

#ifdef DEBUG
  #define debug(s,...) printf(__FILE__"/"__func__"(): "##s"\n" ,##__VA_ARGS__)
#else
  #define debug(x, ...)
#endif



#define SYMBOLS_FILE   "lnk.efw"

SOMEXTERN FILE *emit(char *file, Entry * cls, Stab * stab)
{

    FILE *fp;
    FILE *deffile;
    SOMTClassEntryC *oCls;
    SOMTModuleEntryC *mod;
    PascalEmitter *emitter;
    SOMTTemplateOutputC *t;

    debug("�室 � ���\n");

    /* if this is a class, rather than a module: */
    if (cls->type == SOMTClassE) {
        debug("��ࠡ�⪠ �����\n");

        file=strcat(file, ""); //  ����䨪��� 䠩��. �᫨ ⠪�� ���� �� ������, � ����� ���� �࠯.
        fp = somtopenEmitFile(file, "lnk");
        debug("������� �����-� ��ꥪ� �࠯���\n");
        oCls = (SOMTClassEntryC *)somtGetObjectWrapper(cls);
        debug("������� ������� ����� ����\n");
        emitter = PascalEmitterNew();
        debug("����뢠�� 楫���� 䠩�\n");
        __set_somtTargetFile(emitter, fp);
        debug("����뢠�� 楫���� �����\n");
        __set_somtTargetClass(emitter, oCls);
        debug("����뢠�� ��� ����\n");
        __set_somtEmitterName(emitter, "pas");
        debug("����砥� 蠡���\n");
        t = __get_somtTemplate(emitter);
        debug("��⠭�������� ⨯ ������ਥ�\n");
        __set_somtCommentStyle(t, somtCPPE);
        debug("���뢠�� ᨬ����� 䠩�\n");
        if (deffile = _somtOpenSymbolsFile(emitter, SYMBOLS_FILE, "r")) {
            debug("��⠥� ��।������ ᥪ権\n");
            _somtReadSectionDefinitions(t, deffile);
            debug("����뢠�� 䠩�\n");
            somtfclose(deffile);
        }
        else {
            debug("�� ���� ������ 䠩� ᨬ����� \" %s \".\n",
                             SYMBOLS_FILE);
            exit(-1);
        }
        debug("������㥬 ᥪ樨\n");
        _somtGenerateSections(emitter);
        debug("�᢮������� ����� ����\n");
        _somFree(emitter);
        debug("�᢮������� ��ꥪ�-�࠯���\n");
        _somFree(oCls);

        return (fp);
    }
    else if (cls->type == SOMTModuleE) {
        debug("��ࠡ�⪠ �����\n");

        fp = somtopenEmitFile(file, "pas");
        mod = (SOMTModuleEntryC *) somtGetObjectWrapper(cls);
        emitter = PascalEmitterNew();
        __set_somtTargetFile(emitter, fp);
        __set_somtTargetModule(emitter, mod);
        t = __get_somtTemplate(emitter);
        __set_somtCommentStyle(t, somtCPPE);
        if (deffile = _somtOpenSymbolsFile(emitter, SYMBOLS_FILE, "r")) {
            _somtReadSectionDefinitions(t, deffile);
            somtfclose(deffile);
        }
        else {
            debug("Cannot open Symbols file \" %s \".\n",
                             SYMBOLS_FILE);
            exit(-1);
        }
        _somtGenerateSections(emitter);
        _somFree(emitter);
        _somFree(mod);

        return (fp);
    }
    else {
      debug("�������⭮, �� ������. ��室��.\n");
      return ((FILE *) NULL);
    }
}
