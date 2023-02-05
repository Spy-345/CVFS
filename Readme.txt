------------------------------------------------ Customised Virtual File System -----------------------------------------------------------------


Name of the project  		: Customised Virtual File System(CVFS)

Technology used  		    : C Programming

User interface used  		: CUI

Platform requirements 		: Works On Any Platform

Hardware requirements 	    : Laptop or PC with basic configuration

Description of the project 	: 

        - This project is a light version file system which is used to manage the files by any operating system.
        - This project demonstrates the File Management Done by any Operating System, which is one of the important task of any operating system.
        - In this project user can give the inputs to the program same as we enter the commands to terminal of the system.
        - In this project we can use create, read, write, delete all this functions of the files by means of specific commands that are provided in the project itself, the same like the normal terminal commands.
        - This project is totally virtual as all the creating, writing, reading of the files is done on the RAM only.
        - The Files created in the project are not saved on the Hard disk of the Device on which you are running this project.
        - Once you terminated the program of the project file on the terminal all the created files and data about them is erased. 
        - This project is totally customisable as we can increase the number of file allowed to create, Size of the each file created, permissions to the file, commands to perform,etc.

Data structures used in the project : Array , Linked List, File Table, Inodes.

Diagram of data structures used in the project  : Linked List, Array, Structures,etc.

Source Code Of the project  : CVFS.cpp

------------------------------------------------------------------------------------------------------------------------------------------------

Flow of the Project :

- Project Execution starts from the main function ,at the start some required variable initialisation such as,
 2D array to store the tokenised command, str to store the accepted string from the shell, arr to store the file data, etc.
- There is call for the two functions which are InitialiseSuperBlock() and CreateDILB().
- Firstly program accept the command from the shell using the fgets() function.
- The Accepted string is then tokenised in the four tokens using the sscanf() function which are then stored in the 2D Aarray named as Command and sscanf() function returns the number of tokens generated as count.
- Now Depending on the number of tokens i.e. value of count,here we have used  if-else ladder to get the specific condition match. 
- Our project accept the maximum four number of tokens, according to that we have seperated the function calls depending on the value of the count i.e. number of tokens in the commnad.
- When the control gets inside the if condion then here we have used the library function to compaire the entered command with our consisting commands.
- Where the entered commands matches with the existing command in the project there is call to the specific funtion according to the condition in the statement.
- There are some functions which are helper functions for the other working funtions which are not directly get called by the main function like Get_FD_From_Name(), Get_Inode(), etc.
- There are the error codes in every function if there is any error while function call then that fucntion is returned with the specific error code for the specific reason of the error,
 which is handeled in the main function, and according to that user gets replied.
- If there is no match found for the entered command, then shell will reply with "Commnd Not Found !" string.


List of The Functions In the Project and there Description :

1. CreatFile() : creat File_Name Permission(1,2,1+2)

        - This funtion is used to creat the new file on the VFS.
        - It accepts two parameters name as name for the newly created file and the permissions for the file i.e. READ or WRITE.
        - Returns the File Descriptor of the file.

2. WriteFile() : write File_name

        - This Function is Used to write the data into the existing file on the VFS.
        - It accepts the three parameters FD as file descriptor of the file, location of the data which will be written on the file, Size of the data in the bytes.
        - Retrun the Size of the data written on the file. 

3. ReadFile() : read File_Name Number_of_bytes_to_read

        - This function is used to read the data from the file.
        - It accepts the three parameters FD of the existing file, location where to store the readed data and Number of bytes to read.
        - retruns the number of bytes readed.

3. OpenFile() : open File_Name mode

        - This Function is Used to open the file in the READ or WRITE mode.
        - It accepts the two parameters as name of the file and mode of the file in which you want to open it.
        - returns the File Descriptor of the file which is open.

4. lseekfile() : lseek File_Name offset_to_be_set Position

        - This function is used to reset the Offset of the file.
        - It accepts the three parameters as FD of the file, number of the bytes to set offset, positon form where to set offset.
        
5. man() : man Name_of_the_Function

        - This function is used to get the information of the perticular function.
        - outputs the Description of the function and its command.

6. Get_Inode() : 

        - This function is used to get the address of the iNode of the File using the file name.
        - return the address of the iNode for the perticular file.

7. rm_file() : rm File_Name

        - This function is used to remove the file form the VFS.
        - It accepts the one parameter as the name of the file.
        - It outputs "File Removed Successfully."

8.CloseFileByName() : close File_Name 

        - This Function is Used to Close the Opened File 
        - This Function Shows the Property of Function overloading, here which is done by changing the type of the parameter.
        - It one accepts the  parameter as the file name and One Accepts the Parameter as FD of the file.

9. fstat_file() : fstat FD_of_the_file

        - This function is used to get the info of the file by using the FD of the file.
        - It accepts the FD of the file.
        - Outputs the Detailed Information about the file.

10. stat_file() : stat File_Name

        - This function is used to get the info of the file by using the Name of the file.
        - It accepts the Name of the file.
        - Outputs the Detailed Information about the file.

11. truncate_file() : truncate File_Name

        - This Function is used to remove the file data from the file.
        - It accepts one paramater as the name of the file.

12. GetFDFromName() : 

        - This function is used to get the file Descriptor of the file by using its name.
        - It accepts the one parameter as the Name of the file.

13. Display() : help

        - This function is used to get the help menu about the Project which contains the Description about the functions and commands to access the functions.

14. CloseAllFiles() : closeall

        - This Function is used to close the all File that are present on the VFS.

15. ls_file() : ls

        - This function is used to get the all file listed which are present on the VFS.

16. CreatDILB() : 

        - This function creates the DILB of the VFS which contains the Linked list of the INODEs on the VFS.

17. InitialiseSuperBlock() :

        - This function creats the superblock of the VFS whic contains the Inforamtion about the Nodes.

