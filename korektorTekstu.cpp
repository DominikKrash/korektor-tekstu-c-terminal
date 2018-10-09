#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <stdio.h>

using namespace std;

const int NAZWA_ROZ = 40;
const int BUFFER = 80;
const int ILOSC_LINI = 5;
const int LINIA_DLUGOSC = 50;

void LOG(const char tekst[]) {
	cout << tekst << endl;
}

bool wczytajPlik(fstream &Plik,char Nazwa[]) {
	Plik.open(Nazwa, ios::in);
	if (Plik.good()) {
		LOG("Plik zostal znaleziony!");
		system("CLS");
		return true;
	}
	else {
		LOG("Plik nie zostal znaleziony!");
		system("CLS");
		return false;
	}
}
bool zamknijPlik(fstream &Plik) {
	Plik.clear();
	Plik.close();
	return false;
}

void renderText(fstream &Plik) {
	if (!Plik.is_open()) return;
	char text;
	while (Plik.good()){
		Plik.get(text);
		if (Plik.eof())return;
		cout << text;
	}
}
//poprawia tekst, usuwa powtorzenia 
void korektor(fstream &Plik,char Nazwa[]){
	if (!Plik.is_open()){
		cout << "NiE MOZNA OTWORZYC PLIKU" << endl;
		return;
	}
	bool start = true;
	bool podwojny = false;
	char litera;
	char poprzednia;
	Plik.clear();
	Plik.seekg(0);
	fstream nowy("nowy.txt", ios::out | ios::trunc);
	while (Plik.good()){
		if (start == true){
		Plik.get(litera);
		if (Plik.eof())break;
		nowy << litera;
		start = false;
		}
		else {
			poprzednia = litera;
			Plik.get(litera);
			if (Plik.eof())break;
			if ((ispunct(litera) != 0 || isspace(litera) != 0) && litera == poprzednia){
				continue;
			}
			nowy << litera;
		}
	}
	Plik.close();
	nowy.close();
	remove(Nazwa);
	rename("nowy.txt", Nazwa);
	Plik.open(Nazwa);
}
//porownuje 2 ciagy liczb, wystepuja 2 tryby ktore uwzgledniaja wielkosc liter//true uzwglednia wielkosc a false nie 
bool porownajCiag(bool tryb, char ciag[], char Nciag[] , char &litera, fstream &Plik, fstream &Nowy){
	int i = 0;
	char temp[BUFFER];
	if (tryb == true){
		if (litera == ciag[i]){
			while (litera == ciag[i]){
				temp[i] = litera;
				Plik.get(litera);
				i++;
				if (Plik.eof())break;
			}
			if (i == strlen(ciag)) {
				Nowy << Nciag;
				Plik.putback(litera);
				return true;
			}
		}
	}
	else{
		if (toupper(litera) == toupper(ciag[i])){
			while (toupper(litera) == toupper(ciag[i])){
				temp[i] = litera;
				Plik.get(litera);
				i++;
				if (Plik.eof())break;
			}
			if (i == strlen(ciag)) {
				Nowy << Nciag;
				Plik.putback(litera);
				return true;
			}
		}
	}
	if (i == 0) {
		temp[i++] = litera;
		temp[i] = '\0';
	}
	else
	temp[i] = '\0';
	Nowy << temp;
    return false;
}
bool porownajCiag(char ciag[], char Nciag[], char &litera, fstream &Plik, fstream &Nowy){
	int i = 0;
	char temp[BUFFER];
		if (litera == ciag[i]){
			while (litera == ciag[i]){
				temp[i] = litera;
				Plik.get(litera);
				i++;
				if (Plik.eof())break;
			}
			if (i == strlen(ciag)) {
				int k = 0;
				while (k < strlen(Nciag)){
					if (k < strlen(ciag)){
						if (isupper(ciag[k]) != 0) Nowy << char(toupper(Nciag[k]));
						else if (islower(ciag[k]) != 0) Nowy << char(tolower(Nciag[k]));
					}
					else Nowy << Nciag[k];
					k++;
				}
				Plik.putback(litera);
				return true;
			}
		}
	if (i == 0) {
		temp[i++] = litera;
		temp[i] = '\0';
	}
	else
		temp[i] = '\0';
	Nowy << temp;
	return false;
}

void zmienCiag(fstream &Plik, char Nazwa[], char ciag[], char Nciag[]){
	if (!Plik.is_open()){
		cout << "NiE MOZNA OTWORZYC PLIKU" << endl;
		return;
	}
	fstream Nowy("nowy.txt", ios::out | ios::trunc);
	int znalezione = 0;
	Plik.clear();
	Plik.seekg(0);
	char slowo;
	bool in = false;
	while (Plik.good()){
		Plik.get(slowo);
		if (Plik.eof()) break;
		if (isalpha(slowo) != 0 && in == false){
			in = true;
		}
		else if (in == true && isspace(slowo) != 0){
			in = false;
		}
		if (in == true) {
			if (porownajCiag(ciag, Nciag, slowo, Plik, Nowy) == true) znalezione++;
		}
		else Nowy << slowo;
	}
	Plik.close();
	Nowy.close();
	remove(Nazwa);
	rename("nowy.txt", Nazwa);
	Plik.open(Nazwa);
	if (znalezione > 0) cout << "zmieniono " << znalezione << " slow" << endl;
	else if (znalezione == 0) cout << "nie znaleziono slow" << endl;
}
//zmienia podany ciag w tekscie na inny ciag
void zmienCiag(fstream &Plik, char Nazwa[],bool Tryb,char ciag[],char Nciag[]){
	if (!Plik.is_open()){
		cout << "NiE MOZNA OTWORZYC PLIKU" << endl;
		return;
	}
	fstream Nowy("nowy.txt", ios::out | ios::trunc);
	int znalezione = 0;
	Plik.clear();
	Plik.seekg(0);
	char slowo;
	bool in = false;
	while (Plik.good()){
		Plik.get(slowo);
		if (Plik.eof()) break;
		if (isalpha(slowo) != 0 && in == false){
			in = true;
		}
		else if (in == true && isspace(slowo) != 0){
			in = false;
		}
		if (in == true) {
			if (porownajCiag(Tryb, ciag, Nciag, slowo, Plik, Nowy) == true) znalezione++;
		}
		else Nowy << slowo;
	}
	Plik.close();
	Nowy.close();
	remove(Nazwa);
	rename("nowy.txt", Nazwa);
	Plik.open(Nazwa);
	if (znalezione > 0) cout << "zmieniono " << znalezione << " slow" << endl;
	else if (znalezione == 0) cout << "nie znaleziono slow" << endl;
}

void czytajLinie(char tablice[][BUFFER], char line[]){
	int i = 0;
	int ktory = 0;
	bool in = false;
	while (line[i] != '\0' && line[i] != '\n'){
		if (in == false && isalpha(line[i]) != 0) in = true;
		if (in == true){
			in = false;
			int j = 0;
			while ((line[i] != '\0' && line[i] != '\n') && isspace(line[i])==0) {
					if(ktory == 0)tablice[ktory][j] = char(tolower(line[i]));
					else tablice[ktory][j] = line[i];
					i++;
					j++;
				}
				tablice[ktory][j] = '\0';
				i--;
				ktory++;
			}
			i++;
		}
	if (ktory < 3){
		while (ktory < 3){
			tablice[ktory][0] = '\0';
				ktory++;
		}
	}
}
void sterowanie(fstream &Plik, bool &CzyOtwarty,bool &Time,char Nazwa[]) {
	char Wybor[BUFFER];
	char Sterowanie[3][BUFFER];
	cout << endl;
	cin.getline(Wybor, 80);
	czytajLinie(Sterowanie, Wybor);
	system("CLS");
	
	if (strcmp(Sterowanie[0], "wczytaj") == 0 && CzyOtwarty == false) {
		CzyOtwarty = wczytajPlik(Plik, Sterowanie[1]);
		strcpy(Nazwa, Sterowanie[1]);
	}
	else if (strcmp(Sterowanie[0],"zamknij") == 0 && CzyOtwarty == true) CzyOtwarty = zamknijPlik(Plik);
	else if (strcmp(Sterowanie[0], "korekta") == 0 && CzyOtwarty == true) {
		korektor(Plik, Nazwa);
	}
	else if (strcmp(Sterowanie[0], "zamien:nwazne") == 0 && CzyOtwarty == true) {
		zmienCiag(Plik, Nazwa,false,Sterowanie[1],Sterowanie[2]);
	}
	else if (strcmp(Sterowanie[0], "zamien:wazne") == 0 && CzyOtwarty == true) {
		zmienCiag(Plik, Nazwa, true,Sterowanie[1], Sterowanie[2]);
	}
	else if (strcmp(Sterowanie[0], "zamien:spec") == 0 && CzyOtwarty == true) {
		zmienCiag(Plik, Nazwa, Sterowanie[1], Sterowanie[2]);
	}
	else if (strcmp(Sterowanie[0], "koniec") == 0){
		Time = false;
		return;
	}
	cin.clear();
}
void menu(const bool CzyOtwarty, fstream &Plik) {
	LOG("Dominik Kraszkiewicz");
	LOG("KOREKTOR TEKSTU 1.0V");
	LOG("---------------MENU---------------");
	if (CzyOtwarty == false) LOG("1.wczytaj");
	if (CzyOtwarty == true) {
		LOG("2.zamknij");
		LOG("3.korekta");
		LOG("4.zamien:nwazne - dowolny ciag");
		LOG("5.zamien:wazne -  wazna wielkosc liter");
		LOG("6.zamien:spec - pierwotnie");
	}
	LOG("7.koniec");
	if (CzyOtwarty == true) {
		LOG("ZAWARTOSC PLIKU TXT");
		renderText(Plik);
	}

}
int main() {
	fstream Plik;
	char Nazwa[BUFFER];
	bool CzyOtwarty = false;
	bool Time = true;
	while (Time) {
		menu(CzyOtwarty, Plik);
		sterowanie(Plik, CzyOtwarty,Time,Nazwa);
	}
	Plik.close();
}