import os
from shutil import copyfile

# Gathering relevant paths
cpp_dir = os.getcwd()
cs_dir = cpp_dir + "/../Dryad"
generated_dir = os.path.join(cpp_dir, "_generated")
if not os.path.exists(generated_dir):
    os.makedirs(generated_dir)

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
print("Running SWIG on dryadfacade.i")
os.system(f"D:\SWIG\swigwin-4.0.2\swig.exe -c++ -csharp -namespace Dryad -outdir {generated_dir} dryadfacade.i")
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
