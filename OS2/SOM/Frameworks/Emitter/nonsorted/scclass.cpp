/**************************************************************************
 *
 *  Copyright 2015, Yuri Prokushev
 *
 *  This file is part of osFree project
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the
 *  Free Software Foundation, either version 2 of the License, or (at your
 *  option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#ifndef SOM_Module_scclass_Source
#define SOM_Module_scclass_Source
#endif

#define SOMTClassEntryC_Class_Source

#define __PRIVATE__
#include <rhbopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rhbsc.h"
#include "scclass.xih"

#include "scconst.xh"
#include "scattrib.xh"

SOM_Scope SOMTTypedefEntryC * SOMLINK somtGetFirstTypedef(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTTypedefEntryC * SOMLINK somtGetNextTypedef(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTEnumEntryC * SOMLINK somtGetNextEnum(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTPassthruEntryC * SOMLINK somtGetNextPassthru(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope boolean SOMLINK _get_somtForwardRef(SOMTClassEntryC * somSelf)
{
  return FALSE;
}

SOM_Scope long SOMLINK _get_somtPrivateMethodCount(SOMTClassEntryC * somSelf)
{
  return 0;
}

SOM_Scope SOMTSequenceEntryC * SOMLINK somtGetFirstSequence(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope boolean SOMLINK somtFilterOverridden(SOMTClassEntryC * somSelf,
                                               /* in */ SOMTMethodEntryC * entry)
{
  return FALSE;
}

SOM_Scope SOMTStructEntryC * SOMLINK somtGetNextStruct(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTModuleEntryC * SOMLINK _get_somtClassModule(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTBaseClassEntryC * SOMLINK somtGetNextBaseClass(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTDataEntryC * SOMLINK somtGetFirstData(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope long SOMLINK _get_somtPrivateDataCount(SOMTClassEntryC * somSelf)
{
  return 0;
}

SOM_Scope long SOMLINK _get_somtVAMethodCount(SOMTClassEntryC * somSelf)
{
  return 0;
}

SOM_Scope string SOMLINK _get_somtCScopedName(SOMTClassEntryC * somSelf)
{
  return SOMTClassEntryC_parent_SOMTEntryC__get_somtCScopedName(somSelf);
}

SOM_Scope void SOMLINK somtSetEntryStruct(SOMTClassEntryC * somSelf,
                                          /* inout */ Entry *es)
{
  SOMTClassEntryC_parent_SOMTEntryC_somtSetEntryStruct(somSelf, es);
}

SOM_Scope SOMTUnionEntryC * SOMLINK somtGetNextUnion(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTStructEntryC * SOMLINK somtGetFirstStruct(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTPassthruEntryC * SOMLINK somtGetFirstPassthru(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTEnumEntryC * SOMLINK somtGetFirstEnum(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTClassEntryC * SOMLINK _get_somtMetaclassFor(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope boolean SOMLINK _get_somtLocalInclude(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope long SOMLINK somtSetSymbolsOnEntry(SOMTClassEntryC * somSelf,
                                             /* in */ SOMTEmitC * emitter,
                                             /* in */ string prefix)
{
  string filestem;
  char buf[250];

  SOMTClassEntryCData *somThis = SOMTClassEntryCGetData(somSelf);
  SOMTClassEntryCMethodDebug("SOMTClassEntryC","somtSetSymbolsOnEntry");

  SOMTClassEntryC_parent_SOMTEntryC_somtSetSymbolsOnEntry(somSelf, emitter, prefix);

  SOMTTemplateOutputC *templ = emitter->_get_somtTemplate();
  
  buf[0]=0;
  strcat(buf, prefix);
  templ->somtSetSymbolCopyBoth(strcat(buf, "SourceFile"), somSelf->_get_somtEntryStruct()->data->defined_in->id);
  
  buf[0]=0;
  strcat(buf, prefix);
  if (somSelf->somtGetModifierValue("filestem"))
  {
    filestem=somSelf->somtGetModifierValue("filestem");
  } 
  else
  {
    filestem=somSelf->_get_somtEntryStruct()->filestem;
  };
  
  templ->somtSetSymbolCopyBoth(strcat(buf, "SourceFileStem"), filestem);

  buf[0]=0;
  strcat(buf, prefix);
  templ->somtSetSymbolCopyBoth(strcat(buf, "ReleaseOrder"), somSelf->somtGetModifierValue("releaseorder"));

  buf[0]=0;
  strcat(buf, prefix);
  templ->somtSetSymbolCopyBoth(strcat(buf, "MajorVersion"), somSelf->somtGetModifierValue("majorversion"));

  buf[0]=0;
  strcat(buf, prefix);
  templ->somtSetSymbolCopyBoth(strcat(buf, "MinorVersion"), somSelf->somtGetModifierValue("minorversion"));

  buf[0]=0;
  strcat(buf, prefix);
  templ->somtSetSymbolCopyBoth(strcat(buf, "Include"), somSelf->_get_somtEntryStruct()->data->defined_in->id);

  return 1;
}

SOM_Scope SOMTDataEntryC * SOMLINK somtGetNextData(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTConstEntryC * SOMLINK somtGetFirstConstant(SOMTClassEntryC * somSelf)
{
  SOMTClassEntryCData *somThis = SOMTClassEntryCGetData(somSelf);
  SOMTClassEntryCMethodDebug("SOMTClassEntryC","somtGetFirstConstant");
  RHBelement * e=somSelf->_get_somtEntryStruct()->data->children();
	while (e)
	{
    RHBconstant *m=e->is_constant();
    if (m) 
    {
      if (_cons)
      {
        SOMFree(_cons->_get_somtEntryStruct());
        delete _cons;
      }
      _cons=new SOMTConstEntryC();
      Entry *es=(Entry *)SOMMalloc(sizeof(Entry));
      es->name=m->id;
      es->type=SOMTConstE;
      es->data=m;
      es->filestem=somSelf->_get_somtEntryStruct()->filestem;
      _cons->somtSetEntryStruct(es);
      return _cons;
    }
		e=e->next();
	}
  return NULL;
}

SOM_Scope SOMTConstEntryC * SOMLINK somtGetNextConstant(SOMTClassEntryC * somSelf)
{
  SOMTClassEntryCData *somThis = SOMTClassEntryCGetData(somSelf);
  SOMTClassEntryCMethodDebug("SOMTClassEntryC","somtGetNextConstant");
  RHBelement * e=_cons->_get_somtEntryStruct()->data;
  e=e->next();
	while (e)
	{
    RHBconstant *m=e->is_constant();
    if (m) 
    {
      Entry *es=(Entry *)SOMMalloc(sizeof(Entry));
      es->name=m->id;
      es->type=SOMTConstE;
      es->data=m;
      es->filestem=somSelf->_get_somtEntryStruct()->filestem;
      
      //SOMFree(_cons->_get_somtEntryStruct());
      //delete _cons;
      _cons=new SOMTConstEntryC();
      _cons->somtSetEntryStruct(es);
      return _cons;
    }
		e=e->next();
	}
  return NULL;
}

SOM_Scope SOMTAttributeEntryC * SOMLINK somtGetFirstAttribute(SOMTClassEntryC * somSelf)
{
  SOMTClassEntryCData *somThis = SOMTClassEntryCGetData(somSelf);
  SOMTClassEntryCMethodDebug("SOMTClassEntryC","somtGetFirstAttribute");
  RHBelement * e=somSelf->_get_somtEntryStruct()->data->children();
	while (e)
	{
    RHBattribute *m=e->is_attribute();
    if (m) 
    {
      if (_attrs)
      {
        SOMFree(_attrs->_get_somtEntryStruct());
        delete _attrs;
      }
      _attrs=new SOMTAttributeEntryC();
      Entry *es=(Entry *)SOMMalloc(sizeof(Entry));
      es->name=m->id;
      es->type=SOMTAttE;
      es->data=m;
      es->filestem=somSelf->_get_somtEntryStruct()->filestem;
      _attrs->somtSetEntryStruct(es);

      return _attrs;
    }
		e=e->next();
	}
  return NULL;
}

SOM_Scope SOMTAttributeEntryC * SOMLINK somtGetNextAttribute(SOMTClassEntryC * somSelf)
{
  SOMTClassEntryCData *somThis = SOMTClassEntryCGetData(somSelf);
  SOMTClassEntryCMethodDebug("SOMTClassEntryC","somtGetNextAttribute");
  RHBelement * e=_attrs->_get_somtEntryStruct()->data;
  e=e->next();
	while (e)
	{
    RHBattribute *m=e->is_attribute();
    if (m) 
    {
      Entry *es=(Entry *)SOMMalloc(sizeof(Entry));
      es->name=m->id;
      es->type=SOMTAttE;
      es->data=m;
      es->filestem=somSelf->_get_somtEntryStruct()->filestem;
        printf("123\n");
      //SOMFree(_attrs->_get_somtEntryStruct());
      //delete _attrs;
      _attrs=new SOMTAttributeEntryC();
      _attrs->somtSetEntryStruct(es);
      return _attrs;
    }
		e=e->next();
	}
  return NULL;
}

SOM_Scope SOMTMethodEntryC * SOMLINK somtGetNextMethod(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope void SOMLINK somDumpSelfInt(SOMTClassEntryC * somSelf,
                                      /* in */ long level)
{
}


SOM_Scope SOMTMetaClassEntryC * SOMLINK _get_somtMetaClassEntry(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope long SOMLINK _get_somtStaticMethodCount(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTDataEntryC * SOMLINK somtGetNextStaticData(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope long SOMLINK _get_somtNewMethodCount(SOMTClassEntryC * somSelf)
{
  return 0;
}

SOM_Scope SOMTBaseClassEntryC * SOMLINK somtGetFirstBaseClass(SOMTClassEntryC * somSelf)
{
  SOMTClassEntryCData *somThis = SOMTClassEntryCGetData(somSelf);
  SOMTClassEntryCMethodDebug("SOMTClassEntryC","somtGetFirstBaseClass");

  RHBelement * e=somSelf->_get_somtEntryStruct()->data->_parents();
	while (e)
	{
    RHBconstant *m=e->is_interface();
    if (m) 
    {
      if (_cons)
      {
        SOMFree(_cons->_get_somtEntryStruct());
        delete _cons;
      }
      _cons=new SOMTConstEntryC();
      Entry *es=(Entry *)SOMMalloc(sizeof(Entry));
      es->name=m->id;
      es->type=SOMTConstE;
      es->data=m;
      es->filestem=somSelf->_get_somtEntryStruct()->filestem;
      _cons->somtSetEntryStruct(es);
      return _cons;
    }
		e=e->next();
	}
  return NULL;
  return NULL;
}

SOM_Scope long SOMLINK _get_somtOverrideMethodCount(SOMTClassEntryC * somSelf)
{
  return 0;
}

SOM_Scope SOMTMethodEntryC * SOMLINK somtGetFirstMethod(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTEntryC * SOMLINK somtGetNextPubdef(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTMethodEntryC * SOMLINK somtGetNextInheritedMethod(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope boolean SOMLINK somtFilterPrivOrPub(SOMTClassEntryC * somSelf,
                                              /* in */ SOMTCommonEntryC * entry)
{
  return FALSE;
}

SOM_Scope SOMTUnionEntryC * SOMLINK somtGetFirstUnion(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTSequenceEntryC * SOMLINK somtGetNextSequence(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope long SOMLINK somtGetReleaseNameList(SOMTClassEntryC * somSelf,
                                              /* in */ string buffer)
{
  return 0;
}

SOM_Scope long SOMLINK _get_somtBaseCount(SOMTClassEntryC * somSelf)
{
  return 0;
}

SOM_Scope boolean SOMLINK somtFilterNew(SOMTClassEntryC * somSelf,
                                        /* in */ SOMTMethodEntryC * entry)
{
  return FALSE;
}

SOM_Scope string SOMLINK _get_somtIDLScopedName(SOMTClassEntryC * somSelf)
{
  return SOMTClassEntryC_parent_SOMTEntryC__get_somtIDLScopedName(somSelf);
}

SOM_Scope long SOMLINK _get_somtPublicDataCount(SOMTClassEntryC * somSelf)
{
  return 0;
}

SOM_Scope string SOMLINK _get_somtSourceFileName(SOMTClassEntryC * somSelf)
{
  SOMTClassEntryCData *somThis = SOMTClassEntryCGetData(somSelf);
  SOMTClassEntryCMethodDebug("SOMTClassEntryC","_get_somtSourceFileName");
  
  Entry *es=SOMTClassEntryC_parent_SOMTEntryC__get_somtEntryStruct(somSelf);
  
  return es->data->defined_in->id;
}

SOM_Scope long SOMLINK _get_somtProcMethodCount(SOMTClassEntryC * somSelf)
{
  return 0;
}

SOM_Scope SOMTMethodEntryC * SOMLINK somtGetFirstInheritedMethod(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTEntryC * SOMLINK somtGetFirstPubdef(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope SOMTDataEntryC * SOMLINK somtGetFirstStaticData(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope string SOMLINK somtGetFirstReleaseName(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope string SOMLINK somtGetNextReleaseName(SOMTClassEntryC * somSelf)
{
  return NULL;
}

SOM_Scope long SOMLINK _get_somtExternalDataCount(SOMTClassEntryC * somSelf)
{
  return 0;
}


SOM_Scope void SOMLINK somDefaultInit(SOMTClassEntryC SOMSTAR somSelf, somInitCtrl *ctrl)
{
  SOMTClassEntryCData *somThis; // set by BeginInitializer 
  somInitCtrl globalCtrl;
  somBooleanVector myMask;
  SOMTClassEntryCMethodDebug("SOMTClassEntryC", "somDefaultInit");
  
  SOMTClassEntryC_BeginInitializer_somDefaultInit;
  SOMTClassEntryC_Init_SOMTEntryC_somDefaultInit(somSelf, ctrl);

  _cons=NULL;
  _attrs=NULL;
}
