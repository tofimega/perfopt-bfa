
nyelv: c++

2 db binkerfa + stdlib map

cél: keresési idő tesztelése

kulcs: size_t

tárolt érték: 2 pointer

## Load gen

paraméterek: adatpontok száma (size_t: n), seed (std::uint_fast32_t: s), tesztek száma (size_t: k)

random engine: rand = minstd_rand(s)

data = std::vector\<long>(n)

n = min(n, data.max_size())

data.reserve(n)

for i in 0..n: data.push_back(i)

data = std::shuffle(data.first(), data.last(), rand)

bint = bintree(data)
bint_cont = bintree_cont(data)

map = std::map\<long, Node>()

for i in data: map.insert(std::pair(i, Node(0, 0)))

## Keresés szimulálása

- tests =  vector\<size_t>(k)
- for i in k: tests.push_back(rand.next)
- for t in tests: measure and record search times

## Fák

-  Node
	- left, right pointers

- IBinTree: közös iface
	- TODO

- BinTree:
	- Node:  root
	
	- Constr(&std::vector\<size_t> data): sorban beszúrja a számokat a fába
	- Destr(): törli a fát postorder módon

	- egy Node pointerei közv a gyerekekre mutatnak
	- a Node-ok közv a heapen vannak

- BinTreeCont:
	- Node: root
	- std::vector\<Node> tree

	- Constr(&std::vector\<size_t> data): sorban beszúrja a számokat a fába és a tree vector-ba
	- Destr(): törléskor a tree vector megoldja

	- egy Node pointerei offset a tree-be
	- a Node-ok folytonosan vannak tárolva a tree vector-ban