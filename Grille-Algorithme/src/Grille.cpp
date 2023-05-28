#include "Grille.h"
#include <iostream>
#include <fstream>


Grille::Grille(std::string filename)
{
    std::ifstream f(filename.c_str());
    int i=0;
    std::string currentStr;
    char currentChar;
    while(!f.eof()){
        f.get(currentChar);
        if(currentChar==' ' or currentChar=='\n' or f.eof()){
            if(i==0){
                taille=std::stoi(currentStr);
                // std::cout<<"taille:"<<taille<<'\n';
            }
            else if(i==1){
                penalite=std::stoi(currentStr);
                // std::cout<<"penalite:"<<penalite<<'\n';
            }
            else if((i-2)<taille*taille){
                // std::cout<<"\nl:"<<(i-2)/taille<<"    c:"<<(i-2)%taille;
                // std::cout<<'\n'<<std::stoi(currentStr)<<'\n';
                matrice[(i-2)/taille][(i-2)%taille]=std::stoi(currentStr);
            }

            currentStr="";
            ++i;
        }else{
            currentStr+=currentChar;
        }
        
        if(i==1){
            matrice=new int*[taille];
            matricePions=new char*[taille];
            for(int i=0;i<taille;++i){
                matrice[i]=new int[taille];
                matricePions[i]=new char[taille];
            }
        }
    }
}

Grille::~Grille()
{
    for(int i=0;i<taille*taille;++i){
        delete[] matrice[i];
        delete[] matricePions[i];
    }
    delete[] matrice;
    delete[] matricePions;
}

int Grille::GetTaille(){
    return taille;
}

int Grille::GetPenalite(){
    return penalite;
}

int Grille::GetCase(unsigned int ligne,unsigned int col){
    if(ligne<0 or col<0 or ligne>=taille or col>=taille)
        return 0;
    else
        return matrice[ligne][col];
}

void Grille::SetPion(unsigned int ligne,unsigned int col,char type){
    matricePions[ligne][col]=type;
}

char Grille::GetPion(int ligne,int col){
    if(ligne<0 or col<0 or ligne>=taille or col>=taille)
        return 'X';
    else
        return matricePions[ligne][col];
}