class CavalcadeMind;
typedef axon* axonToFire;

class neuron
{
public:
    neuron();
    neuron(int,_RGB);
    neuron(int,PlotPoint,double,double,double,_RGB);

    void Fire();
    void Fire_TW();

    void Stimulate(double);
    void Stimulate_TW(double);
    
    int impulses_recieved;
    int impulses_fired;
    
    void UpdatePotentials();
    void IntroduceDemand(double);
    void PropagatedDemand(double magnitude);
    
    double calcSLevel_TW();

    ~neuron(void);

    //neuroplasticity variables (Supply)
   


    //neuron firing potential variables
    double amplitude_peak; //max amplitude at last stimulation
    double fire_threshold; //threshold for firing the neuron
    double exc_TC; //excitability epsilon
    int refractory_period; //resting period time
    int refractory_countdown; //counts down the refractory time
    double excitation_level; //current excitation level of neuron

    int ID;
    //graphical output variables
    PlotPoint position;
    _RGB neuCol;
    int ParentGroupID; //the group this neuron belongs to

    //list of axon fire functions this neuron is connected to
    vector<axonToFire> outgoingAxons;

    //used in TW network update methods
    CavalcadeMind* ParentMind;
    long unsigned int LastFireTimestamp; //time since this neuron last fired
    long unsigned int LastStimTimestamp; //time since this neuron was last stimulated

    
    double S_peak; //max amplitude at last supply spike
    static neuron* FindNeuronByID(int ID, vector<neuron*>* neuronList);

private:
    double excitation_time; //time since last excitation
    double S_excitation_time; //excitation time for Supply level
    double S_TC; //Time constant for decay of Supply potential
    double S_level; //current level of supply
    
};

