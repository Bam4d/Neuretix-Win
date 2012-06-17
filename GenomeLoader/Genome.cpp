/* 
 * File:   Genome.cpp
 * Author: bam4d
 * 
 * Created on 23 May 2012, 1:28
 */

#include "GenomeLoaderGlobals.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <iostream>

using namespace std;


Genome::Genome() {

    CoCreateGuid(&GUUID); //generate a UUID for this genome
}

Genome::Genome(const Genome& orig) {
}

int Genome::SaveGenome(string directory)
{
    char *guuid_s;
    UuidToString(&GUUID,(RPC_CSTR*)guuid_s);

    string guuid_ss = string(guuid_s);
    string _genomeDir = directory + "/Genome (" + guuid_ss +")";

    //create a directory with the ClusterMap UUID number
	if(CreateDirectory(_genomeDir.c_str(),NULL) == ERROR_ALREADY_EXISTS)
        return 2; //ClusterMap already saved....
    // save all the ClusterMaps in the genome
    for(int num_ClusterMaps = 0; num_ClusterMaps<ClusterMaps.size(); num_ClusterMaps++)
        ClusterMaps[num_ClusterMaps].Save(_genomeDir);

    //save all the ClusterMap links
    for(int num_links = 0; num_links<ClusterMapLinks.size(); num_links++)
        ClusterMapLinks[num_links].Save(_genomeDir);

}

int Genome::LoadGenome(string directory, string genomeID, bool delAfterLoad)
{
    char buf[255];
    sprintf(buf,"loading genome: %s\n",genomeID);
    cout<<buf;

    string _genomePath = directory + "/Genome ("+string(genomeID)+")";

	UuidFromString((RPC_CSTR)genomeID.c_str(),&GUUID);

    Clear();
    vector<string> _clusterMaps;
    vector<string> _clusterMapLinks;
    vector<string> _sensorDescriptionLists;
    string  _agentData;
    HANDLE _genomeDirEnt;

	WIN32_FIND_DATA ffd;
     //get Chromosomes
    _genomeDirEnt = FindFirstFile((_genomePath+"/*").c_str(),&ffd);
    if (_genomeDirEnt != INVALID_HANDLE_VALUE)
    {
       do 
       {
		   if(strcmp(ffd.cFileName,".")!=0 && strcmp(ffd.cFileName,"..")!=0) //filter current directiory and previous directory
           {
               string _headerToFind;
			   string _headerDir = _genomePath+"/"+string(ffd.cFileName);

               _headerToFind = _headerDir+"/header.chr";
               if(GetFileAttributes(_headerToFind.c_str())!=INVALID_FILE_ATTRIBUTES)
               _clusterMaps.push_back(string(ffd.cFileName));

               _headerToFind = _headerDir+"/header.lnk";
               if(GetFileAttributes(_headerToFind.c_str())!=INVALID_FILE_ATTRIBUTES)
               _clusterMapLinks.push_back(string(ffd.cFileName));

            }
        }
		while (FindNextFile(_genomeDirEnt,&ffd)); //iterate through the files in the genome
        FindClose (_genomeDirEnt);
    }
    else return 1;


    for(int _clusterMapsCount = 0; _clusterMapsCount<_clusterMaps.size(); _clusterMapsCount++)
    {
        if(LoadClusterMap(_genomePath,_clusterMaps[_clusterMapsCount])==0)
                   cout<<"\t\t-ClusterMap loaded!\n\n";
    }

     for(int _clusterMapLinksCount = 0; _clusterMapLinksCount<_clusterMapLinks.size(); _clusterMapLinksCount++)
    {
        if(LoadLink(_genomePath,_clusterMapLinks[_clusterMapLinksCount])==0)
                   cout<<"\t\t-Link loaded!\n\n";
    }
    
    if(delAfterLoad) DirectoryDelete(_genomePath.c_str());
     
   return 0;
}

int Genome::DirectoryDelete(const char* dirName)
{
    struct dirent *entry;
    char path[MAX_PATH];
	HANDLE _genomeDirEnt;

	WIN32_FIND_DATA ffd;

	
    _genomeDirEnt = FindFirstFile(dirName,&ffd);
    if (_genomeDirEnt == INVALID_HANDLE_VALUE) {
        perror("Error opendir()");
        return 1;
    }

    while (FindNextFile(_genomeDirEnt,&ffd)) {
        if (strcmp(ffd.cFileName, ".") && strcmp(ffd.cFileName, "..")) {
            _snprintf(path, (size_t) MAX_PATH, "%s/%s", dirName, ffd.cFileName);
			if (ffd.dwFileAttributes == _A_SUBDIR) {
                DirectoryDelete(path);
            }

             DeleteFile(path);
             
        }

    }
    FindClose(_genomeDirEnt);

    //remove the folder
    RemoveDirectory(dirName);
    
    return 0;
}

int Genome::LoadClusterMap(string genomeDir, string ClusterMapUUID)
{
    char buf[255];
    string _ClusterMapDirStr;
    CMarkup _ClusterMapHeader;
	DWORD _dwAttrib;
	DWORD x = INVALID_FILE_ATTRIBUTES;

    _ClusterMapDirStr = genomeDir + "/" + ClusterMapUUID;
	_dwAttrib = GetFileAttributes(_ClusterMapDirStr.c_str());
    
	if (_dwAttrib!=INVALID_FILE_ATTRIBUTES)
	{
       //check for normal ClusterMap first
       if(_ClusterMapHeader.Load((_ClusterMapDirStr+"/header.chr").c_str()) == true)
       {
           sprintf(buf,"\tClusterMap: %s\n",ClusterMapUUID);
           cout<<buf;
           _ClusterMapHeader.FindElem("ClusterMap");
           ClusterMaps.push_back(ClusterMap(atoi(_ClusterMapHeader.GetAttrib("Dominance").c_str()),ClusterMapUUID));

           //load the clusters
           int _clustersCount = SaveLoadCNN::LoadClusterData(_ClusterMapDirStr+"/clusters.xml",&ClusterMaps.back().clusters);
           sprintf(buf,"\t\t-Loaded: %d clusters\n",_clustersCount);
           cout<<buf;

           //load the axons
           int _axonsCount = SaveLoadCNN::LoadAxonData(_ClusterMapDirStr+"/axons.xml",&ClusterMaps.back().Axons,&ClusterMaps.back().clusters);
           sprintf(buf,"\t\t-Loaded: %d axons.\n",_axonsCount);
           cout<<buf;
       }
       else return 1; //is not a ClusterMap
   }
   else return 2; //could not open the ClusterMap directory

   //closedir (_ClusterMapDir);

    return 0;
}

int Genome::LoadLink(string genomeDir, string ClusterMapLinkUUID)
{
	// in header.lnk SrcUUID="2821bf5b-24b2-43ec-b7cc-1d31f3612422" DstUUID="c39a659c-b49e-4778-bed8-34fde93aea06"/>
	char buf[255];
	string _linkDirStr;
	CMarkup _linkHeader;
	DWORD _dwAttrib;

	_linkDirStr = genomeDir + "/" + ClusterMapLinkUUID;

	_dwAttrib = GetFileAttributes(_linkDirStr.c_str());
    
	if (!_dwAttrib&INVALID_FILE_ATTRIBUTES)
	{
       //check for normal ClusterMap first
       if(_linkHeader.Load((_linkDirStr+"/header.lnk").c_str()) == true)
       {
           sprintf(buf,"\tLink: %s\n",ClusterMapLinkUUID.c_str());
           cout<<buf;

           GUID _srcUUID;
           GUID _destUUID;
           _linkHeader.FindElem("LinkData");
           UuidFromString((RPC_CSTR)_linkHeader.GetAttrib("SrcUUID").c_str(),&_srcUUID);
           UuidFromString((RPC_CSTR)_linkHeader.GetAttrib("DstUUID").c_str(),&_destUUID);

           ClusterMapLinks.push_back(ClusterMapLink(_destUUID,_srcUUID,ClusterMapLinkUUID));

           ClusterMap* _srcChrom = ClusterMap::FindClusterMapByUUID(_srcUUID,&ClusterMaps);
           if(_srcChrom == NULL)
           {
               char uuid_s[36];
               UuidToString(&_srcUUID,(RPC_CSTR *)uuid_s);
               sprintf(buf,"\t\t-ClusterMap with UUID: %s not found in genome. Aborting link load.",uuid_s);
               cout<<buf;
               return 3;
           }
           ClusterMap* _destChrom = ClusterMap::FindClusterMapByUUID(_destUUID,&ClusterMaps);
           if(_destChrom == NULL)
           {
               char uuid_s[36];
               UuidToString(&_destUUID,(RPC_CSTR *)uuid_s);
               sprintf(buf,"\t\t-ClusterMap with UUID: %s not found in genome. Aborting link load.",uuid_s);
               cout<<buf;
               return 3;
           }

           //load the axons from the link
           int _axonsCount = SaveLoadCNN::LoadAxonData(_linkDirStr+"/axons.xml",&ClusterMapLinks.back().join_axons,&_destChrom->clusters,&_srcChrom->clusters);
           
           sprintf(buf,"\t\t\t-Loaded: %d axons.\n",_axonsCount);
           cout<<buf;
       }
       else return 1; //is not a link
   }
   else return 2; //could not open the link directory

   //closedir (_linkDir);

    return 0;
}



void Genome::MoveAllClusters(PlotPoint newLocation)
{
    for(int num_ClusterMaps = 0; num_ClusterMaps<ClusterMaps.size(); num_ClusterMaps++)
        for(int num_clusters = 0; num_clusters<ClusterMaps[num_ClusterMaps].clusters.size(); num_clusters++)
            ClusterMaps[num_ClusterMaps].clusters[num_clusters]->Move(ClusterMaps[num_ClusterMaps].clusters[num_clusters]->grouppos+newLocation);
}

void Genome::Clear()
{
    if(ClusterMaps.size()>0)
    {
        for(int maps = 0; maps<ClusterMaps.size(); maps++)
            ClusterMaps[maps].Clear();
        
        ClusterMaps.clear();
    }
    if(ClusterMapLinks.size()>0)
    {
        for(int maps = 0; maps<ClusterMapLinks.size(); maps++)
            ClusterMapLinks[maps].Clear();
        
        ClusterMapLinks.clear();
    }
}

Genome::~Genome() {
    
}

