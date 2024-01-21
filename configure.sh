git --version
rm -r include/glfw
rm -r include/glm
rm -r .git
rm .gitmodules

sudo apt install libxinerama-dev
sudo apt install libxcursor-dev

git init
git submodule add https://github.com/glfw/glfw.git include/glfw
git submodule add https://github.com/g-truc/glm.git include/glm
mkdir build
cd build
cmake -S ..
make .

./Gina
