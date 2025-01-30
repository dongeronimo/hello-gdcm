import subprocess
import os

def setup_gdcm():
    try:
        # 1. Update git submodules
        print("Updating git submodules...")
        subprocess.run(
            ["git", "submodule", "update", "--init", "--recursive"],
            capture_output=True,
            text=True,
            check=True
        )
        
        # 2. Create the required folders
        folders = ['GDCM-build', 'libgdcm']
        print("Creating folders...")
        for folder in folders:
            os.makedirs(folder, exist_ok=True)
            print(f"Created folder: {folder}")
        
        # 3. Run CMake from GDCM-build directory
        print("Running CMake configuration...")
        install_dir = os.path.abspath('./libgdcm')
        gdcm_source = os.path.abspath('./GDCM')
        
        cmake_command = [
            "cmake",
            gdcm_source,
            f"-DCMAKE_INSTALL_PREFIX={install_dir}"
        ]
        
        # Change to build directory and run cmake
        os.chdir('./GDCM-build')
        subprocess.run(cmake_command, check=True)
        
        # 4. Build and install
        print("Building GDCM...")
        cmake_build = [
            "cmake",
            "--build",
            ".",
            "--config", "Release"
        ]
        subprocess.run(cmake_build, check=True)
        
        print("Installing to ./libgdcm...")
        cmake_install = [
            "cmake",
            "--install",
            ".",
            "--prefix", install_dir
        ]
        subprocess.run(cmake_install, check=True)
        
        print("Setup, build, and installation completed successfully!")
        return True
        
    except subprocess.CalledProcessError as e:
        print(f"Error in subprocess execution: {e.stderr if hasattr(e, 'stderr') else e}")
        return False
    except OSError as e:
        print(f"Error in file operations: {e}")
        return False

if __name__ == "__main__":
    setup_gdcm()