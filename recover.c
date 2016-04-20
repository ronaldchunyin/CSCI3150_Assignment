#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#pragma pack(push, 1)
struct BootEntry {
	unsigned char BS_jmpBoot[3]; /* Assembly instruction to jump to boot code */
	unsigned char BS_OEMName[8]; /* OEM Name in ASCII */
	unsigned short BPB_BytsPerSec; /* Bytes per sector. Allowed values include 512, 1024, 2048, and 4096 */
	unsigned char BPB_SecPerClus; /* Sectors per cluster (data unit). Allowed values are powers of 2, but the cluster size must be 32KB or smaller */
	unsigned short BPB_RsvdSecCnt; /* Size in sectors of the reserved area */
	unsigned char BPB_NumFATs; /* Number of FATs */
	unsigned short BPB_RootEntCnt; /* Maximum number of files in the root directory for FAT12 and FAT16. This is 0 for FAT32 */
	unsigned short BPB_TotSec16; /* 16-bit value of number of sectors in file system */
	unsigned char BPB_Media; /* Media type */
	unsigned short BPB_FATSz16; /* 16-bit size in sectors of each FAT for FAT12 and FAT16. For FAT32, this field is 0 */
	unsigned short BPB_SecPerTrk; /* Sectors per track of storage device */
	unsigned short BPB_NumHeads; /* Number of heads in storage device */
	unsigned int BPB_HiddSec; /* Number of sectors before the start of partition */
	unsigned int BPB_TotSec32; /* 32-bit value of number of sectors in file system. Either this value or the 16-bit value above must be 0 */
	unsigned int BPB_FATSz32; /* 32-bit size in sectors of one FAT */
	unsigned short BPB_ExtFlags; /* A flag for FAT */
	unsigned short BPB_FSVer; /* The major and minor version number */
	unsigned int BPB_RootClus; /* Cluster where the root directory can be found */
	unsigned short BPB_FSInfo; /* Sector where FSINFO structure can be found */
	unsigned short BPB_BkBootSec; /* Sector where backup copy of boot sector is located */
	unsigned char BPB_Reserved[12]; /* Reserved */
	unsigned char BS_DrvNum; /* BIOS INT13h drive number */
	unsigned char BS_Reserved1; /* Not used */
	unsigned char BS_BootSig; /* Extended boot signature to identify if the next three values are valid */
	unsigned int BS_VolID; /* Volume serial number */
	unsigned char BS_VolLab[11]; /* Volume label in ASCII. User defines when creating the file system */
	unsigned char BS_FilSysType[8]; /* File system type label in ASCII */
};
struct DirEntry{
	unsigned char DIR_Name[11]; /* File name */
	unsigned char DIR_Attr; /* File attributes */
	unsigned char DIR_NTRes; /* Reserved */
	unsigned char DIR_CrtTimeTenth; /* Created time (tenths of second) */
	unsigned short DIR_CrtTime; /* Created time (hours, minutes, seconds) */
	unsigned short DIR_CrtDate; /* Created day */
	unsigned short DIR_LstAccDate; /* Accessed day */
	unsigned short DIR_FstClusHI; /* High 2 bytes of the first cluster address */
	unsigned short DIR_WrtTime; /* Written time (hours, minutes, seconds */
	unsigned short DIR_WrtDate; /* Written day */
	unsigned short DIR_FstClusLO; /* Low 2 bytes of the first cluster address */
	unsigned int DIR_FileSize; /* File size in bytes. (0 for directories) */
};
#pragma pack(pop)

struct DirEntry *dir;
struct BootEntry boot;

void print_usage(char* argv) {
    printf("Usage: %s -d [device filename] [other arguments]\n", argv);
    printf("-i\t\t\tPrint file system information\n");
    printf("-l\t\t\tList the root directory\n");
    printf("-r target -o dest\tRecover the target deleted file\n");
    printf("-x target \t\tCleanse the target deleted file\n");
    exit(1);
}

int main( int argc, char *argv[] ) {

    if (argv[1] == NULL) {
        print_usage(argv[0]);
    }
    int opt = 0;
    int dflag = 0;

    while ( (opt = getopt(argc, argv, "d:ilr:o:x:")) != -1) {
        switch (opt) {
            case 'd' :
                if(argv[2] == NULL || strcmp(argv[2], "-i") == 0  || strcmp(argv[2], "-l") == 0  || strcmp(argv[2], "-r") == 0  || strcmp(argv[2], "-x") == 0 ) {
                    print_usage(argv[0]);
                }
                // printf("argv[2]: %s\n", argv[2]);
                printf("-d: %s\n", argv[2]);
                dflag++;
                break;
            case 'i' :
                if (dflag == 0) {
                    print_usage(argv[0]);
                } else {
                    printf("-i\n");
                    FILE *fptr = fopen(argv[2],"r+");
                    fread(&boot,sizeof(struct BootEntry),1,fptr);
                    printf("Number of FATs = %u\n", boot.BPB_NumFATs);
                    printf("Number of bytes per sector = %d\n", boot.BPB_BytsPerSec);
                    printf("Number of sectors per cluster = %u\n", boot.BPB_SecPerClus);
                    printf("Numer of reserved sectors = %d\n", boot.BPB_RsvdSecCnt);
                    printf("First FAT starts at byte = %d\n", boot.BPB_BytsPerSec * boot.BPB_RsvdSecCnt);
                    printf("Data area starts at byte = %d\n", (((boot.BPB_FATSz32 * boot.BPB_NumFATs) + boot.BPB_RsvdSecCnt) * boot.BPB_BytsPerSec) );
                    break;
                }
            case 'l' :
                if (dflag == 0) {
                    print_usage(argv[0]);
                } else {
                    printf("-l\n");
                    break;
                }
            case 'r' :
                if (dflag == 0) {
                    print_usage(argv[0]);
                } else {
                    printf("-r\n");
                    break;
                }
            case 'o' :
                if (dflag == 0) {
                    print_usage(argv[0]);
                } else {
                    printf("-o\n");
                    break;
                }
            case 'x' :
                if (dflag == 0) {
                    print_usage(argv[0]);
                } else {
                    printf("-x\n");
                    break;
                }
        }
    }



    return 0;
}