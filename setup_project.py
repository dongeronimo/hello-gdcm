import subprocess
import os

def setup_project():
    try:
        # 1. Create the required folders
        folders = ['build', 'bin']
        print("Creating folders...")
        for folder in folders:
            os.makedirs(folder, exist_ok=True)
            print(f"Created folder: {folder}")
        
        # 2. Run CMake from build directory
        print("Running CMake configuration...")
        install_dir = os.path.abspath('./bin')
        source_dir = os.path.abspath('.')  # Root directory with CMakeLists.txt
        
        cmake_command = [
            "cmake",
            source_dir,
            f"-DCMAKE_INSTALL_PREFIX={install_dir}"
        ]
        
        # Change to build directory and run cmake
        os.chdir('./build')
        subprocess.run(cmake_command, check=True)
        
        # 3. Build and install
        print("Building project...")
        cmake_build = [
            "cmake",
            "--build",
            ".",
            "--config", "Release"
        ]
        subprocess.run(cmake_build, check=True)
        
        print("Installing to ./bin...")
        cmake_install = [
            "cmake",
            "--install",
            ".",
            "--prefix", install_dir
        ]
        subprocess.run(cmake_install, check=True)
        
        print("Project setup, build, and installation completed successfully!")
        return True
        
    except subprocess.CalledProcessError as e:
        print(f"Error in subprocess execution: {e.stderr if hasattr(e, 'stderr') else e}")
        return False
    except OSError as e:
        print(f"Error in file operations: {e}")
        return False

if __name__ == "__main__":
    setup_project()