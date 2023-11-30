import sys
import os
import subprocess
from tempfile import NamedTemporaryFile
import pandas as pd

### check for sufficient input parameters
if len(sys.argv) < 5:
    raise ValueError("Invalid number of arguments. Try 'persistentfeature-to-ply.py input coefficient_field #features output'")

### define global variables
script_directory = os.path.dirname(__file__);
point_cloud = sys.argv[1];
coefficient_field = sys.argv[2]
number_features = int(sys.argv[3]);
output = sys.argv[4]
delaunay=os.path.join(script_directory,"delaunay_3d_filtration")
infiltrator=os.path.join(script_directory,"infiltrator")

### create augmented_boundaries
with NamedTemporaryFile(suffix=".txt") as temp_boundaries:
    with NamedTemporaryFile(suffix=".txt") as temp_filtration:
        proc = subprocess.Popen(delaunay+str(" ")+point_cloud+str(" ")+temp_filtration.name,shell=True)
        proc.wait()
        # ratio=1.06
        proc = subprocess.Popen(infiltrator+str(" ")+temp_filtration.name+str(" ")+temp_boundaries.name+" --modulus "+ coefficient_field,shell=True)
        proc.wait()
        augmented_boundaries=pd.read_csv(temp_boundaries.name,sep=";",names=["column","triangle1","triangle2","triangle3","value","oriented1","oriented2","oriented3"],header=None)

### create barcode and most persistent features
barcode=augmented_boundaries[augmented_boundaries.triangle3==-1].drop(columns=["triangle3","value","oriented1","oriented2","oriented3"]).rename(columns={"triangle1":"birth", "triangle2":"death"})
barcode["ratio"]=barcode["death"]/barcode["birth"]
boundaries=augmented_boundaries[augmented_boundaries.triangle3!=-1]

barcode_mostpersistent=barcode.sort_values(by="ratio", ascending=False).head(number_features).reset_index(drop=True)

### create ply files
points=pd.read_csv("point_cloud_sorted.txt",sep=" ",header=None).values.tolist()

for index, row in barcode_mostpersistent.iterrows():
    chosen_column=int(row[0])
    ratio=round(row[3],2)
    File_object=open(os.path.join(script_directory,output+"_"+str(index+1)+"_column_"+str(chosen_column)+"_ratio_"+str(ratio)+".ply"),"w+")
    File_object.write("ply\nformat ascii " +str(1.0)+"\nelement vertex "+str(len(points))+"\nproperty float x\nproperty float y\nproperty float z\nelement face " +str(len(boundaries[boundaries.column==chosen_column]))+"\nproperty list int int vertex_indices\nend_header")
    for vert in points:
        File_object.write("\n"+str(vert[0])+" "+str(vert[1])+" "+str(vert[2]))
    for x,y,z in zip(boundaries[boundaries.column==chosen_column].oriented1,boundaries[boundaries.column==chosen_column].oriented2,boundaries[boundaries.column==chosen_column].oriented3):
        File_object.write("\n3 "+str(x)+" "+str(y)+" "+str(z))
    File_object.write("\n")
    File_object.close()

### cleanup    
os.remove("point_cloud_sorted.txt")