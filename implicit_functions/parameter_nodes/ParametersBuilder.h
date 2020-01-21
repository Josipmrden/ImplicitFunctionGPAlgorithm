//
// Created by josipmrden on 20. 01. 2020..
//

#ifndef SYMBREGEXERCISE_PARAMETERSBUILDER_H
#define SYMBREGEXERCISE_PARAMETERSBUILDER_H


#include <string>
#include <map>
#include <iostream>
using namespace std;

class ParametersBuilder {
public:
    ParametersBuilder(string parametersFileName);
    string getParameters();
    void writeParameters();
    void deleteFile();
    void addAlgorithmEntry(string key, string value);
    void addGenotypeEntry(string key, string value);
    void addRegistryEntry(string key, string value);
    void setAlgorithm(string value);
    void setGenotype(string value);
    void fillDefault();
private:
    string parametersFileName;
    string genotypeName;
    string algorithmName;
    map<string, string> algorithmNode;
    map<string, string> genotypeNode;
    map<string, string> registryNode;
    void addEntry(map<string, string> &node, string key, string value);
    static string getBeginXml();
    static string getBeginECFXml();
    static string getEndingECFXml();
    string getAlgorithmParametersXml();
    string getGenotypeParametersXml();
    string getRegistryParametersXml();
    string getXml();
    string getIndentedParameter(string key, string value, int indentation);
    static string getParameterXml(string key, string value);
};


#endif //SYMBREGEXERCISE_PARAMETERSBUILDER_H
