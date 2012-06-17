/* 
 * File:   GenomeLoaderGlobals.h
 * Author: bam4d
 *
 * Created on 23 May 2012, 13:27
 */

#ifndef GENOMELOADERGLOBALS_H

#include "../CavalCade/CNNGlobals.h"

__inline void UuidCopy(GUID * dst,GUID * src)
{
	dst->Data1 = src->Data1;
	dst->Data2 = src->Data2;
	dst->Data3 = src->Data3;

	for(int i = 0; i<8; i++)
		dst->Data4[i] = src->Data4[i];
}

#include "Genetic.h"
#include "ClusterMap.h"
#include "ClusterMapLink.h"
#include "Genome.h"
#include "GAEngine.h"


#define	GENOMELOADERGLOBALS_H



#endif	/* GENOMELOADERGLOBALS_H */

