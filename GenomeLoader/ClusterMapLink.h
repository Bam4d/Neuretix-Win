/* 
 * File:   ClusterMapLink.h
 * Author: bam4d
 *
 * Created on 29 April 2011, 16:37
 */

#ifndef CLUSTERMAPLINK_H
#define	CLUSTERMAPLINK_H

class ClusterMapLink : public Genetic {
public:
    ClusterMapLink();
    ClusterMapLink(GUID dest, GUID src);
    ClusterMapLink(GUID dest, GUID src,string);
    virtual ~ClusterMapLink();

    int Save(string);
    
    void Mutate();
    void Cross(void*);

    void Clear();

    GUID SourceClusterMapUUID;
    GUID DestinationClusterMapUUID;

    axonList join_axons;
private:

    void SaveLinkData(string,string,string);

};

#endif	/* CLUSTERMAPLINK_H */

