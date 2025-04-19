#include <iostream>
#define N 2

using namespace std;


struct no{
    int volta;
    no *links[N];
    double custos[N];
    bool pitstop;
};

double h(no *s, no *t, double g, double tempoBase){
    double valorIdeal = g;
    no *h;
    h = s;
    while(h != t){
        valorIdeal += tempoBase;
        if(h->links[1] != nullptr){
            h = h->links[1];
        }
        else{
            h = h->links[0];
        }
    }
    return valorIdeal;
}

no *melhorVertice(no *v[], int n, double f[]){
    double menorValor = INT16_MAX;
    no *menorNo = nullptr;
    for(int i = 0; i < n; i++){
        if (f[i] < menorValor){
           menorValor = f[i];
           menorNo = v[i]; 
        }
    }
    return menorNo;
}

template <typename T>
bool isIn(T v[], int n, T s){
    for (int i = 0; i < n; i++){
        if (v[i] == s){
            return true;
        }
    }
    return false;
}

template <typename T>
void remove(T *vetor, int n, T valor){
    int i = 0;
    while (vetor[i] != valor){
        i++;
    }
    for(i; i < n - 1; i++){
        vetor[i] = vetor[i + 1];
    }
    vetor[i] = 0;
}

no **aStar(no *s, no *t, double tempoBase, int totalNos, bool achou){
    double f[totalNos], g[totalNos];
    no *pai[totalNos], *abertos[totalNos], *fechados[totalNos];
    int novoF, abertosNum = 0, fechadosNum = 0;

    g[s->volta] = 0;
    f[s->volta] = g[s->volta] + h(s, t, g[s->volta], tempoBase);
    pai[s->volta] = nullptr;
    abertos[abertosNum] = s;
    abertosNum++;
    achou = false;

    while (abertosNum != 0 && !achou)
    {
        no *v;
        v = melhorVertice(abertos, abertosNum, f);
        if (v == t){
            achou = true;
        }
        no *u;
        for (int i = 0; i < N; i++){
            if(v->links[i] != nullptr){
                u = v->links[i];
                novoF = v->custos[i] + h(u, t, g[v->volta], tempoBase);
                if (!((isIn(fechados, fechadosNum, u) || isIn(abertos, abertosNum, u)) && (novoF >= f[u->volta]))){
                    pai[u->volta] = v;
                    g[u->volta] = g[v->volta] + v->custos[i];
                    f[u->volta] = novoF;
                    if (isIn(fechados, fechadosNum, u)){
                        remove(fechados, fechadosNum, u);
                        fechadosNum--;
                    }
                    if (isIn(abertos, abertosNum, u)){
                        remove(abertos, abertosNum, u);
                        abertosNum--;
                    }
                    abertos[abertosNum] = u;
                    abertosNum++;
                }
            }
        }
        

        fechados[fechadosNum] = v;
        fechadosNum++;
    }

    if (achou){
        int i = 0;
        no *a;
        a = t;
        while (t != s){
            t = pai[t->volta];
            i++;
        }
        no *v[i];
        a = t;
        for (int j = 0; j < i; j++){
            v[j] = t;
            t = pai[t->volta];
        }
        return v;
    }
    else {
        return nullptr;
    }
}

no *constroiArvore(no *V, int voltasSemPit, int voltaAtual, int numVoltas, int minVoltasPit, int maxVoltasPit, double variancaTempos[], double custoPit){
    if (voltaAtual > numVoltas || voltasSemPit > maxVoltasPit || voltasSemPit < minVoltasPit){
        return nullptr;
    }
    else {
        no *M;
        M = new no;
        M->volta = voltaAtual;
        if (voltasSemPit == 0){
            M->pitstop = true;
        }
        else {
            M->pitstop = false;
        }
        V->links[0] = constroiArvore(M, 0, voltaAtual + 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit);
        if(V->links[0] != nullptr){
            M->custos[0] = custoPit;
        }
        else {
            M->custos[0] = INT16_MAX;
        }
        V->links[1] = constroiArvore(M, voltasSemPit + 1, voltaAtual + 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit);
        if(V->links[1] != nullptr){
            M->custos[1] = variancaTempos[voltasSemPit];
        }
        else {
            M->custos[1] = INT16_MAX;
        }
        return M;
    }
}

/*
no *constroiArvore(int numVoltas, int minVoltasPit, int maxVoltasPit, double variancaTempos[], double custoPit){
    no *M;
    M = new no;
    M->volta = 1;
    M->pitstop = false;
    M->links[0] = adicionaNo(M, 0, 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit);
    if(M->links[0] != nullptr){
        M->custos[0] = custoPit;
    }
    else {
        M->custos[0] = NULL;
    }
    M->links[1] = adicionaNo(M, 0, 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit);
    if(M->links[1] != nullptr){
        M->custos[1] = variancaTempos[1];
    }
    else {
        M->custos[1] = NULL;
    }
    return M;
}
    */

int main(){

    return 0;
}