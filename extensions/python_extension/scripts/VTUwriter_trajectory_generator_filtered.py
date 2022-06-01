#=================================================================================
#
#  Project   :  3DCRUST
#  Module    :  VTUwriter_trajectory_generator.py
#  Software  :  Sofistik 2020
#  Outputs   : .vtu(structure), .vtp(trajectories), .vtp(trajectories/filtered)
#
#    Python module for: 
#                       1)Reading data from CDB file to write into VTU file.
#                       2)Generating trajectories of traced structure. 
#                       3)Rearranging the vtp file for Paraview's Threshold filter.
#
#  Developers  : Emin Can Oezen; Juan Carlos Alvarado Morones; Rafeek Serhal
#=================================================================================
import pyCDB_Reader
import pyCDB_Reader
import pygotraceit as gt
import pyvtkextension as gtvtk
import numpy as np
import math
import matplotlib.pyplot as plt
import progressbar
import os

CDB_path="D:\\SL\\newpro\\Volume model Bridge.cdb"                                                          # Give the path for CDB file.
Load_Case=10                                                                                                # Define the desired load case (check Sofistik).
Sofistik_dll="D:\\Program Files\\2020\\SOFiSTiK 2020\\interfaces\\64bit\\sof_cdb_w_edu-70.dll"              # Give the path for sof_cdb_w-70.dll (or sof_cdb_w_edu-70.dll for educational version)   
VTU_file= "D:\\SL\\newpro\\3D_visuilization_with_stresses.vtu"                                              # Output path for VTU file. (This file contains geometric and mechanical information of the structure)
VTP_output="D:\\SL\\newpro\\trajectories.vtp"                                                               # Output path for VTP file. (This file contains trajectories of the structure traced)
VTP_filtered="D:\\SL\\newpro\\trajectories_filtered.vtp"                                                    # Output path for filtered VTP file. (This file contains trajectories of the structure traced)          
                                                                                                           
### Data collector function ######################################################################                                                                                                           
pyCDB_Reader.vtuwriter(CDB_path, Load_Case,Sofistik_dll,VTU_file)                                           
                                                                                                            
                                                                                                            
                                                                                                            
### gotraceit ####################################################################################                        
                                                                                                            
def getPrincipalStress(point,direction,frame):                                                              
    npDirection = np.array(direction)                                                                       
    eigenValues = frame[0]                                                                                  
    eigenVectors = [np.array(x) for x in frame[1]]                                                          
    dotProducts = [math.fabs(np.dot(direction,x)) for x in eigenVectors]                                    
    maxIndex = np.argmax(dotProducts)                                                                       
    return [eigenValues[maxIndex]]                                                                          
                                                                                                            
def getPrincipalDirection(point,direction,frame):                                                           
    npDirection = np.array(direction)                                                                       
    eigenValues = frame[0]                                                                                  
    eigenVectors = [np.array(x) for x in frame[1]]                                                          
    dotProducts = [math.fabs(np.dot(direction,x)) for x in eigenVectors]                                    
    maxIndex = np.argmax(dotProducts)                                                                       
    return eigenVectors[maxIndex].tolist()                                                                  
                                                                                                            
                                                                                                            
def main():                                                                                                 
    vtuField = gtvtk.VtuField3D(VTU_file,"Stress")                                                          
    field = gt.TensorField3D(vtuField.getValueFunctor(),vtuField.getIndicatorFunctor())                     
    stepSize = 0.001                                                                                        
    maximumNumberOfSteps = 1000                                                                             
                                                                                                            
    boxDimensions = np.array([0.5, 0.5, 3.0])                                                               # Define a box.                                                               
    boxOrigin  = np.array([-0.5, -3.5, 0])                                                                  # Define the box origin.        
                                                                                                            
    seedPoints = []                                                                                         
    seedDirections = []                                                                                     
                                                                                                            
    for x in np.linspace(boxOrigin[0],boxOrigin[0]+boxDimensions[0],20):                                    # Input information regarding seeds direction and number of seeds.                     
        for y in np.linspace(boxOrigin[1],boxOrigin[1]+boxDimensions[1],20):                                #       Check documentation and/or gotraceit repo on https://gitlab.lrz.de/cie_sam/gotraceit.git               
            seedPoints.append([x,y,boxOrigin[2]])                                                           #
                                                                                                            #
    seedDirections = [[0.0,0.0,1.0]] * len(seedPoints)                                                      #                  
   
    paths = []
    with progressbar.ProgressBar(max_value=len(seedPoints)) as bar:
        for i,seedPoint in enumerate(seedPoints):
            paths.append(gt.TracePath3D(seedPoint,seedDirections[i],field,stepSize,maximumNumberOfSteps))
            bar.update(i)

    writer = gt.VtpFileWriter3D()
    writer.registerArray1SectionWriter("principalStress",getPrincipalStress)
    writer.registerArray3SectionWriter("principalDirection",getPrincipalDirection)
    writer.writeFile(VTP_output,paths)

if __name__ == "__main__":
    main()



### Rearranging the vtp file for Paraview's Threshold filter #####################################


tmp = open('tmp000.vtp', 'w')

offsets = [0]
with open(VTP_output) as input:
    for line in input:
        if '<Lines>' in line:
            tmp.write(line)
            line  = input.readline()
            tmp.write(line)
            for line in input:
                if '</DataArray>' in line:
                    tmp.write(line)
                    break

                items = line.split()

                # A line element consisting of a single node.
                # Here, the node Id needs to be duplicated.
                # Also removes empty line elements.
                if len(items) == 1:
                    tmp.write(f'						{items[0]} {items[0]}\n')
                    offsets.append(offsets[-1] + 2)
                elif len(items) == 0:
                    continue
                else:
                    tmp.write(line)
                    offsets.append(offsets[-1] + len(items))

            continue

        elif 'offsets' in line:
            tmp.write(line)
            for offset in offsets[1:]:
                tmp.write(f'						{offset}\n')
            for line in input:
                if '</DataArray>' in line:
                    tmp.write(line)
                    break
        else:
            tmp.write(line)

tmp.close()

# The number of line elements may have been changed.
num_of_lines = len(offsets) - 1

# Redefine the number of line elements at the beginning
out = open(VTP_filtered, 'w')
with open('tmp000.vtp') as tmp:
    for line in tmp:
        if 'NumberOfLines' in line:
            # Get the number of points
            items = ' '.join(line.replace('="',' ').replace('"',' ').split()).split()
            num_of_points = int(items[2])
            out.write(f'			<Piece NumberOfPoints="{num_of_points}" NumberOfLines="{num_of_lines}">\n')
        else:
            out.write(line)

out.close()
os.remove('tmp000.vtp')