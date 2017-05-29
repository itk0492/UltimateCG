//
// Created by iroas on 22/05/2017.
//

#include <math.h>
#include "transformations.h"

// Esta función se encarga de rotar la figura con respecto a x
void xAxisRotation(vertexes** vList, int vListSize, double theta, double CoRx, double CoRy, double CoRz){
    translation(vList, vListSize, -CoRx, -CoRy, -CoRz);
    for (int i = 0; i < vListSize; ++i) {
        vList[0][i].y=(vList[0][i].y*(cos(theta*(radi)))+vList[0][i].z*(sin(theta*(radi))));
        vList[0][i].z=(vList[0][i].z*(cos(theta*(radi)))+vList[0][i].y*(-sin(theta*radi)));
    }
    translation(vList, vListSize, CoRx, CoRy, CoRz);
    return;
}

// Esta función se encarga de rotar la figura con respecto a y
void yAxisRotation(vertexes** vList, int vListSize, double theta, double CoRx, double CoRy, double CoRz){
    translation(vList, vListSize, -CoRx, -CoRy, -CoRz);
    for (int i = 0; i < vListSize; ++i) {
        vList[0][i].z=(vList[0][i].z*(cos(theta*(radi)))+vList[0][i].x*(sin(theta*(radi))));
        vList[0][i].x=(vList[0][i].x*(cos(theta*(radi)))+vList[0][i].z*(-sin(theta*(radi))));
    }
    translation(vList, vListSize, CoRx, CoRy, CoRz);
    return;
}

// Esta función se encarga de rotar la figura con respecto a zBuf
void zAxisRotation(vertexes** vList, int vListSize, double theta, double CoRx, double CoRy, double CoRz){
    translation(vList, vListSize, -CoRx, -CoRy, -CoRz);
    for (int i = 0; i < vListSize; ++i) {
        vList[0][i].x=(vList[0][i].x*(cos(theta*(radi)))+vList[0][i].y*(sin(theta*(radi))));
        vList[0][i].y=(vList[0][i].y*(cos(theta*(radi)))+vList[0][i].x*(-sin(theta*(radi))));
    }
    translation(vList, vListSize, CoRx, CoRy, CoRz);
    return;
}

// Esta función se encarga de escalar la figura
void scaling(vertexes** vList, int vListSize, double scale){
    for (int i = 0; i < vListSize; ++i) {
        (vList[0][i].x)*=scale;
        (vList[0][i].y)*=scale;
        (vList[0][i].z)*=scale;
    }
    return;
}

// Esta función se encarga de trasladar la figura
void translation(vertexes** vList, int vListSize, double tx, double ty, double tz){
    for (int i = 0; i < vListSize; ++i) {
        vList[0][i].x+=tx;
        vList[0][i].y+=ty;
        vList[0][i].z+=tz;
    }
    return;;
}
