//
// Created by josipmrden on 23. 04. 2020..
//

#include "AbstractEvaluateOp.h"
AbstractEvaluateOp::AbstractEvaluateOp(){}
AbstractEvaluateOp::AbstractEvaluateOp(string datasetFileName, ParetoFrontier* paretoFrontier)
{
    this->_datasetFileName = datasetFileName;
    this->_paretoFrontier = paretoFrontier;
}