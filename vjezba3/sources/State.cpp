//
// Created by mihael on 3/8/22.
//

#include "State.h"
#include <iostream>

State::State() {
    std::cout << "Uspješno stvoren State objekt!" << std::endl;
    red_color = 0;
    green_color = 0;
    blue_color = 0;
    trenutna_boja = 0; // inicijaliziramo crvenu kao boju koju na početku mijenjamo
    width = 1000;
    height = 1000;
    curr_x = 0;
    curr_y = 0;
    indeksirani_x = -1;
    indeksirani_y = -1;
}

State::~State() {
    std::cout << "Uspješno uništen State objekt!" << std::endl;
}

void State::promijeni_trenutnu_boju(int nova_boja) {
    trenutna_boja = nova_boja;
    switch(nova_boja) {
        case 0:
            std::cout << "Trenutna boja je crvena (r)" << std::endl;
            break;
        case 1:
            std::cout << "Trenutna boja je zelena (g)" << std::endl;
            break;
        case 2:
            std::cout << "Trenutna boja je plava (b)" << std::endl;
            break;
    }
}

void State::povecaj_trenutnu_boju(int diff) {
    switch(trenutna_boja) {
        case 0:
            if (red_color < 255) {
                red_color += diff;
                red_color = std::min(red_color, 255);
            }
            else
                std::cout << "Ne mogu povećati crvenu boju!" << std::endl;
            break;
        case 1:
            if (green_color < 255) {
                green_color += diff;
                green_color = std::min(green_color, 255);
            }
            else
                std::cout << "Ne mogu povećati zelenu boju!" << std::endl;
            break;
        case 2:
            if (blue_color < 255) {
                blue_color += diff;
                blue_color = std::min(blue_color, 255);
            }
            else
                std::cout << "Ne mogu povećati plavu boju!" << std::endl;
            break;
    }
    std::cout << "Trenutna boja je (" << red_color << ", " << green_color << ", " << blue_color << ")" << std::endl;
}

void State::smanji_trenutnu_boju(int diff) {
    switch(trenutna_boja) {
        case 0:
            if (red_color > 0) {
                red_color -= diff;
                red_color = std::max(red_color, 0);
            }
            else
                std::cout << "Ne mogu smanjiti crvenu boju!" << std::endl;
            break;
        case 1:
            if (green_color > 0) {
                green_color -= diff;
                green_color = std::max(green_color, 0);
            }
            else
                std::cout << "Ne mogu smanjiti zelenu boju!" << std::endl;
            break;
        case 2:
            if (blue_color > 0) {
                blue_color -= diff;
                blue_color = std::max(blue_color, 0);
            }
            else
                std::cout << "Ne mogu smanjiti plavu boju!" << std::endl;
            break;
    }
    std::cout << "Trenutna boja je (" << red_color << ", " << green_color << ", " << blue_color << ")" << std::endl;
}

void State::dodaj_vrh() {
    if (mapa_boja.count(curr_x) && mapa_boja[curr_x].count(curr_y)) {
        std::cout << "Nije moguće dodati vrh jer već postoji!" << std::endl;
        return;
    }

    vrhovi.push_back(std::make_pair(curr_x, curr_y));
    mapa_boja[curr_x][curr_y] = {red_color, green_color, blue_color};

    // ažuriranje indeksa
    indeksirani_vrhovi.push_back((curr_x - width/(float)2) / (width/(float)2));
    indeksirani_vrhovi.push_back((height/(float)2 - curr_y) / (height/(float)2));
    indeksirani_vrhovi.push_back(0);
    indeksirane_boje.push_back(red_color / (float) 255);
    indeksirane_boje.push_back(green_color / (float) 255);
    indeksirane_boje.push_back(blue_color / (float) 255);
    if (vrhovi.size() >= 3) {
        indeksi.push_back(vrhovi.size() - 3);
        indeksi.push_back(vrhovi.size() - 2);
        indeksi.push_back(vrhovi.size() - 1);
    }
}