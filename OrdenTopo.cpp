// OrdenTopo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#define RAIZ 1
using namespace std;

//Structs
struct Nodo {
	int ady;
	Nodo* sig;
};

struct NodoHeap {
	int ver;
	int nivel;
};

//Atributos
int cant;
Nodo** grafo;
int* gradoEntrada;
NodoHeap* colaP;
int tope;
int largo;

//Heap - Cola de prioridad
bool esMenorQue(int pos1, int pos2) {
	if (colaP[pos1].nivel < colaP[pos2].nivel) {
		return true;
	}
	else if (colaP[pos1].nivel > colaP[pos2].nivel) {
		return false;
	}
	else {
		if (colaP[pos1].ver < colaP[pos2].ver) {
			return true;
		}
		else {
			return false;
		}
		
	}
}

int posicionPadre(int pos)
{
	return pos / 2;
}

int posicionHijoIzq(int pos)
{
	return pos * 2;
}

int posicionHijoDer(int pos)
{
	return pos * 2 + 1;
}

int obtenerPosMinimoHijo(int pos)
{
	int posIzq = posicionHijoIzq(pos);
	int posDer = posicionHijoDer(pos);
	if (posDer > tope)
	{
		return posIzq;
	}
	else
	{
		// return heap[posIzq] > heap[posDer] ? posDer : posIzq;
		//colaP[posIzq].prioridad > colaP[posDer].prioridad
		if (esMenorQue(posDer, posIzq))
		{
			return posDer;
		}
		else
		{
			return posIzq;
		}
	}
}

bool esHoja(int pos)
{
	return posicionHijoIzq(pos) > tope;
}

void intercambiar(int pos1, int pos2)
{
	int verticeAux = colaP[pos1].ver;
	int nivelAux = colaP[pos1].nivel;
	colaP[pos1].ver = colaP[pos2].ver;
	colaP[pos1].nivel = colaP[pos2].nivel;
	colaP[pos2].ver = verticeAux;
	colaP[pos2].nivel = nivelAux;

}

void flotar(int pos)
{
	if (pos != RAIZ)
	{
		int posPadre = posicionPadre(pos);
		// colaP[pos].prioridad < colaP[posPadre].prioridad
		if (esMenorQue(pos, posPadre))
		{
			intercambiar(pos, posPadre);
			flotar(posPadre);
		}
	}
}

void hundir(int pos)
{
	if (!esHoja(pos))
	{
		int posMinimoHijo = obtenerPosMinimoHijo(pos);
		// colaP[posMinimoHijo].prioridad < colaP[pos].prioridad
		if (esMenorQue(posMinimoHijo, pos))
		{
			intercambiar(posMinimoHijo, pos);
			hundir(posMinimoHijo);
		}
	}
}

bool esVacia() {
	return tope == 0;
}

void insertarEnHeap(int vertice, int nivel)
{
	tope++;
	colaP[tope].ver = vertice;
	colaP[tope].nivel = nivel;
	flotar(tope);
}

void eliminarMinimo()
{
	colaP[RAIZ] = colaP[tope];
	tope--;
	hundir(RAIZ);
}

NodoHeap* Desencolar() {
	NodoHeap* ret = new NodoHeap();
	ret->ver = colaP[RAIZ].ver;
	ret->nivel = colaP[RAIZ].nivel;
	colaP[RAIZ] = colaP[tope];
	tope--;
	hundir(RAIZ);
	return ret;
}

int obtenerMinimo()
{
	return colaP[RAIZ].ver;
}


//int buscar(string e) {
//	int pos = 0;
//	for (int i = 1; i < largo; i++)
//	{
//		if (colaP[i].elemento == e) {
//			pos = i;
//		}
//	}
//	return pos;
//}
//
//void modificar(string e, int p) {
//	int pos = buscar(e);
//	if (pos != 0) {
//		colaP[pos].prioridad = p;
//		hundir(pos);
//		flotar(pos);
//	}
//}
//
//void eliminar(string e) {
//	int pos = buscar(e);
//	if (pos != 0) {
//		colaP[pos].elemento = colaP[tope].elemento;
//		colaP[pos].prioridad = colaP[tope].prioridad;
//		tope--;
//		hundir(pos);
//		flotar(pos);
//	}
//}

int* datosGrafo(string linea) {
	int* ret = new int[3];
	int posV = linea.find(' ');
	string verAux = linea.substr(0, posV);
	int vertice = stoi(verAux);
	int posA = linea.find(' ', posV + 1);
	string adyAux = linea.substr(posV + 1, posA - posV - 1);
	int adyacente = stoi(adyAux);
	string costAux = linea.substr(posA + 1, linea.length());
	int costo = stoi(costAux);
	ret[0] = vertice;
	ret[1] = adyacente;
	ret[2] = costo;
	return ret;
}

void agregarEnGrafo(int* datos) {
	int vertice = datos[0];
	int adyacente = datos[1];
	Nodo* nodo = new Nodo();
	nodo->ady = adyacente;
	nodo->sig = grafo[vertice];
	grafo[vertice] = nodo;
}




int main()
{
	int cantVertice;
	cin >> cantVertice;
	grafo = new Nodo* [cantVertice + 1];
	gradoEntrada = new int[cantVertice + 1];
	colaP = new NodoHeap [cantVertice + 1];
	for (int i = 0; i < cantVertice + 1; i++)
	{
		gradoEntrada[i] = 0;
		grafo[i] = NULL;
		colaP[i].nivel = 0;
	}
	int cantAristas;
	cin >> cantAristas;
	string linea;
	getline(cin, linea);
	for (int i = 1; i <= cantAristas; i++)
	{
		string linea;
		getline(cin, linea);
		int* datos = datosGrafo(linea);
		agregarEnGrafo(datos);
		gradoEntrada[datos[1]]++;

	}
	for (int i = 1; i <= cantVertice + 1; i++)
	{
		if (gradoEntrada[i] == 0) {
			insertarEnHeap(i, 0);
		}
	}
	while (!esVacia()) {
		NodoHeap* nodo = Desencolar();
		cout << nodo->ver <<  endl;
		Nodo * aux = grafo[nodo->ver];
		while (aux != NULL) {
			gradoEntrada[aux->ady]--;
			if (gradoEntrada[aux->ady] == 0) {
				insertarEnHeap(aux->ady, nodo->nivel + 1);
			}
			aux = aux->sig;
		}
		
	}


	return 0;
}
