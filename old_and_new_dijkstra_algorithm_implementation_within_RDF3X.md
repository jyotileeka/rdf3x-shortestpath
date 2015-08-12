# Introduction #

Dijkstra algorithm implementation within RDF-3X -- a state-of-the-art RDF engine.

# Usage #

1. cd rdf3x-shortestpath/rdf3x/

2. make

3. Load database using:

> 3.1.  cd rdf3x-shortestpath/rdf3x/bin

> 3.2.  ./rdf3xload db RDF\_Dataset\_Name.ttl

4. Find source to destination distance using:

> 4.1.  ./rdf3xquery db inputFile.txt

> 4.2.  In the above step inputFile.txt contains source-destination pairs in the following form:
> > <http://Source1>   <http://Destination1>


> <http://Source2>   <http://Destination2>

> 4.3. New Dijkstra algorithm follows the LDM-3N RDF model proposed in: A Formal Graph Model for RDF and Its Implementation (http://mor2.nlm.nih.gov/pubs/pdf/2015-www-vn.pdf)


# Details #

Old and New dijkstra algorithm are implemented within RDF-3X (Neumann, Thomas, and Gerhard Weikum. "RDF-3X: a RISC-style engine for RDF." Proceedings of the VLDB Endowment 1.1 (2008): 647-659.) -- a high performance RDF engine.

# Contact #
Jyoti Leeka

PhD Student

IIIT-Delhi

India