
# Bevezetés

A beadandóm két egyszerű bináris keresőfa és egy map keresési idejét hasonlítja össze a tárolt adatok számától függően. A két fához saját implementációt használok, a map-hez a standard könyvtár implementációját használom. A futási időt a [nanobench](https://nanobench.ankerl.com/) keretrendszer segítségével végzem.

# Fák implementálása

A két fa csúcsai Node objektumokból állnak. A map is ilyen objektumokat tárol. A Node osztály két mutatót tartalmaz a gyerekcsúcsokra. A kulcs egy egész szám. Az egyik fa közvetlenül a heap-en tárolja a csúcsait, a másik egy vector segítségével folytonosan tárolja őket. A fák létrehozásuk után nem módosíthatók.

# Load generálás

A csúcsok generálása véletlenszerűen történik. Paraméterként megadható a létrehozandó csúcsok száma (N) és a random generátor kezdőértéke (seed).
Egy vector (data) feltöltődik 0..N egész számokkal, majd a random generátor megkeveri ezeket. Ezekből az értékekből jönnek létre a fák, és a map-ben ezek lesznek a kulcsok.

# Keresés szimulálása

A tesztelés két paramétert fogad: tesztek száma (K) és a random generátor kezdőértéke (test_seed). A random generátor segítségével a data vector-ból egy véletlen kulcs megkeresése lefut a három adatszerkezetben, ezeknek a futási idejét a program kiírja/elmenti. Ez a folyamat K-szor megismétlődik.