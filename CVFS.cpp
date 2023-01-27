
#include <stdio.h>
#include <stdlib.h>f
#include <string.h>
#include <unistd.h>
#include <iostream>
// #include<io.h>

// #defines Used In  the Project

#define MAXINODE 5
#define READ 1
#define WRITE 2
#define MAXFILESIZE 1024
#define REGULAR 1
#define SPECIAL 2
#define START 0
#define CURRENT 1
#define END 2

struct superblock
{
    int TotalInodes;
    int FreeInodes;
};
typedef struct superblock SUPERBLOCK;   // This is typedef for the struct superblock.
typedef struct superblock *PSUPERBLOCK; // This is the pointer of type struct superblock.

struct inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    char *Buffer;
    int LinkCount;
    int ReferenceCount;
    int permission;     // File Permissions - read -1, write - 2, execute - 3
    struct inode *next; // self referencial pointer 94
};
typedef struct inode INODE;     // This is typedef for the struct inode.
typedef struct inode *PINODE;   // This is the pointer of type struct inode.
typedef struct inode **PPINODE; // This is the pointer of type struct inode.

struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode;
    PINODE ptrinode;
};
typedef struct filetable FILETABLE;
typedef struct filetable *PFILETABLE;

struct ufdt
{
    PFILETABLE ptrfiletable;
};
typedef struct ufdt UFDT;

// ----------------------------------
// Global Variables
UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCK_OBJ;
PINODE head = NULL;
// -------------------------------

// This function is used to set All values in the UFDTArr to the NULL.
void InitialiseSuperBlock()
{
    int i = 0;
    while (i < MAXINODE)
    {
        UFDTArr[i].ptrfiletable = NULL;
        i++;
    }

    SUPERBLOCK_OBJ.TotalInodes = MAXINODE;
    SUPERBLOCK_OBJ.FreeInodes = MAXINODE;
    printf("Superblock is Initialised Succesfully.\n");
};

// This Function is used to create all Inodes in the DILB block i.e. it creates the linked list of Inodes for the DILB.
void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(i <= MAXINODE)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        newn -> LinkCount = 0;
        newn -> ReferenceCount = 0;
        newn -> FileType = 0;
        newn -> FileSize = 0;

        newn -> Buffer = NULL;
        newn -> next = NULL;

        newn -> InodeNumber = i;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp -> next = newn;
            temp = temp -> next;
        }
        i++;
    }
    printf("DILB Created Suuccesfully.\n");
}

// This Function is used to All files on our VF System.
void ls_file()
{
    int i = 0;
    PINODE temp = head;

    if (SUPERBLOCK_OBJ.FreeInodes == MAXINODE)
    {
        printf("Error : There are no files in VFS.\n");
        return;
    }

    printf("\nFile Name \t Inode Number \t File Size \t Link Count \t Reference Count\n");
    printf("____________________________________________________________________________________________\n");

    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            printf("\n%s\t\t%d\t\t%d\t\t%d\t\t%d\n", temp->FileName, temp->InodeNumber, temp->ActualFileSize, temp->LinkCount, temp-> ReferenceCount);
        }
        temp = temp->next;
    }
    printf("____________________________________________________________________________________________\n");
}

// This Function is Used to Close all the Files on the VF System.
void CloseAllFiles()
{
    int i = 0;
    while (i < 50)
    {
        if (UFDTArr[i].ptrfiletable != NULL)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
            (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
            break;
        }
        i++;
    };
}

// This Function is used to Display the instructions and Information about the all commands in the VFS.
void DisplayHelp()
{
    printf("\nCommand\t\tDescription\n");
    printf("\n-----------------------------------------------------------\n\n");

    printf("ls          : To List out all files\n");
    printf("clear       : To Clear console\n");
    printf("creat       : To Create the new File in the System.\n");
    printf("read        : To Read the contents from file \n");
    printf("write       : To Write contents into file\n");
    printf("open        : To Open the file\n");
    printf("close       : To Close the file \n");
    printf("closeall    : To Close all opened files \n");
    printf("exit        : To Terminate file system \n");
    printf("stat        : To Display information of file using name\n");
    printf("fstat       : To Display information of file using file descriptor\n");
    printf("truncate    : To Remove all data from file \n");
    printf("rm          : To Delete the file\n");

    printf("\n-----------------------------------------------------------\n");
};

// This Function is used to get the File Descriptor of the file by using the name of the File.
int GetFDFromName(char *name)
{
    int i = 0;

    while (i < 50)
    {
        if (UFDTArr[i].ptrfiletable != NULL)
        {
            if (strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName), name) == 0)
            {
                break;
            }
        }
        i++;
    }

    if (i == 50)
    {
        return -1;
    }
    else
    {
        return i;
    }
};

// This Function is Used to Get the File Statistics.
int stat_file(char *name)
{
    PINODE temp = head;
    int i = 0;

    if (name == NULL)
        return -1;

    while (temp != NULL)
    {
        if (strcmp(name, temp->FileName) == 0)
            break;
        temp = temp->next;
    }

    if (temp == NULL)
        return -2;

    printf("\n---------Statical Information About File---------\n\n");
    printf("File Name : %s\n", temp->FileName);
    printf("Inode Number : %d\n", temp->InodeNumber);
    printf("File Size : %d\n", temp->FileSize);
    printf("Actual File Size : %d\n", temp->ActualFileSize);
    printf("Link Count : %d\n", temp->LinkCount);
    printf("Reference Count : %d\n", temp->ReferenceCount);

    if (temp->permission == 1)
    {
        printf("File Permission : Read Only\n");
    }
    else if (temp->permission == 2)
    {
        printf("File Permission : Write Only\n");
    }
    else if (temp->permission == 3)
    {
        printf("File Permission : Read and Write\n");
        printf("_______________________________________________________\n\n");
    }

    return 0;
};

// This Function is Used to remove the Data Form the End Of the File.
int truncate_file(char *name)
{
    int fd = GetFDFromName(name);
    if (fd == -1)
    {
        return -1;
    }

    memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer, 0, 1024);
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize = 0;

    printf("\nAll Data From File is Erased Successfully.\n");
};

// This Function is Used to Get the Statistical information of the File.
int fstat_file(int fd)
{
    PINODE temp = head;
    int i = 0;
    if (fd < 0)
        return -1;

    if (UFDTArr[fd].ptrfiletable == NULL)
        return -2;

    temp = UFDTArr[fd].ptrfiletable->ptrinode;

    printf("\n--------- Statistical Inforamtion About File ------------\n\n");

    printf("File name : %s\n", temp->FileName);
    printf("Inode Number %d\n", temp->InodeNumber);
    printf("File size : %d\n", temp->FileSize);
    printf("Actual File size : %d\n", temp->ActualFileSize);
    printf("Link count : %d\n", temp->LinkCount);
    printf("Reference count : %d\n", temp->ReferenceCount);

    if (temp->permission == 1)
        printf("File Permission : Read Only\n");
    else if (temp->permission == 2)
        printf("File Permission : Write Only\n");
    else if (temp->permission == 3)
        printf("File Permission : Read & Write\n");
    printf("___________________________________________________________\n\n");

    return 0;
};

// This funtion is used to Close the File by using the file name.
void CloseFileByName(int fd)
{
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    (UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}

int CloseFileByName(char *name)
{
    int fd = 0;
    fd = GetFDFromName(name);

    if (fd == -1)
        return -1;

    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    (UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
    return 0;
};

// This function is used to delete the File from the system
int rm_file(char *name)
{
    int fd = 0;

    fd = GetFDFromName(name);

    if (fd == -1) return -1;

    (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

    if (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
    {
        UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
        free(UFDTArr[fd].ptrfiletable->ptrinode->Buffer); 
        free(UFDTArr[fd].ptrfiletable);
    }
    UFDTArr[fd].ptrfiletable = NULL;
    (SUPERBLOCK_OBJ.FreeInodes)++;

    printf("\nFile is Removed Successfully.\n");

};

// This Function Displays the Description of the All the Commands in the VFS.
void man(char *name)
{
    if (name == NULL)
        return;

    if (strcmp(name, "creat") == 0)
    {
        printf("Description : Used to create new regular file \n");
        printf("Usage: create File_name Permission \n");
    }
    else if (strcmp(name, "read") == 0)
    {
        printf("Description: Used to read data from regular file \n");
        printf("Usage : read File_name No_Of_Bytes_To_Read\n");
    }
    else if (strcmp(name, "write") == 0)
    {
        printf("Description : Used to write into regular file\n");
        printf("Usage: write File_name\n After this enter the data that we want to write\n");
    }
    else if (strcmp(name, "ls") == 0)
    {
        printf("Description: Used to list all information of files \n");
        printf("Usage : ls\n");
    }
    else if (strcmp(name, "stat") == 0)
    {
        printf("Description: Used to display information of file\n");
        printf("Usage : stat File_name\n");
    }
    else if (strcmp(name, "fstat") == 0)
    {
        printf("Description : Used to display information of file \n");
        printf("Usage : stat File Descriptor\n");
    }
    else if (strcmp(name, "truncate") == 0)
    {
        printf("Description : Used to remove data from file\n");
        printf("Usage : truncate File_name\n");
    }
    else if (strcmp(name, "open") == 0)
    {
        printf("Description : Used to open existing file \n");
        printf("Usage : open File name mode\n");
    }
    else if (strcmp(name, "close") == 0)
    {
        printf("Description : Used to close opened file \n");
        printf("Usage : close File name n");
    }
    else if (strcmp(name, "closeall") == 0)
    {
        printf("Description: Used to close all opened file \n");
        printf("Usage: closeall\n");
    }
    else if (strcmp(name, "lseek") == 0)
    {
        printf("Description : Used to change file offset.\n");
        printf("Usage : lseek File_Name ChangeInOffset StartPoint\n");
    }
    else if (strcmp(name, "rm") == 0)
    {
        printf("Description : Used to delete the file\n");
        printf("Usage : rm File_Name\n");
    }
    else
    {
        printf("ERROR : No Manual Entry available.\n");
    }
};

PINODE Get_Inode(char *name)
{
    PINODE temp = head;
    int i = 0;

    if (name == NULL)
        return NULL;

    while (temp != NULL)
    {
        if (strcmp(name, temp->FileName) == 0)
            break;
        temp = temp->next;
    }

    return temp;
}

// This Function is Used to Read or Write the data from any point in the File.
int lseekfile(int fd, int size, int from)
{
    if ((fd < 0) || (from > 2))
        return -1;

    if (UFDTArr[fd].ptrfiletable == NULL)
        return -1;

    if ((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ + WRITE))
    {
        if (from == CURRENT)
        {
            if (((UFDTArr[fd].ptrfiletable->readoffset) + size) > UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize)
                return -1;

            if (((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0)
                return -1;

            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + size;
        }
        else if (from == START)
        {
            if (size > (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize))
                return -1;
            if (size < 0)
                return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = size;
        }
        else if (from == END)
        {
            if ((UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + size > MAXFILESIZE)
                return -1;

            if (((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0)
                return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + size;
        }
    }
    else if (UFDTArr[fd].ptrfiletable->mode == WRITE)
    {
        if (from == CURRENT)
        {
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) > MAXFILESIZE) 
                return -1;
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
                return -1;
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize))
            {
                (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
                (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
            };
        }
        else if (from == START)
        {
            if (size > MAXFILESIZE)
                return -1;
            if (size < 0)
                return -1;
            if (size > (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize))
            {
                (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) = size;
                (UFDTArr[fd].ptrfiletable->writeoffset) = size;
            }
        }
        else if (from == END)
        {
            if ((UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + size > MAXFILESIZE)
                return -1;

            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
                return -1;

            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + size;
        }
    }
};

// This Function is Used to Create the New File
int CreatFile(char *name, int permission)
{
    int i = 1; //This is Set to 1 to allocate the file fd from 1st Index. 0th index is blank.
    PINODE temp = head;

    if ((name == NULL) || (permission == 0) || (permission > 3)) return -1;

    if (SUPERBLOCK_OBJ.FreeInodes == 0) return -2;

    (SUPERBLOCK_OBJ.FreeInodes)--;

    if (Get_Inode(name) != NULL) return -3;

    while (temp != NULL)
    {
        if (temp->FileType == 0) break;

        temp = temp->next;
    }

    while (i < 50)
    {
        if (UFDTArr[i].ptrfiletable == NULL) break;

        i++;
    }
    // *
    UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));

    UFDTArr[i].ptrfiletable->count = 1;
    UFDTArr[i].ptrfiletable->mode = permission;
    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;

    UFDTArr[i].ptrfiletable->ptrinode = temp;

    strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName, name);
    UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
    UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
    UFDTArr[i].ptrfiletable->ptrinode->ActualFileSize = 0;
    UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
    UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    //*
    return i;
}

// This Function is Used to Write the Data in the regular File.
int WriteFile(int fd, char *arr, int iSize)
{
    if (((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ + WRITE))
    {
        return -1;
    }

    if (((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != READ + WRITE))
    {
        return -1;
    }

    if ((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)
    {
        return -2;
    }

    if ((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR)
    {
        return -3;
    }

    strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset), arr, iSize);

    (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + iSize;

    (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) = (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) + iSize;

    return iSize;
};

// This Function is Used to read the Data From the File.
int ReadFile(int fd, char *arr, int iSize)
{
    int read_size = 0;

    if (UFDTArr[fd].ptrfiletable == NULL)
    {
        return -1;
    }
    if (UFDTArr[fd].ptrfiletable -> mode != READ && UFDTArr[fd].ptrfiletable -> mode != READ + WRITE)
    {
        return -2;
    }

    if (UFDTArr[fd].ptrfiletable->ptrinode->permission != READ && UFDTArr[fd].ptrfiletable->ptrinode->permission != READ + WRITE)
    {
        return -2;
    }
    if (UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize)
    {
        return -3;
    }
    if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
    {
        return -4;
    }

    read_size = (UFDTArr[fd].ptrfiletable->ptrinode->ActualFileSize) - (UFDTArr[fd].ptrfiletable->readoffset);

    if (read_size < iSize)
    {
        strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset), read_size);
        UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
    }
    else
    {
        strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset), iSize);
        (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + iSize;
    }

    return iSize;
};

int OpenFile(char *name, int mode)
{
    int i = 0;
    PINODE temp = NULL;

    if (name == NULL || mode <= 0)
        return -1;
    temp = Get_Inode(name);
    if (temp == NULL)
        return -2;
    if (temp->permission < mode)
        return -3;
    while(i < 50)
    {
        if (UFDTArr[i].ptrfiletable == NULL)
            break;
        i++;
    }
    UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));

    if (UFDTArr[i].ptrfiletable == NULL) return -1;

    UFDTArr[i].ptrfiletable->count = 1;
    UFDTArr[i].ptrfiletable->mode = mode;
    if (mode == READ + WRITE)
    {
        UFDTArr[i].ptrfiletable->readoffset = 0;
        UFDTArr[i].ptrfiletable->writeoffset = 0;
    }
    else if (mode == READ)
    {
        UFDTArr[i].ptrfiletable->readoffset = 0;
    }
    else if (mode == WRITE)
    {
        UFDTArr[i].ptrfiletable->writeoffset = 0;
    }
    UFDTArr[i].ptrfiletable->ptrinode = temp;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
    return i;
}

int main()
{
    char *ptr = NULL;
    int ret = 0, fd = 0, count = 0;
    char command[4][80]; // This is an Multi dimentional array to store the tokens from the str[80].
    char str[80];        // This is array of characters of length 80 bytes to store the entered command in the shell.
    char arr[1024];      // This is an array used as an

    InitialiseSuperBlock();
    CreateDILB();

    while (1)
    {
        fflush(stdin);   // This is library function which cleans the input buffer(i.e from keyboard) every time while loop starts.
        strcpy(str, ""); // This is library function which copy the string from one location to another, But here it is used to make the string str empty.

        printf("\nVFS shell : > ");

        fgets(str, 80, stdin); // This function is which is used to accept the input from the keyboard just like scanf().instead we can also use : scanf("%[^'\n']s",str);

        // This sscanf() function seperate the words(tokens) from the str and stores it in the 2D array i.e. command.
        //  It returns the number of tokens in the command from the str which is enters on the shell.
        count = sscanf(str, "%s %s %s %s", command[0], command[1], command[2], command[3]);
        
        if (count == 1)
        {
            if (strcmp(command[0], "ls") == 0)
            {
                ls_file();
            }
            else if (strcmp(command[0], "closeall") == 0)
            {
                CloseAllFiles();
                printf("All Files closed Successfully!\n");
                continue;
            }
            else if (strcmp(command[0], "clear") == 0)
            {
                system("clear");
                continue;
            }
            else if (strcmp(command[0], "help") == 0)
            {
                DisplayHelp();
                continue;
            }
            else if (strcmp(command[0], "exit") == 0)
            {
                printf("Terminating the Virtual File System shell.\n");
                break;
            }
            else
            {
                printf("\nERROR : Command Not Found !\n");
                continue;
            }
        }
        else if (count == 2)
        {
            if (strcmp(command[0], "stat") == 0)
            {
                ret = stat_file(command[1]);

                if (ret == -1)
                {
                    printf("ERROR : Incorrect Parameters.\n");
                }
                if (ret == -2)
                {
                    printf("ERROR : There is no such file.\n");
                    continue;
                }
            }
            else if (strcmp(command[0], "fstat") == 0)
            {
                ret = fstat_file(atoi(command[1]));

                if (ret == -1)
                {
                    printf("ERROR : Incorrect Parameters.\n");
                }
                if (ret == -2)
                {
                    printf("ERROR : There is no such file or Please Enter the file fd.\n");
                    continue;
                }
            }
            else if (strcmp(command[0], "close") == 0)
            {
                ret = CloseFileByName(command[1]);
                if (ret == -1)
                {
                    printf("ERROR : There is No such File.\n");
                }
                continue;
            }
            else if (strcmp(command[0], "rm") == 0)
            {
                ret = rm_file(command[1]);

                if (ret == -1)
                {
                    printf("ERROR : There is No such File.\n");
                }
                continue;
            }
            else if (strcmp(command[0], "man") == 0)
            {
                man(command[1]);
            }
            else if (strcmp(command[0], "write") == 0)
            {
                fd = GetFDFromName(command[1]);

                if (fd == -1)
                {
                    printf("Error : Incorrect parameter.\n");
                    continue;
                }
                printf("Enter the Data : \n");
                scanf("%[^'\n']s", arr);

                ret = strlen(arr);

                if (ret == 0)
                {
                    printf("Error : Incorrect Parameter.\n");
                    continue;
                }

                ret = WriteFile(fd, arr, ret);

                if (ret == -1)
                {
                    printf("Error : Permission denied\n");
                }
                if (ret == -2)
                {
                    printf("Error : There is no sufficient memory to write.\n");
                }
                if (ret == -3)
                {
                    printf("Error : It is Not Regular file.\n");
                }
            }
            else if (strcmp(command[0], "truncate") == 0)
            {
                ret = truncate_file(command[1]);
                if (ret == -1)
                {
                    printf("Error : Incorrect parameter\n");
                }
            }
            else
            {
                printf("\nERROR : Command Not Found !");
                continue;
            }
            
        }
        else if (count == 3)
        {
            if (strcmp(command[0], "creat") == 0)
            {
                
                ret = CreatFile(command[1], atoi(command[2]));

                if (ret >= 0)
                {
                    printf("File is Successfully created with file Descriptor : %d\n", ret);
                }

                if (ret == -1)
                {
                    printf("ERROR : Incorrect parameters\n");
                }

                if (ret == -2)
                {
                    printf("ERROR : There is no inodes.\n");
                }

                if (ret == -3)
                {
                    printf("ERROR : File Already exists.\n");
                }

                if (ret == -4)
                {
                    printf("ERROR : Memory Allocation Failure.\n");
                }
                continue;
            }
            else if (strcmp(command[0], "open") == 0)
            {
                ret = OpenFile(command[1], atoi(command[2]));
                if (ret >= 0)
                {
                    printf("File is successfully opened with file descriptor : %d\n", ret);
                }
                if (ret == -1)
                {
                    printf("ERROR: Incorrect parameters\n");
                }
                if (ret == -2)
                {
                    printf("ERROR : File not present\n");
                }
                if (ret == -3)
                {
                    printf("ERROR: Permission denied\n");
                }
                continue;
            }
            else if (strcmp(command[0], "read") == 0)
            {
                fd = GetFDFromName(command[1]);

                if (fd == -1)
                {
                    printf("Error : Incorrect parameters.\n");
                    continue;
                }

                ptr = (char *)malloc(sizeof(atoi(command[2])) + 1);

                if (ptr == NULL)
                {
                    printf("Error : Memory Allocation failure.\n");
                    continue;
                }

                ret = ReadFile(fd, ptr, atoi(command[2]));

                if (ret == -1)
                {
                    printf("ERROR : File Does not exists\n");
                }

                if (ret == -2)
                {
                    printf("ERROR : Permission denied\n");
                }

                if (ret == -3)
                {
                    printf("ERROR : Reached at end of the file\n");
                }
                if (ret == -4)
                {
                    printf("ERROR : It's Not the regular file\n");
                }
                if (ret == 0)
                {
                    printf("ERROR : Empty File.\n");
                }

                if (ret > 0)
                {
                    write(2, ptr, ret);
                }
                continue;
            }
            else
            {
                printf("\nERROR : Command Not Found!!\n");
            }
        }
        else if (count == 4)
        {
            if (strcmp(command[0], "lseek") == 0)
            {
                fd = GetFDFromName(command[1]);

                if (fd == -1)
                {
                    printf("Error : Inccorect Prameter\n");
                    continue;
                }

                ret = lseekfile(fd, atoi(command[2]), atoi(command[3]));

                if (ret == -1)
                {
                    printf("ERROR : Unable tp perform lseek\n");
                }
            }
            else
            {
                printf("\nERROR : Command Not Found !!\n");
                continue;
            }
        }
        else
        {
            printf("\nERROR : Command Not Found !!\n");
            continue;
        }
    }

    return 0;
}
