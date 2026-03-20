
nyelv: c++

2 db binkerfa + stdlib map

cél: keresési idő tesztelése

kulcs: size_t

tárolt érték: 2 pointer

## Load gen

paraméterek: beszúrandó elemek száma (std::size_t: n), seed (std::uint_fast32_t: s), tesztek száma (std::size_t: k)

random engine: rand = minstd_rand(s)

data = std::vector\<std::int_fast32_t>()

n = min(n, data.max_size())

data.reserve(n)

for i in 0..n: data.push_back(i)

data = std::shuffle(data.first(), data.last(), rand)

bint = BinTree(data)
bint_cont = BinTreeCont(data)

map = std::map\<std::int_fast32_t,  Node>()

for i in data: map.insert({i, Node(0, 0)})

## Keresés szimulálása, benchmark

Keretrendszer: [nanobench](https://nanobench.ankerl.com/)

tests =  vector\<std::int_fast32_t>

tests.reseve(k)
 
for i in k: tests.push_back(data\[rand()%data.size()])
 
for t in tests: measure and record search times

## Fák

-  Node
	- left, right pointers

- IBinTree: közös iface
	- TODO

- BinTree:
	- Node:  root
	
	- Constr(&std::vector\<std::int_fast32_t> data): sorban beszúrja a számokat a fába
	- Destr(): törli a fát postorder módon

	- egy Node pointerei közv a gyerekekre mutatnak
	- a Node-ok közv a heapen vannak

- BinTreeCont:
	- Node: root
	- std::vector\<Node> tree

	- Constr(&std::vector\<std::int_fast32_t> data): sorban beszúrja a számokat a fába és a tree vector-ba
	- Destr(): törléskor a tree vector megoldja

	- egy Node pointer-ei offset a tree vector-ba
	- a Node-ok folytonosan vannak tárolva a tree vector-ban