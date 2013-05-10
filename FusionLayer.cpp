#include "FusionLayer.h"

FusionLayer* FusionLayer::p_instance = nullptr;

FusionLayer* FusionLayer::instance(){
	if(p_instance == nullptr){
		p_instance = new FusionLayer;
	}
	return p_instance;
}

void FusionLayer::add_belief(const std::unordered_map<int, float>& count_belief_map){
    m_belief_map_vector.push_back(count_belief_map);
    for(auto &p : count_belief_map){
        m_count_set.insert(p.first);
    }
}

std::unordered_map<int, float> FusionLayer::get_fusion_belief(){
    std::unordered_map<int, std::vector<float> > belief_map;
    for(auto map : m_belief_map_vector){
        smooth(map);
        for(auto &p : map){
            belief_map[p.first].push_back(p.second);
        }
    }
    std::unordered_map<int, float> fusion_belief_map;
    for(auto &p : belief_map){
        fusion_belief_map[p.first] = 1.f;
    }
    for(auto &p : belief_map){
        for(auto &v : p.second){
            fusion_belief_map[p.first] *= v;
        }
    }
    Utility::l1_normalize(fusion_belief_map);
    return fusion_belief_map;
}

void FusionLayer::smooth(std::unordered_map<int, float>& map){
    float d = static_cast<float>(m_count_set.size());
    for(auto &p : map){
        p.second = (p.second + 1.f) / (1.f + d);
    }
}
