if not exist build (
    mkdir build
)

echo Updating project

git pull || exit /b

cd build

echo Build project
cmake .. || exit \b
cmake --build . || exit \b

echo Succes, for run project go to /build
