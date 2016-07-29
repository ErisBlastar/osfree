//������������������������������������������������������������������������������
//� XTRCDSK - Initial0, program data
//� Author:  Alan B. Arnett
//� Copyright 1997, 1998 by Alan B. Arnett
//� 16 Feb 98 - split initialize into local and common sections
//� 04 Jul 98 - add crc options to parm table
//������������������������������������������������������������������������������
//�
//� This file is part of the source code to dskxtrct and is distributed under
//� the terms of the GNU General Public Licence.
//�
//������������������������������������������������������������������������������

enum
{
	OptSource,
	OptTarget,
	OptLogReplace,
	OptLogAppend,
	OptLogSkip,
	OptLog,
	OptQuietMode,
	OptMsgLevel,
	OptCrcIgnore,
	OptCrcOnly,
	OptComment,
	OptViewList,
	OptHelp,
	OptReplaceAll,
	OptReplaceNone,
	OptCount
};

enum
{
	ParmString, ParmSwitch
};

struct
{
	int Type;
	char Str[8];
}
	OptEntry[OptCount] =
	{
		{ ParmString, "s" },
		{ ParmString, "t" },
		{ ParmSwitch, "lr" },
		{ ParmSwitch, "la" },
		{ ParmSwitch, "ls" },
		{ ParmString, "l" },
		{ ParmSwitch, "q" },
		{ ParmString, "m" },
		{ ParmSwitch, "crcx" },
		{ ParmSwitch, "crc" },
		{ ParmSwitch, "c" },
		{ ParmSwitch, "v" },
		{ ParmSwitch, "?" },
		{ ParmSwitch, "ra" },
		{ ParmSwitch, "rn" },
	};
