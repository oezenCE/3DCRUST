//=================================================================================
//
//  Project     :  3DCRUST (Software Lab 2021/22)
//  Software    :  Sofistik 2020
//  Outputs     : .vtu(structure), .vtp(trajectories)
//  
//
//  Purpose: 
//                       1)Reading data from CDB file to write into VTU file.
//                       2)Generating trajectories of traced structure. 
//
//                       
//  Developers  : Emin Can Oezen; Juan Carlos Alvarado Morones; Rafeek Serhal
//  Supervisors : PD Dr.-Ing. habil. Stefan Kollmannsberger (TUM)
//                Dr.-Ing. Johannes Kreutz (Khoch3 GmbH)
//=================================================================================
#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>
#include<map>;
#include "vtu11/vtu11.hpp"
#include "cdbase.h"
#include "cdbtypeall.h"
#include "cdbtypecon.h"
#include "cdbtypegeo.h"
#include "cdbtypelfc.h"
#include "cdbtypemat.h"
#include "cdbtypesct.h"
#include "cdbtypesys.h"
#include "cdbtypeten.h"



using namespace std;

typedef unsigned int pckcode;
typedef void(*PS2CS)(pckcode*, char*, int);

typedef int(*INITPRT)(char*, int);
typedef int(*STATUSPRT)(int);
typedef int(*CLOSEPRT)(int);
typedef int(*GETPRT)(int, int, int, void*, int*, int);

using namespace std;



void print1(int x)
{

    std::cout << x;
}


void vtuwriter(char* fileName, int loadcase, char* cdb_dll, const std::string& vtu11writer_path)
{
    //********check library status 
    // Load the library
    HINSTANCE hInst = LoadLibrary(cdb_dll);
    if (!hInst)
    {
        cout << "Coult not Load the Library" << endl;
        system("pause");
    }

    // Resolve the function adress
    INITPRT sof_cdb_init = (INITPRT)GetProcAddress(hInst, "sof_cdb_init");
    if (!sof_cdb_init)
    {
        cout << "Could not locate the function!" << endl;
        system("pause");
    }

    STATUSPRT sof_cdb_status = (STATUSPRT)GetProcAddress(hInst, "sof_cdb_status");
    if (!sof_cdb_status)
    {
        cout << "Could not locate the function!" << endl;
        system("pause");
    }

    PS2CS sof_lib_ps2cs = (PS2CS)GetProcAddress(hInst, "sof_lib_ps2cs");
    if (!sof_lib_ps2cs)
    {
        cout << "Could not locate the function!" << endl;
        system("pause");
    }

    CLOSEPRT sof_cdb_close = (CLOSEPRT)GetProcAddress(hInst, "sof_cdb_close");
    if (!sof_cdb_close)
    {
        cout << "Could not locate the function!" << endl;
        system("pause");
    }

    GETPRT sof_cdb_get = (GETPRT)GetProcAddress(hInst, "sof_cdb_get");
    if (!sof_cdb_get)
    {
        cout << "Could not locate the function!" << endl;
    }

    
   
    //************** check CDB status

    int index = 99;
    int ie = sof_cdb_init(fileName, index);
    if (ie < 0)
    {
        cout << "ERROR: Index= " << ie << " < 0 - see clib1.h for meaning of error codes" << endl;
        cout << "Press any <key> to close the program";
        system("pause");
    }
    else if (ie == 0)
    {
        cout << "ERROR: Index= " << ie << " - The File is not a database" << endl;
        cout << "Press any <key> to close the program";
        system("pause");
    }



    int datalengrpdivisor = sizeof(typeCDB_SYST);    /// m_igdiv; 
    int* datalenptrgrpdivisor;
    datalenptrgrpdivisor = &datalengrpdivisor;
    typeCDB_SYST grpdivisor;

    int datalen = sizeof(tagCDB_NODE);
    int* datalenptr;
    datalenptr = &datalen;
    tagCDB_NODE cnode;

    int datalenBRIC = sizeof(tagCDB_BRIC);
    int* datalenptrBRIC;
    datalenptrBRIC = &datalenBRIC;
    tagCDB_BRIC cbric;

    int datalenforbricstress = sizeof(tagCDB_BRIC_NST);
    int* datalenptrstress;
    datalenptrstress = &datalenforbricstress;
    tagCDB_BRIC_NST nodestressesofbric;


    int datalenBRICdisp = sizeof(tagCDB_BRIC);
    int* datalenptrBRICdisp;
    datalenptrBRICdisp = &datalenBRICdisp;
    tagCDB_N_DISP cbricdisp;
    


    /*Create data for 3x2 quad mesh: (x, y, z) coordinates of mesh vertices*/
    std::vector<double> points;  
    std::map<pair<int, int>, double> mapfornodes;
    std::map<tuple<double, double, int>, double> mapforstresseswithgroupnumberin311;
    std::map<tuple<double, double, int>, double> mapforstresseswithelementnumberin300;


    //////// Separate cells in connectivity array
    std::vector<vtu11::VtkIndexType> offsets;

    ////// Cell types of each cell
    std::vector<vtu11::VtkCellType> types;

    ////// Vertex indices of all cells
    std::vector<vtu11::VtkIndexType> connectivity;

    // Create some data associated to pointsand cells
    std::vector<double> cellData;
    std::vector<double> pointData;


    // DICTIONARY FOR THE AND XYZ COORDINATE
    while (sof_cdb_get(ie, 20, 00, &cnode, &datalen, 1) < 2)
    {
        mapfornodes[make_pair(cnode.m_nr, 0)] = cnode.m_xyz[0];
        mapfornodes[make_pair(cnode.m_nr, 1)] = cnode.m_xyz[1];
        mapfornodes[make_pair(cnode.m_nr, 2)] = cnode.m_xyz[2];
    }


    int k = 0;
    while (sof_cdb_get(ie, 311, loadcase , &nodestressesofbric, &datalenforbricstress, 1) < 2)
    {
        if (k >= 2)
        {
            mapforstresseswithgroupnumberin311[make_tuple(nodestressesofbric.m_ng, nodestressesofbric.m_nr, 0)] = nodestressesofbric.m_sigx; // (group, nodenumberinjumping, stressxx)=value;
            mapforstresseswithgroupnumberin311[make_tuple(nodestressesofbric.m_ng, nodestressesofbric.m_nr, 1)] = nodestressesofbric.m_sigy;
            mapforstresseswithgroupnumberin311[make_tuple(nodestressesofbric.m_ng, nodestressesofbric.m_nr, 2)] = nodestressesofbric.m_sigz;
            mapforstresseswithgroupnumberin311[make_tuple(nodestressesofbric.m_ng, nodestressesofbric.m_nr, 3)] = nodestressesofbric.m_tvxy;
            mapforstresseswithgroupnumberin311[make_tuple(nodestressesofbric.m_ng, nodestressesofbric.m_nr, 4)] = nodestressesofbric.m_tvxz;
            mapforstresseswithgroupnumberin311[make_tuple(nodestressesofbric.m_ng, nodestressesofbric.m_nr, 5)] = nodestressesofbric.m_tvyz;

        }
        k++;
    }

    
    int divisor = 0;

    while (sof_cdb_get(ie, 10, 00, &grpdivisor, &datalengrpdivisor, 1) < 2)
    {
        divisor = grpdivisor.m_igdiv;

    }



    vtu11::VtkIndexType i = 1;

    int c = 0;


    while (sof_cdb_get(ie, 300, 00, &cbric, &datalenBRIC, 1) < 2)
    {
        if (cbric.m_node[3] == 0 && cbric.m_node[4] == cbric.m_node[5] && cbric.m_node[5] == cbric.m_node[6] && cbric.m_node[6] == cbric.m_node[7])
        {
            for (int j = 0; j < 5; j++)
            {
                if (j != 3)
                {

                    //for node
                    points.push_back(mapfornodes[make_pair(cbric.m_node[j], 0)]);
                    points.push_back(mapfornodes[make_pair(cbric.m_node[j], 1)]);
                    points.push_back(mapfornodes[make_pair(cbric.m_node[j], 2)]);


                    //for stresses
                    int grpnumber = cbric.m_nr / divisor;


                    pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 0)]); //(group, nodenumberinjumping, stressxx)
                    pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 1)]);
                    pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 2)]);
                    pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 3)]);
                    pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 4)]);
                    pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 5)]);

                    //for connectivity 
                    connectivity.push_back(c);
                    c++;
                }
            }


            offsets.push_back(4 * i++);
            types.push_back(10);
            cellData.push_back(1);
        }
        else
        {

            for (int j = 0; j < 8; j++)
            {
                //for node
                points.push_back(mapfornodes[make_pair(cbric.m_node[j], 0)]);
                points.push_back(mapfornodes[make_pair(cbric.m_node[j], 1)]);
                points.push_back(mapfornodes[make_pair(cbric.m_node[j], 2)]);



                int grpnumber;
				
                //for stresses
                if (divisor == 0)
                {
                    grpnumber = 0;
                }
                else
                {
                    grpnumber = cbric.m_nr / divisor;
                }
                pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 0)]); //(group, nodenumberinjumping, stressxx)
                pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 1)]);
                pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 2)]);
                pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 3)]);
                pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 4)]);
                pointData.push_back(mapforstresseswithgroupnumberin311[make_tuple(grpnumber, cbric.m_node[j], 5)]);

                //for connectivity 
                connectivity.push_back(c);
                c++;

            }


            offsets.push_back(8 * i++);
            types.push_back(12);
            cellData.push_back(1);
        }


    }
    
    // Creating small proxy mesh type
    vtu11::Vtu11UnstructuredMesh mesh{ points, connectivity, offsets, types };
    std::vector<vtu11::DataSetInfo> dataSetInfo
    {
        { "Stress", vtu11::DataSetType::PointData, 6},
        { "Domain index", vtu11::DataSetType::CellData, 1 },
    };

    
    vtu11::writeVtu(vtu11writer_path, mesh, dataSetInfo, { pointData, cellData }, "Ascii");
    // Close the CDB
    sof_cdb_close(0);

    
 
}
