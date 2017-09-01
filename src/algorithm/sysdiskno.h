
#ifndef __AUTHORITYSERVICE__SYSHARDDISK_H
#define __AUTHORITYSERVICE__SYSHARDDISK_H

#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QSettings>

#ifdef __linux__
#else
#include <windows.h> 
#include <tchar.h>
#include <strsafe.h>
#endif

#ifdef __linux__
#else
#define  m_cVxDFunctionIdesDInfo    1 
#define MAX_IDE_DRIVES				1
#define  DFP_GET_VERSION			0x00074080  
#define  IDE_ATAPI_IDENTIFY			0xA1
#define  IDE_ATA_IDENTIFY			0xEC
#define  DFP_RECEIVE_DRIVE_DATA		0x0007c088 
#define  FILE_DEVICE_SCSI           0x0000001b 
#define  IOCTL_SCSI_MINIPORT		0x0004D008 
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)  
#define  SENDIDLENGTH  sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE 


typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.  
	BYTE bRevision;     // Binary driver revision.  
	BYTE bReserved;     // Not used.  
	BYTE bIDEDeviceMap; // Bit map of IDE devices.  
	DWORD fCapabilities; // Bit mask of driver capabilities.  
	DWORD dwReserved[4]; // For future use.  
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength;
	UCHAR Signature[8];
	ULONG Timeout;
	ULONG ControlCode;
	ULONG ReturnCode;
	ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

#pragma pack(1)  

typedef struct _IDENTIFY_DATA {
	USHORT GeneralConfiguration;            // 00 00  
	USHORT NumberOfCylinders;               // 02  1  
	USHORT Reserved1;                       // 04  2  
	USHORT NumberOfHeads;                   // 06  3  
	USHORT UnformattedBytesPerTrack;        // 08  4  
	USHORT UnformattedBytesPerSector;       // 0A  5  
	USHORT SectorsPerTrack;                 // 0C  6  
	USHORT VendorUnique1[3];                // 0E  7-9  
	USHORT SerialNumber[10];                // 14  10-19  
	USHORT BufferType;                      // 28  20  
	USHORT BufferSectorSize;                // 2A  21  
	USHORT NumberOfEccBytes;                // 2C  22  
	USHORT FirmwareRevision[4];             // 2E  23-26  
	USHORT ModelNumber[20];                 // 36  27-46  
	UCHAR  MaximumBlockTransfer;            // 5E  47  
	UCHAR  VendorUnique2;                   // 5F  
	USHORT DoubleWordIo;                    // 60  48  
	USHORT Capabilities;                    // 62  49  
	USHORT Reserved2;                       // 64  50  
	UCHAR  VendorUnique3;                   // 66  51  
	UCHAR  PioCycleTimingMode;              // 67  
	UCHAR  VendorUnique4;                   // 68  52  
	UCHAR  DmaCycleTimingMode;              // 69  
	USHORT TranslationFieldsValid : 1;        // 6A  53  
	USHORT Reserved3 : 15;
	USHORT NumberOfCurrentCylinders;        // 6C  54  
	USHORT NumberOfCurrentHeads;            // 6E  55  
	USHORT CurrentSectorsPerTrack;          // 70  56  
	ULONG  CurrentSectorCapacity;           // 72  57-58  
	USHORT CurrentMultiSectorSetting;       //     59  
	ULONG  UserAddressableSectors;          //     60-61  
	USHORT SingleWordDMASupport : 8;        //     62  
	USHORT SingleWordDMAActive : 8;
	USHORT MultiWordDMASupport : 8;         //     63  
	USHORT MultiWordDMAActive : 8;
	USHORT AdvancedPIOModes : 8;            //     64  
	USHORT Reserved4 : 8;
	USHORT MinimumMWXferCycleTime;          //     65  
	USHORT RecommendedMWXferCycleTime;      //     66  
	USHORT MinimumPIOCycleTime;             //     67  
	USHORT MinimumPIOCycleTimeIORDY;        //     68  
	USHORT Reserved5[2];                    //     69-70  
	USHORT ReleaseTimeOverlapped;           //     71  
	USHORT ReleaseTimeServiceCommand;       //     72  
	USHORT MajorRevision;                   //     73  
	USHORT MinorRevision;                   //     74  
	USHORT Reserved6[50];                   //     75-126  
	USHORT SpecialFunctionsEnabled;         //     127  
	USHORT Reserved7[128];                  //     128-255  
} IDENTIFY_DATA, *PIDENTIFY_DATA;

#pragma pack()  

typedef struct _rt_IdeDInfo_
{
	BYTE IDEExists[4];
	BYTE DiskExists[8];
	WORD DisksRawInfo[8 * 256];
} rt_IdeDInfo, *pt_IdeDInfo;
#endif
/***********************************************************/
/***********************************************************/
/***********************************************************/

class SysDisk
{
private:
	SysDisk();
	~SysDisk(){};

public:
	static SysDisk* GetInstance()
	{
		if (instance == NULL)
		{
			instance = new SysDisk();
		}
		return instance;
	}
public:
	char*   getSerialNo(){ return HardDriveSerialNumber; }

private:	
#ifdef __linux__
#else
	bool	DoIDENTIFY(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,	PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,PDWORD lpcbBytesReturned);
	char *	ConvertToString(unsigned long diskdata[256],int firstIndex,int lastIndex,char* buf);
	void	WriteConstantString(char *entry, char *string);
	void	PrintIdeInfo(int drive, unsigned long diskdata[256]);
	int		ReadPhysicalDriveInNTWithAdminRights(void);
	int		ReadIdeDriveAsScsiDriveInNT(void);
	long	getHardDriveComputerID();
	int		ReadPhysicalDriveInNTWithZeroRights(void);
	int		ReadDrivePortsInWin9X(void);
	int		ReadPhysicalDriveInNTUsingSmart(void);
	char *  flipAndCodeBytes(const char * str, int pos, int flip, char * buf);
#endif
private:
	static SysDisk* instance;

	char HardDriveSerialNumber[1024];
	char HardDriveModelNumber[1024];
#ifdef __linux__
#else
	BYTE IdOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
#endif
};


class SerailNo
{

private:
	SerailNo();
	~SerailNo(){};

public:
	static SerailNo* GetInstance()
	{
		if (instance == NULL)
		{
			instance = new SerailNo();
		}
		return instance;
	}
public:
	QString   getSerialNo(){ return HardDriveSerialNumber; }

#ifdef __linux__
#else

	void CreateChildProcess(void);
	void WriteToPipe(void);
	void ReadFromPipe(void);
#endif

private:
	static SerailNo* instance;
	QString HardDriveSerialNumber;

};


#endif