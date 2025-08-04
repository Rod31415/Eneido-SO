#include "headers/vfs.h"

#define STARTCLUSTER  0x400000
#define STARTDATA     0x1400000
/*
#define MAXDIRECTIONS 4096

#define MAXCLUSTERFILE 16
#define MAXCLUSTERFILESYSTEM 16384

#define DIRPERCLUSTER 8

#define FIRSTDATACLUSTER 512
*/
#define SECTORSIZE 512
#define CLUSTERSIZE 8
#define BYTSPERCLUS SECTORSIZE*CLUSTERSIZE
#define MAXCLUSTERS 0x1000000


//#define DEBUG


uint32 actualClusterDIR = 0;
uint32 actualDirectoryDIR = 0;

DIR root;
typedef uint32 CLUSTER;
typedef uint32 ENTRY;

#define COMBINE_WORD(msb,lsb) (((uint32)(msb)<<16)|lsb)

void initVFS(){

}

void readDirectoryFromN(uint8* buffer, ENTRY N, CLUSTER P){

}


void* getClusterEntryFromP(CLUSTER P){
return (void*)(STARTCLUSTER+P*4);
}

void freeCluster(CLUSTER P){
	*(uint32*)getClusterEntryFromP(P)=0xFFFFFFFF;
}

void freeDirectoryEntry(CLUSTER P){
	
	for(uint32 i=0;i<BYTSPERCLUS;i+=32){
		((uint8*)i)[0]=0;
	}
}

void prepareClusters(){
	for(uint32 i=0;i<MAXCLUSTERS;i++){
		freeCluster(i);
		//freeDirectoryEntry(i);
	}
}


CLUSTER getNextCluster(CLUSTER P){
	return *(uint32*)getClusterEntryFromP(P);
}

CLUSTER findFirstCluster(CLUSTER startP=0){
	for(CLUSTER i=startP;i<MAXCLUSTERS;i++){
		if(getNextCluster(i)==0xFFFFFFFF)
			return i;
	}
	return 0xFFFFFFFF;
}

void assignCluster(CLUSTER P, CLUSTER pointer){
*((uint32*)getClusterEntryFromP(P))=pointer;
#ifdef DEBUG
printf("Cluster asignado :%d /n",*((uint32*)getClusterEntryFromP(P)));
#endif
}

CLUSTER assignNewClustersFreeFile(int32 size){

CLUSTER freeCluster=findFirstCluster();
CLUSTER retCluster=freeCluster;
	for(int32 i=0;i<size-1;i++){
		CLUSTER nextCluster=findFirstCluster(freeCluster+1);
		assignCluster(freeCluster,nextCluster);
		freeCluster=nextCluster;		
	}
	assignCluster(freeCluster,0xFFFFFFF0);
	return retCluster;
}

void assignNewClustersFullFile(CLUSTER P,uint32 size){
CLUSTER freeCluster=P;
CLUSTER aux;
	do{
		aux=freeCluster;
		freeCluster=getNextCluster(freeCluster);
	}while(freeCluster!=0xFFFFFFF0);
	freeCluster=aux;
	for(uint32 i=0;i<size;i++){
		CLUSTER nextCluster=findFirstCluster(freeCluster+1);
		assignCluster(freeCluster,nextCluster);
		freeCluster=nextCluster;		
	}
	
}

void* getClusterDataFromP(CLUSTER P){
return (void*)(STARTDATA+P*BYTSPERCLUS);
}

uint32 searchFreeEntry(CLUSTER P){
	for(uint32 i=0;i<BYTSPERCLUS;i+=32){
		DIR aux=(DIR)(getClusterDataFromP(P)+i);
		#ifdef DEBUG
		printf("Entry :%d/n",aux->Name[0]);
		#endif
		if((uint8)aux->Name[0]==0xFF || aux->Name[0]==0xE5){
			#ifdef DEBUG
			printf("Entry vacia encontrada en : %d/n",(i>>5));
			#endif
			return (i>>5);
		}
	}
}

DIR assignDirectoryOnEntryN(CLUSTER P,ENTRY N,DIR entryDir){
	uint32 address=(uint32)(getClusterDataFromP(P)+(N<<5));
	#ifdef DEBUG
	printf("assigment in addr: %d - %d - %d/n",address,P,N);
	#endif
memcpy((uint32)entryDir,address,32);
return (DIR)address;
}

void assignNewDirectoryEntry(DIR directory,DIR entryDir){
	
CLUSTER point=assignNewClustersFreeFile(entryDir->FileSize);
entryDir->FstClusHI=(point>>16)&0xFFFF;
entryDir->FstClusLO=(point&0xFFFF);
#ifdef DEBUG
printf("CLUSTER ENCONTRADO %d/n",COMBINE_WORD(directory->FstClusHI,directory->FstClusLO));
#endif
uint32 e=searchFreeEntry(COMBINE_WORD(directory->FstClusHI,directory->FstClusLO));

 assignDirectoryOnEntryN(COMBINE_WORD(directory->FstClusHI,directory->FstClusLO),e,entryDir);
}

DIR seeDirectoryEntry(CLUSTER P, ENTRY N){ 
       return (DIR)(getClusterDataFromP(P)+(N<<5));
}

void seeDirectoriesOnClusterP(CLUSTER P,uint32 stack){
	for(uint32 i=0;i<CLUSTERSIZE*16;i++){
		DIR f=seeDirectoryEntry(P,i);
		if((uint8)f->Name[0]==0xFF)break;
		for(uint32 i=0;i<stack;i++)
		printf("   ");
		printf(f->Name);
		printf("/n");
		if(f->Attr==0x10&&f->Name[0]!='.'){
			seeDirectoriesOnClusterP(COMBINE_WORD(f->FstClusHI,f->FstClusLO),stack+1);
		}
	}
}

void seeAllVFSFromClusterP(CLUSTER P){
	for(uint32 i=0;i<CLUSTERSIZE*16;i++){
		DIR f=seeDirectoryEntry(P,i);
		if((uint8)f->Name[0]==0xFF)break;
		printf(f->Name);
		printf("/n");
		if(f->Attr==0x10&&f->Name[0]!='.'){
			P=COMBINE_WORD(f->FstClusHI,f->FstClusLO);
		}
	}
}


DIR createNewFile(DIR directory,char* name,uint32 fileSize){
	DIR newDir;
		newDir->Attr=0x20;
		strcpy(newDir->Name,name,11);
		newDir->FileSize=fileSize;
		assignNewDirectoryEntry(directory,newDir);
}

DIR createNewDirectory(DIR directory,char* name,uint32 fileSize){
	DIR newDir;
		newDir->Attr=0x10;
		strcpy(newDir->Name,name,11);
		newDir->FileSize=fileSize;
	
		DirectoryStruct returnDir;

		assignNewDirectoryEntry(directory,newDir);
		returnDir.Attr=0x10;
		strcpy(returnDir.Name,"..         ",11);
		returnDir.FileSize=0;
		returnDir.FstClusHI=directory->FstClusHI;
		returnDir.FstClusLO=directory->FstClusLO;
		assignDirectoryOnEntryN(COMBINE_WORD(newDir->FstClusHI,newDir->FstClusLO),0,&returnDir);
		
}

DIR getDirectoryEntryByName(DIR directory,char* name){
for(uint32 i=0;i<CLUSTERSIZE*16;i++){
		DIR f=seeDirectoryEntry(COMBINE_WORD(directory->FstClusHI,directory->FstClusLO),i);
		if((uint8)f->Name[0]==0xFF)break;
		if(strcmp(f->Name,name,11)==0)return f;
	}
	return directory;
}

void seeClusterEntries(){
	for(uint32 i=0;i<16;i++){
		printf("%d ",*(uint32*)getClusterEntryFromP(i));
	}
}

DIR GlobalDir;
void initRootDirectory(){
	prepareClusters();
	DIR rootDIRaddress,root;
	strcpy(root->Name,".          ",11);
	root->Attr=0x10;
	root->FstClusHI=0;
	root->FstClusLO=0;
	root->FileSize=0;
	rootDIRaddress=root;
	DIR dot;
	memcpy((uint32)root,(uint32)dot,32);
	assignNewDirectoryEntry(root,dot);

	strcpy(dot->Name,"..         ",11);
	assignNewDirectoryEntry(root,dot);

	createNewFile(root,"LEEME   TXT",1);
	//seeDirectoriesOnClusterP(0,0);

	GlobalDir=root;
}


POSFILE GlobalPos=0;

FILE* fopen(char* filename){
 return getDirectoryEntryByName(GlobalDir,filename);
}



void fputs(char* data, FILE* f){
uint32 lenght=lenghtStr(data);
char* fptr=(char*)(getClusterDataFromP(COMBINE_WORD(f->FstClusHI,f->FstClusLO))+GlobalPos);
strcpy(fptr,data,lenght);
GlobalPos+=lenght;
}

void fgets(char* data, uint32 lenght, FILE* f){
char* fptr=(char*)(getClusterDataFromP(COMBINE_WORD(f->FstClusHI,f->FstClusLO))+GlobalPos);
strcpy(data,fptr,lenght);
GlobalPos+=lenght;
}

void fseek(POSFILE point,FILE* f){
	GlobalPos=point;
}