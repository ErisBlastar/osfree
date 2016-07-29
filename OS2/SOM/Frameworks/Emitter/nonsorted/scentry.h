/* generated from scentry.idl */
/* internal conditional is SOM_Module_scentry_Source */
#ifndef SOM_Module_scentry_Header_h
	#define SOM_Module_scentry_Header_h 1
	#include <som.h>
	#include <somobj.h>
	#include <sctypes.h>
	#include <sm.h>
	#include <somstrt.h>
	#ifndef _IDL_SOMTEmitC_defined
		#define _IDL_SOMTEmitC_defined
		typedef SOMObject SOMTEmitC;
	#endif /* _IDL_SOMTEmitC_defined */
	#ifndef _IDL_SEQUENCE_SOMTEmitC_defined
		#define _IDL_SEQUENCE_SOMTEmitC_defined
		SOM_SEQUENCE_TYPEDEF_NAME(SOMTEmitC SOMSTAR ,sequence(SOMTEmitC));
	#endif /* _IDL_SEQUENCE_SOMTEmitC_defined */
	#ifndef _IDL_SOMTEntryC_defined
		#define _IDL_SOMTEntryC_defined
		typedef SOMObject SOMTEntryC;
	#endif /* _IDL_SOMTEntryC_defined */
	#ifndef _IDL_SEQUENCE_SOMTEntryC_defined
		#define _IDL_SEQUENCE_SOMTEntryC_defined
		SOM_SEQUENCE_TYPEDEF_NAME(SOMTEntryC SOMSTAR ,sequence(SOMTEntryC));
	#endif /* _IDL_SEQUENCE_SOMTEntryC_defined */
	#ifdef __IBMC__
		typedef Entry *(somTP_SOMTEntryC__get_somtEntryStruct)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtEntryStruct,system)
		typedef somTP_SOMTEntryC__get_somtEntryStruct *somTD_SOMTEntryC__get_somtEntryStruct;
	#else /* __IBMC__ */
		typedef Entry *(SOMLINK * somTD_SOMTEntryC__get_somtEntryStruct)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef void (somTP_SOMTEntryC__set_somtEntryName)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ string somtEntryName);
		#pragma linkage(somTP_SOMTEntryC__set_somtEntryName,system)
		typedef somTP_SOMTEntryC__set_somtEntryName *somTD_SOMTEntryC__set_somtEntryName;
	#else /* __IBMC__ */
		typedef void (SOMLINK * somTD_SOMTEntryC__set_somtEntryName)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ string somtEntryName);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef string (somTP_SOMTEntryC__get_somtEntryName)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtEntryName,system)
		typedef somTP_SOMTEntryC__get_somtEntryName *somTD_SOMTEntryC__get_somtEntryName;
	#else /* __IBMC__ */
		typedef string (SOMLINK * somTD_SOMTEntryC__get_somtEntryName)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef void (somTP_SOMTEntryC__set_somtElementType)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ SOMTTypes somtElementType);
		#pragma linkage(somTP_SOMTEntryC__set_somtElementType,system)
		typedef somTP_SOMTEntryC__set_somtElementType *somTD_SOMTEntryC__set_somtElementType;
	#else /* __IBMC__ */
		typedef void (SOMLINK * somTD_SOMTEntryC__set_somtElementType)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ SOMTTypes somtElementType);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef SOMTTypes (somTP_SOMTEntryC__get_somtElementType)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtElementType,system)
		typedef somTP_SOMTEntryC__get_somtElementType *somTD_SOMTEntryC__get_somtElementType;
	#else /* __IBMC__ */
		typedef SOMTTypes (SOMLINK * somTD_SOMTEntryC__get_somtElementType)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef string (somTP_SOMTEntryC__get_somtElementTypeName)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtElementTypeName,system)
		typedef somTP_SOMTEntryC__get_somtElementTypeName *somTD_SOMTEntryC__get_somtElementTypeName;
	#else /* __IBMC__ */
		typedef string (SOMLINK * somTD_SOMTEntryC__get_somtElementTypeName)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef string (somTP_SOMTEntryC__get_somtEntryComment)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtEntryComment,system)
		typedef somTP_SOMTEntryC__get_somtEntryComment *somTD_SOMTEntryC__get_somtEntryComment;
	#else /* __IBMC__ */
		typedef string (SOMLINK * somTD_SOMTEntryC__get_somtEntryComment)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef unsigned long (somTP_SOMTEntryC__get_somtSourceLineNumber)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtSourceLineNumber,system)
		typedef somTP_SOMTEntryC__get_somtSourceLineNumber *somTD_SOMTEntryC__get_somtSourceLineNumber;
	#else /* __IBMC__ */
		typedef unsigned long (SOMLINK * somTD_SOMTEntryC__get_somtSourceLineNumber)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef TypeCode (somTP_SOMTEntryC__get_somtTypeCode)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtTypeCode,system)
		typedef somTP_SOMTEntryC__get_somtTypeCode *somTD_SOMTEntryC__get_somtTypeCode;
	#else /* __IBMC__ */
		typedef TypeCode (SOMLINK * somTD_SOMTEntryC__get_somtTypeCode)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef boolean (somTP_SOMTEntryC__get_somtIsReference)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtIsReference,system)
		typedef somTP_SOMTEntryC__get_somtIsReference *somTD_SOMTEntryC__get_somtIsReference;
	#else /* __IBMC__ */
		typedef boolean (SOMLINK * somTD_SOMTEntryC__get_somtIsReference)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef string (somTP_SOMTEntryC__get_somtIDLScopedName)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtIDLScopedName,system)
		typedef somTP_SOMTEntryC__get_somtIDLScopedName *somTD_SOMTEntryC__get_somtIDLScopedName;
	#else /* __IBMC__ */
		typedef string (SOMLINK * somTD_SOMTEntryC__get_somtIDLScopedName)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef string (somTP_SOMTEntryC__get_somtCScopedName)(
			SOMTEntryC SOMSTAR somSelf);
		#pragma linkage(somTP_SOMTEntryC__get_somtCScopedName,system)
		typedef somTP_SOMTEntryC__get_somtCScopedName *somTD_SOMTEntryC__get_somtCScopedName;
	#else /* __IBMC__ */
		typedef string (SOMLINK * somTD_SOMTEntryC__get_somtCScopedName)(
			SOMTEntryC SOMSTAR somSelf);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef string (somTP_SOMTEntryC_somtGetModifierValue)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ string modifierName);
		#pragma linkage(somTP_SOMTEntryC_somtGetModifierValue,system)
		typedef somTP_SOMTEntryC_somtGetModifierValue *somTD_SOMTEntryC_somtGetModifierValue;
	#else /* __IBMC__ */
		typedef string (SOMLINK * somTD_SOMTEntryC_somtGetModifierValue)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ string modifierName);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef boolean (somTP_SOMTEntryC_somtGetFirstModifier)(
			SOMTEntryC SOMSTAR somSelf,
			/* inout */ string *modifierName,
			/* inout */ string *modifierValue);
		#pragma linkage(somTP_SOMTEntryC_somtGetFirstModifier,system)
		typedef somTP_SOMTEntryC_somtGetFirstModifier *somTD_SOMTEntryC_somtGetFirstModifier;
	#else /* __IBMC__ */
		typedef boolean (SOMLINK * somTD_SOMTEntryC_somtGetFirstModifier)(
			SOMTEntryC SOMSTAR somSelf,
			/* inout */ string *modifierName,
			/* inout */ string *modifierValue);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef boolean (somTP_SOMTEntryC_somtGetNextModifier)(
			SOMTEntryC SOMSTAR somSelf,
			/* inout */ string *modifierName,
			/* inout */ string *modifierValue);
		#pragma linkage(somTP_SOMTEntryC_somtGetNextModifier,system)
		typedef somTP_SOMTEntryC_somtGetNextModifier *somTD_SOMTEntryC_somtGetNextModifier;
	#else /* __IBMC__ */
		typedef boolean (SOMLINK * somTD_SOMTEntryC_somtGetNextModifier)(
			SOMTEntryC SOMSTAR somSelf,
			/* inout */ string *modifierName,
			/* inout */ string *modifierValue);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef long (somTP_SOMTEntryC_somtFormatModifier)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ string buffer,
			/* in */ string name,
			/* in */ string value);
		#pragma linkage(somTP_SOMTEntryC_somtFormatModifier,system)
		typedef somTP_SOMTEntryC_somtFormatModifier *somTD_SOMTEntryC_somtFormatModifier;
	#else /* __IBMC__ */
		typedef long (SOMLINK * somTD_SOMTEntryC_somtFormatModifier)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ string buffer,
			/* in */ string name,
			/* in */ string value);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef long (somTP_SOMTEntryC_somtGetModifierList)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ string buffer);
		#pragma linkage(somTP_SOMTEntryC_somtGetModifierList,system)
		typedef somTP_SOMTEntryC_somtGetModifierList *somTD_SOMTEntryC_somtGetModifierList;
	#else /* __IBMC__ */
		typedef long (SOMLINK * somTD_SOMTEntryC_somtGetModifierList)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ string buffer);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef long (somTP_SOMTEntryC_somtSetSymbolsOnEntry)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ SOMTEmitC SOMSTAR emitter,
			/* in */ string prefix);
		#pragma linkage(somTP_SOMTEntryC_somtSetSymbolsOnEntry,system)
		typedef somTP_SOMTEntryC_somtSetSymbolsOnEntry *somTD_SOMTEntryC_somtSetSymbolsOnEntry;
	#else /* __IBMC__ */
		typedef long (SOMLINK * somTD_SOMTEntryC_somtSetSymbolsOnEntry)(
			SOMTEntryC SOMSTAR somSelf,
			/* in */ SOMTEmitC SOMSTAR emitter,
			/* in */ string prefix);
	#endif /* __IBMC__ */
	#ifdef __IBMC__
		typedef void (somTP_SOMTEntryC_somtSetEntryStruct)(
			SOMTEntryC SOMSTAR somSelf,
			/* inout */ Entry *es);
		#pragma linkage(somTP_SOMTEntryC_somtSetEntryStruct,system)
		typedef somTP_SOMTEntryC_somtSetEntryStruct *somTD_SOMTEntryC_somtSetEntryStruct;
	#else /* __IBMC__ */
		typedef void (SOMLINK * somTD_SOMTEntryC_somtSetEntryStruct)(
			SOMTEntryC SOMSTAR somSelf,
			/* inout */ Entry *es);
	#endif /* __IBMC__ */
	#ifndef SOMTEntryC_MajorVersion
		#define SOMTEntryC_MajorVersion   2
	#endif /* SOMTEntryC_MajorVersion */
	#ifndef SOMTEntryC_MinorVersion
		#define SOMTEntryC_MinorVersion   1
	#endif /* SOMTEntryC_MinorVersion */
	typedef struct SOMTEntryCClassDataStructure
	{
		SOMClass SOMSTAR classObject;
		somMToken _get_somtEntryName;
		somMToken _set_somtEntryName;
		somMToken _get_somtElementType;
		somMToken _set_somtElementType;
		somMToken _get_somtEntryComment;
		somMToken _get_somtSourceLineNumber;
		somMToken _get_somtTypeCode;
		somMToken _get_somtIsReference;
		somMToken somtGetModifierValue;
		somMToken somtGetFirstModifier;
		somMToken somtGetNextModifier;
		somMToken somtFormatModifier;
		somMToken somtGetModifierList;
		somMToken somtSetSymbolsOnEntry;
		somMToken somtSetEntryStruct;
		somMToken _get_somtEntryStruct;
		somMToken somtShowAssocEntry;
		somMToken _get_somtCScopedName;
		somMToken _get_somtIDLScopedName;
		somMToken _get_somtElementTypeName;
	} SOMTEntryCClassDataStructure;
	typedef struct SOMTEntryCCClassDataStructure
	{
		somMethodTabs parentMtab;
		somDToken instanceDataToken;
	} SOMTEntryCCClassDataStructure;
	#ifdef SOM_RESOLVE_DATA
		#if defined(SOM_Module_scentry_Source) || defined(SOMTEntryC_Class_Source)
			SOMEXTERN struct SOMTEntryCClassDataStructure _SOMTEntryCClassData;
			#ifndef SOMTEntryCClassData
				#define SOMTEntryCClassData    _SOMTEntryCClassData
			#endif /* SOMTEntryCClassData */
		#else
			SOMEXTERN struct SOMTEntryCClassDataStructure * SOMLINK resolve_SOMTEntryCClassData(void);
			#ifndef SOMTEntryCClassData
				#define SOMTEntryCClassData    (*(resolve_SOMTEntryCClassData()))
			#endif /* SOMTEntryCClassData */
		#endif
	#else /* SOM_RESOLVE_DATA */
		SOMEXTERN 
		#if defined(SOM_Module_scentry_Source) || defined(SOMTEntryC_Class_Source)
			#ifdef SOMDLLEXPORT
				SOMDLLEXPORT
			#endif /* SOMDLLEXPORT */
		#else /* SOM_Module_scentry_Source || SOMTEntryC_Class_Source */
			#ifdef SOMDLLIMPORT
				SOMDLLIMPORT
			#endif /* SOMDLLIMPORT */
		#endif /* SOM_Module_scentry_Source || SOMTEntryC_Class_Source */
		struct SOMTEntryCClassDataStructure SOMDLINK SOMTEntryCClassData;
	#endif /* SOM_RESOLVE_DATA */
	#ifdef SOM_RESOLVE_DATA
		#if defined(SOM_Module_scentry_Source) || defined(SOMTEntryC_Class_Source)
			SOMEXTERN struct SOMTEntryCCClassDataStructure _SOMTEntryCCClassData;
			#ifndef SOMTEntryCCClassData
				#define SOMTEntryCCClassData    _SOMTEntryCCClassData
			#endif /* SOMTEntryCCClassData */
		#else
			SOMEXTERN struct SOMTEntryCCClassDataStructure * SOMLINK resolve_SOMTEntryCCClassData(void);
			#ifndef SOMTEntryCCClassData
				#define SOMTEntryCCClassData    (*(resolve_SOMTEntryCCClassData()))
			#endif /* SOMTEntryCCClassData */
		#endif
	#else /* SOM_RESOLVE_DATA */
		SOMEXTERN 
		#if defined(SOM_Module_scentry_Source) || defined(SOMTEntryC_Class_Source)
			#ifdef SOMDLLEXPORT
				SOMDLLEXPORT
			#endif /* SOMDLLEXPORT */
		#else /* SOM_Module_scentry_Source || SOMTEntryC_Class_Source */
			#ifdef SOMDLLIMPORT
				SOMDLLIMPORT
			#endif /* SOMDLLIMPORT */
		#endif /* SOM_Module_scentry_Source || SOMTEntryC_Class_Source */
		struct SOMTEntryCCClassDataStructure SOMDLINK SOMTEntryCCClassData;
	#endif /* SOM_RESOLVE_DATA */
	extern 
	#if defined(SOM_Module_scentry_Source) || defined(SOMTEntryC_Class_Source)
		#ifdef SOMDLLEXPORT
			SOMDLLEXPORT
		#endif /* SOMDLLEXPORT */
	#else /* SOM_Module_scentry_Source || SOMTEntryC_Class_Source */
		#ifdef SOMDLLIMPORT
			SOMDLLIMPORT
		#endif /* SOMDLLIMPORT */
	#endif /* SOM_Module_scentry_Source || SOMTEntryC_Class_Source */
	SOMClass SOMSTAR SOMLINK SOMTEntryCNewClass
			(integer4 somtmajorVersion,integer4 somtminorVersion);
	#define _SOMCLASS_SOMTEntryC (SOMTEntryCClassData.classObject)
	#ifndef SOMGD_SOMTEntryC
		#if (defined(_SOMTEntryC) || defined(__SOMTEntryC))
			#undef _SOMTEntryC
			#undef __SOMTEntryC
			#define SOMGD_SOMTEntryC 1
		#else
			#define _SOMTEntryC _SOMCLASS_SOMTEntryC
		#endif /* _SOMTEntryC */
	#endif /* SOMGD_SOMTEntryC */
	#define SOMTEntryC_classObj _SOMCLASS_SOMTEntryC
	#define _SOMMTOKEN_SOMTEntryC(method) ((somMToken)(SOMTEntryCClassData.method))
	#ifndef SOMTEntryCNew
		#define SOMTEntryCNew() ( _SOMTEntryC ? \
			(SOMClass_somNew(_SOMTEntryC)) : \
			( SOMTEntryCNewClass( \
				SOMTEntryC_MajorVersion, \
				SOMTEntryC_MinorVersion), \
			SOMClass_somNew(_SOMTEntryC))) 
	#endif /* NewSOMTEntryC */
	#ifndef SOMTEntryC__get_somtEntryStruct
		#define SOMTEntryC__get_somtEntryStruct(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtEntryStruct)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtEntryStruct
				#if defined(__get_somtEntryStruct)
					#undef __get_somtEntryStruct
					#define SOMGD___get_somtEntryStruct
				#else
					#define __get_somtEntryStruct SOMTEntryC__get_somtEntryStruct
				#endif
			#endif /* SOMGD___get_somtEntryStruct */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtEntryStruct */
	#ifndef SOMTEntryC__set_somtEntryName
		#define SOMTEntryC__set_somtEntryName(somSelf,somtEntryName) \
			SOM_Resolve(somSelf,SOMTEntryC,_set_somtEntryName)  \
				(somSelf,somtEntryName)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___set_somtEntryName
				#if defined(__set_somtEntryName)
					#undef __set_somtEntryName
					#define SOMGD___set_somtEntryName
				#else
					#define __set_somtEntryName SOMTEntryC__set_somtEntryName
				#endif
			#endif /* SOMGD___set_somtEntryName */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__set_somtEntryName */
	#ifndef SOMTEntryC__get_somtEntryName
		#define SOMTEntryC__get_somtEntryName(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtEntryName)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtEntryName
				#if defined(__get_somtEntryName)
					#undef __get_somtEntryName
					#define SOMGD___get_somtEntryName
				#else
					#define __get_somtEntryName SOMTEntryC__get_somtEntryName
				#endif
			#endif /* SOMGD___get_somtEntryName */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtEntryName */
	#ifndef SOMTEntryC__set_somtElementType
		#define SOMTEntryC__set_somtElementType(somSelf,somtElementType) \
			SOM_Resolve(somSelf,SOMTEntryC,_set_somtElementType)  \
				(somSelf,somtElementType)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___set_somtElementType
				#if defined(__set_somtElementType)
					#undef __set_somtElementType
					#define SOMGD___set_somtElementType
				#else
					#define __set_somtElementType SOMTEntryC__set_somtElementType
				#endif
			#endif /* SOMGD___set_somtElementType */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__set_somtElementType */
	#ifndef SOMTEntryC__get_somtElementType
		#define SOMTEntryC__get_somtElementType(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtElementType)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtElementType
				#if defined(__get_somtElementType)
					#undef __get_somtElementType
					#define SOMGD___get_somtElementType
				#else
					#define __get_somtElementType SOMTEntryC__get_somtElementType
				#endif
			#endif /* SOMGD___get_somtElementType */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtElementType */
	#ifndef SOMTEntryC__get_somtElementTypeName
		#define SOMTEntryC__get_somtElementTypeName(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtElementTypeName)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtElementTypeName
				#if defined(__get_somtElementTypeName)
					#undef __get_somtElementTypeName
					#define SOMGD___get_somtElementTypeName
				#else
					#define __get_somtElementTypeName SOMTEntryC__get_somtElementTypeName
				#endif
			#endif /* SOMGD___get_somtElementTypeName */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtElementTypeName */
	#ifndef SOMTEntryC__get_somtEntryComment
		#define SOMTEntryC__get_somtEntryComment(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtEntryComment)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtEntryComment
				#if defined(__get_somtEntryComment)
					#undef __get_somtEntryComment
					#define SOMGD___get_somtEntryComment
				#else
					#define __get_somtEntryComment SOMTEntryC__get_somtEntryComment
				#endif
			#endif /* SOMGD___get_somtEntryComment */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtEntryComment */
	#ifndef SOMTEntryC__get_somtSourceLineNumber
		#define SOMTEntryC__get_somtSourceLineNumber(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtSourceLineNumber)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtSourceLineNumber
				#if defined(__get_somtSourceLineNumber)
					#undef __get_somtSourceLineNumber
					#define SOMGD___get_somtSourceLineNumber
				#else
					#define __get_somtSourceLineNumber SOMTEntryC__get_somtSourceLineNumber
				#endif
			#endif /* SOMGD___get_somtSourceLineNumber */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtSourceLineNumber */
	#ifndef SOMTEntryC__get_somtTypeCode
		#define SOMTEntryC__get_somtTypeCode(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtTypeCode)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtTypeCode
				#if defined(__get_somtTypeCode)
					#undef __get_somtTypeCode
					#define SOMGD___get_somtTypeCode
				#else
					#define __get_somtTypeCode SOMTEntryC__get_somtTypeCode
				#endif
			#endif /* SOMGD___get_somtTypeCode */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtTypeCode */
	#ifndef SOMTEntryC__get_somtIsReference
		#define SOMTEntryC__get_somtIsReference(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtIsReference)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtIsReference
				#if defined(__get_somtIsReference)
					#undef __get_somtIsReference
					#define SOMGD___get_somtIsReference
				#else
					#define __get_somtIsReference SOMTEntryC__get_somtIsReference
				#endif
			#endif /* SOMGD___get_somtIsReference */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtIsReference */
	#ifndef SOMTEntryC__get_somtIDLScopedName
		#define SOMTEntryC__get_somtIDLScopedName(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtIDLScopedName)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtIDLScopedName
				#if defined(__get_somtIDLScopedName)
					#undef __get_somtIDLScopedName
					#define SOMGD___get_somtIDLScopedName
				#else
					#define __get_somtIDLScopedName SOMTEntryC__get_somtIDLScopedName
				#endif
			#endif /* SOMGD___get_somtIDLScopedName */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtIDLScopedName */
	#ifndef SOMTEntryC__get_somtCScopedName
		#define SOMTEntryC__get_somtCScopedName(somSelf) \
			SOM_Resolve(somSelf,SOMTEntryC,_get_somtCScopedName)  \
				(somSelf)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD___get_somtCScopedName
				#if defined(__get_somtCScopedName)
					#undef __get_somtCScopedName
					#define SOMGD___get_somtCScopedName
				#else
					#define __get_somtCScopedName SOMTEntryC__get_somtCScopedName
				#endif
			#endif /* SOMGD___get_somtCScopedName */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC__get_somtCScopedName */
	#ifndef SOMTEntryC_somtGetModifierValue
		#define SOMTEntryC_somtGetModifierValue(somSelf,modifierName) \
			SOM_Resolve(somSelf,SOMTEntryC,somtGetModifierValue)  \
				(somSelf,modifierName)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD__somtGetModifierValue
				#if defined(_somtGetModifierValue)
					#undef _somtGetModifierValue
					#define SOMGD__somtGetModifierValue
				#else
					#define _somtGetModifierValue SOMTEntryC_somtGetModifierValue
				#endif
			#endif /* SOMGD__somtGetModifierValue */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC_somtGetModifierValue */
	#ifndef SOMTEntryC_somtGetFirstModifier
		#define SOMTEntryC_somtGetFirstModifier(somSelf,modifierName,modifierValue) \
			SOM_Resolve(somSelf,SOMTEntryC,somtGetFirstModifier)  \
				(somSelf,modifierName,modifierValue)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD__somtGetFirstModifier
				#if defined(_somtGetFirstModifier)
					#undef _somtGetFirstModifier
					#define SOMGD__somtGetFirstModifier
				#else
					#define _somtGetFirstModifier SOMTEntryC_somtGetFirstModifier
				#endif
			#endif /* SOMGD__somtGetFirstModifier */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC_somtGetFirstModifier */
	#ifndef SOMTEntryC_somtGetNextModifier
		#define SOMTEntryC_somtGetNextModifier(somSelf,modifierName,modifierValue) \
			SOM_Resolve(somSelf,SOMTEntryC,somtGetNextModifier)  \
				(somSelf,modifierName,modifierValue)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD__somtGetNextModifier
				#if defined(_somtGetNextModifier)
					#undef _somtGetNextModifier
					#define SOMGD__somtGetNextModifier
				#else
					#define _somtGetNextModifier SOMTEntryC_somtGetNextModifier
				#endif
			#endif /* SOMGD__somtGetNextModifier */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC_somtGetNextModifier */
	#ifndef SOMTEntryC_somtFormatModifier
		#define SOMTEntryC_somtFormatModifier(somSelf,buffer,name,value) \
			SOM_Resolve(somSelf,SOMTEntryC,somtFormatModifier)  \
				(somSelf,buffer,name,value)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD__somtFormatModifier
				#if defined(_somtFormatModifier)
					#undef _somtFormatModifier
					#define SOMGD__somtFormatModifier
				#else
					#define _somtFormatModifier SOMTEntryC_somtFormatModifier
				#endif
			#endif /* SOMGD__somtFormatModifier */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC_somtFormatModifier */
	#ifndef SOMTEntryC_somtGetModifierList
		#define SOMTEntryC_somtGetModifierList(somSelf,buffer) \
			SOM_Resolve(somSelf,SOMTEntryC,somtGetModifierList)  \
				(somSelf,buffer)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD__somtGetModifierList
				#if defined(_somtGetModifierList)
					#undef _somtGetModifierList
					#define SOMGD__somtGetModifierList
				#else
					#define _somtGetModifierList SOMTEntryC_somtGetModifierList
				#endif
			#endif /* SOMGD__somtGetModifierList */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC_somtGetModifierList */
	#ifndef SOMTEntryC_somtSetSymbolsOnEntry
		#define SOMTEntryC_somtSetSymbolsOnEntry(somSelf,emitter,prefix) \
			SOM_Resolve(somSelf,SOMTEntryC,somtSetSymbolsOnEntry)  \
				(somSelf,emitter,prefix)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD__somtSetSymbolsOnEntry
				#if defined(_somtSetSymbolsOnEntry)
					#undef _somtSetSymbolsOnEntry
					#define SOMGD__somtSetSymbolsOnEntry
				#else
					#define _somtSetSymbolsOnEntry SOMTEntryC_somtSetSymbolsOnEntry
				#endif
			#endif /* SOMGD__somtSetSymbolsOnEntry */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC_somtSetSymbolsOnEntry */
	#ifndef SOMTEntryC_somtSetEntryStruct
		#define SOMTEntryC_somtSetEntryStruct(somSelf,es) \
			SOM_Resolve(somSelf,SOMTEntryC,somtSetEntryStruct)  \
				(somSelf,es)
		#ifndef SOM_DONT_USE_SHORT_NAMES
			#ifndef SOMGD__somtSetEntryStruct
				#if defined(_somtSetEntryStruct)
					#undef _somtSetEntryStruct
					#define SOMGD__somtSetEntryStruct
				#else
					#define _somtSetEntryStruct SOMTEntryC_somtSetEntryStruct
				#endif
			#endif /* SOMGD__somtSetEntryStruct */
		#endif /* SOM_DONT_USE_SHORT_NAMES */
	#endif /* SOMTEntryC_somtSetEntryStruct */

#ifndef SOM_DONT_USE_INHERITED_MACROS
#define SOMTEntryC_somInit SOMObject_somInit
#define SOMTEntryC_somUninit SOMObject_somUninit
#define SOMTEntryC_somFree SOMObject_somFree
#define SOMTEntryC_somGetClass SOMObject_somGetClass
#define SOMTEntryC_somGetClassName SOMObject_somGetClassName
#define SOMTEntryC_somGetSize SOMObject_somGetSize
#define SOMTEntryC_somIsA SOMObject_somIsA
#define SOMTEntryC_somIsInstanceOf SOMObject_somIsInstanceOf
#define SOMTEntryC_somRespondsTo SOMObject_somRespondsTo
#define SOMTEntryC_somDispatch SOMObject_somDispatch
#define SOMTEntryC_somClassDispatch SOMObject_somClassDispatch
#define SOMTEntryC_somCastObj SOMObject_somCastObj
#define SOMTEntryC_somResetObj SOMObject_somResetObj
#define SOMTEntryC_somPrintSelf SOMObject_somPrintSelf
#define SOMTEntryC_somDumpSelf SOMObject_somDumpSelf
#define SOMTEntryC_somDumpSelfInt SOMObject_somDumpSelfInt
#define SOMTEntryC_somDefaultInit SOMObject_somDefaultInit
#define SOMTEntryC_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define SOMTEntryC_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define SOMTEntryC_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define SOMTEntryC_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define SOMTEntryC_somDefaultAssign SOMObject_somDefaultAssign
#define SOMTEntryC_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define SOMTEntryC_somDefaultVAssign SOMObject_somDefaultVAssign
#define SOMTEntryC_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define SOMTEntryC_somDestruct SOMObject_somDestruct
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#include <sctypes.h>
#include <scemit.h>
#include <sctmplt.h>
#endif /* SOM_Module_scentry_Header_h */
