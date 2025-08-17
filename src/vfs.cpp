#include "headers/vfs.h"
#define BEFOREFRAMEBUFFER 0xD000000
#define AFTERFRAMEBUFFER 0xD12C000
#define GOODPLACEMENT 0x1000000
#define STARTCLUSTER  GOODPLACEMENT
//					  0xD000000
#define STARTDATA     GOODPLACEMENT+0x2000
/*
#define MAXDIRECTIONS 4096

#define MAXCLUSTERFILE 16
#define MAXCLUSTERFILESYSTEM 16384

#define DIRPERCLUSTER 8

#define FIRSTDATACLUSTER 512
*/
#define SECTORSIZE 512
#define CLUSTERSIZE 4
#define BYTSPERCLUS 2048
#define MAXCLUSTERS 0x200000


//#define DEBUG


uint32 actualClusterDIR = 0;
uint32 actualDirectoryDIR = 0;

DIR root;

#define COMBINE_WORD(msb,lsb) (((uint32)(msb)<<16)|lsb)

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
	#ifdef DEBUG
	printf("Primer cluster libre: %d/n",freeCluster);
	#endif
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
		printf("Entry :%d/n",(i>>5));
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

DIR assignNewDirectoryEntry(DIR directory,DIR entryDir){
	
CLUSTER point=assignNewClustersFreeFile(entryDir->FileSize);
entryDir->FstClusHI=(point>>16)&0xFFFF;
entryDir->FstClusLO=(point&0xFFFF);
#ifdef DEBUG
printf("CLUSTER ENCONTRADO %d/n",COMBINE_WORD(directory->FstClusHI,directory->FstClusLO));
#endif
uint32 e=searchFreeEntry(COMBINE_WORD(directory->FstClusHI,directory->FstClusLO));

 return assignDirectoryOnEntryN(COMBINE_WORD(directory->FstClusHI,directory->FstClusLO),e,entryDir);
}

DIR seeDirectoryEntry(CLUSTER P, ENTRY N){ 
       return (DIR)(getClusterDataFromP(P)+(N<<5));
}

void seeDirectoriesOnClusterP(CLUSTER P){
	for(uint32 i=0;i<CLUSTERSIZE*16;i++){
		DIR f=seeDirectoryEntry(P,i);
		if((uint8)f->Name[0]==0xFF)break;
		printf(f->Name);
		printf("/n");
	}
}

void seeAllVFSFromClusterP(CLUSTER P,uint32 stack){
	for(uint32 i=0;i<CLUSTERSIZE*16;i++){
		DIR f=seeDirectoryEntry(P,i);
		if((uint8)f->Name[0]==0xFF)break;
		for(uint32 i=0;i<stack;i++)
		printf("   ");
		printf(f->Name);
		printf("/n");
		if(f->Attr==0x10&&f->Name[0]!='.'){
			seeAllVFSFromClusterP(COMBINE_WORD(f->FstClusHI,f->FstClusLO),stack+1);
		}
	}
}


DIR createNewFile(DIR directory,char* name,uint32 fileSize){
	DIR newDir;
		newDir->Attr=0x20;
		strcpy(newDir->Name,name,11);
		newDir->FileSize=fileSize;
		return assignNewDirectoryEntry(directory,newDir);
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
		strcpy(newDir->Name,".          ",11);
		assignDirectoryOnEntryN(COMBINE_WORD(newDir->FstClusHI,newDir->FstClusLO),0,newDir);
		assignDirectoryOnEntryN(COMBINE_WORD(newDir->FstClusHI,newDir->FstClusLO),1,&returnDir);
		strcpy(newDir->Name,name,11);
		return newDir;
		
}
char parsename[12];
char* parseFATnames(char* filename){

uint32 index=0;
bool ret=false;

if(filename[0]=='.'){parsename[0]='.';index++;ret=true;}
if(filename[0]=='.'&&filename[1]=='.'){parsename[0]='.';parsename[1]='.';index++;ret=true;}

if(ret){
	while(index<11){
					parsename[index]=' ';
					index++;
				}
}
else
for(uint32 i=0;i<11;i++){
		
		if(filename[i]==0){
			while(index<11){
					parsename[index]=' ';
					index++;
				}
				break;
		}
        if(filename[i]=='.'){
				while(index<8){
					parsename[index]=' ';
					index++;
				}
        }
        else if(filename[i]>'Z'){
        parsename[index]=filename[i]-32;
		index++;
        }
        else{parsename[index]=filename[i];
		index++;}
}
parsename[11]=0;

return parsename;
}


DIR getDirectoryEntryByName(DIR directory,char* name){
for(uint32 i=0;i<CLUSTERSIZE*16;i++){
		DIR f=seeDirectoryEntry(COMBINE_WORD(directory->FstClusHI,directory->FstClusLO),i);
		if((uint8)f->Name[0]==0xFF)break;
		if(strcmp(f->Name,name,11)==0)return f;
	}
	return (DIR)-1;
}



void seeClusterEntries(){
	for(uint32 i=0;i<16;i++){
		printf("%d ",*(uint32*)getClusterEntryFromP(i));
	}
}

DIR GlobalDir;
DIR BinDirGlobal;

bool isBMP(void* start){
char *buf=(char*)start;
	if(buf[0]=='B'&&buf[1]=='M'){
		return true;
	}
	return false;
}
bool isELF(void* start){
	char *buf=(char*)start;
	if(buf[1]=='E'&&buf[2]=='L'&&buf[3]=='F'){
		return true;
	}
	return false;
}
uint32 imagesN=0;
uint32 elfN=0;
void createModulesFiles(DIR root,void* start,uint32 size,char* name){
	char file[11];
	
	strcpy(file,name);
	/*char img[11]="img";
	char exe[11]="file";


	if(isBMP(start)){
		imagesN++;
		char bmp[6]={imagesN+48,'.','B','M','P',0};
		strcpy(img+3,bmp,6);*/
		DIR f=createNewFile(root, parseFATnames(name),size/BYTSPERCLUS+1);
		char* fptr=(char*)(getClusterDataFromP(COMBINE_WORD(f->FstClusHI,f->FstClusLO)));
		memcpy((uint32)start,(uint32)fptr,size);
	/*}
	if(isELF(start)){
		elfN++;
		char elf[6]={elfN+48,'.','E','L','F',0};
		strcpy(exe+4,elf,6);
		DIR f=createNewFile(root,parseFATnames(exe),size/BYTSPERCLUS+1);
		char* fptr=(char*)(getClusterDataFromP(COMBINE_WORD(f->FstClusHI,f->FstClusLO)));
		memcpy((uint32)start,(uint32)fptr,size);
	}*/
}

static char* binNames[7]= {"cat         ",
				 		   "touch       ",
						   "echo        ",
						   "help        ",
						   "license     ",
						   "editor      ",
						   "hexdump     "};

void initRootDirectoryWithModules(multiboot_info *mb){


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

	
	
	
	createNewDirectory(root,          "BIN        ",1);
	root=getDirectoryEntryByName(root,parseFATnames("bin"));
	BinDirGlobal=root;


	createNewFile(root,"LEEME   TXT",1);

	
	DIR f=getDirectoryEntryByName(root,"LEEME   TXT");
	char* fptr=(char*)(getClusterDataFromP(COMBINE_WORD(f->FstClusHI,f->FstClusLO)));
	char data[50]={'A','B','C','N','C',0xA,'H','O','L','A',-1};
	memcpy((uint32)data,(uint32)fptr,11);

	//root=getDirectoryEntryByName(root,parseFATnames(".."));

	module_info* md=(module_info*)mb->mods_addr;
	for(uint32 i=0;i<mb->mods_count;i++){
		createModulesFiles(root,(void*)md[i].mod_start,(md[i].mod_end-md[i].mod_start),binNames[i]);
		
	}
	//seeDirectoriesOnClusterP(0,0);

	GlobalDir=root;
}

DIR retBinDirectory(){
return BinDirGlobal;
}

void listDirectories(){
	seeDirectoriesOnClusterP(COMBINE_WORD(GlobalDir->FstClusHI,GlobalDir->FstClusLO));
}

bool changeDirectory(char* str){
	DIR auxDir;
	auxDir=getDirectoryEntryByName(GlobalDir,parseFATnames(str));
	if(auxDir==(DIR)-1||auxDir->Attr!=0x10)return false;
	if(str[0]=='.'&&str[1]=='.'){
		auxDir=seeDirectoryEntry(COMBINE_WORD(auxDir->FstClusHI,auxDir->FstClusLO),0);
	}
	GlobalDir=auxDir;
	return true;
}

DIR fCreateNewDirectory(char* str){
	return createNewDirectory(GlobalDir,parseFATnames(str),1);
}

DIR fCreateNewFile(char* str){
	return createNewFile(GlobalDir,parseFATnames(str),1);
}



KFILE* allocFileDescriptor(DIR f,uint32 flags){
	KFILE* ptr=(KFILE*)kalloc(20);
	ptr->entry=f;
	ptr->pos=(flags&O_APPEND)?f->FileSize:0;
	ptr->size=f->FileSize;
	ptr->used=1;
	ptr->flags=flags;
	return ptr;
}


bool freeFileDescriptor(KFILE* file){
	return kfree((void*)file);
}





KFILE* kopen(char* filename,uint32 flags){
	char auxFilename[12];
	memcpy((uint32)parseFATnames(filename),(uint32)auxFilename,12);
 	DIR f=getDirectoryEntryByName(GlobalDir,auxFilename);
	if(f==(DIR)-1){
		if(!(flags&O_CREAT))
		return (KFILE*)-1;
		
		f=fCreateNewFile(filename);
	}
	if(flags&O_EXCL)
		return (KFILE*)-1;
	return allocFileDescriptor(f,0);
}

bool kclose(KFILE* file){
	return freeFileDescriptor(file);
}

DIR kopendir(DIR directory,char* filename){
	memcpy((uint32)parseFATnames(filename),(uint32)filename,12);
 return getDirectoryEntryByName(GlobalDir,filename);
}

void kputs(char* data, uint32 lenght, KFILE* f){
char* fptr=(char*)(getClusterDataFromP(COMBINE_WORD(f->entry->FstClusHI,f->entry->FstClusLO))+f->pos);
strcpy(fptr,data,lenght);
f->pos+=lenght;
}

void kgets(char* data, uint32 lenght, KFILE* f){
char* fptr=(char*)(getClusterDataFromP(COMBINE_WORD(f->entry->FstClusHI,f->entry->FstClusLO))+f->pos);
strcpy(data,fptr,lenght);
f->pos+=lenght;
}

void* kgetpointer(KFILE* f){
return (void*)(getClusterDataFromP(COMBINE_WORD(f->entry->FstClusHI,f->entry->FstClusLO)));
}

void kseek(POSFILE point,KFILE* f){
	f->pos=point;
}