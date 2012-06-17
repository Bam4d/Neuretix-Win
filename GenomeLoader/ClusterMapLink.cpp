/* 
 * File:   ClusterMapLink.cpp
 * Author: bam4d
 * 
 * Created on 23 May 2012, 1:28
 */

#include "GenomeLoaderGlobals.h"
#include <errno.h>
#include <sys/stat.h>

ClusterMapLink::ClusterMapLink()
{
    UuidCreate(&UUID_inst);
}

ClusterMapLink::ClusterMapLink(GUID dest, GUID src) {

    UuidCreate(&UUID_inst);
    UuidCopy(&SourceClusterMapUUID,&src);
    UuidCopy(&DestinationClusterMapUUID,&dest);

}

ClusterMapLink::ClusterMapLink(GUID dest, GUID src, string uuid_str) {

    UuidCopy(&SourceClusterMapUUID,&src);
    UuidCopy(&DestinationClusterMapUUID,&dest);
    UuidFromString((RPC_CSTR)uuid_str.c_str(),&UUID_inst);

}

void ClusterMapLink::Cross(void* link)
{

}

void ClusterMapLink::Mutate()
{

}

int ClusterMapLink::Save(string directory)
{
    char uuid_s[36];
    char uuid_src[36], uuid_dest[36];

    UuidToString(&SourceClusterMapUUID,(RPC_CSTR*)uuid_src);
    UuidToString(&DestinationClusterMapUUID,(RPC_CSTR*)uuid_dest);

    UuidToString(&UUID_inst,(RPC_CSTR*)uuid_s);

    string uuid_ss = string(uuid_s);
    string _ClusterMapLinkDir = directory + "/" + uuid_ss;

	LPSECURITY_ATTRIBUTES attr = NULL;

    //create a directory with the ClusterMap UUID number
	CreateDirectory(_ClusterMapLinkDir.c_str(),attr);
    //mkdir(_ClusterMapLinkDir.c_str(),S_IFDIR | S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if(errno == EEXIST)
        return 2; //ClusterMapLink already saved....

    SaveLoadCNN::SaveAxonData(join_axons,_ClusterMapLinkDir+"/axons.xml");

    SaveLinkData(uuid_src,uuid_dest,_ClusterMapLinkDir+"/header.lnk");


    return 0;
    
    
}

void ClusterMapLink::SaveLinkData(string _sUUID,string _dUUID,string filename)
{
    char buf[50];//used for conversion operations

    CMarkup LinkXml;

    LinkXml.AddElem( "LinkData" );
    sprintf(buf,"%s",_sUUID.c_str());
    LinkXml.SetAttrib("SrcUUID",buf);
    sprintf(buf,"%s",_dUUID.c_str());
    LinkXml.SetAttrib("DstUUID",buf);

    LinkXml.Save(filename);

}

void ClusterMapLink::Clear()
{
    join_axons.Empty(); //clear linked list and delete all axons
}


ClusterMapLink::~ClusterMapLink() {

}

