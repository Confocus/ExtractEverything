#include <stdio.h>
#include<dirent.h>   
#include<sys/stat.h> 
#include<sys/types.h>
#include <string.h>
#include <string>

extern "C" int IsPackedBy7z(const char* szFilePath);

void traverse(const char* szDir)
{
	DIR * dir; //目录流指针
    struct dirent * entry; //目录项结构体指针
    struct stat statbuf;
	std::string sDir = szDir;

	if(!(dir = opendir(szDir)))
	{
		printf("Error:No such directory..\n");
        return ; 
	}

	while((entry = readdir(dir)) != NULL)
    {
		std::string sTmpDir = sDir;
		sTmpDir += "/";
		sTmpDir += entry->d_name;
        lstat(sTmpDir.c_str(), &statbuf);
        if(S_ISDIR(statbuf.st_mode))
        {
            if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            {
                continue;
            }
			//printf("dir: %s\n", sTmpDir.c_str());
            traverse(sTmpDir.c_str());
        }
        else if(S_ISREG(statbuf.st_mode)) 
        {
			std::string sTmpFile = sDir + "/";
			sTmpFile += entry->d_name;
            printf("Scanning:%s\n", sTmpFile.c_str());
			IsPackedBy7z(sTmpFile.c_str());
        }
    }

	closedir(dir);
	return;
}

int main(int argc, char* argv[])
{
//	if(argc < 2)
//	{
//		printf("Not enough parameters, input \"--help\" to consult the usage..\n");
//		return 0;
//	}
//
//	traverse(argv[1]);

	return 1;
}
