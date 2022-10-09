#include <iostream>
#include <fstream>
#define INFINIT 999999
#pragma warning(disable : 4996)  

using namespace std;

float AdiacenteV[1001][1001], AdiacenteI[1001][1001]; // memoria de date
float Adiacente[1001][1001];     // matricea adiacenta pentru functia Dijkstra
float cost[1001][1001], timp[1001]; 
int noduri_vizitate[1001];
int noduri_precedente[1001]; // Dj
int noduri_precedente_V[1001], noduri_precedente_I[1001];
float timpi_V_I[1001][2];  // timpii in fiecare nod ai lui Vasile si ion

void Dijkstra(float Adiacente[1001][1001], int n, int punct_pornire, int noduri_precedente[1001]);

int main() {
	int n, m;   // numar noduri si laturi  ----  variabile de mici dimensiuni care pot fi alocate in memoria fixa(stack)
	int I, V;      //punctele de intrare ale lui Ion si Victor
	int vI, vV, vT;  // vitezele lui Ion, Vasile si Teleferic
	int T, A, B, D;  // tip legatura (teleferic sau partie), nod i (A), nod j (B), distanta intre nodurile i si j

	ifstream fisier_intrare("ski.in");
	if (fisier_intrare.is_open()) {
		fisier_intrare >> n >> m >> I >> V;
		fisier_intrare >> vI >> vV >> vT;
		if (n <= 1 || n > 1000) {
			cout << "n trebuie sa fie mai mare decat 1 si mai mic sau egal cu 1000";
			return 0;
		}
		if (I <= 1 || I > n || V <= 1 || V > n) {
			cout << "I si V trebuie sa fie mai mari decat 1 si mai mici sau egali cu n (" << n << ")";
			return 0;
		}
		if (vI < 1 || vI > 1000 || vV < 1 || vV > 1000 || vT < 1 || vT > 1000) {
			cout << "vI, vV si vT trebuie sa fie mai mari decat 1 si mai mici sau egali cu 1000";
			return 0;
		}
		for (int i = 0; i < m; i++) {      
			fisier_intrare >> T >> A >> B >> D;
			if (D < 1 || D > 1000) {      
				cout << "D trebuie sa fie mai mare decat 1 si mai mic sau egal cu 1000";
				return 0;
			}
			if (T == 1) {                            
				AdiacenteV[A][B] = (float)D / vT;
				AdiacenteI[A][B] = (float)D / vT;
			}
			else {                                  
				AdiacenteV[A][B] = (float)D / vV;
				AdiacenteI[A][B] = (float)D / vI;
			}
		}
		fisier_intrare.close();
	}
	else {
		cout << "Fisierul ski.in nu poate fi deschis!";
		return 0;
	}

	Dijkstra(AdiacenteV, n, V, noduri_precedente_V);
	for (int i = 1; i <= n; i++) {
		timpi_V_I[i][0] = timp[i]; 
	}

	Dijkstra(AdiacenteI, n, I, noduri_precedente_I);
	for (int i = 1; i <= n; i++) {
		timpi_V_I[i][1] = timp[i];    
	}

	float timp_min = INFINIT;  
	int punct_intalnire = 0;
	for (int i = 1; i <= n; i++) {
		if (timpi_V_I[i][0] > timpi_V_I[i][1]) {
			if (timpi_V_I[i][0] < timp_min) {
				timp_min = timpi_V_I[i][0];    // se retine timpul minim (ca maxim dintre timpii cand ajung cei doi in acelasi punct)
				punct_intalnire = i;           // se retine punctul de intalnire pentru timpul cel mai mic
			}
		}
		else {
			if (timpi_V_I[i][1] < timp_min) {
				timp_min = timpi_V_I[i][1];
				punct_intalnire = i;
			}
		}
	}

	ofstream fisier_iesire("ski.out");
	if (fisier_iesire.is_open()) {
		fisier_iesire << punct_intalnire << endl << (int)timp_min << ":" << (int)((timp_min - (int)timp_min) * 60) << ":0";
		fisier_iesire.close();
	}
	else {
		cout << "Fisierul ski.out nu poate fi creat!";
		return 0;
	}

	// drumurile parcurse de Vasile si Ion
	int j = punct_intalnire;
	cout << "Drum parcurs de Vasile " << punct_intalnire;
	do {
		j = noduri_precedente_V[j];
		printf("<-%d", j);
	} while (j != V);
	cout << " timp (" << (int)timpi_V_I[punct_intalnire][0] << ":" << (int)((timpi_V_I[punct_intalnire][0] - (int)timpi_V_I[punct_intalnire][0]) * 60) << ":0)" << endl;
	cout << "Drum parcurs de Ion " << punct_intalnire;
	j = punct_intalnire;
	do {
		j = noduri_precedente_I[j];
		printf("<-%d", j);
	} while (j != I);
	cout << " timp (" << (int)timpi_V_I[punct_intalnire][1] << ":" << (int)((timpi_V_I[punct_intalnire][1] - (int)timpi_V_I[punct_intalnire][1]) * 60) << ":0)" << endl;

	return 0;
}


void Dijkstra(float Adiacente[1001][1001], int n, int punct_pornire, int noduri_precedente[1001]) {
	float timp_minim;
	int nodul_urmator = 0;

	for (int i = 1; i <= n; i++) {         // in matricea de costuri se pun pe infinit muchiile nodurilor care nu sunt unite
		for (int j = 1; j <= n; j++) {
			if (Adiacente[i][j] == 0) {
				cost[i][j] = INFINIT;
			}
			else {
				cost[i][j] = Adiacente[i][j];
			}
		}
	}
	for (int i = 1; i <= n; i++) {    	
		timp[i] = cost[punct_pornire][i];
		noduri_precedente[i] = punct_pornire;
		noduri_vizitate[i] = 0;
	}
	timp[punct_pornire] = 0;
	noduri_vizitate[punct_pornire] = 1;
	for (int k = 1; k <= n; k++) {
		timp_minim = INFINIT;
		for (int i = 1; i <= n; i++) {
			if (timp[i] < timp_minim && !noduri_vizitate[i]) {	//nodul_urmator cu timp minim si nevizitat
				timp_minim = timp[i];
				nodul_urmator = i;
			}
		}
		noduri_vizitate[nodul_urmator] = 1;
		for (int i = 1; i <= n; i++) {   		//verificam daca exista o cale cu timp mai mic catre nodul_urmator
			if (!noduri_vizitate[i]) {
				if (timp_minim + cost[nodul_urmator][i] < timp[i]) {    //daca exista un timp mai mic
					timp[i] = timp_minim + cost[nodul_urmator][i];      // se relaxeaza costul (timpul)
					noduri_precedente[i] = nodul_urmator;               // si se retine nodul urmator
				}
			}
		}
	}
}