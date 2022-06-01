#=================================================================================
#
#  Project   : 3DCRUST
#  Module    : VTUwriter_primitive(cdb_to_vtu).py
#  Software  : Sofistik 2020
#  Outputs   : .vtu(structure) 
#
#    Base python module for reading data from CDB file to write into VTU file.
#     
#  Developers  : Emin Can Oezen; Juan Carlos Alvarado Morones; Rafeek Serhal
#=================================================================================
import pyCDB_Reader



CDB_path="D:\\SL\\newpro\\Volume model Bridge.cdb"                                                  # Give the path for CDB file.
Load_Case=10                                                                                        # Define the desired load case (check Sofistik).
Sofistik_dll="D:\\Program Files\\2020\\SOFiSTiK 2020\\interfaces\\64bit\\sof_cdb_w_edu-70.dll"      # Give the path for sof_cdb_w-70.dll (or sof_cdb_w_edu-70.dll for educational version).    
VTU_file= "D:\\SL\\newpro\\3D_visuilization_with_stresses.vtu"                                      # Output path for VTU file. (This file contains geometric and mechanical information of the structure.)


### Data collector function ###                                                                                                    
pyCDB_Reader.vtuwriter(CDB_path, Load_Case,Sofistik_dll,VTU_file)                                    
