import os
import subprocess
import time
from shutil import copyfile

# Gathering relevant paths
cpp_dir = os.getcwd()
cs_dir = cpp_dir + "/../DryadUnity"
generated_dir = os.path.join(cpp_dir, "Generated")

# Listing files
cpp_dir_files = os.listdir(cpp_dir)
cs_dir_files = os.listdir(cs_dir)
generated_dir_files = os.listdir(generated_dir)

# Cleaning older generated files
print("Cleaning up old generated files")
for file in cpp_dir_files:
    if file.endswith(".cs") or file.endswith(".cxx"):
        file_path = os.path.join(cpp_dir, file)
        print("Deleting " + file)
        os.remove(file_path)
for file in cs_dir_files:
    if file.endswith(".cs") or file.endswith(".cxx"):
        file_path = os.path.join(cs_dir, file)
        print("Deleting " + file)
        os.remove(file_path)
for file in generated_dir_files:
    if file.endswith(".cs") or file.endswith(".cxx"):
        file_path = os.path.join(generated_dir, file)
        print("Deleting " + file)
        os.remove(file_path)

# Executing SWIG
print("Running SWIG on dryadcpp.i")
os.system("D:\SWIG\swigwin-4.0.2\swig.exe -c++ -csharp -namespace DryadUnity -outdir Generated dryadapi.i")
generated_dir_files = os.listdir(generated_dir)

# Dispatching generated files
for file in generated_dir_files:
    if file.endswith(".cs"):
        file_src = os.path.join(generated_dir, file)
        file_dst = os.path.join(cs_dir, file)
        print(f"Copying {file} to {file_dst}")
        copyfile(file_src, file_dst)
    elif file.endswith(".cxx"):
        file_src = os.path.join(generated_dir, file)
        file_dst = os.path.join(cpp_dir, file)
        print(f"Copying {file} to {file_dst}")
        copyfile(file_src, file_dst)
