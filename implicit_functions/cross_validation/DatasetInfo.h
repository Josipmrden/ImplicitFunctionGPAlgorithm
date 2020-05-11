//
// Created by josipmrden on 28. 04. 2020..
//

#ifndef SYMBREGEXERCISE_DATASETINFO_H
#define SYMBREGEXERCISE_DATASETINFO_H

using namespace std;

#include <string>

class DatasetInfo{
public:
    string fileName;
    string name;
    string terminalset;
    string functionset;
    DatasetInfo();
    DatasetInfo(string name, string fileName, string terminalset, string functionset);
};


#endif //SYMBREGEXERCISE_DATASETINFO_H
