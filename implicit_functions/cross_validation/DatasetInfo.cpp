//
// Created by josipmrden on 28. 04. 2020..
//

#include "DatasetInfo.h"
DatasetInfo::DatasetInfo()
{

}
DatasetInfo::DatasetInfo(string name, string fileName, string terminalset, string functionset)
{
    this->fileName = fileName;
    this->name = name;
    this->terminalset = terminalset;
    this->functionset = functionset;
}