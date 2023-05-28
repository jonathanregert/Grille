#include <iostream>
class Grille
{
private:
    unsigned int taille;
    int penalite;
    int** matrice;
    char** matricePions;
public:
    Grille(std::string filename);
    ~Grille();
    int GetTaille();
    int GetPenalite();
    int GetCase(unsigned int ligne,unsigned int col);
    void SetPion(unsigned int ligne,unsigned int col,char type);
    char GetPion(int ligne,int col);
};
