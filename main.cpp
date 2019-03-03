#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void openFile(string data[][9], string namaFile){ //untuk membaca file dan merubah ke array

    ifstream DataTugas;
    string id;
    string age, workclass, education, maritalStatus, occupation, relationship, hoursPerWeek, income;
    string Header;

    DataTugas.open(namaFile.c_str());// Ganti nama filenya disini..

    if(!DataTugas.is_open())
        cout<<"ERROR : File Open" <<"\n";

    int i = 0;
    while(DataTugas.good()){
       if(i > 0){
        getline(DataTugas, id, ',');
        getline(DataTugas, age, ',');
        getline(DataTugas, workclass, ',');
        getline(DataTugas, education, ',');
        getline(DataTugas, maritalStatus, ',');
        getline(DataTugas, occupation, ',');
        getline(DataTugas, relationship, ',');
        getline(DataTugas, hoursPerWeek, ',');
        getline(DataTugas, income, '\n');
        if(id != ""){
            data[i][0] = id.c_str();
            data[i][1] = age.c_str();
            data[i][2] = workclass.c_str();
            data[i][3] = education.c_str();
            data[i][4] = maritalStatus.c_str();
            data[i][5] = occupation.c_str();
            data[i][6] = relationship.c_str();
            data[i][7] = hoursPerWeek.c_str();
            data[i][8] = income.c_str();
        }else{
            return;
        }
       }else{
        getline(DataTugas, Header); //Buang header
       }
       i++;
    }
    DataTugas.close();
}

void output_file(string data[][9], int total, string namaFile){ // Output dari array ke Excel
    ofstream hasil;
    hasil.open(namaFile.c_str(), ios::app);
    int i = 1;
    hasil <<"id"<<','<<"age"<<','<<"workclass"<<','<<"education"<<','<<"marital-status"<<','<<"occupation"<<','<<"relationship"<<','<<"hoursPerWeek"<<','<<"income"<<endl;
    while(i <= total){
        hasil << data[i][0]<<','<< data[i][1]<<','<< data[i][2]<<','<< data[i][3]<<','<< data[i][4]<<','<<data[i][5]<<','<<data[i][6]<<','<<data[i][7]<<','<<data[i][8]<<endl;
        i++;
    }
}

void probabilitasHasil(string data[][9], int totalData, float HasilProbabilitas[2]){ //fungsi ini untuk mengetahui banyaknya <=50K dan >50K
    int i = 0;
    cout<<totalData<<endl;
    while(i < totalData){
        if(data[i][8] == ">50K"){
            HasilProbabilitas[0] += 1;
        }else{
            HasilProbabilitas[1] += 1;
        }
        i++;
    }

    HasilProbabilitas[0] = HasilProbabilitas[0]/totalData;
    HasilProbabilitas[1] = HasilProbabilitas[1]/totalData;

    cout<< totalData <<endl;


}

string hitung_probabilitas(double HasilTrue[], double HasilFalse[], int jumlahTrue, int jumlahFalse, int posisiData){
    int i = 0;
    double FloatTrue, FloatFalse; // penampungan hasil dari perhitungan probabilitas semua data yang akan dites
    float persenTrue = float(jumlahTrue) / (float(jumlahTrue + jumlahFalse)); // untuk mendapatkan jumlah >50K dalam bentuk %
    float persenFalse = float(jumlahFalse) / (float(jumlahTrue + jumlahFalse)); // untuk mendapatkan jumlah <=50K dalam bentuk %

    cout<<"------------------------------------------------------------------"<<endl;
    cout<<" Ini adalah Data Ke - "<<posisiData<<endl;

    while(i < 7){ //looping ini menjelaskan untuk merubahan data probabilitas pecahan menjadi float
        if(i == 0){
            FloatTrue = HasilTrue[i] / float(jumlahTrue);
            FloatFalse = HasilFalse[i] / float(jumlahFalse);
        }else{
            FloatTrue = FloatTrue * (HasilTrue[i] / jumlahTrue);
            FloatFalse = FloatFalse * (HasilFalse[i] / jumlahFalse);

        }
        cout<<"ini adalah hasil data yang >50K : "<<HasilTrue[i]<<" yang akan dibagi dengan jumlah >50K : "<<jumlahTrue<<endl;
        cout<<"ini adalah hasil data yang <=50K : "<<HasilFalse[i]<<" yang akan dibagi dengan jumlah <=50K : "<<jumlahFalse<<endl;
        cout<<endl;
        i++;

    }

    FloatTrue *= persenTrue; // Hasil dari perhitungan probabilitas yang dijadikan float dikalikan dengan probabilitas >50K atau 0.75
    FloatFalse *= persenFalse; // Hasil dari perhitungan probabilitas yang dijadikan float dikalikan dengan probabilitas <=50K

    cout<<"Hasil >50K : "<<FloatTrue<<" VS  Hasil <=50K : "<<FloatFalse<<endl;

    if(FloatTrue > FloatFalse){ // jika data yang >50K lebih besar, maka hasil dari tebakan yaitu >50K
        cout<<"Hasil akhir : "<<FloatTrue<<endl;
         cout<<endl<<endl;
        return ">50K";
    }else{ // data hasil jadi <=50K
        cout<<"Hasil akhir : "<<FloatFalse<<endl;
         cout<<endl<<endl;
        return "<=50K";
    }
}

void perhitunganTahap2(string dataTrain[][9], string dataTest[][9], int totalDataTrain, int totalDataTest, int jumlahTrue, int jumlahFalse){
    int x = 1   ;
    while(x < totalDataTest+1){
        int y = 1;
        double HasilTrue[7] = {0}; // untuk menyimpan hasil sementara probabulitas >50K
        double HasilFalse[7] = {0};// untuk menyimpan hasil sementara probabilitas <=50L
        while(y < totalDataTrain){
            int i = 1;
            while(i < 8){ // looping untuk setiap data probabilitas meliputi age, workclass dll
                if(dataTest[x][i] == dataTrain[y][i]){ // untuk menghitung data yang sama
                    if(dataTrain[y][8] == ">50K"){ // untuk menghitung data yang incomenya >50K
                        HasilTrue[i-1] += 1;
                    }else{ // untuk menghitung data yang incomenya <=50K
                        HasilFalse[i-1] += 1;
                    }
                }
                //Jika data tidak ditemukan, hanya dilewat saja
                i++;
            }
            y++;
        }
        string income = hitung_probabilitas(HasilTrue, HasilFalse, jumlahTrue, jumlahFalse, x); // menghitung hasil income
        dataTest[x][8] = income; // hasil yang didapat ditaruh pada data test
        x++;
    }
}

int main()
{
    // ------------------- Deklarasi Awal ----------------------------------//
    string dataTrain[161][9]; // Deklarasi jumlah data trainnya
    string dataTest[41][9]; // Deklarasi jumlah data test nya

    int totalDataTrain = (sizeof dataTrain/sizeof dataTrain[0]) - 1;
    int totalDataTest = (sizeof dataTest/sizeof dataTest[0]) - 1;
    float HasilProbabilitas[2] = {0}; // Array probabilitas yang berisi hasil ya atau tidak nya
    float dataHasil[801][2]; // Deklarasi jumlah data hasil test nya

    string namaFileTrain = "TrainsetTugas1ML.csv"; //  Nama File Train yang akan digunakan
    string namaFileTest = "TestsetTugas1ML.csv"; //  Nama File Test yang akan digunakan
    openFile(dataTrain, namaFileTrain);
    openFile(dataTest, namaFileTest);

    // -------------------------- Mulai Programnya ------------------------------ //

    probabilitasHasil(dataTrain, totalDataTrain, HasilProbabilitas); // untuk mendapatkan probabilitas antara >50K dan >=50K

    int jumlahTrue = sizeof dataTrain/sizeof dataTrain[0] * HasilProbabilitas[0]; // untuk mengatahui jumlah >50K
    int jumlahFalse = sizeof dataTrain/sizeof dataTrain[0] * HasilProbabilitas[1]; // untuk mengetahui jumlah <=50K

    perhitunganTahap2(dataTrain, dataTest, totalDataTrain, totalDataTest, jumlahTrue, jumlahFalse); //bagian perhitungan inti

    cout<<"------- Informasi --------------"<<endl;
    cout<<"Persenan >50K : "<<HasilProbabilitas[0]<<"   Persenan <=50K: "<<HasilProbabilitas[1]<<endl;
    cout<<"Jumlah >50K : "<<jumlahTrue<<"   Jumlah <=50K : "<<jumlahFalse<<endl;
    cout<<"Total Data Train : "<<totalDataTrain<<"   Total Data Test : "<<totalDataTest<<endl;
    cout<<endl;
    cout<<"Hasil filenya berupa csv yang bernama HasilTugas.csv"<<endl;
    cout<<"Note : jika ingin compile ulang, hapus dulu excel hasilnya, agar data tidak memanjang kebawah"<<endl;
    output_file(dataTest, totalDataTest, "HasilTugas.csv");

    return 0;
}
