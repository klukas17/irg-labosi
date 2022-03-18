// Local Headers

// System Headers
#include "Grafika.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>

/*
 * izvor za računanje orijentacije poligona: https://en.wikipedia.org/wiki/Curve_orientation
 * izvor za računanje udaljenost točke od pravca: https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
 */

int width = 101, height = 101;
bool poligon_nacrtan = false;
int orijentacija = 0; // -1 za smjer kazaljke sata, 1 za suprotni smjer kazaljke sata
std::vector<std::pair<int, int>> klikovi;
std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> linije;
std::vector<glm::vec3> lijevi_bridovi, desni_bridovi;
int min_x = 101, max_x = -1, min_y = 101, max_y = -1;

std::vector<std::pair<int, int>> tocke_unutar_poligona;
std::vector<std::pair<int, int>> tocke_izvan_poligona;

glm::vec3 boja_tocke = glm::vec3(0.6, 0.2, 0.2);
glm::vec3 boja_linije = glm::vec3(0.2, 0.2, 0.6);
glm::vec3 boja_tocke_unutar_poligona = glm::vec3(0.2, 0.6, 0.2);
glm::vec3 boja_tocke_izvan_poligona = glm::vec3(0.6, 0.2, 0.2);

int umnozakTockeIBrida(std::pair<std::pair<int, int>, std::pair<int, int>> brid, glm::vec3 T);
double udaljenostTockeIBrida(std::pair<std::pair<int, int>, std::pair<int, int>> brid, glm::vec3 T);
void odrediLijeveIDesneBridove();
void bresenham_v1(Grafika &grafika, int x0, int y0, int x1, int y1);
void bresenham_v2(Grafika &grafika, int x0, int y0, int x1, int y1);

void klikMisa(int x, int y, int vrsta) {

    if (vrsta == 0 && !poligon_nacrtan) {
        std::cout << "Registriran lijevi klik miša!";
        std::cout << " Koordinate su " << x << " " << y<< std::endl;
        std::pair<int,int> nova_tocka = std::make_pair(x, y);

        // ako je orijentacija nepoznata, sigurno dodajemo označenu točku
        if (orijentacija == 0) {
            if (klikovi.size() > 0)
                linije.push_back({klikovi.back(), nova_tocka});
            klikovi.push_back(nova_tocka);
        }

        // orijentacija je poznata, potrebno provjeriti narušava li dodavanje označene točke konveksnost poligona
        else {
            int zastavica = 1;

            for (auto brid : linije) {
                if (umnozakTockeIBrida(brid, glm::vec3(x, y, 1)) * orijentacija > 0) {
                    zastavica = 0;
                    break;
                }
            }

            if (zastavica) {
                std::pair<std::pair<int, int>, std::pair<int, int>> zadnja_test_linija = {klikovi.back(), nova_tocka};
                if (umnozakTockeIBrida(zadnja_test_linija, glm::vec3(klikovi.front().first, klikovi.front().second, 1)) * orijentacija > 0)
                    zastavica = 0;
            }

            if (zastavica) {
                linije.push_back(std::pair<std::pair<int, int>, std::pair<int, int>>{klikovi.back(), nova_tocka});
                klikovi.push_back(nova_tocka);
            } else {
                std::cout << "Upozorenje: Dodavanje označene točke bi učinilo poligon konkavnim!" << std::endl;
            }
        }

        // određivanje orijentacije poligona
        if (orijentacija == 0 && klikovi.size() >= 3) {
            auto tocka_1 = klikovi.rbegin()[0], tocka_2 = klikovi.rbegin()[1], tocka_3 = klikovi.rbegin()[2];
            glm::mat3 matrica = glm::mat3(1, tocka_1.first, tocka_1.second, 1, tocka_2.first, tocka_2.second, 1, tocka_3.first, tocka_3.second);
            int determinanta = glm::determinant(matrica);
            if (determinanta > 0) {
                orijentacija = 1;
                std::cout << "Orijentacija je suprotnog smjera od kazaljke sata!" << std::endl;
            }
            else if (determinanta < 0) {
                orijentacija = -1;
                std::cout << "Orijentacija je smjera kazaljke sata!" << std::endl;
            }
        }
    }

    else if (vrsta == 1) {
        std::cout << "Registriran desni klik miša!" << std::endl;
        if (!poligon_nacrtan) {
            if (klikovi.size() < 3) std::cout << "Nije označeno dovoljno točaka!" << std::endl;
            else {
                poligon_nacrtan = true;
                linije.push_back({klikovi.back(), klikovi.front()});
                odrediLijeveIDesneBridove();
                std::cout << "Povezujem početni i krajnji vrh!" << std::endl;
            }
        }
        else {
            glm::vec3 T = glm::vec3(x, y, 1);

            if (orijentacija == 1) {
                bool zastavica = true;
                for (auto brid : linije) {
                    if (umnozakTockeIBrida(brid, T) > 0) {
                        if (udaljenostTockeIBrida(brid, T) > 0.5) {
                            zastavica = false;
                            break;
                        }
                    }
                }
                if (zastavica && y <= max_y && y >= min_y & x <= max_x && x >= min_x)
                    tocke_unutar_poligona.push_back(std::make_pair(x, y));
                else
                    tocke_izvan_poligona.push_back(std::make_pair(x, y));
            }

            else if (orijentacija == -1) {
                bool zastavica = true;
                for (auto brid : linije) {
                    if (umnozakTockeIBrida(brid, T) < 0) {
                        if (udaljenostTockeIBrida(brid, T) > 0.5) {
                            zastavica = false;
                            break;
                        }
                    }
                }
                if (zastavica && y <= max_y && y >= min_y & x <= max_x && x >= min_x)
                    tocke_unutar_poligona.push_back(std::make_pair(x, y));
                else
                    tocke_izvan_poligona.push_back(std::make_pair(x, y));
            }
        }
    }
}

int umnozakTockeIBrida(std::pair<std::pair<int, int>, std::pair<int, int>> brid, glm::vec3 T) {
    auto t0 = brid.first;
    auto t1 = brid.second;
    glm::vec3 G_transponirano = glm::vec3(t0.second - t1.second, -(t0.first - t1.first), -(t0.second - t1.second) * t0.first + (t0.first - t1.first) * t0.second);
    glm::vec3 s = T * G_transponirano;
    int umnozak = s[0] + s[1] + s[2];
    return umnozak;
}

double udaljenostTockeIBrida(std::pair<std::pair<int, int>, std::pair<int, int>> brid, glm::vec3 T) {
    auto t0 = brid.first;
    auto t1 = brid.second;
    glm::vec3 G_transponirano = glm::vec3(t0.second - t1.second, -(t0.first - t1.first), -(t0.second - t1.second) * t0.first + (t0.first - t1.first) * t0.second);
    return abs(umnozakTockeIBrida(brid, T)) / (double) (std::sqrt(G_transponirano[0]*G_transponirano[0] + G_transponirano[1]*G_transponirano[1]));
}

void odrediLijeveIDesneBridove() {
    for (auto brid : linije) {
        auto t0 = brid.first;
        auto t1 = brid.second;

        // traženje najveće i najmanje x i y koordinate poligona
        if (t0.first < min_x) min_x = t0.first;
        if (t0.first > max_x) max_x = t0.first;
        if (t0.second < min_y) min_y = t0.second;
        if (t0.second > max_y) max_y = t0.second;
        if (t1.first < min_x) min_x = t1.first;
        if (t1.first > max_x) max_x = t1.first;
        if (t1.second < min_y) min_y = t1.second;
        if (t1.second > max_y) max_y = t1.second;

        // izraz (2.12) iz udžbenika : (y1 - y2)x - (x1 - x2)y - (y1 - y2)x1 + (x1 - x2)y1 = 0
        if (t0.second > t1.second)
            lijevi_bridovi.push_back(glm::vec3(t0.second - t1.second, -(t0.first - t1.first), -(t0.second - t1.second) * t0.first + (t0.first - t1.first) * t0.second));
        else if (t0.second < t1.second)
            desni_bridovi.push_back(glm::vec3(t0.second - t1.second, -(t0.first - t1.first), -(t0.second - t1.second) * t0.first + (t0.first - t1.first) * t0.second));
    }
}

// rađeno po algoritmu iz 2. minilekcije
void obojiPoligon(Grafika &grafika) {
    for (int y = min_y; y <= max_y; y++) {
        int L = min_x, D = max_x;

        // varijabli S se pribraja 0.5 jer tako bridovi budu ispravno nacrtani, empirijski utvrđeno
        for (auto lijevi_brid : lijevi_bridovi) {
            int S = -(lijevi_brid[1] * y + lijevi_brid[2]) / lijevi_brid[0] + 0.5;
            if (orijentacija == -1) if (S > L) L = S;
            if (orijentacija == 1) if (S < D) D = S;
        }
        for (auto desni_brid : desni_bridovi) {
            int S = -(desni_brid[1] * y + desni_brid[2]) / desni_brid[0] + 0.5;
            if (orijentacija == -1) if (S < D) D = S;
            if (orijentacija == 1) if (S > L) L = S;
        }
        for (int x = L; x <= D; x++)
            grafika.osvijetliFragment(x, height - y - 1, boja_linije);
    }
}

void iscrtajLiniju(Grafika &grafika, int x0, int y0, int x1, int y1) {
    if (x0 <= x1) {
        if (y0 <= y1) {
            bresenham_v1(grafika, x0, y0, x1, y1);
        } else {
            bresenham_v2(grafika, x0, y0, x1, y1);
        }
    } else {
        if (y0 >= y1) {
            bresenham_v1(grafika, x1, y1, x0, y0);
        } else {
            bresenham_v2(grafika, x1, y1, x0, y0);
        }
    }
}

void bresenham_v1(Grafika &grafika, int x0, int y0, int x1, int y1) {
    int x, y, f, a, korekcija;
    if (y1 - y0 <= x1 - x0) {
        a = 2 * (y1 - y0);
        y = y0;
        f = - (x1 - x0);
        korekcija = -2 * (x1 - x0);
        for (x = x0; x <= x1; x++) {
            grafika.osvijetliFragment(x, height - y - 1, boja_linije);
            f = f + a;
            if (f >= 0) {
                f = f + korekcija;
                y = y + 1;
            }
        }
    } else {
        x = x1;
        x1 = y1;
        y1 = x;
        x = x0;
        x0 = y0;
        y0 = x;
        a = 2 * (y1 - y0);
        y = y0;
        f = - (x1 - x0);
        korekcija = -2 * (x1 - x0);
        for (x = x0; x <= x1; x++) {
            grafika.osvijetliFragment(y, height - x - 1, boja_linije);
            f = f + a;
            if (f >= 0) {
                f = f + korekcija;
                y = y + 1;
            }
        }
    }
}

void bresenham_v2(Grafika &grafika, int x0, int y0, int x1, int y1) {
    int x, y, f, a, korekcija;
    if (-(y1 - y0) <= x1 - x0) {
        a = 2 * (y1 - y0);
        y = y0;
        f = x1 - x0;
        korekcija = 2 * (x1 - x0);
        for (x = x0; x <= x1; x++) {
            grafika.osvijetliFragment(x, height - y - 1, boja_linije);
            f = f + a;
            if (f <= 0) {
                f = f + korekcija;
                y = y - 1;
            }
        }
    } else {
        x = x1;
        x1 = y0;
        y0 = x;
        x = x0;
        x0 = y1;
        y1 = x;
        a = 2 * (y1 - y0);
        y = y0;
        f = x1 - x0;
        korekcija = 2 * (x1 - x0);
        for (x = x0; x <= x1; x++) {
            grafika.osvijetliFragment(y, height - x - 1, boja_linije);
            f = f + a;
            if (f <= 0) {
                f = f + korekcija;
                y = y - 1;
            }
        }
    }
}

int main(int argc, char * argv[]) {

	int width = 101, height = 101;
	Grafika grafika(width, height, glm::vec3(0, 0, 0), argv[0]);

	//prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	grafika.registrirajFunkcijuZaKlikMisa(klikMisa);

	while (grafika.trebaZatvoriti()) {

		grafika.pobrisiProzor();

		//iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
			for (int j = 0; j < width; j += 1) {
				if ((i + j) % 2 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
			}

        // iscrtavanje bridova poligona
        for (auto pair : linije)
            iscrtajLiniju(grafika, pair.first.first, pair.first.second, pair.second.first, pair.second.second);

        // bojanje poligona
        if (poligon_nacrtan)
            obojiPoligon(grafika);

		//iscrtavanje posljednje točke
		//ishodiste koordinatnog sustava za operacijski sustav je u gornjem lijevom kutu, a za OpenGL je u donjem lijevom, pa je potrebno okrenuti predznak
        if (klikovi.size() > 0 && !poligon_nacrtan)
		    grafika.osvijetliFragment(klikovi.back().first, height - klikovi.back().second -1, boja_tocke);

        // iscrtavanje točaka unutar poligona
        for (auto tocka : tocke_unutar_poligona) {
            grafika.osvijetliFragment(tocka.first, height - tocka.second - 1, boja_tocke_unutar_poligona);
        }

        // iscrtavanje točaka izvan poligona
        for (auto tocka : tocke_izvan_poligona) {
            grafika.osvijetliFragment(tocka.first, height - tocka.second - 1, boja_tocke_izvan_poligona);
        }

		grafika.iscrtajRaster();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}   

    return EXIT_SUCCESS;
}
