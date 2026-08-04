#include <iostream>

#include "list.hpp"

int main() {

    list<int> l;
    l.print();

    l.push_back(5);
    l.push_back(2);
    l.print();

    l.push_back(-3);
    l.push_back(4);
    l.print();

    l.delete_at(2);
    l.print();

    l.insert(1, 0);
    l.print();

    l.push_front(8);
    l.print();

    l[3] = 10; // operator[] : syntaxe plus claire que l.set(3, 10);
    l.print();

    const int index = 1;
    // l[index] au lieu de l.get(index)
    std::cout << "list[" << index << "] = " << l[index] << std::endl;
    l.print();

    list rev = l.reverse(); // Déduction du paramètre de template (<int>)
    rev.print();

    // Avantage du C++ : pas besoin d'appeler les destructeurs, ils seront appelés automatiquement

    //////////////////////////////////

    list<int> range;
    for (int i = 10; i >= 0; i--)
        range.push_front(i);
    range.print();

    const std::function<int(int)> f = [] (int x) -> int { return x * x; };
    list squares = range.map(f);
    squares.print();

    //////////////////////////////////

    // Démonstration de la sûreté du C++ par rapport au C

    list<int> ll;
    ll.push_back(2);
    ll.push_back(3);

    std::cout << "ll: ";
    ll.print();

    // list<int> ll2 = ll; // Impossible : affectation par copie supprimée
    list ll2 = std::move(ll); // On peut seulement faire une affectation par move

    std::cout << "ll: ";
    ll.print();

    std::cout << "ll2: ";
    ll2.print();

    ll.~list(); // Destruction de ll

    std::cout << "ll: ";
    ll.print();

    std::cout << "ll2: ";
    ll2.print(); // Ne cause pas d'erreur, ll2 possède la ressource
    
    // On pouvait également définir un constructeur et une affectation par copie (profonde) :
    // copie de chaque élément dans la nouvelle liste

    return EXIT_SUCCESS;
}