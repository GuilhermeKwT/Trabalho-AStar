#include <iostream>
#include <fstream>
#include <regex>
#define N 2
#define X 10
#define TotalNos INT16_MAX

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

no *aStar(no *s, no *t, double tempoBase, bool achou){
    double f[TotalNos], g[TotalNos];
    no *pai[TotalNos], *abertos[TotalNos], *fechados[TotalNos];
    int novoF, abertosNum = 0, fechadosNum = 0;

    g[s->volta] = 0;
    f[s->volta] = g[s->volta] + h(s, t, g[s->volta], tempoBase);
    pai[s->volta] = nullptr;
    abertos[abertosNum] = s;
    abertosNum++;
    achou = false;
    cout << "caralha iniciou" << endl;

    int x = 0;

    while (abertosNum != 0 && !achou)
    {   
        no *v;
        v = melhorVertice(abertos, abertosNum, f);
        cout << t->volta << endl;
        if (v->volta == t->volta){
            achou = true;
            continue;
        }
        no *u;
        for (int i = 0; i < N; i++){
            if(v->links[i] != nullptr){
                u = v->links[i];
                novoF = v->custos[i] + h(u, t, g[v->volta], tempoBase);
                for (int z = 0; z < abertosNum; z++){
                }
                cout << endl;
                if (((isIn(fechados, fechadosNum, u) || isIn(abertos, abertosNum, u)) && (novoF >= f[u->volta]))){
                    continue;
                }
                else {
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
        remove(abertos, abertosNum, v);
        abertosNum--;
        x++;
    }

    if (achou){
        int i = 0;
        no *a;
        a = t;
        while (t != s){
            t = pai[t->volta];
            i++;
            
        }
        cout << "biava" << endl;
        cout << "pais :";
        for (int p = 0; p < i; p++){
            cout << pai[p] << endl;
        }
        no *v[i];
        a = t;
        for (int j = 0; j < i; j++){
            cout << "biavas" << j << endl;
            v[j] = t;
            t = pai[t->volta];
        }
        return *v;
    }
    else {
        
        return nullptr;
    }
}

no *constroiArvore(int voltasSemPit, int voltaAtual, int numVoltas, int minVoltasPit, int maxVoltasPit, double variancaTempos[], double custoPit, no *Fim){
    if (voltaAtual > numVoltas){
        return Fim;
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
        if (voltasSemPit <= minVoltasPit){
            M->links[0] = nullptr;
            M->custos[0] = INT16_MAX;
        }
        else {
            M->links[0] = constroiArvore(0, voltaAtual + 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit, Fim);
            M->custos[0] = custoPit;
        }
        if (voltasSemPit >= maxVoltasPit){
            M->links[1] = nullptr;
            M->custos[1] = INT16_MAX;
        }
        else {
            M->links[1] = constroiArvore(voltasSemPit + 1, voltaAtual + 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit, Fim);
            M->custos[1] = variancaTempos[voltasSemPit];
        }

        return M;
    }
}

// Faz a leitura do arquivo de entrada e o coloca em uma string com as linhas dividadas por \n
string lerTxt()
{
    ifstream file("input.txt");
    if (!file)
    {
        cout << "num abriu :(" << endl;
    }
    string str, input;
    while (getline(file, str))
    {

        input += str;
        input.push_back(' ');
    }
    return input;
}

void lerInput(string input, int *numVoltas, int *minVoltasPit, int *maxVoltasPit, double *tempoBase, double variancaTempos[], double *custoPit){
    regex termoRegex("voltas:\\s*(\\d+)\\s*min voltas antes do pitstop:\\s*(\\d+)\\s*max voltas antes do pitstop:\\s*(\\d+)\\s*tempo base:\\s*(\\d+[.]?\\d*)\\s*tempo de pitstop:\\s*(\\d+[.]?\\d*)");
    smatch match;

    auto it = input.cbegin();
    regex_search(it, input.cend(), match, termoRegex);
    *numVoltas = stoi(match.str(1));
    *minVoltasPit = stoi(match.str(2));
    *maxVoltasPit = stoi(match.str(3));
    *tempoBase = stod(match.str(4));
    *custoPit = stod(match.str(5));

    it = match.suffix().first;
    regex varianca(".*tempo perdido por volta sem pit:");
    regex_search(it, input.cend(), match, varianca);
    regex tempos("(\\d+[.]?\\d*)");
    int i = 0;
    while(regex_search(it, input.cend(), match, tempos)){
        variancaTempos[i] = stod(match.str(1));
        it = match.suffix().first;
        i++;
    }
    
}

    int maximo(int a, int b)
    {
    return (a > b)? a : b;
    }
 
    int altura_AVL(no *p)
    {
        if (p == NULL)
            return 0;
        return 1+maximo(altura_AVL(p->links[0]), altura_AVL(p->links[1]));
    }

void mostraPorNivel(no *T, int nivel, int prox_nivel) {
	if (T == NULL) return;
		if (nivel == prox_nivel) {
				cout << T->volta << " - ";
	} else if (nivel < prox_nivel) {
				mostraPorNivel(T->links[0],nivel + 1, prox_nivel);
				mostraPorNivel(T->links[1],nivel + 1, prox_nivel);
	}
}

void largura_AVL(no *T) {
	int altura = altura_AVL(T);
	for (int i = 1; i <= altura+1; ++i) {
		mostraPorNivel(T, 1, i);
		cout << endl;
	}
}

int main(){
    string input;
    input = lerTxt();
    int numVoltas, minVoltasPit, maxVoltasPit;
    double tempoBase, custoPit, variancaTempos[X];
    cout << input << endl;
    lerInput(input, &numVoltas, &minVoltasPit, &maxVoltasPit, &tempoBase, variancaTempos, &custoPit);
    cout << numVoltas << "  " << minVoltasPit << "  " << maxVoltasPit << "  " << tempoBase << "  " << custoPit << endl;

    
    no *Fim = new no;
    Fim->volta = numVoltas + 1;
    Fim->links[0]  = nullptr;
    Fim->links[1] = nullptr;
    Fim->pitstop = false;
    Fim->custos[0] = 0;
    Fim->custos[1] = 0;
    no *s;
    s = constroiArvore(0, 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit, Fim);
    largura_AVL(s);

    cout << "Fim: " << &Fim << endl << endl;

    no *caminhoFinal;
    bool achou = false;
    caminhoFinal = aStar(s, Fim, tempoBase, &achou);
    cout << caminhoFinal[0].volta << endl;


    return 0;
}