#!/bin/bash

# Run using curl -Ls https://github.com/wml-frc/CJ-Vision/blob/cj-vision-2.0/bootstrap/bootstrapLocal_mac.sh?raw=1 | bash


echo Running mac bootstrap


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
brew brew tap homebrew/science



# If installing python for the first time using Homebrew,
# else skip the 3 lines below and upgrade.
brew install python python3
brew link python
brew link python3

# NOTE : If you have python already installed using homebrew,
# it might ask you to upgrade.
# Upgrade the python using new homebrew formulae.
brew upgrade python
brew upgrade python3

# Check whether Python using homebrew install correctly
which python2  # it should output /usr/local/bin/python2
which python3  # it should output /usr/local/bin/python3

# Check Python versions
python2 --version
python3 --version

export PATH="/usr/local/opt/python/libexec/bin:$PATH"


# Install virtual environment
pip install virtualenv virtualenvwrapper
echo "# Virtual Environment Wrapper"
echo "VIRTUALENVWRAPPER_PYTHON=/usr/local/bin/python2" >> ~/.bash_profile
echo "source /usr/local/bin/virtualenvwrapper.sh" >> ~/.bash_profile
source ~/.bash_profile

############ For Python 2 ############
# Create virtual environment
mkvirtualenv facecourse-py2 -p python2
workon facecourse-py2

# Now install python libraries within this virtual environment
pip install numpy scipy matplotlib scikit-image scikit-learn ipython pandas

# Quit virtual environment
deactivate
######################################

############ For Python 3 ############
# Create virtual environment
mkvirtualenv facecourse-py3 -p python3
workon facecourse-py3

# Now install python libraries within this virtual environment
pip install numpy scipy matplotlib scikit-image scikit-learn ipython pandas

# Quit virtual environment
deactivate
######################################

brew install cmake
brew install opencv
brew install pkg-config
export PKG_CONFIG_PATH="/usr/local/opt/opencv@4/lib/pkgconfig"
export PKG_CONFIG_PATH="/usr/local/opt/opencv@/lib/pkgconfig"

# ############ For Python 2 ############
# # This is a single line command.
# echo /usr/local/opt/opencv/lib/python2.7/site-packages >>
# /usr/local/lib/python2.7/site-packages/opencv3.pth

# ############ For Python 3 ############
# # This is a single line command
# echo /usr/local/opt/opencv/lib/python3.6/site-packages >>
# /usr/local/lib/python3.6/site-packages/opencv3.pth

# find /usr/local/opt/opencv3/lib/ -name cv2*.so

# ############ For Python 3 ############
# cd ~/.virtualenvs/facecourse-py3/lib/python3.6/site-packages/
# ln -s /usr/local/opt/opencv3/lib/python3.6/site-packages/cv2.cpython-36m-darwin.so cv2.so

# brew install cmake

# cd /opt
# sudo git clone https://github.com/opencv/opencv.git
# sudo git clone https://github.com/opencv/opencv_contrib.git

# cd opencv
# sudo mkdir release
# cd release
# sudo cmake -D BUILD_TIFF=ON -D WITH_CUDA=OFF -D ENABLE_AVX=OFF -D WITH_OPENGL=OFF -D WITH_OPENCL=OFF -D WITH_IPP=OFF -D WITH_TBB=ON -D BUILD_TBB=ON -D WITH_EIGEN=OFF -D WITH_V4L=OFF -D WITH_VTK=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules /opt/opencv/
# sudo make -j4
# sudo make install
# sudo update_dyld_shared_cache