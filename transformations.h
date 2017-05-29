//
// Created by iroas on 22/05/2017.
//

#ifndef FINAL2_TRANSFORMATIONS_H
#define FINAL2_TRANSFORMATIONS_H

#include "objLists.h"

#define PI 3.14159265
#define radi PI/180

void xAxisRotation(vertexes** vList, int vListSize, double theta, double CoRx, double CoRy, double CoRz);
void yAxisRotation(vertexes** vList, int vListSize, double theta, double CoRx, double CoRy, double CoRz);
void zAxisRotation(vertexes** vList, int vListSize, double theta, double CoRx, double CoRy, double CoRz);

void scaling(vertexes** vList, int vListSize, double scale);

void translation(vertexes** vList, int vListSize, double tx, double ty, double tz);

#endif //FINAL2_TRANSFORMATIONS_H
