#include "Grille.h"
#include <iostream>
#include <fstream>
#include <array>

#include <chrono>

int Score(Grille& grl);
void AffichageMatriceCases(Grille& grl);
void AffichageMatricePions(Grille& grl);
void Algo1(Grille& grl);
void Algo2(Grille& grl);

void SaveFile(Grille& grl,std::string f);

int main(){
    std::string filename;
    std::cout<<"Nom du fichier:\n";
    std::cin>>filename;
    Grille grl(filename);
    AffichageMatriceCases(grl);
    bool algo1=false;
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    if(algo1){
        std::cout<<"\nALGO 1: \n";
        Algo1(grl);
    }else{
        std::cout<<"\nALGO 2: \n";
        Algo2(grl);
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout <<"Temps ecoule: " << elapsed_seconds.count() << "s\n";

    SaveFile(grl,filename);
    
    return 0;
}

void SaveFile(Grille& grl,std::string f){
    // Creer le chemin de sortie
    bool contains=false;
    int i=0;
    while(i<f.length() and !contains){
        if(f[i]=='/')
            contains=true;
        ++i;
    }
    std::string output;
    if(contains){
        std::size_t pos=f.find_last_of("/");
        output=f.substr(0,pos+1)+"resultat_"+f.substr(pos+1);
    } else{
        output="resultat_"+f;
    }
    //

    std::ofstream of(output.c_str());
    for(int i=0;i<grl.GetTaille();++i){
        for(int j=0;j<grl.GetTaille();++j){
            of<<grl.GetPion(i,j);
            of<<" ";
        }
        of<<'\n';
    }
    of<<Score(grl);
}

void AffichageMatriceCases(Grille& grl){
    std::cout<<"\nAFFICHAGE DE LA MATRICE DES CASES\n";
    for(int i=0;i<grl.GetTaille();++i){
        for(int a=0;a<grl.GetTaille();++a){
            std::cout<<grl.GetCase(i,a)<<" ";
        }
        std::cout<<std::endl;
    }
}

void AffichageMatricePions(Grille& grl){
    std::cout<<"\nAFFICHAGE DE LA MATRICE DES PIONS\n";
    for(int i=0;i<grl.GetTaille();++i){
        for(int a=0;a<grl.GetTaille();++a){
            std::cout<<grl.GetPion(i,a)<<" ";
        }
        std::cout<<std::endl;
    }
}

char* PionsAdjacents(Grille& grl,int ligne,int col){
    char* pionsAdj=new char[8];

    pionsAdj[0]=grl.GetPion(ligne+1,col);
    pionsAdj[1]=grl.GetPion(ligne-1,col);
    pionsAdj[2]=grl.GetPion(ligne,col-1);
    pionsAdj[3]=grl.GetPion(ligne,col+1);
    pionsAdj[4]=grl.GetPion(ligne-1,col-1);
    pionsAdj[5]=grl.GetPion(ligne+1,col-1);
    pionsAdj[6]=grl.GetPion(ligne-1,col+1);
    pionsAdj[7]=grl.GetPion(ligne+1,col+1);

    return pionsAdj;
}

char* PionsAdjacentsSuivants(Grille& grl,int ligne,int col){
    char* pionsAdj=new char[4];

    pionsAdj[0]=grl.GetPion(ligne+1,col);
    pionsAdj[1]=grl.GetPion(ligne,col+1);
    pionsAdj[2]=grl.GetPion(ligne+1,col-1);
    pionsAdj[3]=grl.GetPion(ligne+1,col+1);

    return pionsAdj;
}

int** CasesAdjacentesVides(Grille& grl,int ligne,int col){
    int** casesAdj=new int*[4];
    for(int i=0;i<4;++i)
        casesAdj[i]=new int[2];

    if(grl.GetPion(ligne,col+1)=='X'){
        casesAdj[0][0]=ligne;
        casesAdj[0][1]=col+1;
    }
    else{
        casesAdj[0][0]=-1;casesAdj[0][1]=-1;
    }
    if(grl.GetPion(ligne+1,col+1)=='X'){
        casesAdj[1][0]=(ligne+1);
        casesAdj[1][1]=col+1;
    }
    else{
        casesAdj[1][0]=-1;casesAdj[1][1]=-1;
    }
    if(grl.GetPion(ligne+1,col)=='X'){
        casesAdj[2][0]=ligne+1;
        casesAdj[2][1]=col;
    }
    else{
        casesAdj[2][0]=-1;casesAdj[2][1]=-1;
    }
    if(grl.GetPion(ligne+1,col-1)=='X'){
        casesAdj[3][0]=ligne+1;
        casesAdj[3][1]=col-1;
    }
    else{
        casesAdj[3][0]=-1;casesAdj[3][1]=-1;
    }

    return casesAdj;
}

int PenalitePionJaune(Grille& grl,unsigned int ligne,unsigned int col){
    char* pionsAdj=PionsAdjacents(grl,ligne,col);
    // std::cout<<"pions adjacent: \n";
    for(unsigned int i=0;i<8;++i){
        // std::cout<<pionsAdj[i]<<"\n";
        if(pionsAdj[i]=='J'){
            delete[] pionsAdj;
            return 0;
        }
    }
    // std::cout<<'\n';
    delete[] pionsAdj;
    return -grl.GetPenalite();
}

int PenaliteVert(Grille& grl,unsigned int ligne,unsigned int col){
    char* pionsAdj=PionsAdjacentsSuivants(grl,ligne,col);
    unsigned int countPen=0;
    for(unsigned int i=0;i<4;++i){
        if(pionsAdj[i]=='V'){
            ++countPen;
        }
    }
    delete[] pionsAdj;
    return countPen*(-grl.GetPenalite());
}

int PenalitePionOrange(Grille& grl,int ligne,int col){
    int count=0;
    int decalageDiagonale=col-ligne;
    int decalageDiagonale2=ligne;
    decalageDiagonale2-=grl.GetTaille()-1-col;
    for(int i=0;i<grl.GetTaille();++i){ 
        if(i!=col and grl.GetPion(ligne,i)=='O'){ // Pions sur la même colonne
            ++count;
        }
        if(i!=ligne and grl.GetPion(i,col)=='O'){ // Pions sur la même ligne
            ++count;
        }
        if(i!=ligne or i+decalageDiagonale!=col){ // Pions sur la 1ère diagonale
            if(i+decalageDiagonale<grl.GetTaille() and i+decalageDiagonale>=0){
                if(grl.GetPion(i,i+decalageDiagonale)=='O')
                    ++count;
            }
        }
        if(i!=ligne or grl.GetTaille()-1-i+decalageDiagonale2!=col){ // Pions sur la 2ème diagonale
            if(grl.GetTaille()-1-i+decalageDiagonale2<grl.GetTaille() and grl.GetTaille()-1-i+decalageDiagonale2>=0){
                if(grl.GetPion(i,grl.GetTaille()-1-i+decalageDiagonale2)=='O')
                    ++count;
            }
        }
    }
    return -count*grl.GetPenalite();
}

int Score(Grille& grl){
    int scoreTotal=0;
    int* nbPionsBleus=new int[3]{0,0,0};
    int nbPionsNoirs=0,scorePionsNoirs=0;
    
    for(int i=0;i<grl.GetTaille();++i){
        for(int j=0;j<grl.GetTaille();++j){
            if (grl.GetPion(i,j)=='J'){
                scoreTotal+=grl.GetCase(i,j);
                int pen=PenalitePionJaune(grl,i,j);
                // std::cout<<"pen : "<<pen<<"\n";
                scoreTotal+=pen;
            }
            else if (grl.GetPion(i,j)=='V'){
                scoreTotal+=grl.GetCase(i,j);
                //Creer fonction points pions verts
                if(j<grl.GetTaille()-1) scoreTotal+=grl.GetCase(i,j+1);
                if(j>0) scoreTotal+=grl.GetCase(i,j-1);
                if(i>0) scoreTotal+=grl.GetCase(i-1,j);
                if(i<grl.GetTaille()-1) scoreTotal+=grl.GetCase(i+1,j);
                
                scoreTotal+=PenaliteVert(grl,i,j);
            }
            else if (grl.GetPion(i,j)=='N'){
                ++nbPionsNoirs;
                scorePionsNoirs+=(grl.GetCase(i,j)-1);
            }
            else if (grl.GetPion(i,j)=='O'){
                //n'ajoute pas de points
                scoreTotal+=PenalitePionOrange(grl,i,j);
                // std::cout<<"\nO\n";
            }
            else if (grl.GetPion(i,j)=='R'){
                scoreTotal-=grl.GetCase(i,j);
            }
            else if(grl.GetPion(i,j)=='B'){
                if(grl.GetCase(i,j)>0){
                    nbPionsBleus[0]+=1;
                }else if(grl.GetCase(i,j)<0){
                    nbPionsBleus[1]+=1;
                }
            }
        }
    }
    {
        if(nbPionsBleus[0]>nbPionsBleus[1])
            nbPionsBleus[2]=nbPionsBleus[0]-nbPionsBleus[1];
        else
            nbPionsBleus[2]=nbPionsBleus[1]-nbPionsBleus[0];

        if(nbPionsBleus[2]>0){
            scoreTotal+=nbPionsBleus[2]*(-grl.GetPenalite());
        }
        delete[] nbPionsBleus;
    }
    {
        if(nbPionsNoirs<=grl.GetTaille())
            scoreTotal+=(scorePionsNoirs*2);
        else
            scoreTotal+=scorePionsNoirs;
    }
    return scoreTotal;
}

void InitialiserPions(Grille& grl){
    for(int i=0;i<grl.GetTaille();++i){
        for(int a=0;a<grl.GetTaille();++a){
            grl.SetPion(i,a,'X');
        }
    }
}


void InverserValeurs(int& a,int& b){
    int temp=a;
    a=b;
    b=temp;
}

int Partionnement(int* cases,int** positions,int d,int f){
    int pivot=cases[f];
    unsigned int j=d;
    for(int i=d;i<f;++i){
        if(cases[i]<pivot){
            InverserValeurs(cases[i],cases[j++]);
            InverserValeurs(positions[i][0],positions[j-1][0]);
            InverserValeurs(positions[i][1],positions[j-1][1]);
        }
    }
    InverserValeurs(cases[f],cases[j]);
    InverserValeurs(positions[f][0],positions[j][0]);
    InverserValeurs(positions[f][1],positions[j][1]);
    return j;
}

void QuickSort(int* cases,int** positions,int d,int f){
    if(f-d>=1){
        int k=Partionnement(cases,positions,d,f);
        
        QuickSort(cases,positions,d,k-1);
        QuickSort(cases,positions,k+1,f);
    }
}

void PlacerRougesEtNoirs(Grille& grl,int& scoreTotal){
    unsigned int t=grl.GetTaille()*grl.GetTaille();
    int* cases=new int[t];
    int** positions=new int*[t];

    for(int i=0;i<t;++i)
        positions[i]=new int[2];

    int scoreNoirs=0;
    int index=0;
    for(int i=0;i<grl.GetTaille();++i){
        for(int j=0;j<grl.GetTaille();++j){
            cases[index]=grl.GetCase(i,j);
            positions[index][0]=i;
            positions[index++][1]=j;
        }
    }

    QuickSort(cases,positions,0,t-1);
    

    //Placer les grl.GetTaille() pions noirs sur les plus grandes cases
    for(int i=t-grl.GetTaille();i<t;++i){
        if(grl.GetPion(positions[i][0],positions[i][1])>0){
            grl.SetPion(positions[i][0],positions[i][1],'N');
            scoreNoirs+=grl.GetCase(positions[i][0],positions[i][1])-1;
        }
    }
    scoreNoirs*=2;
    // std::cout<<"noir: "<<scoreTotal<<'\n';


    //Placer le pion Rouge sur la plus petite case
    grl.SetPion(positions[0][0],positions[0][1],'R');
    // grl.SetPion(positions[1][0],positions[1][1],'O');
    // std::cout<<"orange: "<<positions[1][0]<<", "<<positions[1][1]<<'\n';

    // for(int i=0;i<t;++i)
    //     std::cout<<grl.GetCase(positions[i][0],positions[i][1])<<'\n';

    //Score final
    scoreTotal-=grl.GetCase(positions[0][0],positions[0][1]);
    scoreTotal+=scoreNoirs;
}

//ALGORITHME 1

struct DeuxPions{
    char pion;
    int score;
    int secondeCaseLigne;
    int secondeCaseCol;
};

DeuxPions ScorePionsBleusEtJaunes(Grille& grl,int ligne,int col){
    DeuxPions combiBleu,combiJaune;
    int** casesAdj=CasesAdjacentesVides(grl,ligne,col);


    //Bleu
    grl.SetPion(ligne,col,'B');
    combiBleu.pion='B';
    combiBleu.score=-1000000;
    int tempScore,tempCase;

    if(grl.GetCase(ligne,col)>0){
        for(int i=0;i<4;++i){
            tempCase=grl.GetCase(casesAdj[i][0],casesAdj[i][1]);
            tempScore=-grl.GetCase(ligne,col)-tempCase;
            if(tempCase<0 and tempScore>combiBleu.score and casesAdj[i][0]>=0){
                combiBleu.score=tempScore;
                combiBleu.secondeCaseLigne=casesAdj[i][0];
                combiBleu.secondeCaseCol=casesAdj[i][1];
            }
        }
    }
    else if(grl.GetCase(ligne,col)<0){
        for(int i=0;i<4;++i){
            tempCase=grl.GetCase(casesAdj[i][0],casesAdj[i][1]);
            tempScore=-grl.GetCase(ligne,col)-tempCase;
            if(tempCase>0 and tempScore>combiBleu.score and casesAdj[i][0]>=0 and casesAdj[i][0]<grl.GetTaille() and casesAdj[i][1]<grl.GetTaille()){
                combiBleu.score=tempScore;
                combiBleu.secondeCaseLigne=casesAdj[i][0];
                combiBleu.secondeCaseCol=casesAdj[i][1];
            }
        }
    }
    
    //Jaune
    grl.SetPion(ligne,col,'J');
    combiJaune.pion='J';
    
    char* pionsAdj=PionsAdjacents(grl,ligne,col);
    bool adj=false;

    for(int i=0;i<8;++i){
        if(pionsAdj[i]=='J')
            adj=true;
    }

    // if(adj)
    //     std::cout<<"Pions jaunes adjacents ? Oui"<<'\n';
    // else
    //     std::cout<<"Pions jaunes adjacents ? Non"<<'\n';

    if(adj){
        combiJaune.score=grl.GetCase(ligne,col);
        combiJaune.secondeCaseLigne=-1;
    }else{
        combiJaune.score=-1000000;
        for(int i=0;i<4;++i){
            // std::cout<<"casesAdj["<<i<<"]: "<<casesAdj[i][0]<<", "<<casesAdj[i][1]<<'\n';
            tempCase=grl.GetCase(casesAdj[i][0],casesAdj[i][1]);
            tempScore=grl.GetCase(ligne,col)+tempCase;
            if(tempScore>combiJaune.score and casesAdj[i][0]>=0 and casesAdj[i][0]<grl.GetTaille() and casesAdj[i][1]<grl.GetTaille()){
                combiJaune.score=tempScore;
                combiJaune.secondeCaseLigne=casesAdj[i][0];
                combiJaune.secondeCaseCol=casesAdj[i][1];
            }
        }
    }

    if(combiJaune.score>=combiBleu.score)
        return combiJaune;
    else
        return combiBleu;
}

int ScorePionVert(Grille& grl,int ligne,int col){
    int scoreTotal=0;
    grl.SetPion(ligne,col,'V');
    scoreTotal+=grl.GetCase(ligne,col);
    if(col<grl.GetTaille()-1) scoreTotal+=grl.GetCase(ligne,col+1);
    if(col>0) scoreTotal+=grl.GetCase(ligne,col-1);
    if(ligne>0) scoreTotal+=grl.GetCase(ligne-1,col);
    if(ligne<grl.GetTaille()-1) scoreTotal+=grl.GetCase(ligne+1,col);
    // std::cout<<"penalite vert: "<<penaliteVert(grl,ligne,col)<<'\n';
    scoreTotal+=PenaliteVert(grl,ligne,col);

    // std::cout<<"Pion Vert: "<<scoreTotal<<'\n';
    return scoreTotal;
}

void MeilleurPion(Grille& grl,int ligne,int col,int& score){
    // std::cout<<'\n';
    grl.SetPion(ligne,col,'V');
    int mScore=ScorePionVert(grl,ligne,col);
    char mPion='V';
    mScore=ScorePionVert(grl,ligne,col);
    DeuxPions combiPion=ScorePionsBleusEtJaunes(grl,ligne,col);
    if(combiPion.score>=mScore){
        mPion=combiPion.pion;
        mScore=combiPion.score;
    }
    
    // std::cout<<"\nmPion: "<<mPion<<'\n';
    // std::cout<<"mScore: "<<mScore<<'\n';

    score+=mScore;

    grl.SetPion(ligne,col,mPion);

    if(mPion=='B' or mPion=='J'){
        if(mPion=='B')
            score-=mScore;
        // std::cout<<"\nmeilleurSecondeCase: "<<meilleurSecondeCase<<'\n';
        if(combiPion.secondeCaseLigne>=0)
            grl.SetPion(combiPion.secondeCaseLigne,combiPion.secondeCaseCol,mPion);
    }
}

void Algo1(Grille& grl){
    InitialiserPions(grl);
    char m;
    int score=0;
    PlacerRougesEtNoirs(grl,score);
    for(int i=0;i<grl.GetTaille();++i){
        for(int a=0;a<grl.GetTaille();++a){
            // std::cout<<"---------------\n";
            if(grl.GetPion(i,a)=='X')
                MeilleurPion(grl,i,a,score);
            // AffichageMatricePions(grl);
            // std::cout<<"---------------\n";
        }
    }
    AffichageMatricePions(grl);
    std::cout<<"\nSCORE\n"<<score;
    std::cout<<"---------------\n";
}
//------------

// ALGORITHME 2

void GrillePions(char* pions,Grille& grl){ // Place les pions contenus dans un tableau de char 
    int index=0;
    for(int i=0;i<grl.GetTaille();++i)
        for(int j=0;j<grl.GetTaille();++j)
            grl.SetPion(i,j,pions[index++]);
}

char* PionsGrille(Grille& grl){ // Convertir la grille en tableau de char 
    char* pions=new char[grl.GetTaille()*grl.GetTaille()];
    int index=0;
    for(int i=0;i<grl.GetTaille();++i)
        for(int j=0;j<grl.GetTaille();++j)
            pions[index++]=grl.GetPion(i,j);

    return pions;
}

struct CombinaisonsPions{
    int score;
    char* pions;
};

int* IndexToCoords(int index,int row){
    int* coords=new int[2];
    int temp=0;
    for(int i=0;i<row;++i){
        for(int j=0;j<row;++j){
            if(temp==index){
                coords[0]=i;
                coords[1]=j;
                return coords;
            }
            ++temp;
        }
    }
    return coords;
}

CombinaisonsPions Backtracking(Grille& grl,int ligne,int col,unsigned int depth,int& nb){
    bool finGrille=ligne==grl.GetTaille()-1 and col==grl.GetTaille()-1;
    if(depth==0 or finGrille){
        ++nb;
        return {Score(grl),PionsGrille(grl)};
    }else{
        char* pions=new char[3]{'J','B','O'};
        CombinaisonsPions temp;

        int nLigne,nCol;
        if(col==grl.GetTaille()-1){
            nLigne=++ligne;
            nCol=0;
        }
        else{
            nLigne=ligne;
            nCol=++col;
        }

        int MeilleurScore=-1000000000;
        int* currentVal=new int[3];
        char* MeilleursCombinaisonsPions;
        bool dontChange=false;
        dontChange=grl.GetPion(nLigne,nCol)=='R' or grl.GetPion(nLigne,nCol)=='V'
        or grl.GetPion(nLigne,nCol)=='N';

        if(!dontChange){
            for(int i=0;i<3;++i){
                grl.SetPion(nLigne,nCol,pions[i]);
                temp=Backtracking(grl,nLigne,nCol,depth-1,nb);

                currentVal[i]=temp.score;
                if(currentVal[i]>MeilleurScore){
                    MeilleurScore=currentVal[i];
                    MeilleursCombinaisonsPions=temp.pions;
                }
            }
        }
        else{
            temp=Backtracking(grl,nLigne,nCol,depth,nb);
            if(temp.score>MeilleurScore){
                MeilleurScore=temp.score;
                MeilleursCombinaisonsPions=temp.pions;
            }
        }
        return {MeilleurScore,MeilleursCombinaisonsPions};
    }
}

int RechercherCaseVide(Grille& grl){
    int i=0,j=0;
    bool continuer=true;

    while(i<grl.GetTaille() and continuer){
        while(j<grl.GetTaille() and continuer){
            if(grl.GetPion(i,j)=='X')
                continuer=false;
            else
                ++j;
        }
        if(continuer){
            j=0;
            ++i;
        }
    }
    if(continuer)
        return -1;
    else
        return i*grl.GetTaille()+j;
}

void CompleterGrille(Grille& grl,int depth){
    char* pions=new char[3]{'B','O','J'};
    int taille=grl.GetTaille()*grl.GetTaille();
    int* coords;
    int l,c;
    std::array<CombinaisonsPions,3> res;
    int coordCaseVide=RechercherCaseVide(grl);
    
    int nbCas=1;
    if(coordCaseVide==0)
        nbCas=3;

    int nb=0;
    int mScore=-1000000;
    char* mGrille=new char[grl.GetTaille()*grl.GetTaille()];
    for(int a=0;a<nbCas;++a){
        if(nbCas==3)
            grl.SetPion(0,0,pions[a]);

        while(coordCaseVide!=-1){
            if(coordCaseVide==0)
                coords=IndexToCoords(coordCaseVide,grl.GetTaille());
            else
                coords=IndexToCoords(coordCaseVide-1,grl.GetTaille());

            l=coords[0];
            c=coords[1];

            // Backtracking
            if(coordCaseVide==0)
                res[a]=Backtracking(grl,l,c,depth-1,nb);
            else
                res[a]=Backtracking(grl,l,c,depth,nb);
                
            int index=0;
            for(int i=0;i<grl.GetTaille();++i){
                for(int j=0;j<grl.GetTaille();++j){
                    grl.SetPion(i,j,res[a].pions[index++]);
                }
            }
            coordCaseVide=RechercherCaseVide(grl);
        }
        int tempScore=Score(grl);
        if(tempScore>mScore){
            mScore=tempScore;
            mGrille=PionsGrille(grl);
        }
    }
    GrillePions(mGrille,grl);
    AffichageMatricePions(grl);
    std::cout<<"Score:"<<mScore<<"\n";
    // std::cout<<"nb grilles testees: "<<nb<<'\n';
}

void PlacerPionsSpecifiques(Grille& grl,char p){
    char currentP;
    int scoreInitial=Score(grl);
    int temp,mScore=scoreInitial;
    int* coords=new int[2];
    for(int i=0;i<grl.GetTaille();++i){
        for(int j=0;j<grl.GetTaille();++j){
            currentP=grl.GetPion(i,j);

            if(currentP!='R')
                grl.SetPion(i,j,p);

            temp=Score(grl);

            grl.SetPion(i,j,currentP);
            if(temp>=mScore){
                mScore=temp;
                coords[0]=i;
                coords[1]=j;
            }
        }
    }
    if(mScore>scoreInitial){
        grl.SetPion(coords[0],coords[1],p);
    }
}

bool ComparerTableauxChar(char* a,char* b,int size){
    bool result=true;
    int i=0;
    while(i<size and result){
        if(a[i]!=b[i])
            result=false;
        else
            ++i;
    }
    return result;
}

void Algo2(Grille& grl){
    int scoreTotal=0;
    InitialiserPions(grl);
    PlacerRougesEtNoirs(grl,scoreTotal);

    int t=grl.GetTaille()*grl.GetTaille();

    // AffichageMatricePions(grl);

    char* pionsGrillePrecedente;
    char* temp;
    char* pions=new char[5]{'V','N','J','B','O'};
    for(int i=0;i<5;++i){
        int j=0;
        bool condition=true;
        while(j<grl.GetTaille()*grl.GetTaille() and condition){
            PlacerPionsSpecifiques(grl,pions[i%5]);
            temp=PionsGrille(grl);
            
            if(j>0){
                if(ComparerTableauxChar(temp,pionsGrillePrecedente,t)){
                    condition=false;
                }
            }

            pionsGrillePrecedente=temp;
            ++j;
        }
        // if(i==0){
        //     AffichageMatricePions(grl);
        // }
    }

    // std::cout<<"\nSCORE MAX POSSIBLE : "<<Score(grl);
    int depth=2;
    if(grl.GetTaille()<6)
        depth=15;
    else if(grl.GetTaille()<8)
        depth=14;
    else if(grl.GetTaille()<10)
        depth=12;
    else if(grl.GetTaille()<12)
        depth=10;
    else if(grl.GetTaille()<30)
        depth=8;
    else if(grl.GetTaille()<34)
        depth=7;
    CompleterGrille(grl,depth);
}