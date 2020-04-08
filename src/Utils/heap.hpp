
/* 
 * ALgoritmia Básica: 2018/2019
 * Practica1: Codigo Huffman
 * Autores: Victor M. Lafuente, José Manuel Romero
 * Nias: 		747325, 			740914
 * 
 * Contenido: Clase Heap (Monticulo). Cola con prioridad de menor a mayor
 */

#pragma once
#include <iostream>
using namespace std;

const int MAX = 100000;

template <typename T>
class Heap{
	int tam = 0;
	T * elementos[MAX];

	public:
		Heap() : tam (0){}
		//getters
		bool isEmpty(){ return this->tam == 0; }
		int tamanyo() {return this->tam;}
		
		//Añade elemento 'e' en el vector, siguiendo el algoritmo del monticulo
		void add(T * e){
			this->elementos[this->tam] = e;
			for (int i = this->tam++; i > 0 && *this->elementos[i] < *this->elementos[(i-1)/2]; i = (i-1)/2){
				T * aux = this->elementos[i];
				this->elementos[i] = this->elementos[(i-1)/2];
				this->elementos[(i-1)/2] = aux;
			}
		}

		bool containsNode(const T * t){
			bool contains = false;
			for(int i = 0; i < tam && !contains; i++){
				contains = *t == *elementos[i];
			}
		}

		//Duelve el primer elemento del monticulo y lo elimina del mismo, siguiendo el algoritmo del monticulo
		T * pop(){
			T * ret = this->elementos[0];
			this->tam--;
			this->elementos[0] = this->elementos[this->tam];
			int hijo = 1;
			for (int i = 0; (i*2)+1 <= this->tam; i = hijo){
				hijo = (i*2)+1;
				if((hijo+1) <= this->tam && *this->elementos[hijo] > *this->elementos[hijo+1]){
					hijo++;
				}
				if( *this->elementos[hijo] >= *this->elementos[i]){
					break;
				}
				T * aux = this->elementos[hijo];
				this->elementos[hijo] = this->elementos[i];
				this->elementos[i] = aux;
			}
			return ret;
		}
	
};