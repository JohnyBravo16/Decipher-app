#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#define N 5
using namespace std;


class Text
{
public:
	string tekst;
	Text()
	{
		tekst = "";
	}
	Text(string wartosc)
	{
		this->tekst = wartosc;
	}
	friend ostream& operator<< (ostream& os, const Text& ob)
	{
		os<<ob.tekst<<endl;
		return os;
	}
};


template <class T, class T2>
class Iterator
{
public:
	friend class Kontener;
	T (**wskaznik);
protected:
	int obecny;
	T2 &container;
public:
	Iterator(T2 &cont):container(cont)
	{
		wskaznik = cont.tab;
		obecny = 0;
	}
	Iterator & na_poczatek()
	{
		wskaznik = container.tab;
		obecny = 0;
		return *this;
	}
	Iterator & na_koniec()
	{
		wskaznik = (container.tab + (N-1));
		obecny = N-1;
		return *this;
	}
	Iterator & za_koniec()
	{
		wskaznik = NULL;
		obecny = -1;
		return *this;
	}
	Iterator & na_wezel(int indeks)
	{
		wskaznik = (container.tab + (indeks - 1));
		obecny = indeks;
		return *this;
	}
	Iterator & operator++()
	{
		if(wskaznik && obecny<(N - 1))
		{
			wskaznik = (container.tab + (obecny + 1));
			obecny += 1;
		}
		else
			cout<<"Iterator jest juz na ostatnim miejscu lub jest poza tablica."<<endl;
		return *this;		
	}
	Iterator & operator--()
	{
		//cout<<endl<<"Obecny: "<<obecny<<endl;
		if(wskaznik && obecny>0)
		{
			wskaznik = (container.tab + (obecny - 1));
			obecny -= 1;
		}
		else
			cout<<"Iterator jest juz na pierwszym miejscu lub jest poza tablica."<<endl;
		return *this;		
	}
	operator int() const
	{
		return int(obecny);
	}
	void wypisz()
	{
		cout<<"Iterator wskazuje na: "<<**wskaznik<<endl;
	}
};

class Kontener
{
public:
	Text **tab;
	friend class Iterator<Text, Kontener>;
public:
	virtual void dodawanie(Iterator<Text, Kontener> &it, string wartosc) = 0;
	virtual void usuwanie(Iterator<Text, Kontener> &it) = 0;
	virtual void kopiowanie(Iterator<Text, Kontener>&it, Iterator<Text, Kontener>&it2) = 0;
    virtual void szyfruj(Iterator<Text, Kontener> &it) = 0;
	virtual void zapis_do_pliku(Iterator <Text, Kontener> &it) = 0;
	virtual void odczyt_z_pliku(Iterator <Text, Kontener> &it) = 0;
	virtual void przenoszenie(Kontener &kont) = 0;
	
};

class Zbior:public Kontener
{
private:
	string cezar(string tekst)
    {
        int klucz;
        cout<<"Podaj klucz szyfrowania: ";
        cin>>klucz;

        for(int i=0;i<=tekst.length();i++)
        {
            if(tekst[i]>=65 && tekst[i]<=90-klucz)
                tekst[i]=int(tekst[i])+klucz; //wielkie liter
            else if(tekst[i]>=91-klucz && tekst[i]<=90)
                tekst[i]=int(tekst[i])-26+klucz; // wielkie litery
            else if(tekst[i]>=97 && tekst[i]<=122-klucz)
                tekst[i]=int(tekst[i])+klucz; //małe liter
            else if(tekst[i]>=123-klucz && tekst[i]<=122)
                tekst[i]=int(tekst[i])-26+klucz; //małe litery
        }
		return tekst;
    }
public:
	Zbior()
	{
		tab = new Text*[N];
		for(int i=0;i<N;i++)
		{
			tab[i]=new Text;
		}
	}
	void dodawanie(Iterator<Text, Kontener> &it, string wartosc)
	{
		**it.wskaznik = Text(wartosc);
	}
	void usuwanie(Iterator<Text, Kontener> &it)
	{
		(**it.wskaznik).tekst = "";
	}
	void wypisz()
	{
		for(int i=0;i<100;i++)
				cout<<"-";
		cout<<endl;
		for(int i=0;i<N;i++)
		{
			/*cout<<tab[i]cout<<": "*/cout<<"|"<<tab[i]->tekst<<" "<<"|"<<endl;
			for(int i=0;i<100;i++)
				cout<<"-";
			cout<<endl;
		}
		cout<<endl;
		//cout<<&tab<<endl;
	}
	/*
	void kopiowanie(Kontener &kont)
	{
		for(int i=0;i<N;i++)
		{
			tab[i]->tekst=kont.tab[i]->tekst;
		}
	}
	*/
	void kopiowanie(Iterator<Text, Kontener> &it, Iterator<Text, Kontener> &it2)
	{
		(**it.wskaznik).tekst = (**it2.wskaznik).tekst;
	}
	virtual void przenoszenie(Kontener &kont)
	{
		for(int i=0;i<N;i++)
		{
			tab[i]->tekst=kont.tab[i]->tekst;
			kont.tab[i]->tekst="";
		}
	}
    void szyfruj(Iterator<Text, Kontener> &it)
    {
		string temp = (**it.wskaznik).tekst;
		temp = cezar(temp);
		(**it.wskaznik).tekst = temp;
    }
	void zapis_do_pliku(Iterator <Text, Kontener> &it)
	{
		string temp = (**it.wskaznik).tekst;
		string temp2 = "test.txt";
		cout<<"Podaj nazwe pliku: ";
		cin>>temp2;
		std::ofstream file(temp2);
		file<<temp;
	}
	void odczyt_z_pliku(Iterator <Text, Kontener> &it)
	{
		string temp = "";
		string temp2 = "";
		cout<<"Podaj nazwe pliku: ";
		cin>>temp2;
		std::ifstream file(temp2);
		std::stringstream buf;
		buf << file.rdbuf();
		temp = buf.str();
		(**it.wskaznik).tekst = temp;
	}
	~Zbior()
	{
		for(int i=0;i<N;i++)
		{
		  delete [] tab[i];
		}
		delete []tab;
	}	
};



int main()
{
	Zbior zb1;
	Iterator<Text, Kontener> i(zb1);
	Iterator<Text, Kontener> i2(zb1);

	int liczba;
	string temp = "";
	char temp2[1000];
	char znak;

	while(liczba != 0)
	{
		cout<<"Wybierz jedna z opcji: "<<endl<<"1 - Dodaj tekst do bazy"<<endl<<"2 - Usun tekst z bazy"<<endl
		<<"3 - Zaszyfruj jeden z tekstow"<<endl<<"4 - Wybierz pozycje"<<endl<<"5 - Zapis do pliku"<<endl<<"6 - Odczyt z pliku"<<endl<<"7 - Kopiuj tekst"<<endl<<"Kazda inna opcja - zakoncz program"<<endl<<endl;
		
		zb1.wypisz();

		cout<<endl;
		i.wypisz();
		
		cout<<"Twoj wybor: ";
		cin>>liczba;

		switch(liczba)
		{
		case 1:
			cout<<"Podaj tekst:"<<endl;
			cin.ignore();
			cin.getline(temp2, sizeof(temp2));
			temp = temp2;
			if((**i.wskaznik).tekst != "")
			{
				cout<<"Miejsce nie jest puste. Nadpisac? (t): ";
				cin>>znak;
				switch(znak)
				{
					case 't':
						zb1.dodawanie(i, temp);
						break;
					default:
						cout<<"Anulowano."<<endl;
						break;
				}
			}
			else
				zb1.dodawanie(i, temp);
			break;
		case 2:
			zb1.usuwanie(i);
			break;
		case 3:
			zb1.szyfruj(i);
			break;
		case 4:
			cout<<"Wybierz jedna z opcji: "<<endl<<"1 - Nastepny"<<endl<<"2 - Poprzedni"<<endl
			<<"3 - Pierwszy"<<endl<<"4 - Ostatni"<<endl<<"5 - Na pozycje"<<endl<<"Kazda inna opcja - Anuluj"<<endl<<endl;
			cin>>liczba;
			switch(liczba)
			{
				case 1:
					++i;
					break;
				case 2:
					--i;
					break;
				case 3:
					i.na_poczatek();
					break;
				case 4:
					i.na_koniec();
					break;
				case 5:
					cout<<"Podaj pozycje: ";
					cin>>liczba;
					i.na_wezel(liczba-1);
					break;
				default:
					break;
			}
			break;
		case 5:
			zb1.zapis_do_pliku(i);
			break;
		case 6:
			zb1.odczyt_z_pliku(i);
			break;
		case 7:
			int wezel;
			cout<<"Podaj nr wiersza, ktory chcesz skopiowac: "<<endl;
			cin>>wezel;
			i2.na_wezel(wezel);
			if((**i.wskaznik).tekst != "")
			{
				cout<<"Miejsce nie jest puste. Nadpisac? (t): ";
				cin>>znak;
				switch(znak)
				{
					case 't':
						zb1.kopiowanie(i, i2);
						break;
					default:
						cout<<"Anulowano."<<endl;
						break;
				}
			}
			else
				zb1.kopiowanie(i, i2);
			break;
		default:
			exit(1);
			break;
		}
		cout<<endl;
	}

	return 0;
}

