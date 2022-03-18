// Local Headers

// System Headers
#include "Grafika.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iostream>

int width = 101, height = 101;
int odsijecanje_x_min = 25, odsijecanje_x_max = 75, odsijecanje_y_min = 25, odsijecanje_y_max = 75;
bool odsijecanje = false;
std::vector<std::pair<int, int>> klikovi;
std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> linije;

glm::vec3 boja_okvira_odsijecanja = glm::vec3(0.2, 0.6, 0.2);
glm::vec3 boja_tocke = glm::vec3(0.6, 0.2, 0.2);
glm::vec3 boja_linije = glm::vec3(0.2, 0.2, 0.6);

void bresenham_v1(Grafika &grafika, int x0, int y0, int x1, int y1);
void bresenham_v2(Grafika &grafika, int x0, int y0, int x1, int y1);

void klikMisa(int x, int y, int vrsta) {

    if (vrsta == 0) {
        std::cout << "Registriran lijevi klik miša!";
        std::cout << " Koordinate su " << x << " " << y<< std::endl;
        klikovi.push_back({x, y});
        if (klikovi.size() == 2) {
            linije.push_back({klikovi[0], klikovi[1]});
            klikovi.clear();
        }
    }

    else if (vrsta == 1) {
        odsijecanje = !odsijecanje;
        std::cout << "Registriran desni klik miša! Odsijecanje je sada " << odsijecanje << std::endl;
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
            if (!odsijecanje || (x > odsijecanje_x_min && x < odsijecanje_x_max && height - y - 1 > odsijecanje_y_min && height - y - 1 < odsijecanje_y_max))
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
            if (!odsijecanje || (y > odsijecanje_x_min && y < odsijecanje_x_max && height - x - 1 > odsijecanje_y_min && height - x - 1 < odsijecanje_y_max))
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
            if (!odsijecanje || (x > odsijecanje_x_min && x < odsijecanje_x_max && height - y - 1 > odsijecanje_y_min && height - y - 1 < odsijecanje_y_max))
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
            if (!odsijecanje || (y > odsijecanje_x_min && y < odsijecanje_x_max && height - x - 1 > odsijecanje_y_min && height - x - 1 < odsijecanje_y_max))
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
	Grafika grafika(width, height, glm::vec3(0, 0, 0), argv[0]);

	//prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	grafika.registrirajFunkcijuZaKlikMisa(klikMisa);

	while (grafika.trebaZatvoriti()) {

		//osvjezavanje pozicija razlicitih objekata i fizikalna simulacija bi se izvrsavala u ovom dijelu

		grafika.pobrisiProzor();

		//iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
			for (int j = 0; j < width; j += 1) {
				if ((i + j) % 2 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
			}

        // iscrtavanje linija
        for (auto pair : linije)
            iscrtajLiniju(grafika, pair.first.first, pair.first.second, pair.second.first, pair.second.second);

		//iscrtavanje pritisnutih fragmenata
		//ishodiste koordinatnog sustava za operacijski sustav je u gornjem lijevom kutu, a za OpenGL je u donjem lijevom, pa je potrebno okrenuti predznak
		for (int i = 0; i < klikovi.size(); i++)
            if (!odsijecanje || (klikovi[i].first > odsijecanje_x_min && klikovi[i].first < odsijecanje_x_max && height - klikovi[i].second - 1 > odsijecanje_y_min && height - klikovi[i].second - 1 < odsijecanje_y_max))
			    grafika.osvijetliFragment(klikovi[i].first, height - klikovi[i].second - 1, boja_tocke);

        // iscrtavanje okvira za odsijecanje
        if (odsijecanje) {
            int x, y;
            for (x = odsijecanje_x_min, y = odsijecanje_y_min; x <= odsijecanje_x_max; x++) {
                grafika.osvijetliFragment(x, height - y - 1, boja_okvira_odsijecanja);
            }
            for (x = odsijecanje_x_min, y = odsijecanje_y_max; x <= odsijecanje_x_max; x++) {
                grafika.osvijetliFragment(x, height - y - 1, boja_okvira_odsijecanja);
            }
            for (x = odsijecanje_x_min, y = odsijecanje_x_min + 1; y <= odsijecanje_y_max - 1; y++) {
                grafika.osvijetliFragment(x, height - y - 1, boja_okvira_odsijecanja);
            }
            for (x = odsijecanje_x_max, y = odsijecanje_x_min + 1; y <= odsijecanje_y_max - 1; y++) {
                grafika.osvijetliFragment(x, height - y - 1, boja_okvira_odsijecanja);
            }
        }

		grafika.iscrtajRaster();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}   

    return EXIT_SUCCESS;
}
