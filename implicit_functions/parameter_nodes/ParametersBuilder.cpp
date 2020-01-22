//
// Created by josipmrden on 20. 01. 2020..
//

#include "ParametersBuilder.h"
#include <utility>
#include <iostream>
#include <fstream>

ParametersBuilder::ParametersBuilder(string parametersFileName) {
    this->parametersFileName = parametersFileName;
    fillDefault();
}

void ParametersBuilder::setAlgorithm(string value) {
    this->algorithmNode.clear();
    this->algorithmName = std::move(value);
}

void ParametersBuilder::setGenotype(string value) {
    this->genotypeNode.clear();
    this->genotypeName = std::move(value);
}

string ParametersBuilder::getParameters() {
    return getXml();
}

void ParametersBuilder::writeParameters() {
    if (algorithmName.empty()) {
        cout << "Algorithm not set!";
        exit(1);
    }
    if (genotypeName.empty()) {
        cout << "Genotype not set!";
        exit(1);
    }

    deleteFile();
    std::string parameters = getParameters();
    ofstream parametersFile;
    parametersFile.open(this->parametersFileName);
    parametersFile << parameters << endl;
    parametersFile.close();
}

void ParametersBuilder::deleteFile() {
    remove(this->parametersFileName.c_str());
}

void ParametersBuilder::addAlgorithmEntry(string key, string value) {
    addEntry(algorithmNode, key, value);
}

void ParametersBuilder::addGenotypeEntry(string key, string value) {
    addEntry(genotypeNode, key, value);
}
void ParametersBuilder::addRegistryEntry(string key, string value) {
    addEntry(registryNode, key, value);
}

void ParametersBuilder::addEntry(map<string, string> &node, string key, string value) {
    if (node.count(key)) {
        node.erase(key);
    }

    node[key] = std::move(value);
}

void ParametersBuilder::fillDefault() {
    setAlgorithm("SteadyStateTournament");
    setGenotype("Tree");
    addRegistryEntry("randomizer.seed", "0");
    addRegistryEntry("log.filename", "log.txt");
    addRegistryEntry("log.level", "3");
    addRegistryEntry("milestone.filename", "m.txt");
}

//Retrieving XML
string ParametersBuilder::getBeginXml() {
    return R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
}
string ParametersBuilder::getBeginECFXml() {
    return "<ECF>";
}
string ParametersBuilder::getEndingECFXml() {
    return "</ECF>";
}

string ParametersBuilder::getXml() {
    string xml;

    xml += getBeginXml() + "\n";
    xml += getBeginECFXml() + "\n";
    xml += getAlgorithmParametersXml() + "\n";
    xml += getGenotypeParametersXml() + "\n";
    xml += getRegistryParametersXml() + "\n";
    xml += getEndingECFXml() + "\n";

    return xml;
}

string ParametersBuilder::getGenotypeParametersXml() {
    string begin = "\t<Genotype>";
    string end = "\t</Genotype>";
    string genotypeNameBegin = "\t\t<" + this->genotypeName + ">";
    string genotypeNameEnd = "\t\t</" + this->genotypeName + ">";
    string genotypeParameters;

    auto it = genotypeNode.begin();

    while (it != genotypeNode.end()) {
        string key = it->first;
        string value = it->second;
        string parameter = getIndentedParameter(key, value, 3);
        genotypeParameters += parameter + "\n";

        it++;
    }

    return begin + "\n" +
           genotypeNameBegin + "\n" +
           genotypeParameters +
           genotypeNameEnd + "\n" +
           end;
}

string ParametersBuilder::getAlgorithmParametersXml() {
    string begin = "\t<Algorithm>";
    string end = "\t</Algorithm>";
    string algorithmNameBegin = "\t\t<" + this->algorithmName + ">";
    string algorithmNameEnd = "\t\t</" + this->algorithmName + ">";
    string algorithmParameters;

    auto it = algorithmNode.begin();

    while (it != algorithmNode.end()) {
        string key = it->first;
        string value = it->second;
        string parameter = getIndentedParameter(key, value, 3);
        algorithmParameters += parameter + "\n";

        it++;
    }

    return begin + "\n"+
            algorithmNameBegin + "\n" +
            algorithmParameters +
            algorithmNameEnd + "\n" +
            end;
}

string ParametersBuilder::getRegistryParametersXml() {
    string begin = "\t<Registry>";
    string end = "\t</Registry>";
    string registryParameters;

    auto it = registryNode.begin();

    while (it != registryNode.end()) {
        string key = it->first;
        string value = it->second;
        string parameter = getIndentedParameter(key, value, 2);
        registryParameters += parameter + "\n";

        it++;
    }

    return begin + "\n" +
           registryParameters +
           end;
}

string ParametersBuilder::getIndentedParameter(string key, string value, int indentation) {
    string parameterString = getParameterXml(key, value);
    string indentedString;
    for (int i = 0; i < indentation; i++) {
        indentedString += '\t';
    }
    return indentedString + parameterString;
}

string ParametersBuilder::getParameterXml(string key, string value) {
    return "<Entry key=\"" + key + "\">" + value + "</Entry>";
}