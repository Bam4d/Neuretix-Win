/* 
 * File:   ClusterMap.cpp
 * Author: bam4d
 * 
 * Created on 23 May 2012, 1:28
 */

#include "GenomeLoaderGlobals.h"
#include <errno.h>
#include <sys/stat.h>

ClusterMap::ClusterMap() {

    UuidCreate(&UUID_inst);
    Dominance = 0;
}

ClusterMap::ClusterMap(int dominance, string uuid_str)
{
    Dominance = dominance;
    UuidFromString((RPC_CSTR)uuid_str.c_str(),&UUID_inst);
}

void ClusterMap::Mutate()
{

}

void ClusterMap::Cross(void* object)
{
}

int ClusterMap::Save(string directory)
{
    
    char uuid_s[36];
     UuidToString(&UUID_inst,(RPC_CSTR*)uuid_s);

    string uuid_ss = string(uuid_s);
    string _ClusterMapDir = directory + "/" + uuid_ss;

	LPSECURITY_ATTRIBUTES attr = NULL;

    //create a directory with the ClusterMap UUID number
	CreateDirectory(_ClusterMapDir.c_str(),attr);
    //mkdir(_ClusterMapDir.c_str(),S_IFDIR | S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if(errno == EEXIST)
        return 2; //ClusterMap already saved....

    SaveLoadCNN::SaveClusterData(clusters,_ClusterMapDir+"/clusters.xml");
    SaveLoadCNN::SaveAxonData(Axons,_ClusterMapDir+"/axons.xml");

    SaveClusterMapData(_ClusterMapDir+"/header.chr");

    return 0;
}

int ClusterMap::SaveClusterMapData(string filename)
{
    char buf[50];//used for conversion operations

    CMarkup ClusterMapXml;

    ClusterMapXml.AddElem( "ClusterMapData" );
    sprintf(buf,"%d",Dominance);
    ClusterMapXml.SetAttrib("Dominance",buf);

    ClusterMapXml.Save(filename);

    return 0;
}

ClusterMap* ClusterMap::FindClusterMapByUUID(GUID uUID, vector<ClusterMap>* ClusterMapList) //retunrs a pointer to the group with ID==ID in a cluster
{
	RPC_STATUS stat;
    if(ClusterMapList!=NULL)
    {
	for(int ch_c = 0; ch_c<ClusterMapList->size(); ch_c++)
		if(UuidCompare(&ClusterMapList->at(ch_c).UUID_inst,&uUID,&stat)==0) return &ClusterMapList->at(ch_c);
    }
	return NULL;
}

void ClusterMap::Clear()
{
        for(int num_groups = 0; num_groups<clusters.size(); num_groups++)
        {
            delete clusters[num_groups];
        }
    
    clusters.clear();

    
    Axons.Empty(); //clear linked list and delete all axons
}


ClusterMap::~ClusterMap() {

    
}

