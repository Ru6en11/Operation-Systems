echo Updating project

git pull

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

echo Build project
cmake ..
cmake --build .

echo Succes, for run project go to /build
