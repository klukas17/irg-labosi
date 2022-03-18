//
// Created by mihael on 3/8/22.
//

#pragma once

#include <set>
#include <vector>
#include <cstdlib>
#include <map>

class State {
public:
    int red_color;
    int green_color;
    int blue_color;
    int trenutna_boja; // 0 za r, 1 za g, 2 za b

    int width;
    int height;
    int curr_x;
    int curr_y;
    float indeksirani_x;
    float indeksirani_y;

    std::vector<std::pair<int,int>> vrhovi;
    std::map<int, std::map<int, std::tuple<int,int,int>>> mapa_boja;

    std::vector<float> indeksirani_vrhovi;
    std::vector<float> indeksirane_boje;
    std::vector<unsigned int> indeksi;

    State();
    ~State();

    void dodaj_vrh();
    void promijeni_trenutnu_boju(int nova_boja);
    void povecaj_trenutnu_boju(int diff);
    void smanji_trenutnu_boju(int diff);
};


