#include "Player.hh"
#include <map>
#include <cmath>
#include <utility>
#include <algorithm>

#define PLAYER_NAME Joan

struct PLAYER_NAME : public Player {

  static Player* factory () {
    return new PLAYER_NAME;
  }

  #define FASEINICIAL 200

  struct Ogro {
    int orkID;
    bool en_ciudad_objetivo;
    int ciudad_objetivo;
    Pos posicion_objetivo;
    Dir direccion;
    Dir direccion_alternativa;
    int mantener_camino;
  };
  map<int,Ogro> ogros; //<ork_id,datos_ork>


  void inicializar() { //Inicializa ogros
  //Llenar ogros
    for (int ork_id : orks(me())) {
      Ogro ogro;
      ogro.en_ciudad_objetivo = false;
      ogro.orkID = ork_id;
      ogro.ciudad_objetivo = -1;
      ogro.direccion = NONE;
      ogro.direccion_alternativa = NONE;
      ogro.mantener_camino = 0;
      ogros[ork_id] = ogro;
    }
  //Asignar ciudades
    int i_max;
    if (nb_cities() < ogros.size()) i_max = nb_cities();
    else i_max = ogros.size();
    for (int i = 0; i < i_max; i++) { // i -> id_ciudad
      map<int,int> proximidad_orks;
      for (int ork_id : orks(me())) {
        if (ogros[ork_id].ciudad_objetivo == -1) {
          int distancia;
          Pos pos;
          bool primer = true;
          Unit unidad = unit(ork_id);
          for (unsigned int j = 0; j < city(i).size(); j++) {  // j -> Pos_casilla
            if (primer) {
                primer = false;
                pos = city(i)[j];
                distancia = abs(unidad.pos.i - pos.i) + abs(unidad.pos.j - pos.j);
                ogros[ork_id].posicion_objetivo = pos;
            } else if (distancia > (abs(unidad.pos.i - pos.i) + abs(unidad.pos.j - pos.j))) {
                pos = city(i)[j];
                distancia = abs(unidad.pos.i - pos.i) + abs(unidad.pos.j - pos.j); //Cambiar asignacion
                ogros[ork_id].posicion_objetivo = pos;
            }
          }
          proximidad_orks[distancia] = ork_id;
        }
      }
      ogros[proximidad_orks.begin()->second].ciudad_objetivo = i;
    }
  
  //Asignar ogros restantes
    for (int ork_id : orks(me())) {
      if (ogros[ork_id].ciudad_objetivo == -1) {//Ogros que faltan por asignar
        map<int,pair<int,Pos> > proximidad_ciudades;
        Unit unidad = unit(ork_id);
        for (int i = 0; i < nb_cities(); i++) {//Todas las ciudades
          int distancia;
          Pos pos;
          bool primer = true;
          for (unsigned int j = 0; j < city(i).size(); j++) {//Todas las casillas de cada ciudad
            if (primer) {
              primer = false;
              pos = city(i)[j];
              distancia = abs(unidad.pos.i - pos.i) + abs(unidad.pos.j - pos.j);
            } else if (distancia > (abs(unidad.pos.i - pos.i) + abs(unidad.pos.j - pos.j))) {
              pos = city(i)[j];
              distancia = abs(unidad.pos.i - pos.i) + abs(unidad.pos.j - pos.j);
            }
          }
          proximidad_ciudades[distancia] = make_pair(i,pos); 
        }
        ogros[ork_id].ciudad_objetivo = proximidad_ciudades.begin()->second.first;
        ogros[ork_id].posicion_objetivo = proximidad_ciudades.begin()->second.second;
      }
    }


  //Resultados ##BORRAR##
    cerr << "#### DATOS ####\nNUM_CIUDADES->" << nb_cities() << " | NUM_OGROS->" << ogros.size() << endl;
    for (int ork_id : orks(me())) {
      cerr << "ork_id->" << ork_id << " | ciudad_obj->" << ogros[ork_id].ciudad_objetivo << " | pos_ciudad->[" << ogros[ork_id].posicion_objetivo.i << "," << ogros[ork_id].posicion_objetivo.j << "]" << endl;
    }

  }

  void andar_ciudad(int ork_id) { //Anda hacia ciudad mas proxima
    Dir direccion;
    Unit ogro = unit(ork_id);
  //Calcular direccion
    /*if ((ogros[ork_id].posicion_objetivo.i - ogro.pos.i) < 0) {direccion = BOTTOM;}
    else if ((ogros[ork_id].posicion_objetivo.i - ogro.pos.i) > 0) {direccion = BOTTOM;}
    else if ((ogros[ork_id].posicion_objetivo.j - ogro.pos.j) < 0) {direccion = BOTTOM;}
    else if ((ogros[ork_id].posicion_objetivo.j - ogro.pos.j) > 0) {direccion = BOTTOM;}*/

    direccion = BOTTOM;

  //Programar movimiento
    execute(Command(ork_id,direccion));
  }

  int en_apuros(int ork_id) {return 0;} //[-1] Puede matar, [0] No esta en apuros, [1] Esta en apuros
  void huir(int ork_id) {} //Hulle de ork con mas vida
  void matar(int ork_id) {} //Mata al ork con menos vida
  void capturar(int ork_id) {} //El ogro se pone en guardia dentro de una ciudad
  void actualizar_ogros() {
  //Añadir/Borrar ogros
  //Reprogramar nuevos ogros
  //Comprobar si han llegado a su destino
  }

  virtual void play () {
    if (round() == 0) inicializar();
    if (true) { //Primeras rondas (round() < FASEINICIAL)
      andar_ciudad(round());
    } 
    /*else {
      actualizar_ogros();
      for (int ork_id : orks(me())) {
        if (ogros[ork_id].en_ciudad_objetivo == false and 1 != en_apuros(ork_id)) {
          andar_ciudad(ork_id);
        }else if (ogros[ork_id].en_ciudad_objetivo == true) {
          capturar(ork_id);
        } else {
          huir(ork_id);
        }
      }
    }*/
  }

};

RegisterPlayer(PLAYER_NAME);
