/*
    Copyright 1999-2003,2007,2011 TiANWEi
    Copyright 2004 tulipfan
    Copyright 2011-2012 Regshot Team

    This file is part of Regshot.

    Regshot is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    Regshot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Regshot.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGSHOT_GLOBAL_H
#define REGSHOT_GLOBAL_H


#ifdef __GNUC__
#include <unistd.h>
#endif
#include <windows.h>
#include <stdio.h>
#include <shlobj.h>
#include "resource.h"

#if defined(_MSC_VER) && (_MSC_VER < 1300)  // before VS 2002 .NET (e.g. VS 6)
#define SetClassLongPtr SetClassLong
#ifndef GCLP_HICON
#define GCLP_HICON GCL_HICON
#endif
#if !defined(LONG_PTR)
typedef long LONG_PTR;
#endif
#endif  // _MSC_VER && (_MSC_VER < 1300)

// added in 1.8.2 to gain a slightly faster speed but it is danger!
#define USEHEAPALLOC_DANGER
//#define DEBUGLOG

#ifdef USEHEAPALLOC_DANGER

// MSDN doc say use HEAP_NO_SERIALIZE is not good for process heap :( so change fromm 1 to 0 20111216, slower than using 1
#define MYALLOC(x)  HeapAlloc(hHeap,0,x)
#define MYALLOC0(x) HeapAlloc(hHeap,8,x) // (HEAP_NO_SERIALIZE|) HEAP_ZERO_MEMORY ,1|8
#define MYFREE(x)   HeapFree(hHeap,0,x)

#else

#define MYALLOC(x)  GlobalAlloc(GMEM_FIXED,x)
#define MYALLOC0(x) GlobalAlloc(GPTR,x)
#define MYFREE(x)   GlobalFree(x)

#endif


// Some definations
#define NOTMATCH        0             // Define modification type in comparison results
#define ISMATCH         1
#define ISDEL           2
#define ISADD           3
#define ISMODI          4

#define KEYADD          1
#define KEYDEL          2
#define VALADD          3
#define VALDEL          4
#define VALMODI         5
#define FILEADD         6
#define FILEDEL         7
#define FILEMODI        8
#define DIRADD          9
#define DIRDEL          10
#define DIRMODI         11

#define ESTIMATE_VALUEDATA_LENGTH 1024*1024 //Define estimated value data in scan
#define REFRESHINTERVAL 110          // Define progress refresh rate
#define MAXPBPOSITION   100          // Define progress bar length
#define COMMENTLENGTH   50           // Define commentfield length on the MainForm
#define HTMLWRAPLENGTH  1000         // Define html out put wrap length
#define MAXAMOUNTOFFILE 10000        // Define out put file counts
#define EXTDIRLEN       MAX_PATH * 4 // Define searching directory field length
#define COMPUTERNAMELEN 64           // Define COMPUTER name length, do not change
#define HIVEBEGINOFFSET 512          // Hive file out put header computerlen*2+sizeof(systemtime)+32 must <hivebeginoffset!!!!!!!!!!!!!!


// Some definitions of MutiLanguage strings [Free space length]
#define SIZEOF_LANGUAGE_SECTIONNAMES_BUFFER 2048
#define SIZEOF_SINGLE_LANGUAGENAME          64
#define SIZEOF_LANGSTRINGS                  16384
#define SIZEOF_ABOUTBOX                     4096


// Struct used for Windows Registry Key
struct _KEYCONTENT {
    LPSTR  lpkeyname;                          // Pointer to key's name
    struct _VALUECONTENT FAR *lpfirstvalue;    // Pointer to key's first value
    struct _KEYCONTENT FAR *lpfirstsubkey;     // Pointer to key's first subkey
    struct _KEYCONTENT FAR *lpbrotherkey;      // Pointer to key's brother
    struct _KEYCONTENT FAR *lpfatherkey;       // Pointer to key's father
    size_t bkeymatch;                          // Flag used at comparing, 1.8.2<= is byte

};
typedef struct _KEYCONTENT KEYCONTENT, FAR *LPKEYCONTENT;


// Struct used for Windows Registry Value
struct _VALUECONTENT {
    DWORD  typecode;                           // Type of value [DWORD,STRING...]
    DWORD  datasize;                           // Value data size in bytes
    LPSTR  lpvaluename;                        // Pointer to value name
    LPBYTE lpvaluedata;                        // Pointer to value data
    struct _VALUECONTENT FAR *lpnextvalue;     // Pointer to value's brother
    struct _KEYCONTENT FAR *lpfatherkey;       // Pointer to value's father[Key]
    size_t bvaluematch;                        // Flag used at comparing, 1.8.2<= is byte
};
typedef struct _VALUECONTENT VALUECONTENT, FAR *LPVALUECONTENT;


// Struct used for Windows File System
struct _FILECONTENT {
    LPSTR  lpfilename;                         // Pointer to filename
    DWORD  writetimelow;                       // File write time [LOW  DWORD]
    DWORD  writetimehigh;                      // File write time [HIGH DWORD]
    DWORD  filesizelow;                        // File size [LOW  DWORD]
    DWORD  filesizehigh;                       // File size [HIGH DWORD]
    DWORD  fileattr;                           // File attributes
    DWORD  chksum;                             // File checksum (planned for the future, currently not used)
    struct _FILECONTENT FAR *lpfirstsubfile;   // Pointer to files[DIRS] first sub file
    struct _FILECONTENT FAR *lpbrotherfile;    // Pointer to files[DIRS] brother
    struct _FILECONTENT FAR *lpfatherfile;     // Pointer to files father
    size_t bfilematch;                         // Flag used when comparing, until 1.8.2 was byte
};
typedef struct _FILECONTENT FILECONTENT, FAR *LPFILECONTENT;


// Adjusted for filecontent saving. 1.8
struct _HEADFILE {
    struct _HEADFILE FAR *lpnextheadfile;      // Pointer to next headfile struc
    LPFILECONTENT   lpfilecontent;             // Pointer to filecontent
};
typedef struct  _HEADFILE HEADFILE, FAR *LPHEADFILE;


// Struct used for comparing result output
struct _COMRESULT {
    LPSTR  lpresult;                           // Pointer to result string
    struct _COMRESULT FAR *lpnextresult;       // Pointer to next _COMRESULT
};
typedef struct _COMRESULT COMRESULT, FAR *LPCOMRESULT;

// Struct for hive file header ,used in saving and loading
// when accessing fields of this structure always put a version check around them, e.g. "if version >= 2 then use fpos_computername"
struct _HIVEHEADER {
    unsigned char signature[12];  // ofs 0 len 12: never convert to Unicode, always use char type and ASCII-127 codes
    DWORD version;                // (v2) ofs 12 len 4: file/hive header version

    DWORD  fpos_HKLM;   // ofs 16 len 4
    DWORD  fpos_HKCU;   // ofs 20 len 4
    DWORD  fpos_FILES;  // ofs 24 len 4
    DWORD  tchar_size;  // (v2) ofs 28 len 4: sizeof(TCHAR), allows to separate between MBCS (1 for char) and Unicode (2 for UTF-16 WCHAR), may become 4 for UTF-32 in the future

    // next two fields are kept and filled for <= 1.8.2 compatibility, see substituting fields in header format version 2
    unsigned char computername[64]; // (v1) ofs 32 len 64: bytes, incl. \0, name maybe truncated
    unsigned char username[64];     // (v1) ofs 96 len 64: bytes, incl. \0, name maybe truncated

    SYSTEMTIME systemtime;  // ofs 160 len 16 bytes

    // new since header version 2
    DWORD fpos_computername;  // (v2) ofs 176 len 4
    DWORD len_computername;   // (v2) ofs 180 len 4: length in bytes incl. \0
    DWORD fpos_username;      // (v2) ofs 184 len 4
    DWORD len_username;       // (v2) ofs 188 len 4: length in bytes incl. \0
    DWORD fpos_locale;        // (v2) ofs 192 len 4
    DWORD len_locale;         // (v2) ofs 196 len 4: length in bytes incl. \0

    // ^^^ here the file/hive header can be extended
    // * increase the version number for the new header format
    // * place a comment with a reference to the new version before the new fields
    // * check all usages and version checks
    // * remember that older versions can not use these additional data
};
typedef struct _HIVEHEADER HIVEHEADER, FAR *LPHIVEHEADER;

// Struct for shot,2012.
struct _REGSHOT {
    LPKEYCONTENT  lpheadlocalmachine;
    LPKEYCONTENT  lpheadusers;
    LPHEADFILE    lpheadfile;
    LPBYTE        lptemphive;
    unsigned char computername[COMPUTERNAMELEN]; //64
    unsigned char username[COMPUTERNAMELEN];
    SYSTEMTIME    systemtime;
    BOOL          isloadfromhive;
};
typedef struct _REGSHOT REGSHOT, FAR *LPREGSHOT;

// Struct for saving designed by maddes

struct _SAVEKEYCONTENT {
    DWORD  fpos_keyname;            // Pointer to key's name
    DWORD  fpos_firstvalue;         // Pointer to key's first value
    DWORD  fpos_firstsubkey;        // Pointer to key's first subkey
    DWORD  fpos_brotherkey;         // Pointer to key's brother
    DWORD  fpos_fatherkey;          // Pointer to key's father
    DWORD  bkeymatch;               // Flag used at comparing, 1.8.2 <= is byte

};
typedef struct _SAVEKEYCONTENT SAVEKEYCONTENT, FAR *LPSAVEKEYCONTENT;


// Struct used for Windows Registry Value
struct _SAVEVALUECONTENT {
    DWORD  typecode;                // Type of value [DWORD,STRING...]
    DWORD  datasize;                // Value data size in bytes
    DWORD  fpos_valuename;          // Pointer to value name
    DWORD  fpos_valuedata;          // Pointer to value data
    DWORD  fpos_nextvalue;          // Pointer to value's brother
    DWORD  fpos_fatherkey;          // Pointer to value's father[Key]
    DWORD  bvaluematch;             // Flag used at comparing, 1.8.2 <= is byte
};
typedef struct _SAVEVALUECONTENT SAVEVALUECONTENT, FAR *LPSAVEVALUECONTENT;


// Struct used for Windows File System
struct _SAVEFILECONTENT {
    DWORD  fpos_filename;            // Pointer to filename
    DWORD  writetimelow;             // File write time [LOW  DWORD]
    DWORD  writetimehigh;            // File write time [HIGH DWORD]
    DWORD  filesizelow;              // File size [LOW  DWORD]
    DWORD  filesizehigh;             // File size [HIGH DWORD]
    DWORD  fileattr;                 // File attributes
    DWORD  chksum;                   // File checksum  (planned for the future, currently not used)
    DWORD  fpos_firstsubfile;        // Pointer to files[DIRS] first sub file
    DWORD  fpos_brotherfile;         // Pointer to files[DIRS] brother
    DWORD  fpos_fatherfile;          // Pointer to files father

    // new since file/hive header version 2
    // new since file content version 2
    DWORD  version;                  // File content structure version
    DWORD  len_filename;             // Length

    // ^^^ here the file content header can be extended
    // * increase the version number for the new header format
    // * place a comment with a reference to the new version before the new fields
    // * check all usages and version checks
    // * remember that older versions can not use these additional data
};
typedef struct _SAVEFILECONTENT SAVEFILECONTENT, FAR *LPSAVEFILECONTENT;


// Adjusted for filecontent saving. 1.8
struct _SAVEHEADFILE {
    DWORD  fpos_nextheadfile;       // Pointer to next headfile struc
    DWORD  fpos_filecontent;        // Pointer to filecontent
};
typedef struct  _SAVEHEADFILE SAVEHEADFILE, FAR *LPSAVEHEADFILE;


// Pointers to compare result [see above]
LPCOMRESULT lpKEYADD;
LPCOMRESULT lpKEYDEL;
LPCOMRESULT lpVALADD;
LPCOMRESULT lpVALDEL;
LPCOMRESULT lpVALMODI;
LPCOMRESULT lpFILEADD;
LPCOMRESULT lpFILEDEL;
LPCOMRESULT lpFILEMODI;
LPCOMRESULT lpDIRADD;
LPCOMRESULT lpDIRDEL;
LPCOMRESULT lpDIRMODI;


LPCOMRESULT lpKEYADDHEAD;
LPCOMRESULT lpKEYDELHEAD;
LPCOMRESULT lpVALADDHEAD;
LPCOMRESULT lpVALDELHEAD;
LPCOMRESULT lpVALMODIHEAD;
LPCOMRESULT lpFILEADDHEAD;
LPCOMRESULT lpFILEDELHEAD;
LPCOMRESULT lpFILEMODIHEAD;
LPCOMRESULT lpDIRADDHEAD;
LPCOMRESULT lpDIRDELHEAD;
LPCOMRESULT lpDIRMODIHEAD;


// Number of Modification detected
DWORD   nKEYADD;
DWORD   nKEYDEL;
DWORD   nVALADD;
DWORD   nVALDEL;
DWORD   nVALMODI;
DWORD   nFILEADD;
DWORD   nFILEDEL;
DWORD   nFILEMODI;
DWORD   nDIRADD;
DWORD   nDIRDEL;
DWORD   nDIRMODI;


// Some DWORD used to show the progress bar and etc
DWORD   nGettingValue;
DWORD   nGettingKey;
DWORD   nComparing;
DWORD   nRegStep;
DWORD   nFileStep;
DWORD   nSavingKey;
DWORD   nGettingTime;
DWORD   nBASETIME;
DWORD   nBASETIME1;
DWORD   nGettingFile;
DWORD   nGettingDir;
DWORD   nSavingFile;
//DWORD   nMask = 0xf7fd;     // not used now, but should be added
//DWORD   nRegMessageCount = 0;
DWORD   NBW;                // that is: NumberOfBytesWritten;


LPREGSHOT lpShot1;
LPREGSHOT lpShot2;
// Pointers to Registry Key
/*
LPKEYCONTENT    lpHeadLocalMachine1;    // Pointer to HKEY_LOCAL_MACHINE 1
LPKEYCONTENT    lpHeadLocalMachine2;    // Pointer to HKEY_LOCAL_MACHINE 2
LPKEYCONTENT    lpHeadUsers1;           // Pointer to HKEY_USERS 1
LPKEYCONTENT    lpHeadUsers2;
LPHEADFILE      lpHeadFile1;            // Pointer to headfile
LPHEADFILE      lpHeadFile2;
LPBYTE          lpTempHive1;            // Pointer for loading hive files
LPBYTE          lpTempHive2;
LPSTR           lpComputerName1;
LPSTR           lpComputerName2;
LPSTR           lpUserName1;
LPSTR           lpUserName2;
SYSTEMTIME FAR *lpSystemtime1, * lpSystemtime2;
*/

// Some pointers need to allocate enough space to working
LPSTR   lpKeyName;   // following used in scan engine
LPSTR   lpValueName;
LPBYTE  lpValueData;
LPBYTE  lpValueDataS;


LPSTR   lpMESSAGE;
LPSTR   lpExtDir;
LPSTR   lpOutputpath;
LPSTR   lpLastSaveDir;
LPSTR   lpLastOpenDir;
LPSTR   lpCurrentLanguage;
LPSTR   lpWindowsDirName;
LPSTR   lpTempPath;
LPSTR   lpStartDir;
LPSTR   lpLanguageIni;  // For language.ini
LPSTR   lpLangStrings;
LPSTR   lpCurrentTranslator;
LPSTR   lpRegshotIni;

LPBYTE  lpRegSkipStrings;
LPBYTE  lpFileSkipStrings;
LPBYTE *lplpRegSkipStrings;
LPBYTE *lplpFileSkipStrings;
LPVOID  lplpLangStrings;
//LPSTR REGSHOTDATFILE = "rgst152.dat";
//LPSTR   lpProgramDir;   // tfx define
//LPSTR   lpSnapKey;
//LPSTR   lpSnapReturn;



// Former definations used at Dynamic Monitor Engine. Not Used NOW
//#define DIOCPARAMSSIZE 20      // 4+4+4+8 bytes DIOCParams size!
//#define MAXLISTBOXLEN  1024
//#define RING3TDLEN     8       // ring3 td name length
//LPSTR str_errorini = "Error create Dialog!";
//INT   tabarray[] = {40,106,426,466};      // the tabstops is the len addup!
//BOOL  bWinNTDetected;
//UINT  WM_REGSHOT = 0;

#ifdef DEBUGLOG
LPSTR   lstrdb1;
#endif

MSG             msg;                // Windows MSG struct
HWND            hWnd;               // The handle of REGSHOT
HMENU           hMenu;              // The handles of shortcut menus
HMENU           hMenuClear;         // The handles of shortcut menus
HANDLE          hFile;              // Handle of file regshot use
HANDLE          hFileWholeReg;      // Handle of file regshot use
HCURSOR         hHourGlass;         // Handle of cursor
HCURSOR         hSaveCursor;        // Handle of cursor
BOOL            is1;                // Flag to determine is the 1st shot
//BOOL            is1LoadFromHive;    // Flag to determine are shots load from hive files
//BOOL            is2LoadFromHive;    // Flag to determine are shots load from hive files
RECT            rect;               // Window RECT
FILETIME        ftLastWrite;        // Filetime struct
BROWSEINFO      BrowseInfo1;        // BrowseINFO struct
OPENFILENAME    opfn;               // Openfilename struct
BOOL            bUseLongRegHead;    // 1.8.1 for compatibility with 1.61e5 and undoreg1.46
HANDLE          hHeap;              // 1.8.2

VOID    LogToMem(DWORD actiontype, LPDWORD lpcount, LPVOID lp);
BOOL    LoadSettingsFromIni(HWND hDlg);
BOOL    SaveSettingsToIni(HWND hDlg);
BOOL    IsInSkipList(LPSTR lpStr, LPBYTE *lpSkipList);
VOID    UpdateCounters(LPBYTE title1, LPBYTE title2, DWORD count1, DWORD count2);
LPBYTE  AtPos(LPBYTE lpMaster, LPBYTE lp, size_t size, size_t sizep);
BOOL    GetLanguageType(HWND hDlg);
VOID    GetDefaultStrings(VOID);
VOID    PointToNewStrings(VOID);
BOOL    GetLanguageStrings(HWND hDlg);
VOID    CreateShotPopupMenu(VOID);
VOID    UI_BeforeShot(DWORD id);
VOID    UI_AfterShot(VOID);
VOID    UI_BeforeClear(VOID);
VOID    UI_AfterClear(VOID);

VOID    Shot(LPREGSHOT lpshot);
BOOL    CompareShots(LPREGSHOT lpshot1, LPREGSHOT lpshot2);
VOID    SaveHive(LPREGSHOT lpshot);
BOOL    LoadHive(LPREGSHOT lpshot);
VOID    FreeAllCompareResults(void);
VOID    FreeAllKeyContent(LPREGSHOT lpshot);
VOID    FreeAllFileHead(LPHEADFILE lp);
VOID    ClearKeyMatchTag(LPKEYCONTENT lpKey);
VOID    GetRegistrySnap(HKEY hkey, LPKEYCONTENT lpFatherKeyContent);    // HWND hDlg, first para deleted in 1.8, return from void * to void
VOID    GetFilesSnap(LPFILECONTENT lpFatherFile);                       // HWND hDlg, first para deleted in 1.8
LPSTR   GetWholeFileName(LPFILECONTENT lpFileContent);
VOID    InitProgressBar(VOID);
VOID    CompareFirstSubFile(LPFILECONTENT lpHead1, LPFILECONTENT lpHead2);
BOOL    ReplaceInValidFileName(LPSTR lpf);
VOID    ErrMsg(LPCSTR note);
VOID    WriteHead(u_char *lpstr, DWORD count, BOOL isHTML);
VOID    WritePart(LPCOMRESULT lpcomhead, BOOL isHTML, BOOL usecolor);
VOID    WriteTitle(LPSTR lph, LPSTR lpb, BOOL isHTML);
VOID    SaveFileContent(LPFILECONTENT lpFileContent, DWORD nFPCurrentFatherFile, DWORD nFPCaller);
VOID    ClearHeadFileMatchTag(LPHEADFILE lpHF);
VOID    FindDirChain(LPHEADFILE lpHF, LPSTR lpDir, size_t nMaxLen);
BOOL    DirChainMatch(LPHEADFILE lphf1, LPHEADFILE lphf2);
VOID    WriteHtmlbegin(void);
VOID    WriteHtmlover(void);
VOID    WriteHtmlbr(void);
VOID    ReAlignFile(LPHEADFILE lpHF, size_t nBase);
VOID    GetAllSubFile(BOOL needbrother, DWORD typedir, DWORD typefile, LPDWORD lpcountdir, LPDWORD lpcountfile, LPFILECONTENT lpFileContent);
VOID    RebuildFromHive_filehead(LPSAVEHEADFILE lpSHF, LPHEADFILE lpHeadFile, LPBYTE lpHiveFileBase);
LPFILECONTENT SearchDirChain(LPSTR lpname, LPHEADFILE lpHF);

#endif // REGSHOT_GLOBAL_H
