
make gfx
make clean
%PYTHON27% setup.py install
%PYTHON27% test.py > log.txt