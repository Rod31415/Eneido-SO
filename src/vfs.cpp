#include "headers/vfs.h"

#define STARTCLUSTER 0x400000
#define STARTDATACLUSTER 0x410000

#define MAXDIRECTIONS 4096
#define CLUSTERSIZE 512
#define MAXCLUSTERFILE 16
#define MAXCLUSTERFILESYSTEM 16384

#define DIRPERCLUSTER 8

#define FIRSTDATACLUSTER 512

FILESYSTEM filesystem;

uint32 actualClusterDIR = 0;
uint32 actualDirectoryDIR = 0;

DIR root;

void writeDirectory(DIR buffer, uint32 cluster, uint32 directory)
{
	for (uint32 i = 0; i < 49; i++)
		*(uint8 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + i) = buffer.name[i];
	*(uint8 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 49) = buffer.flags;
	*(uint32 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 50) = buffer.size;
	*(uint32 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 54) = buffer.dataCluster;
	*(uint8 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 58) = buffer.dataDirection;
	*(uint32 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 59) = buffer.nextCluster;
	*(uint8 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 63) = buffer.nextDirection;
}
void writeCluster(uint8 *buffer, uint32 cluster)
{
	for (uint32 i = 0; i < 512; i++)
		*(uint8 *)(STARTDATACLUSTER + cluster * CLUSTERSIZE + i) = buffer[i];
}
void readDirectory(DIR *buffer, uint32 cluster, uint32 directory)
{
	for (uint32 i = 0; i < 49; i++)
		buffer->name[i] = *(int8 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + i);
	buffer->flags = *(uint8 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 49);
	buffer->size = *(uint32 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 50);
	buffer->dataCluster = *(uint32 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 54);
	buffer->dataDirection = *(uint8 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 58);
	buffer->nextCluster = *(uint32 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 59);
	buffer->nextDirection = *(uint8 *)(STARTCLUSTER + cluster * CLUSTERSIZE + directory * 64 + 63);
}
void readCluster(uint8 *buffer, uint32 cluster)
{
	for (uint32 i = 0; i < 512; i++)
		buffer[i] = *(uint8 *)(STARTDATACLUSTER + cluster * CLUSTERSIZE + i);
}

void initVFS()
{

	filesystem.directions = (uint32)STARTCLUSTER;
	filesystem.clusters = (uint32)STARTDATACLUSTER;
	root.name[0] = '.';
	root.flags = 2;
	root.size = 0;
	root.dataCluster = 1;
	root.dataDirection = 0;
	root.nextCluster = 0;
	root.nextDirection = 0;
	writeDirectory(root, 0, 0);

	DIR file = {"Juegos", 2, 1, 20, 0, 0, 0};
	writeDirectory(file, 1, 0);
	file = {"..", 3, 1, 1, 0, 20, 1};
	writeDirectory(file, 20, 0);

	file = {"snake.exe", 1, 1, 65, 0, 0, 0};
	writeDirectory(file, 20, 1);

	actualClusterDIR = root.dataCluster;
	actualDirectoryDIR = 0;
}

uint32 retClusterFree(uint32 in)
{
	DIR ref;
	for (uint32 x = in; x < 512; x++)
	{
		for (uint32 i = 0; i < 8; i++)
		{
			readDirectory(&ref, x, i);
			if (ref.flags == 0)
	return x * 8 + i;
		}
	}
	return 0;
}

int createDirectory(char name[49])
{
	uint32 cl = retClusterFree(1);
	uint32 cs = retClusterFree(32);
	DIR folder = {"", 2, 1, (uint32)(cs / 8), (uint8)(cs % 8), 0, 0};
	strcpy(folder.name, name);
	writeDirectory(folder, (uint32)(cl / 8), (uint8)(cl % 8));
	DIR reg = {"..", 3, 1, actualClusterDIR, (uint8)actualDirectoryDIR, 0, 0};
	writeDirectory(reg, (uint32)(cs / 8), cs % 8);

	uint32 aux1 = actualClusterDIR, aux2 = actualDirectoryDIR;

	uint32 ax, ay;
	DIR actualDir = {};
	int i = 0;
	do
	{
		ax = actualDir.nextCluster;
		ay = actualDir.nextDirection;
		readDirectory(&actualDir, actualClusterDIR, actualDirectoryDIR);

		actualClusterDIR = actualDir.nextCluster;
		actualDirectoryDIR = actualDir.nextDirection;
		i++;
	} while (actualClusterDIR != 0);

	actualDir.nextCluster = (uint32)(cl / 8);
	actualDir.nextDirection = cl % 8;
	if (i == 1)
	{
		ax = aux1;
		ay = aux2;
	}
	writeDirectory(actualDir, ax, ay);

	actualClusterDIR = aux1;
	actualDirectoryDIR = aux2;
	return 1;
}
// int createDirectory
//
int modifyFile(char name[49], DIR File)
{
	uint32 aux1 = actualClusterDIR, aux2 = actualDirectoryDIR;
	DIR actualDir = {};
	do
	{
		readDirectory(&actualDir, actualClusterDIR, actualDirectoryDIR);

		if (strcmp(actualDir.name, name) == 0)
		{
			writeDirectory(File, actualClusterDIR, actualDirectoryDIR);
		}
		actualClusterDIR = actualDir.nextCluster;
		actualDirectoryDIR = actualDir.nextDirection;

	} while (actualClusterDIR != 0);
	actualClusterDIR = aux1;
	actualDirectoryDIR = aux2;
	return 1;
}

int createFile(char name[49])
{
	uint32 cl = retClusterFree(1);
	uint32 cs = retClusterFree(64);
	DIR folder = {"", 1, 10, (uint32)(cs / 8), (uint8)(cs % 8), 0, 0};
	strcpy(folder.name, name);
	writeDirectory(folder, (uint32)(cl / 8), cl % 8);
	DIR data = {".", 10, 1, 0, 0, 0, 0};
	writeDirectory(folder, (uint32)(cs / 8), cs % 8);

	uint32 aux1 = actualClusterDIR, aux2 = actualDirectoryDIR;

	uint32 ax, ay;
	DIR actualDir = {};
	int i = 0;
	do
	{
		ax = actualDir.nextCluster;
		ay = actualDir.nextDirection;
		readDirectory(&actualDir, actualClusterDIR, actualDirectoryDIR);

		actualClusterDIR = actualDir.nextCluster;
		actualDirectoryDIR = actualDir.nextDirection;
		i++;
	} while (actualClusterDIR != 0);

	actualDir.nextCluster = (uint32)(cl / 8);
	actualDir.nextDirection = cl % 8;
	if (i == 1)
	{
		ax = aux1;
		ay = aux2;
	}
	writeDirectory(actualDir, ax, ay);

	actualClusterDIR = aux1;
	actualDirectoryDIR = aux2;
	return 1;
}

DIR searchFile(char *name)
{
	int a = 0;
	uint32 aux1 = actualClusterDIR, aux2 = actualDirectoryDIR;
	DIR actualDir = {};
	do
	{
		readDirectory(&actualDir, actualClusterDIR, actualDirectoryDIR);

		actualClusterDIR = actualDir.nextCluster;
		actualDirectoryDIR = actualDir.nextDirection;
		if (strcmp(actualDir.name, name) == 0 && actualDir.flags == 1)
		{
			actualClusterDIR = aux1;
			actualDirectoryDIR = aux2;
			return actualDir;
		}
	} while (actualClusterDIR != 0);
	actualClusterDIR = aux1;
	actualDirectoryDIR = aux2;
	printf("No se encontro el archivo");
	actualDir.name[0] = 0;
	return actualDir;
}

int readFiles()
{
	DIR actualDir = {};
	int i = 0;
	uint32 aux1 = actualClusterDIR, aux2 = actualDirectoryDIR;
	do
	{
		i++;
		readDirectory(&actualDir, actualClusterDIR, actualDirectoryDIR);
		actualClusterDIR = actualDir.nextCluster;
		actualDirectoryDIR = actualDir.nextDirection;
		if (i != 1)
			printf("/n");
		changeColor(actualDir.flags == 1 ? 0x0f : 0x03);
		printf("-");
		printf(actualDir.name);
	} while (actualClusterDIR != 0);
	actualClusterDIR = aux1;
	actualDirectoryDIR = aux2;
	return i;
}

int treeFileSystem()
{
	DIR actualDir = {};
	int i = 0;
	int stack = 1;

	uint32 stackCluster[100], stackDirectory[100];
	uint32 aux1 = actualClusterDIR, aux2 = actualDirectoryDIR;
	uint32 max1 = actualClusterDIR, max2 = actualDirectoryDIR;
	do
	{
		i++;
		readDirectory(&actualDir, actualClusterDIR, actualDirectoryDIR);
		actualClusterDIR = actualDir.nextCluster;
		actualDirectoryDIR = actualDir.nextDirection;
		if (i != 1)
			printf("/n");
		changeColor(actualDir.flags == 1 ? 0x0f : 0x03);
		for (int p = 0; p < stack; p++)
			printf("-");
		printf(actualDir.name);
		if (actualDir.flags == 2)
		{
			stackCluster[stack] = max1;
			stackDirectory[stack] = max2;

			changeDirectory(actualDir.name);
			stack++;
		}
		if (actualClusterDIR == 0 && max1 != aux1 && max2 != aux2)
		{
		}

	} while (actualClusterDIR != 0);
	actualClusterDIR = aux1;
	actualDirectoryDIR = aux2;
	return i;
}

int changeDirectory(char *name)
{
	int a = 0;
	uint32 aux1 = actualClusterDIR, aux2 = actualDirectoryDIR;
	DIR actualDir = {};
	do
	{
		readDirectory(&actualDir, actualClusterDIR, actualDirectoryDIR);

		actualClusterDIR = actualDir.nextCluster;
		actualDirectoryDIR = actualDir.nextDirection;
		if (strcmp(actualDir.name, name, 0) == 0 && actualDir.flags > 1)
		{
			a = a | 1;
			actualClusterDIR = actualDir.dataCluster;
			actualDirectoryDIR = actualDir.dataDirection;
			break;
		}
		else
		{
			a = a | 0;
		}
	} while (actualClusterDIR != 0);
	if (a == 0)
	{
		actualClusterDIR = aux1;
		actualDirectoryDIR = aux2;
	}
	return a;
}
