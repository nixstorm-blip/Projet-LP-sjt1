#pragma once
#include "Media.cpp"
class Serie :public Media{
    private:
    int nombreSaisons;
    int nombreEpisodes;

    public:
    Serie(int id, string titre,string annee, string note,string commentaire,bool possede, int nombreSaisons,int nombreEpisodes)
    :Media(id,titre,annee,note,commentaire,possede),
    nombreSaisons(nombreSaisons),
    nombreEpisodes(nombreEpisodes)
    {

    }
 int getNombreSaisons() const {
    return nombreSaisons;
 }
 
  void setNombreSaisons(int nombre){
    if (nombre < 0){
        throw std::invalid_argument(
            "Le nombre de saisons ne peut pas etre negatif"
        );
    }

    nombreSaisons=nombre;
  }


 int getNombreEpisodes()const {
    return nombreEpisodes;
 }

 void setnombreEpisodes(int nombre){
    if(nombre <0 ){
        throw std::invalid_argument(
            "le nombres d episodes ne doit pas etre negatif "
        );
    }
    nombreEpisodes=nombre;
 }

string toString() override {
    return Media::toString()
        + "\nNombre de saisons : "
        + to_string(nombreSaisons)
        + "\nNombre d'episodes : "
        + to_string(nombreEpisodes);
}

};

