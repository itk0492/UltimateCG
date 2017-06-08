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
    double* mid=NULL;           // Variable para alojar el punto medio del modelo
    char fileName[60];          // Variable para alojar el nombre de los frames
    double theta=.5;             // Variable para rotar el modelo
    // Inicilización de variables
    char* file=argv[1];     // Variables para alojar el nombre del archivo RAW
    int secs=strtol(argv[2], NULL, 10);     // Variable para alojar los segundos del video
    int frames=secs*24;     // Variable para alojar los frames que se producirán
    // Creación del raster de tamaño 1080x1920 (Full HD) con color en formato RGB
    raster=createRasterX(1080, 1920, 3);
    rasProt=createRasProtX(1080,1920);
    printf("\nModel processing started");
    vListCreator(&vList, file, &vListSize);
    //printf("\nvList: %d", vListSize);
    eListCreator(&eList, file, vListSize, vList, &eListSize);
    //printf("\neList: %d", eListSize);
    fListCreator(&fList, file, vListSize, &vList, eListSize, eList, &fListSize);
    //printf("\nfList: %d", fListSize);
    printf("\nModel processing successful\nImage rendering started");
    vListProjCreator(&vListProj, vListSize);
    //getMiddle(&mid, vList, vListSize);
    scaling(&vList, vListSize, 2);
    translation(&vList, vListSize, 110, -30, 0);
    /*
    xAxisRotation(&vList, vListSize, 48, mid[0], mid[1], mid[2]);
    recalcNormals(fList, fListSize, eList, &vList);
    */
    // Ciclo para generar cada frame
    for (int i = 0; i < frames; ++i) {
        getMiddle(&mid, vList, vListSize);
        xAxisRotation(&vList, vListSize, theta, mid[0], mid[1], mid[2]);
        recalcNormals(fList, fListSize, eList, &vList);
        snprintf(fileName, sizeof(fileName), "gen%i.ppm", i);
        vListProjection(vList, vListSize, &vListProj, 40, 0, 0);
        //printf("\nvListProj created and projected.");
        drawFace2Raster(rasProt, fList, fListSize, eList, vListProj, vList, raster);
        //printf("\nFaces draw.");
        printRaster2FileRGB(fileName, raster, 1080, 1920);
        printf("\nImage %i produced successfully", i+1);
        cleanRasProt(rasProt, 1080, 1920);
        cleanRasterRGB(raster, 1080, 1920);
        //printf("\n%lf %lf %lf %lf %lf", vList[0].x, vList[1].x, vList[2].x, vList[500].x, vList[vListSize-1].x);
    }
    return 0;
}