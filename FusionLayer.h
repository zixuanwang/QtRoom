#ifndef FUSIONLAYER_H
#define FUSIONLAYER_H

#include "Utility.h"

class FusionLayer
{
public:
	static FusionLayer* instance();
    void add_belief(const std::unordered_map<int, float>& count_belief_map);
    std::unordered_map<int, float> get_fusion_belief();
    void smooth(std::unordered_map<int, float>& map); // laplace smoothing
private:
	FusionLayer(){}
	FusionLayer(const FusionLayer&){}
	FusionLayer& operator=(const FusionLayer&){}
    std::vector<std::unordered_map<int, float> > m_belief_map_vector;
    std::unordered_set<int> m_count_set;
	static FusionLayer* p_instance;
};

#endif // FUSIONLAYER_H
