#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include"Layer.h"

//#include <cmath>
using namespace std;
int main(){
	ifstream in;
	in.open("Averaging_Model.txt");
	double V_Cont = 0;
	double Total_V = 0;
	double ConstructionCoef = 0;
	int SizeLayers = 0; // Количество слоёв
	double MassPuTotal = 0;
	double TotalMass = 0;
	double C_Pu_average = 0;
	double FillCoef = 0;
	double Dispersion_O = 0;
	double Dispersion_p = 0;
	double dencity = 0;// как я понял, плотность считаем одинаковой у обоих компонентов
	double F = 0;// Константа скорости смешения
	int n_Optimal = 50;// Частота вращ. оптимальная
	int n_Extreme = 70;// Частота вращ. крайняя
	int n = 0;
	int AveragingTime = 2000;
	string s;
	getline(in,s);
	in>>V_Cont>>n>>ConstructionCoef>>SizeLayers;
	getline(in,s);
	vector<Layer> layers(SizeLayers);
	for(Layer& layer: layers){
		in>>layer.m_Pu>>layer.Mass>>layer.p>>layer.V>>layer.C_Pu;
		MassPuTotal += layer.m_Pu;
		TotalMass += layer.Mass;
		Total_V += layer.V;
	}

	C_Pu_average = MassPuTotal/Total_V;

	FillCoef = Total_V/V_Cont;

	// Рассчёт константы скорости
	////////////////////////////////////////////////////////////////////////////////////////////////////
	if(n > 0 && n <= n_Optimal)
		F = ConstructionCoef * (1 - FillCoef) * ( (2 * n / n_Optimal) - n*n / (n_Optimal * n_Optimal));
	else if(n > n_Optimal && n <= n_Extreme)
		F = ConstructionCoef * (1 - FillCoef) * 
		(1 + (2 * n * n_Optimal - n * n - pow(n_Optimal, 2)) / (pow(n_Extreme - n_Optimal, 2)));
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	dencity = TotalMass/Total_V;

	// Рассчёт дисперсий
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	for(Layer& layer: layers){
		Dispersion_p += pow(layer.C_Pu - C_Pu_average,2)*layer.V;
	}
	Dispersion_p /= Total_V;

	Dispersion_O = C_Pu_average*(dencity - C_Pu_average);
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ofstream out;
	out.open("Z(t).csv");
	out<<"t;Z(t)"<<"\n";
	double Z = 0;
	for (size_t t = 0; t < AveragingTime; t++){
		Z = 1 - exp(-F*t);
		out<<t<<";"<<Z<<"\n";
	}
	out.close();

	return 0;
}


