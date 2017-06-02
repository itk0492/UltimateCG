//
// Created by iroas on 17/05/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "objLists.h"

// Esta función se encarga de cargar los datos del archivo RAW a la lista de vértices
int vListCreator(vertexes** vList, char* fileName, int* size) {
    FILE * f;
    *vList = NULL;
    vertexes aux;
    if(!(f=fopen(fileName, "r"))){
        printf("Error while opening file.\n");
        return -1;
    }
    while(!feof(f)){
        fscanf(f, "%lf %lf %lf", &aux.x, &aux.y, &aux.z);
        addPoint(vList, aux, size);
    }
    //sortVList(*vList, *size);
    fclose(f);
    return 0;
}

// Esta función se encarga de cargar los datos de aristas en la lista de aristas
int eListCreator(edges** eList, char* fileName, int vListSize, vertexes* vList, int* size){
    FILE * f;
    *eList=NULL;
    vertexes aux[3];
    if(!(f=fopen(fileName, "r"))){
        printf("Error while opening file.\n");
        return -1;
    }
    while(!feof(f)){
        fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", &aux[0].x, &aux[0].y, &aux[0].z, &aux[1].x, &aux[1].y, &aux[1].z, &aux[2].x, &aux[2].y, &aux[2].z);
        addEdge(eList, aux[0], aux[1], vList, vListSize, size);
        addEdge(eList, aux[1], aux[2], vList, vListSize, size);
        addEdge(eList, aux[2], aux[0], vList, vListSize, size);
    }
    fclose(f);
    return 0;
}

// Esta función se encarga de cargar los datos de caras en la lista de caras
int fListCreator(faces** fList, char* fileName, int vListSize, vertexes** vList, int eListSize, edges* eList, int* size){
    FILE * f;
    *fList=NULL;
    vertexes auxV[3];
    if(!(f=fopen(fileName, "r"))){
        printf("Error while opening file.\n");
        return -1;
    }
    while(!feof(f)){
        fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", &auxV[0].x, &auxV[0].y, &auxV[0].z, &auxV[1].x, &auxV[1].y, &auxV[1].z, &auxV[2].x, &auxV[2].y, &auxV[2].z);
        addFace(fList, eList, eListSize, auxV, vList, vListSize, size);
    }
    fclose(f);
    return 0;
}

// Esta función se encarga de alojar memoria para la lista de vertices proyectada
int vListProjCreator(vertexesProj** vListProj, int vListSize){
    vertexesProj* aux=NULL;
    aux=(vertexesProj*)malloc(sizeof(vertexesProj)*vListSize);
    *vListProj=aux;
    return 0;
}

// Esta función se encarga de agregar puntos a la lista de vértices
int addPoint(vertexes** vList, vertexes point, int* size) {
    vertexes* vListAux;
    if((*size)==0){
        ++(*size);
        vListAux=realloc(*vList, ((*size)+1)* sizeof(vertexes));
        *vList=vListAux;
        vList[0][(*size)-1].x=point.x;
        vList[0][(*size)-1].y=point.y;
        vList[0][(*size)-1].z=point.z;
        return 2; //Retorna 2 que significa que se inicializó la lista
    } else if(searchPoint(*vList, point, (*size)) == -1){
        ++(*size);
        vListAux=realloc(*vList, ((*size)+1)* sizeof(vertexes));
        *vList=vListAux;
        vList[0][(*size)-1].x=point.x;
        vList[0][(*size)-1].y=point.y;
        vList[0][(*size)-1].z=point.z;
        vList[0][(*size)-1].normal[0]=0;
        vList[0][(*size)-1].normal[1]=0;
        vList[0][(*size)-1].normal[2]=0;
        return 1; //Retorna 1 que significa que el punto se agregó
    }
    return 0; //Retorna 0 que significa que el punto no fue agregado
}

// Esta función se encarga de agregar aristas a la lista de aristas
int addEdge(edges** eList, vertexes point1, vertexes point2, vertexes* vList, int vListSize, int* size){
    edges* eListAux;
    edges aux;
    aux.v1=searchPoint(vList, point1, vListSize);
    aux.v2=searchPoint(vList, point2, vListSize);
    if((*size)==0){
        ++(*size);
        eListAux=realloc(*eList, ((*size)+1)* sizeof(edges));
        *eList=eListAux;
        eList[0][(*size)-1].v1=aux.v1;
        eList[0][(*size)-1].v2=aux.v2;
        return 2; //Retorna 2 que significa que se inicializó la lista
    } else if (searchEdge(*eList, aux, (*size)) == -1) {
        ++(*size);
        eListAux=realloc(*eList, ((*size)+1)* sizeof(edges));
        *eList=eListAux;
        eList[0][(*size)-1].v1=aux.v1;
        eList[0][(*size)-1].v2=aux.v2;
        return 1; //Retorna 1 que significa que la arista se agregó
    }
    return 0; //Retorna 0 que significa que la arista no fue agregada
}

// Esta función se encarga de agregar las aristas correspondientes a la cara
int addFace(faces** fList, edges* eList, int eListSize, vertexes* auxV, vertexes** vList, int vListSize, int* size){
    faces* fListAux;
    edges aux[3];
    double normal[3], length;
    aux[0].v1=searchPoint(*vList, auxV[0], vListSize);
    aux[0].v2=searchPoint(*vList, auxV[1], vListSize);
    aux[1].v1=searchPoint(*vList, auxV[1], vListSize);
    aux[1].v2=searchPoint(*vList, auxV[2], vListSize);
    aux[2].v1=searchPoint(*vList, auxV[2], vListSize);
    aux[2].v2=searchPoint(*vList, auxV[0], vListSize);
    // Calculo de la normal de la cara (con normalización)
    normal[0]=(((auxV[1].y-auxV[0].y)*(auxV[2].z-auxV[1].z))-((auxV[1].z-auxV[0].z)*(auxV[2].y-auxV[1].y)));
    normal[1]=(((auxV[1].z-auxV[0].z)*(auxV[2].x-auxV[1].x))-((auxV[1].x-auxV[0].x)*(auxV[2].z-auxV[1].z)));
    normal[2]=(((auxV[1].x-auxV[0].x)*(auxV[2].y-auxV[1].y))-((auxV[1].y-auxV[0].y)*(auxV[2].x-auxV[1].x)));
    length=sqrt(pow(normal[0], 2)+pow(normal[1], 2) + pow(normal[2],2));
    normal[0]/=length;
    normal[1]/=length;
    normal[2]/=length;
    // Asignación de las normales a los vertices
    for (int i = 0; i < 3; ++i) {
        if(vList[0][aux[i].v1].normal[0]==0 && vList[0][aux[i].v1].normal[1]==0 && vList[0][aux[i].v1].normal[2]==0){
            vList[0][aux[i].v1].normal[0]=normal[0];
            vList[0][aux[i].v1].normal[1]=normal[1];
            vList[0][aux[i].v1].normal[2]=normal[2];
        } else{
            vList[0][aux[i].v1].normal[0]+=(vList[0][aux[i].v1].normal[0]-normal[0])/2;
            vList[0][aux[i].v1].normal[1]+=(vList[0][aux[i].v1].normal[1]-normal[1])/2;
            vList[0][aux[i].v1].normal[2]+=(vList[0][aux[i].v1].normal[2]-normal[2])/2;
        }
    }
    ++(*size);
    fListAux=realloc(*fList, ((*size)+1)* sizeof(faces));
    *fList=fListAux;
    fList[0][(*size)-1].e1=searchEdge(eList, aux[0], eListSize);
    fList[0][(*size)-1].e2=searchEdge(eList, aux[1], eListSize);
    fList[0][(*size)-1].e3=searchEdge(eList, aux[2], eListSize);
    return 0;
}

// Esta función se encarga de encontrar un punto en la lista de vértices
int searchPoint(vertexes* vList, vertexes point, int size) {

    for (int i = 0; i < size; ++i) {
        if (vList[i].x == point.x && vList[i].y == point.y && vList[i].z == point.z) {
            return i; //Retorna la posición del valor en la lista
        }
    }
    return -1; //Retorna -1 que significa que el punto no existe en la lista

    /*
    int first, middle, last, i, j;
    first=0;
    last=size-1;
    middle=(first-last)/2;
    while(first<=last){
        if(vList[middle].x==point.x){
            for (i = 0; middle+i<=last; ++i) {
                if(vList[middle+i].x==point.x){
                    if(vList[middle+i].x==point.x && vList[middle+i].y==point.y && vList[middle+i].zBuf && point.zBuf)
                        return middle+i;
                } else {
                    break;
                }
            }
            for (j = -1; middle+j>=first; --j) {
                if(vList[middle+j].x==point.x){
                    if(vList[middle+j].x==point.x && vList[middle+j].y==point.y && vList[middle+j].zBuf && point.zBuf)
                        return middle+j;
                } else {
                    break;
                }
            }
            return -1;
        } else if (vList[middle].x<point.x){
            first=middle+1;
        } else if (vList[middle].x>point.x){
            last=middle-1;
        }
        middle = (first + last)/2;
    }
    return -1;
    */
}

// Esta función se encarga de encontrar una arista en la lista de aristas
int searchEdge(edges* eList, edges e1, int size){
    for (int i = 0; i < size; ++i) {
        if (eList[i].v1==e1.v1 && eList[i].v2 == e1.v2) {
            return i; //Retorna la posición del valor en la lista
        }
    }
    return -1; //Retorna -1 que significa que la arista no existe en la lista
}

// Esta función se encarga de hacer la proyección de perspectiva
int vListProjection(vertexes* vList, int vListSize, vertexesProj** vListProj, double Ez, double Ex, double Ey){
    for (int i = 0; i < vListSize; ++i) {
        /*
        vListProj[0][i].x=((Ez*(vList[i].x-Ex))/(Ez+vList[i].z))+Ex;
        vListProj[0][i].y=((Ez*(vList[i].y-Ey))/(Ez+vList[i].z))+Ey;
        vListProj[0][i].zBuf=vList[i].z;
         */
        /*
        vListProj[0][i].x=(vList[i].x)/(1-(vList[i].z/Ez));
        vListProj[0][i].y=(vList[i].y)/(1-(vList[i].z/Ez));
        vListProj[0][i].zBuf=vList[i].z;
         */
        vListProj[0][i].x=(int)(vList[i].x*Ez)/10;
        vListProj[0][i].y=(int)(vList[i].y*Ez)/10;
        vListProj[0][i].zBuf=vList[i].z;
        vListProj[0][i].normal[0]=vList[i].normal[0];
        vListProj[0][i].normal[1]=vList[i].normal[1];
        vListProj[0][i].normal[2]=vList[i].normal[2];
    }
}

// Esta función se encarga de obtener el centro de la figura dados los puntos 'x', 'y' y 'zBuf'
void getMiddle(double** mid, vertexes* vList, int vListSize){
    double* aux;
    aux=realloc(*mid, 3* sizeof(double));
    *mid=aux;
    double minz, miny, maxz, maxy;
    miny=vList[0].y;
    maxy=vList[0].y;
    minz=vList[0].z;
    maxz=vList[0].z;
    for (int i = 0; i < vListSize; ++i) {
        if(maxy<vList[i].y)
            maxy=vList[i].y;
        if(miny>vList[i].y)
            miny=vList[i].y;
        if(maxz<vList[i].z)
            maxz=vList[i].z;
        if(minz>vList[i].z)
            minz=vList[i].z;
    }
    mid[0][0]=vList[0].x+((vList[vListSize-1].x-vList[0].x)/2);
    mid[0][1]=miny+((maxy-miny)/2);
    mid[0][2]=minz+((maxz-minz)/2);
    return;
}