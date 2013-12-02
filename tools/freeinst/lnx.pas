type
  TStatFS = record
    f_Type: LongInt;
    f_BSize: LongInt;
    f_Blocks: LongInt;
    f_BFree: LongInt;
    f_BAvail: LongInt;
    f_Files: LongInt;
    f_FFree: LongInt;
    f_FSid: array[0..1] of LongInt;
    f_NameLen: LongInt;
    f_Spare: array[0..5] of LongInt;
  end;

function LnxStatFS(Path: PChar; var StatFS: TStatFS): LongInt; forward;

function SysGetBootDrive: Char;
begin
  SysGetBootDrive := 'C';
end;

function SysGetDriveType(Drive: Char): TDriveType;
var
  StatFS: TStatFS;
begin
  if (Drive <> 'C') and (Drive <> 'c') then
  begin
    SysGetDriveType := dtInvalid;
    Exit;
  end;

  LnxStatFS('/', StatFS);
  with StatFS do
  begin
    if f_fsid[0] = $00004D44 then
      SysGetDriveType := dtHDFAT
    else if f_fsid[0] = $F995E849 then
      SysGetDriveType := dtHDHPFS
    else if (f_fsid[0] = $0000EF51) or (f_fsid[0] = $0000EF53) then
      SysGetDriveType := dtHDEXT2
    else
      SysGetDriveType := dtInvalid;
  end;
end;

function SysGetValidDrives: Longint;
begin
  SysGetValidDrives := 4; // 000..000100 -- drive C: only
end;

function LnxStatFS(Path: PChar; var StatFS: TStatFS): LongInt; {&uses ebx,ecx,edx,esi,edi}
begin
{$ASMMODE intel}
asm
  mov eax, 99;
  mov ebx, Path;
  mov ecx, StatFS;
  int $80;
end; 
end;

