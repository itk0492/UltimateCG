//Practica Final de Computer Graphics
//Proyección de objeto 3D

#include <stdio.h>
#include <stdlib.h>
#include "rasterizer.h"
#include "objLists.h"
#include "transformations.h"

int main(int argc, char** argv) {
    unsigned char*** raster;          // Variable para alojar el raster
    pixels** rasProt;   // Variable para alojar el raster prototipo
    vertexes* vList;    // Variable para alojar la lista de vértices
    edges* eList;       // Variable para alojar la lista de aristas
    faces* fList;       // Variable para alojar la lista de caras
    vertexesProj* vListProj; //Variable para alojar la lista de vértices proyectados
    int vListSize = 0;          // Variable para alojar el tamaño de la lista de vértices
    int eListSize = 0;          // Variable para alojar el tamaño de la lista de aristas
    int fListSize = 0;          // Variable para alojar el tamaño de la lista de caras
    double* mid=NULL;
    // Inicilización de variables
    char* file=argv[1];     // Variables para alojar el nombre del archivo RAW
    // Creación del raster de tamaño 1080x1920 (Full HD) con color en formato RGB
    raster=createRasterX(1080, 1920, 3);
    rasProt=createRasProtX(1080,1920);
    vListCreator(&vList, file, &vListSize);
    printf("\nvList: %d", vListSize);
    eListCreator(&eList, file, vListSize, vList, &eListSize);
    printf("\neList: %d", eListSize);
    fListCreator(&fList, file, vListSize, &vList, eListSize, eList, &fListSize);
    printf("\nfList: %d", fListSize);
    vListProjCreator(&vListProj, vListSize);
    getMiddle(&mid, vList, vListSize);
    //scaling(&vList, vListSize, 100);
    //translation(&vList, vListSize, -100, 0, 0);
    //zAxisRotation(&vList, vListSize, 180, mid[0], mid[1], mid[2]);
    vListProjection(vList, vListSize, &vListProj, 40, 0, 0);
    printf("\nvListProj created and projected.");
    drawFace2Raster(rasProt, fList, fListSize, eList, vListProj, vList, raster);
    printf("\nFaces draw.");
    printRaster2FileRGB("gen.ppm", raster, 1080, 1920);
    cleanRasProt(rasProt, 1080, 1920);
    cleanRasterRGB(raster, 1080, 1920);
    printf("\n%lf %lf %lf %lf %lf", vList[0].x, vList[1].x, vList[2].x, vList[500].x, vList[vListSize-1].x);
    return 0;
}