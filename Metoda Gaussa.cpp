// POP_2014_12_19_projekt_2_Polaszek_Jêdrzej_EiT_3_154952
// Code:Blocks Studio 13.12
#include <iostream>
#include <cstdlib> 
#include <cmath>
#include <ctime> 


using namespace std;

const double EPSILON=1E-10; // double maja problemy z zerem z zalozenia

struct rownanie {
    int liczba_zmiennych;
    double *wspolczynniki;
};


rownanie dodaj_rownania (rownanie a, rownanie b, double wsp_b=1)
{
    rownanie wynik;
    wynik.liczba_zmiennych=0;

    if(a.liczba_zmiennych!=b.liczba_zmiennych) // gdy ilosc zmiennych jest rowna zwroc rownanie bez zmiennych
        return wynik;

    wynik.liczba_zmiennych=a.liczba_zmiennych;

    wynik.wspolczynniki= new double[a.liczba_zmiennych+1]; 

    for(int x=0; x<a.liczba_zmiennych+1; x++)
    {
        wynik.wspolczynniki[x]=a.wspolczynniki[x]+wsp_b*b.wspolczynniki[x]; // dodawanie
        if(fabs(wynik.wspolczynniki[x])<EPSILON)
            wynik.wspolczynniki[x]=0;
    }

    return wynik;
}

//sluzy do mnozenia, wynik zwracany
rownanie mnozenie_rownania (rownanie a, double skalar)
{
    rownanie wynik;

    wynik.liczba_zmiennych=a.liczba_zmiennych;

    wynik.wspolczynniki= new double[a.liczba_zmiennych+1]; 
    for(int x=0; x<a.liczba_zmiennych+1; x++)
    {
        wynik.wspolczynniki[x]=skalar*a.wspolczynniki[x] ;// mno¿enie
    }

    return wynik;
}


int ile_niezerowych_wspolczynnikow(rownanie a)
{
    int licznik; 
    licznik = 0;

    for(int x=0; x<a.liczba_zmiennych; x++)
    {
        if(fabs(a.wspolczynniki[x])>EPSILON) // bo przy odejmowaniu w doublu jest bardzo mala wartosc a nie 0 jak wynika z matematyki
            licznik++;
    }
    return licznik;
}

rownanie losuj_rownanie(int ile_zmiennych)
{
    rownanie wynik;

    wynik.liczba_zmiennych=ile_zmiennych;

    wynik.wspolczynniki=new double[ile_zmiennych+1];//no bo jeszcze wyraz wolny

    for (int x=0; x<ile_zmiennych+1; x++)
    {
        wynik.wspolczynniki[x]=(rand()%100)/double(10); 
    }

    return wynik;
}

void wypisz_rownanie(rownanie a)
{

    

    for(int x=0; x<a.liczba_zmiennych; x++)
    {
        cout<< ((x)?"+ (":"");
        cout.width(7);
        cout.precision(4);
        cout<<a.wspolczynniki[x]<<((x)?")":"")<<"*x"<<x+1 << " " ;
    }
    cout << "= "<<a.wspolczynniki[a.liczba_zmiennych]<<endl;
}

int czy_rownanie_sprzeczne(rownanie a)
{
    if (ile_niezerowych_wspolczynnikow(a))
        return 0;
    if (fabs(a.wspolczynniki[a.liczba_zmiennych])>EPSILON)
        return 1;
    return 0;
}

// a, b rownania do zamiany, indeksowane od 0
void zamien_rownania(rownanie **tab, int a, int b)
{
    rownanie *temp;
    temp=tab[a];
    tab[a]=tab[b];
    tab[b]=temp;
    
}

void wypisz_uklad_rownan(rownanie **tab, int ile_rownan)
{
    for (int x=0; x<ile_rownan; x++)
    {
        wypisz_rownanie(*(tab[x]));
    }
    cout << endl;
}

int zrob_macierz_schodkowa(rownanie **tab, int ile_rownan)
{
    int ile_zmiennych;
    ile_zmiennych = tab[0]->liczba_zmiennych;
    int a=0;
    int x;

    for(x=0; x+a<ile_zmiennych&& x< ile_rownan; x++) //eliminacja
    {

        if(fabs(tab[x]->wspolczynniki[x+a])<EPSILON)
        {
            int flag=1;

            for(int y=x+1; y<ile_rownan; y++)
            {
                if((tab[y]->wspolczynniki[x+a]>EPSILON))
                {
                    zamien_rownania(tab, x, y);

                    flag=0;
                    break;
                }

            }
            if (flag) //if (flag!=0)
            {
                a++;
                continue;

            }
                
        }

        *(tab[x])=mnozenie_rownania(*(tab[x]), 1 / tab[x]->wspolczynniki[x+a]); 
        for(int y=x+1; y<ile_rownan; y++) 
        {
            *(tab[y])=dodaj_rownania(*(tab[y]), *(tab[x]), -tab[y]->wspolczynniki[x+a]);//zerowanie wspolczynnikow
        }
    }

    //wypisz_uklad_rownan(tab, ile_rownan);

    int rzad=0;

    for(int q=0; q<ile_rownan; q++)
    {
        if(czy_rownanie_sprzeczne(*(tab[q])))
        {
            cout<< "uklad sprzeczny!!"<<endl;
            return 1;
        }
    }


    for(int q=0; q<ile_zmiennych && q<ile_rownan; q++)
    {
        if(ile_niezerowych_wspolczynnikow(*(tab[q])))
            rzad++;
    }
   

    int parametrow=ile_zmiennych-rzad;
    int y=rzad-1;
    for (x-=1; x>=0; x--)
    {
     
        if(fabs(tab[x]->wspolczynniki[y])<EPSILON)
            continue;


        *(tab[x])=mnozenie_rownania(*(tab[x]), 1 / tab[x]->wspolczynniki[y]);
        for(int r=x-1; r>=0; r--)
        {
            *(tab[r])=dodaj_rownania(*(tab[r]), *(tab[x]), -tab[r]->wspolczynniki[y]);

    
        }
        y--;
        if(y==0)
            break;
    }

    cout<< "rozwiazanie zalezne od " << parametrow << " parametrow"<<endl;

    for (int x=0; x<rzad; x++)
    {
        cout << "x"<<x+1<<" = ";
        cout.width(7);
        cout.precision(3);
        cout<<tab[x]->wspolczynniki[ile_zmiennych];
        for(int y=0; y<parametrow; y++)
        {
            cout<< "-( ";
            cout.width(7);
            cout.precision(3);
            cout<<tab[x]->wspolczynniki[ile_zmiennych-(y+1)]<<")*x"<<ile_zmiennych-y << " ";
        }
        cout<<endl;
    }
}

int main()
{
    srand(time(NULL));

    int rownan, zmiennych;
    cout << "Podaj ilosc zmiennych: ";
    cin >> zmiennych;
    cout << "Podaj ilosc rownan: ";
    cin >> rownan;

    rownanie **tab;
    tab=new  rownanie*[rownan];

    for (int x=0; x<rownan; x++)
    {
        tab[x]=new rownanie;
        tab[x]->liczba_zmiennych=zmiennych;
        tab[x]->wspolczynniki=new double[zmiennych+1];
    }

    cout << "Podaj wspolczynniki rownan i wyrazy wolne dla kolejnych rownan: " << endl;
    for(int x=0; x<rownan; x++)
    {
        for (int y=0; y<=zmiennych; y++)
        {
            cin >> tab[x]->wspolczynniki[y];
        }
    }
    cout << endl;

    wypisz_uklad_rownan(tab, rownan);
    zrob_macierz_schodkowa(tab, rownan);

    system("pause");

         

    for (int x=0; x<rownan; x++)
    {
       
        double* tmp;
        tmp=tab[x]->wspolczynniki;
        delete[] tmp;
        
        delete tab[x];

        
    }
    delete[] tab;



    return 0;
}
