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
#include "extensions/vtkextension/inc/VtuField.hpp"
#include "inc/gotraceit_typedefs.hpp"
#include "inc/TensorField.hpp"
#include "inc/Trace.hpp"
#include "inc/PrincipalValueComputer3D.hpp"
#include "inc/VtpFileWriter.hpp"
#include "inc/AdaptiveRungeKuttaFourth.hpp"
#include"headers.hpp"
#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>
#include <map>
#include "cdbase.h"
#include "cdbtypeall.h"
#include "cdbtypecon.h"
#include "cdbtypegeo.h"
#include "cdbtypelfc.h"
#include "cdbtypemat.h"
#include "cdbtypesct.h"
#include "cdbtypesys.h"
#include "cdbtypeten.h"
#include "vtu11/vtu11.hpp"




using namespace gotraceit;

auto principalStress = [](const ThreeDimensionalFieldPolicy::PointType& point, const ThreeDimensionalFieldPolicy::PointType& direction, const ThreeDimensionalFieldPolicy::EigenFrameType& frame)
{
    std::array<double, 3> dotProducts;
    auto eigenValues = std::get<0>(frame);
    auto eigenVectors = std::get<1>(frame);

    std::transform(eigenVectors.begin(), eigenVectors.end(), dotProducts.begin(), [&](const ThreeDimensionalFieldPolicy::PointType& vector)
        {
            return std::fabs(gotraceit::dot(direction, vector));
        });

    auto index = std::distance(dotProducts.begin(), std::max_element(dotProducts.begin(), dotProducts.end()));
    return std::array<double, 1>{eigenValues[index]};
};

int main(void)
{



    print1(2);

    vtuwriter("D:\\SL\\newpro\\Volume model Bridge.cdb",
        10,
        "C:\\Program Files\\SOFiSTiK\\2020\\SOFiSTiK 2020\\interfaces\\64bit\\sof_cdb_w_edu-70.dll",
        "D:\\SL\\newpro\\3D_visuilization_with_stresses.vtu");


    VtuField<ThreeDimensionalFieldPolicy> vtuField("D:\\SL\\newpro\\3D_visuilization_with_stresses.vtu", "Stress");

    TensorField<ThreeDimensionalFieldPolicy> field(vtuField.getValueFunctor(), vtuField.getIndicatorFunctor());



    double xmin = 3;
    double ymin = 6.7;
    double zmin = 0.07;
    double xmax = 1.4;
    double ymax = 6.5;
    double zmax = 3.6;

    size_t numberOfSeedsX =50;
    size_t numberOfSeedsY = 20;
    size_t numberOfSeedsZ = 20;

   

    std::array<double, 3> boxDimensions{ {(6), (0.1), (-5)} }; 

    std::array<double, 3> boxOrigin{ {-3,6.5,3.2} };





    auto boxMidPoint = boxOrigin + boxDimensions * 0.5;
    double deltaX = (boxDimensions[0]) / (numberOfSeedsX - 1.);
    double deltaY = (boxDimensions[1]) / (numberOfSeedsY - 1.);
    double deltaZ = (boxDimensions[2]) / (numberOfSeedsZ - 1.);
    typedef std::pair<ThreeDimensionalFieldPolicy::PointType, ThreeDimensionalFieldPolicy::PointType> SeedLocation;
    std::vector<SeedLocation> seedLocations;



    for (size_t i = 0; i < numberOfSeedsZ; i++)
    {
        double z = boxOrigin[2] + i * deltaZ;
        for (size_t j = 0; j < numberOfSeedsX; j++)
        {
            double x = boxOrigin[0] + j * deltaX;
            seedLocations.push_back(std::make_pair(ThreeDimensionalFieldPolicy::PointType{ x,boxOrigin[1] + 0.001,z }, ThreeDimensionalFieldPolicy::PointType{ 0.0,-1.0,0.0 }));
         
        }
    }


    double stepSize = 0.00000000001;
    size_t maxNumberOfSteps = 1000000;

    std::vector<Path<ThreeDimensionalFieldPolicy> > paths(seedLocations.size());

    std::transform(seedLocations.begin(), seedLocations.end(), paths.begin(), [&](const SeedLocation& seed)
        {
            std::cout << "Tracing path..." << std::endl;
            return Trace<ThreeDimensionalFieldPolicy, PrincipalValueComputer3D, AdaptiveRungeKuttaFourth>::tracePath(seed.first, seed.second, field, stepSize, maxNumberOfSteps);
        });


    VtpFileWriter<ThreeDimensionalFieldPolicy> writer;
    writer.registerSectionWriter("principalStress", principalStress);
    writer.writeFile("D:\\SL\\newpro\\zdirection.vtp", paths);

    return 0;
}

