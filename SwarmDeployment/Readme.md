Compilation:
    rm CMakeCache.txt
    cmake -DCMAKE_BUILD_TYPE=ReleaseNoGui .
    make

TODO: do not search for config file, get it obtained by argc
TODO: zkontrolovat načítání konfigurace na windows i linuxu, mít defaultně ve stejném adresáři, jinak podle argumentu
TODO: přehodit konfiguraci do jsonu