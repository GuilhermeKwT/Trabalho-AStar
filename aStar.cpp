#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <climits>
#include <iomanip>

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


/**
 * Realiza o cálculo da heuristica definida.
 * @param s     [IN]  Nó de onde se deseja calcular a heuristica.
 * @param t     [IN]  Nó destino.
 * @param tempoBase    [IN]  O tempo base para cada volta.
 * @retval double  Valor da heuristica.
 */
double h(no *s, no *t, double tempoBase){
    double valorIdeal = 0;
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

/**
 * Encontra a melhor vértice presente no vetor passado.
 * @param v     [IN]  Vetor de onde se deseja encontrar a melhor vértice.
 * @param n     [IN]  Tamanho do vetor v.
 * @param f    [IN]  Map contendo todos os nós e seus respectivos custos estimados.
 * @retval no*  A melhor vértice encontrado no vetor passado.
 */
no *melhorVertice(no *v[], int n, map<no*, double> f){
    double menorValor = INT16_MAX;
    no *menorNo = nullptr;
    for(int i = 0; i < n; i++){
        if (f.at(v[i]) < menorValor){
           menorValor = f.at(v[i]);
           menorNo = v[i]; 
        }
    }
    return menorNo;
}

/**
 * Verifica se um valor está presente no vetor passado.
 * @param v     [IN]  Vetor de onde se deseja procurar.
 * @param n     [IN]  Tamanho do vetor v.
 * @param s    [IN]  Item que se dejesa procurar no vetor v.
 * @retval bool  Se o valor está contido ou não no vetor.
 */
template <typename T>
bool isIn(T v[], int n, T s){
    for (int i = 0; i < n; i++){
        if (v[i] == s){
            return true;
        }
    }
    return false;
}

/**
 * Remove o valor desejado do vetor passado.
 * @param v     [IN]  Vetor de onde se deseja remover o valor.
 * @param n     [IN]  Tamanho do vetor v.
 * @param valor    [IN]  Valor que se deseja remover do vetor v.
 */
template <typename T>
void remove(T *v, int n, T valor){
    int i = 0;
    while (v[i] != valor){
        i++;
    }
    for(i; i < n - 1; i++){
        v[i] = vetor[i + 1];
    }
    v[i] = 0;
}

/**
 * Calcula o caminho de menor custo do ponto inicial até o ponto final.
 * @param s     [IN]  Nó inicial.
 * @param t     [IN]  Nó final.
 * @param tempoBase    [IN]  Tempo base para cada volta.
 * @retval no*  O vetor contendo o melhor caminho.
 * @retval nullptr  Não encontrou um caminho
 */
no *aStar(no *s, no *t, double tempoBase){
    double g[TotalNos], novoF;
    no *abertos[TotalNos], *fechados[TotalNos];
    map<no*, no*> pai;
    map<no*, double> f;
    int abertosNum = 0, fechadosNum = 0;
    bool achou = false;

    g[s->volta] = 0;
    f.insert({s, g[s->volta] + h(s, t, tempoBase)});
    abertos[abertosNum] = s;
    abertosNum++;

    while (abertosNum != 0 && !achou)
    {   
        no *v;
        v = melhorVertice(abertos, abertosNum, f);
        if (v->volta == t->volta){
            achou = true;
        }
        no *u;
        for (int i = 0; i < N; i++){
            if(v->links[i] != nullptr){
                u = v->links[i];
                novoF = v->custos[i] + h(u, t, tempoBase);
                if (!((isIn(fechados, fechadosNum, u) || isIn(abertos, abertosNum, u)) && (f.find(u) != f.end() ? (novoF >= f.at(u)) : false))){
                    pai.insert({u, v});
                    
                    g[u->volta] = g[v->volta] + v->custos[i];
                    if(f.find(u) != f.end()){
                        f.at(u) = novoF;
                    }
                    else {
                        f.insert({u, novoF});
                    }
                    
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
    }


    if (achou){
        no *caminho, *c;
        caminho = new no[t->volta]; 
        c = t;
        int k = 0;
        while (c != s){
            caminho[k] = *c;
            c = pai.at(c);
            k++;
        }
        caminho[k] = *c;
        return caminho;
    }
    else {
        return nullptr;
    }
}

/**
 * Constroi a arvore binária contendo as voltas e os custos.
 * @param voltasSemPit     [IN]  Número de voltas que não ocorrem um pitstop.
 * @param voltaAtual     [IN]  Número da volta atual.
 * @param minVoltasPit    [IN]  O mínimo de voltas antes de um pistop poder ocorrer.
 * @param maxVoltasPit    [IN]  O máximo de voltas até de um pistop poder ocorrer.
 * @param variancaTempos    [IN]  Um vetor contendo a perda de tempo por volta cada volta que está sem ocorrer um pitstop.
 * @param custoPit    [IN]  Custo de tempo para ocorrer um pitstop.
 * @param Fim    [IN]  Nó final que demostra o término da corrida.
 * @retval no*  O nó contruido.
 */
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
        if (voltasSemPit < minVoltasPit){
            M->links[0] = nullptr;
            M->custos[0] = INT16_MAX;
        }
        else {
            M->links[0] = constroiArvore(0, voltaAtual + 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit, Fim);
            M->custos[0] = custoPit;
        }
        if (voltasSemPit > maxVoltasPit){
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

/**
 * Faz a leitura do arquivo de entrada e o coloca em uma string.
 * @retval string  A string lida do arquivo.
 */
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

/**
 * Separa cada parte do input para sua váriavel própria.
 * @param input    [IN]  String de entrada.
 * @param numVoltas     [OUT]  Número total de voltas da corrida.
 * @param minVoltasPit    [OUT]  O mínimo de voltas antes de um pistop poder ocorrer.
 * @param maxVoltasPit    [OUT]  O máximo de voltas até de um pistop poder ocorrer.
 * @param tempoBase    [OUT]  Tempo base para cada volta.
 * @param variancaTempos    [OUT]  Um vetor contendo a perda de tempo por volta cada volta que está sem ocorrer um pitstop.
 * @param custoPit    [OUT]  Custo de tempo para ocorrer um pitstop.
 */
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

/**
 * Imprime o caminho passado.
 * @param caminho     [IN]  Caminho para ser imprimido.
 * @param t     [IN]  Tamanho do caminho.
 */
void imprimirResultado(no caminho[], int n){
    for (int i = n - 1; i >= 0; i--){
        string titulo = " NO ";
        string pit = caminho[i].pitstop ? "Sim" : "Nao";
        stringstream ss;
        ss << fixed << setprecision(2);

        cout << "\e[36m-----------------\e[39m" << endl;
        cout << " \e[31mVolta:\e[39m  " << setw(4) << caminho[i].volta << endl;
        cout << " Pitstop: " << setw(3) << pit << endl;
        cout << "\e[36m-----------------\e[39m" << endl;

    }
}

int main(){
    string input;
    input = lerTxt();
    int numVoltas, minVoltasPit, maxVoltasPit;
    double tempoBase, custoPit, variancaTempos[X];
    cout << input << endl;
    lerInput(input, &numVoltas, &minVoltasPit, &maxVoltasPit, &tempoBase, variancaTempos, &custoPit);
    
    no *Fim = new no;
    Fim->volta = numVoltas + 1;
    Fim->links[0]  = nullptr;
    Fim->links[1] = nullptr;
    Fim->pitstop = false;
    Fim->custos[0] = 0;
    Fim->custos[1] = 0;
    no *s;
    s = constroiArvore(1, 1, numVoltas, minVoltasPit, maxVoltasPit, variancaTempos, custoPit, Fim);

    no *caminhoFinal;
    bool achou = false;
    caminhoFinal = aStar(s, Fim, tempoBase);
    if (caminhoFinal != nullptr){
        imprimirResultado(caminhoFinal, Fim->volta);
    }
    else {
        cout << "Não foi possível encontrar um caminho" << endl;
    }
    


    return 0;
}