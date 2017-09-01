//#include "authorityservice.h"
#include "sysdiskno.h"
//
SysDisk* SysDisk::instance = NULL;

SysDisk::SysDisk()
{
	HardDriveSerialNumber[1024] = {};
	HardDriveModelNumber[1024] = {};
#ifdef __linux__
#else
	IdOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1] = {};
	getHardDriveComputerID();
#endif
}

#ifdef __linux__
#else
bool SysDisk::DoIDENTIFY(
						HANDLE hPhysicalDriveIOCTL, 
						PSENDCMDINPARAMS pSCIP,
						PSENDCMDOUTPARAMS pSCOP, 
						BYTE bIDCmd, 
						BYTE bDriveNum,
						PDWORD lpcbBytesReturned)
{
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg = 0;
	pSCIP->irDriveRegs.bSectorCountReg = 1;
	pSCIP->irDriveRegs.bCylLowReg = 0;
	pSCIP->irDriveRegs.bCylHighReg = 0;

	pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

	pSCIP->irDriveRegs.bCommandReg = bIDCmd;
	pSCIP->bDriveNumber = bDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	return (DeviceIoControl(hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		(LPVOID)pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID)pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL));
}

char * SysDisk::ConvertToString(unsigned long diskdata[256],
								int firstIndex,
								int lastIndex,
								char* buf)
{
	int index = 0;
	int position = 0;

	for (index = firstIndex; index <= lastIndex; index++)
	{
		buf[position++] = (char)(diskdata[index] / 256);
		buf[position++] = (char)(diskdata[index] % 256);
	}

	buf[position] = '\0';
	for (index = position - 1; index > 0 && isspace(buf[index]); index--)
		buf[index] = '\0';

	return buf;
}

void SysDisk::WriteConstantString(char *entry, char *string)
{
}

void SysDisk::PrintIdeInfo(int drive, unsigned long diskdata[256])
{
	char serialNumber[1024];
	char modelNumber[1024];
	char revisionNumber[1024];
	char bufferSize[32];

	__int64 sectors = 0;
	__int64 bytes = 0;

	ConvertToString(diskdata, 10, 19, serialNumber);
	ConvertToString(diskdata, 27, 46, modelNumber);
	ConvertToString(diskdata, 23, 26, revisionNumber);
	sprintf(bufferSize, "%u", diskdata[21] * 512);

	if (0 == HardDriveSerialNumber[0] &&
		(isalnum(serialNumber[0]) || isalnum(serialNumber[19])))
	{
		strcpy(HardDriveSerialNumber, serialNumber);
		strcpy(HardDriveModelNumber, modelNumber);
	}
}

int SysDisk::ReadPhysicalDriveInNTWithAdminRights(void)
{
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;
		char driveName[256];
		sprintf(driveName, "\\\\.\\PhysicalDrive%d", drive);

		hPhysicalDriveIOCTL = CreateFileA(driveName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);


		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{
			0;
		}
		else
		{
			GETVERSIONOUTPARAMS VersionParams;
			DWORD               cbBytesReturned = 0;
			memset((void*)&VersionParams, 0, sizeof(VersionParams));

			if (!DeviceIoControl(hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL,
				0,
				&VersionParams,
				sizeof(VersionParams),
				&cbBytesReturned, NULL))
			{
				DWORD err = GetLastError();
			}

			if (VersionParams.bIDEDeviceMap <= 0){
			}
			else
			{
				BYTE             bIDCmd = 0;
				SENDCMDINPARAMS  scip;

				bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
				IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

				memset(&scip, 0, sizeof(scip));
				memset(IdOutCmd, 0, sizeof(IdOutCmd));

				if (DoIDENTIFY(hPhysicalDriveIOCTL,
					&scip,
					(PSENDCMDOUTPARAMS)&IdOutCmd,
					(BYTE)bIDCmd,
					(BYTE)drive,
					&cbBytesReturned))
				{
					DWORD diskdata[256];
					int ijk = 0;
					USHORT *pIdSector = (USHORT *)
						((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer;

					for (ijk = 0; ijk < 256; ijk++)
						diskdata[ijk] = pIdSector[ijk];

					PrintIdeInfo(drive, diskdata);

					done = TRUE;
				}
			}

			CloseHandle(hPhysicalDriveIOCTL);
		}
	}

	return done;
}

int SysDisk::ReadIdeDriveAsScsiDriveInNT(void)
{
	int done = FALSE;
	int controller = 0;

	for (controller = 0; controller < 16; controller++)
	{
		HANDLE hScsiDriveIOCTL = 0;
		char   driveName[256];

		sprintf(driveName, "\\\\.\\Scsi%d:", controller);

		hScsiDriveIOCTL = CreateFileA(driveName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);

		if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			int drive = 0;

			for (drive = 0; drive < 2; drive++)
			{
				char buffer[sizeof(SRB_IO_CONTROL) + SENDIDLENGTH];
				SRB_IO_CONTROL *p = (SRB_IO_CONTROL *)buffer;
				SENDCMDINPARAMS *pin =
					(SENDCMDINPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
				DWORD dummy;

				memset(buffer, 0, sizeof(buffer));
				p->HeaderLength = sizeof(SRB_IO_CONTROL);
				p->Timeout = 10000;
				p->Length = SENDIDLENGTH;
				p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy((char *)p->Signature, "SCSIDISK", 8);

				pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin->bDriveNumber = drive;

				if (DeviceIoControl(hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT,
					buffer,
					sizeof(SRB_IO_CONTROL) +
					sizeof(SENDCMDINPARAMS) - 1,
					buffer,
					sizeof(SRB_IO_CONTROL) + SENDIDLENGTH,
					&dummy, NULL))
				{
					SENDCMDOUTPARAMS *pOut =
						(SENDCMDOUTPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
					IDSECTOR *pId = (IDSECTOR *)(pOut->bBuffer);
					if (pId->sModelNumber[0])
					{
						DWORD diskdata[256];
						int ijk = 0;
						USHORT *pIdSector = (USHORT *)pId;

						for (ijk = 0; ijk < 256; ijk++)
							diskdata[ijk] = pIdSector[ijk];

						PrintIdeInfo(controller * 2 + drive, diskdata);

						done = TRUE;
					}
				}
			}
			CloseHandle(hScsiDriveIOCTL);
		}
	}

	return done;
}

char * SysDisk::flipAndCodeBytes(const char * str,
	int pos,
	int flip,
	char * buf)
{
	int i;
	int j = 0;
	int k = 0;

	buf[0] = '\0';
	if (pos <= 0)
		return buf;

	if (!j)
	{
		char p = 0;

		// First try to gather all characters representing hex digits only.  
		j = 1;
		k = 0;
		buf[k] = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = tolower(str[i]);

			if (isspace(c))
				c = '0';

			++p;
			buf[k] <<= 4;

			if (c >= '0' && c <= '9')
				buf[k] |= (unsigned char)(c - '0');
			else if (c >= 'a' && c <= 'f')
				buf[k] |= (unsigned char)(c - 'a' + 10);
			else
			{
				j = 0;
				break;
			}

			if (p == 2)
			{
				if (buf[k] != '\0' && !isprint(buf[k]))
				{
					j = 0;
					break;
				}
				++k;
				p = 0;
				buf[k] = 0;
			}

		}
	}

	if (!j)
	{
		// There are non-digit characters, gather them as is.  
		j = 1;
		k = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = str[i];

			if (!isprint(c))
			{
				j = 0;
				break;
			}

			buf[k++] = c;
		}
	}

	if (!j)
	{
		// The characters are not there or are not printable.  
		k = 0;
	}

	buf[k] = '\0';

	if (flip)
		// Flip adjacent characters  
		for (j = 0; j < k; j += 2)
		{
		char t = buf[j];
		buf[j] = buf[j + 1];
		buf[j + 1] = t;
		}

	// Trim any beginning and end space  
	i = j = -1;
	for (k = 0; buf[k] != '\0'; ++k)
	{
		if (!isspace(buf[k]))
		{
			if (i < 0)
				i = k;
			j = k;
		}
	}

	if ((i >= 0) && (j >= 0))
	{
		for (k = i; (k <= j) && (buf[k] != '\0'); ++k)
			buf[k - i] = buf[k];
		buf[k - i] = '\0';
	}

	return buf;
}

int SysDisk::ReadPhysicalDriveInNTWithZeroRights(void)
{
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

	
		char driveName[256];
		sprintf(driveName, "\\\\.\\PhysicalDrive%d", drive);

		hPhysicalDriveIOCTL = CreateFileA(driveName, 0,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{
			0;
		}
		else
		{
			STORAGE_PROPERTY_QUERY query;
			DWORD cbBytesReturned = 0;
			char buffer[10000];

			memset((void *)& query, 0, sizeof(query));
			query.PropertyId = StorageDeviceProperty;
			query.QueryType = PropertyStandardQuery;

			memset(buffer, 0, sizeof(buffer));

			if (DeviceIoControl(hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY,
				&query,
				sizeof(query),
				&buffer,
				sizeof(buffer),
				&cbBytesReturned, NULL))
			{
				STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *)& buffer;
				char serialNumber[1000];
				char modelNumber[1000];
				char vendorId[1000];
				char productRevision[1000];

		flipAndCodeBytes(buffer,
					descrip->VendorIdOffset,
					0, vendorId);
				flipAndCodeBytes(buffer,
					descrip->ProductIdOffset,
					0, modelNumber);
				flipAndCodeBytes(buffer,
					descrip->ProductRevisionOffset,
					0, productRevision);
				flipAndCodeBytes(buffer,
					descrip->SerialNumberOffset,
					1, serialNumber);

				if (0 == HardDriveSerialNumber[0] &&					
					(isalnum(serialNumber[0]) || isalnum(serialNumber[19])))
				{
					strcpy(HardDriveSerialNumber, serialNumber);
					strcpy(HardDriveModelNumber, modelNumber);
					done = TRUE;
				}

				memset(buffer, 0, sizeof(buffer));
				if (!DeviceIoControl(hPhysicalDriveIOCTL,
					IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
					NULL,
					0,
					&buffer,
					sizeof(buffer),
					&cbBytesReturned,
					NULL))
				{
					0;
				}
				else
				{
					DISK_GEOMETRY_EX* geom = (DISK_GEOMETRY_EX*)&buffer;
					int fixed = (geom->Geometry.MediaType == FixedMedia);
					//__int64 size = geom->DiskSize.QuadPart;
				}
			}
			else
			{
				DWORD err = GetLastError();
			}

			CloseHandle(hPhysicalDriveIOCTL);
		}
	}

	return done;
}


int SysDisk::ReadPhysicalDriveInNTUsingSmart(void)
{
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;
		char driveName[256];

		sprintf(driveName, "\\\\.\\PhysicalDrive%d", drive);

		hPhysicalDriveIOCTL = CreateFileA(driveName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 0, NULL);
		// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)  
		//    printf ("Unable to open physical drive %d, error code: 0x%lX\n",  
		//            drive, GetLastError ());  

		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{
			0;
		}
		else
		{
			GETVERSIONINPARAMS GetVersionParams;
			DWORD cbBytesReturned = 0;
			memset((void*)& GetVersionParams, 0, sizeof(GetVersionParams));

			if (!DeviceIoControl(hPhysicalDriveIOCTL, SMART_GET_VERSION,
				NULL,
				0,
				&GetVersionParams, sizeof(GETVERSIONINPARAMS),
				&cbBytesReturned, NULL))
			{
				0;
			}
			else
			{		
				ULONG CommandSize = sizeof(SENDCMDINPARAMS) + IDENTIFY_BUFFER_SIZE;
				PSENDCMDINPARAMS Command = (PSENDCMDINPARAMS)malloc(CommandSize);				
#define ID_CMD          0xEC             
				Command->irDriveRegs.bCommandReg = ID_CMD;
				DWORD BytesReturned = 0;
				if (!DeviceIoControl(hPhysicalDriveIOCTL,
					SMART_RCV_DRIVE_DATA, Command, sizeof(SENDCMDINPARAMS),
					Command, CommandSize,
					&BytesReturned, NULL))
				{
					// Print the error  
					0;
				}
				else
				{
					DWORD diskdata[256];
					USHORT *pIdSector = (USHORT *)
						(PIDENTIFY_DATA)((PSENDCMDOUTPARAMS)Command)->bBuffer;

					for (int ijk = 0; ijk < 256; ijk++)
						diskdata[ijk] = pIdSector[ijk];

					PrintIdeInfo(drive, diskdata);
					done = TRUE;
				}
				// Done  
				CloseHandle(hPhysicalDriveIOCTL);
				free(Command);
			}
		}
	}

	return done;
}

int SysDisk::ReadDrivePortsInWin9X(void)
{
	int done = FALSE;
	unsigned long int i = 0;

	HANDLE VxDHandle = 0;
	pt_IdeDInfo pOutBufVxD = 0;
	DWORD lpBytesReturned = 0;

	BOOL status =
		// SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);  
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	// SetPriorityClass (GetCurrentProcess (), HIGH_PRIORITY_CLASS);  



	if (0 == status)
		// printf ("\nERROR: Could not SetThreadPriority, LastError: %d\n", GetLastError ());  
		//printf("\nERROR: Could not SetPriorityClass, LastError: %d\n", GetLastError());
		;

	rt_IdeDInfo info;
	pOutBufVxD = &info;
	
	ZeroMemory(&info, sizeof(info));
	
	//char StartupDirectory [2048];  
	//char shortFileNamePath [2048];  
	//char *p = NULL;  
	//char vxd [2048]; 
	//GetModuleFileName (hInst, (LPSTR) StartupDirectory, sizeof (StartupDirectory));  	
	//p = &(StartupDirectory [strlen (StartupDirectory) - 1]);  
	//while (p >= StartupDirectory && *p && '\\' != *p) p--;  
	//*p = '\0';     
	//GetShortPathName (StartupDirectory, shortFileNamePath, 2048);  
	//sprintf (vxd, "\\\\.\\%s\\IDE21201.VXD", shortFileNamePath);  
	//VxDHandle = CreateFile (vxd, 0, 0, 0,  
	//               0, FILE_FLAG_DELETE_ON_CLOSE, 0);    

	VxDHandle = CreateFileA("\\\\.\\IDE21201.VXD", 0, 0, 0,
		0, FILE_FLAG_DELETE_ON_CLOSE, 0);

	if (VxDHandle != INVALID_HANDLE_VALUE)
	{
		DeviceIoControl(VxDHandle, m_cVxDFunctionIdesDInfo,
			0, 0, pOutBufVxD, sizeof(pt_IdeDInfo), &lpBytesReturned, 0);

		CloseHandle(VxDHandle);
	}
	else
		/*MessageBoxA(NULL, "ERROR: Could not open IDE21201.VXD file",
		TITLE, MB_ICONSTOP);*/
		;

	for (i = 0; i<8; i++)
	{
		if ((pOutBufVxD->DiskExists[i]) && (pOutBufVxD->IDEExists[i / 2]))
		{
			DWORD diskinfo[256];
			for (int j = 0; j < 256; j++)
				diskinfo[j] = pOutBufVxD->DisksRawInfo[i * 256 + j];

			PrintIdeInfo(i, diskinfo);
			done = TRUE;
		}
	}
	// SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_NORMAL);  
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);

	return done;
}

long SysDisk::getHardDriveComputerID()
{
	int done = FALSE;
	__int64 id = 0;
	OSVERSIONINFO version;
	strcpy(HardDriveSerialNumber, "");

	memset(&version, 0, sizeof(version));
	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

#pragma warning(disable:4996)

	GetVersionEx(&version);

	if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		done = ReadPhysicalDriveInNTWithAdminRights();
		//if (!done)
		{
			done = ReadIdeDriveAsScsiDriveInNT();
		}
		//if (!done)
		{
			done = ReadPhysicalDriveInNTWithZeroRights();
		}
		//if (!done)  
		{
			done = ReadPhysicalDriveInNTUsingSmart();
		}
	}
	else
	{
		int attempt = 0;
		for (attempt = 0;
			attempt < 10 && !done && 0 == HardDriveSerialNumber[0];
			attempt++)
			done = ReadDrivePortsInWin9X();
	}

	if (HardDriveSerialNumber[0] > 0)
	{
		char *p = HardDriveSerialNumber;
		WriteConstantString("HardDriveSerialNumber", HardDriveSerialNumber);
		if (!strncmp(HardDriveSerialNumber, "WD-W", 4))
			p += 5;
		for (; p && *p; p++)
		{
			if ('-' == *p)
				continue;
			id *= 10;
			switch (*p)
			{
			case '0': id += 0; break;
			case '1': id += 1; break;
			case '2': id += 2; break;
			case '3': id += 3; break;
			case '4': id += 4; break;
			case '5': id += 5; break;
			case '6': id += 6; break;
			case '7': id += 7; break;
			case '8': id += 8; break;
			case '9': id += 9; break;
			case 'a': case 'A': id += 10; break;
			case 'b': case 'B': id += 11; break;
			case 'c': case 'C': id += 12; break;
			case 'd': case 'D': id += 13; break;
			case 'e': case 'E': id += 14; break;
			case 'f': case 'F': id += 15; break;
			case 'g': case 'G': id += 16; break;
			case 'h': case 'H': id += 17; break;
			case 'i': case 'I': id += 18; break;
			case 'j': case 'J': id += 19; break;
			case 'k': case 'K': id += 20; break;
			case 'l': case 'L': id += 21; break;
			case 'm': case 'M': id += 22; break;
			case 'n': case 'N': id += 23; break;
			case 'o': case 'O': id += 24; break;
			case 'p': case 'P': id += 25; break;
			case 'q': case 'Q': id += 26; break;
			case 'r': case 'R': id += 27; break;
			case 's': case 'S': id += 28; break;
			case 't': case 'T': id += 29; break;
			case 'u': case 'U': id += 30; break;
			case 'v': case 'V': id += 31; break;
			case 'w': case 'W': id += 32; break;
			case 'x': case 'X': id += 33; break;
			case 'y': case 'Y': id += 34; break;
			case 'z': case 'Z': id += 35; break;
			}
		}
	}

	id %= 100000000;
	if (strstr(HardDriveModelNumber, "IBM-"))
		id += 300000000;
	else if (strstr(HardDriveModelNumber, "MAXTOR") ||
		strstr(HardDriveModelNumber, "Maxtor"))
		id += 400000000;
	else if (strstr(HardDriveModelNumber, "WDC "))
		id += 500000000;
	else
		id += 600000000;

	return (long)id;
}

#endif



#define BUFSIZE 4096 

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;
HANDLE g_hInputFile = NULL;

//
SerailNo* SerailNo::instance = NULL;

SerailNo::SerailNo()
{
	HardDriveSerialNumber.clear();
#ifdef __linux__
#else
	

#endif
}

void SerailNo::CreateChildProcess()
{
	TCHAR szCmdline[] = TEXT("diskpart");
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	bSuccess = CreateProcess(NULL,
		szCmdline,     
		NULL,          
		NULL,          
		TRUE,          
		0,             
		NULL,          
		NULL,          
		&siStartInfo,  
		&piProcInfo);  

	if (!bSuccess)
		ExitProcess(6);
	else
	{
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
	}
}

void SerailNo::WriteToPipe(void)
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;
	for (;;)
	{
		bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;
		
		/*bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
		if (!bSuccess)
			ExitProcess(7);*/
		WriteFile(g_hChildStd_IN_Wr, "select disk 0", dwRead, &dwWritten, NULL);
		Sleep(1000);
		WriteFile(g_hChildStd_IN_Wr, "detail disk", dwRead, &dwWritten, NULL);
		Sleep(1000);
		
	}


	if (!CloseHandle(g_hChildStd_IN_Wr))
		ExitProcess(5);
}

void SerailNo::ReadFromPipe(void)
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
	if (!bSuccess || dwRead == 0)
		ExitProcess(3);
	else
	{	
		WriteFile(g_hChildStd_IN_Wr, "exit 0", dwRead, &dwWritten, NULL);

		QString infor(chBuf);		
		QString content("");
		QStringList list = infor.split("\n", QString::SkipEmptyParts);
		foreach(QString str, list)
		{
			if (str.startsWith(("╢еел ID:"))){
				content = str;
				break;
			}
		}
		if (content.isEmpty())
			return;
		if (content.split(":").size() < 2)
			return;
		HardDriveSerialNumber = content.split(":").at(1);	
	}
	/*bSuccess = WriteFile(hParentStdOut, chBuf,
		dwRead, &dwWritten, NULL);
		if (!bSuccess) break;*/
}
