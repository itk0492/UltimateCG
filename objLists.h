//
// Created by iroas on 17/05/2017.
//

#ifndef FINAL2_OBJLISTS_H
#define FINAL2_OBJLISTS_H

// Estructura para almacenar los vértices.
typedef struct vertex{
    double x;
    double y;
    double z;
    double normal[3];
}vertexes;

// Estructura para almacenar las aristas a través de vértices.
typedef struct edge{
    int v1;
    int v2;
}edges;

// Estructura para almacenar las caras a través de aristas.
typedef struct face{
    int e1;
    int e2;
    int e3;
}faces;

typedef struct vertexP{
    int x;
    int y;
    double zBuf, normal[3];
}vertexesProj;

int vListCreator(vertexes** vList, char* fileName, int* size);
int eListCreator(edges** eList, char* fileName, int vListSize, vertexes* vList, int* size);
int fListCreator(faces** fList, char* fileName, int vListSize, vertexes** vList, int eListSize, edges* eList, int* size);
int vListProjCreator(vertexesProj** vListProj, int vListSize);

int addPoint(vertexes** vList, vertexes point, int* size);
int addEdge(edges** eList, vertexes point1, vertexes point2, vertexes* vList, int vListSize, int* size);
int addFace(faces** fList, edges* eList, int eListSize, vertexes* auxV, vertexes** vList, int vListSize, int* size);

int searchPoint(vertexes* vList, vertexes point, int size);
int searchEdge(edges* eList, edges e1, int size);

void sortVList(vertexes* vList, int vListSize);
void mergeSort(vertexes *vList, int size1, int size2);

int vListProjection(vertexes* vList, int vListSize, vertexesProj** vListProj, double Ez, double Ex, double Ey);

void getMiddle(double** mid, vertexes* vList, int vListSize);

#endif //FINAL2_OBJLISTS_H
