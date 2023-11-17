import sys
from plyfile import PlyData,PlyElement

if len(sys.argv) <3:
    raise ValueError("Invalid number of arguments. Try 'python ply-to-pts.py input.ply output'")

input_file = sys.argv[1];
output_file= sys.argv[2];

plydata=PlyData.read(input_file)

vertices=[[x[0],x[1],x[2]] for x in plydata["vertex"]]

file=open(output_file,"w+")
for item in vertices:
    file.write("%s" % item[0])
    file.write(" %s" % item[1])
    file.write(" %s\n" % item[2])
file.close()