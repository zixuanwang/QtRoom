#ifndef FUSIONLAYER_H
#define FUSIONLAYER_H

#include "Utility.h"

class FusionLayer
{
public:
    FusionLayer();
    void add_belief(const std::unordered_map<int, float>& count_belief_map);
    std::unordered_map<int, float> get_fusion_belief();
    void smooth(std::unordered_map<int, float>& map); // laplace smoothing
private:
    std::vector<std::unordered_map<int, float> > m_belief_map_vector;
    std::unordered_set<int> m_count_set;
};

#endif // FUSIONLAYER_H
