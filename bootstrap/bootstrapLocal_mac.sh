  
#!/bin/bash

# Run using /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/CJBuchel/robotSimFRC/master/bootstrap/bootstrapLocal_mac.sh)"


echo "Running mac bootstrap"


/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# update homebrew
brew update

# Add Homebrew path in PATH
echo "# Homebrew" >> ~/.bash_profile
echo "export PATH=/usr/local/bin:$PATH" >> ~/.bash_profile
source ~/.bash_profile

# Homebrew recently moved popular formuale to homebrew-core
# So this is not needed anymore to install OpenCV and you can skip this step.
# tap science repo of home
brew tap homebrew/science


brew install cmake
brew install pkg-config

cd /opt
sudo git clone https://github.com/opencv/opencv.git
sudo git clone https://github.com/opencv/opencv_contrib.git

cd opencv
sudo mkdir release
cd release
sudo cmake -D BUILD_TIFF=ON -D OPENCV_GENERATE_PKGCONFIG=ON -D WITH_CUDA=OFF -D ENABLE_AVX=OFF -D WITH_OPENGL=OFF -D WITH_OPENCL=OFF -D WITH_IPP=OFF -D WITH_TBB=ON -D BUILD_TBB=ON -D WITH_EIGEN=OFF -D WITH_V4L=OFF -D WITH_VTK=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules /opt/opencv/
sudo make -j4
sudo make install
sudo ln -s /usr/local/opt/opencv@4/lib/pkgconfig/opencv4.pc $PKG_CONFIG_PATH
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/
source ~/.profile
pkg-config --modversion opencv4