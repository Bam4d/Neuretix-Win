/* 
 * File:   CavalcadeMind.h
 * Author: bam4d
 *
 * Created on 28 November 2010, 14:57
 */

#ifndef CAVALCADEMIND_H
#define	CAVALCADEMIND_H

#include <windows.h>

class CavalcadeMind {
public:
    axonList Axons; //store the axons as a pointer array
    vector<group*> Cluster; //vector containing neuron clusters
    vector<ControlTimer> SimulationController;

    CavalcadeMind();
    //int Save(string);
    //int Load(string);


    void iterate();
    void iterate_TW();
    HANDLE TWmutex; //mutex for stopping TW iterations from corrupting with async input
    HANDLE SyncMutex; //mutex for stopping Sync iteration from corrupting with async input

    int StartMindThread_TW(int,bool);
    int StopMindThread_TW();

    int StartMindThread(int,bool);
    int StopMindThread();

    void Clear();
    void MoveClusters(PlotPoint);


    virtual ~CavalcadeMind();

    bool RunNetwork;
    bool RenderGraphics;
    int net_timer;
    int Timer;

    
    TemporalWindow *_t_Window;

    long unsigned int CurTime;

private:
	DWORD mindProcessorID;
    HANDLE mindProcessor;
    char databuf[255]; //buffer to store data in before written to file
    axon * axon2Update;

};

#endif	/* CAVALCADEMIND_H */

