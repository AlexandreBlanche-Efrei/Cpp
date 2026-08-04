#include <iostream>
#include <vector>

#include "array.hpp"

// Classe d'exemple, pour expliciter les opérations de construction, move, copie, destruction
class A {
    static inline int cpt = 0;
    int x;

    public:
        A()               : x(cpt++) {          std::cout << "A() (x = "            << x << ") " << std::endl;               }
        A(A&& a) noexcept : x(a.x)   {          std::cout << "A(A&&) (x = "         << x << ") " << std::endl;               }
        A(const A& a)     : x(a.x)   {          std::cout << "A(const A&) (x = "    << x << ") " << std::endl;               }
        A& operator=(A&& a) noexcept { x = a.x; std::cout << "A = (A&&) (x = "      << x << ") " << std::endl; return *this; }
        A& operator=(const A& a)     { x = a.x; std::cout << "A = (const A&) (x = " << x << ") " << std::endl; return *this; }
        ~A() noexcept                {          std::cout << "~A() (x = "           << x << ") " << std::endl;               }

        friend std::ostream& operator<<(std::ostream& os, const A& a) { os << "A{" << a.x << "}"; return os; }
        static void reset() { cpt = 0; }
};

void demo_memory_management() {

    std::cout << std::endl;

    // Gestion automatique de la mémoire
    array<A> t(4);
    std::cout << "t = ";
    t.print();

    std::cout << std::endl << "Reallocation : " << std::endl;
    t.push_back(A());
    t.print();

    /*
        Reallocation : 
        A() (x = 4)                              -> élément à ajouter : val
        A() (x = 5) ... A() (x = 12)             -> new_data (taille 2 * t.capacity = 8)
        A = (A&&) (x = 0) ... A = (A&&) (x = 3)  -> move des éléments de data vers new_data
        ~A() (x = 3) ... ~A() (x = 0)            -> destruction des éléments de data (en ordre inverse)
        A = (const A&) (x = 4)                   -> ajout de l'élément val à la fin de data (par copie)
        ~A() (x = 4)                             -> destruction de l'élément temporaire (A())
        [ A{0}, A{1}, A{2}, A{3}, A{4} ]
        ~A() (x = 12) ... ~A() (x = 10)          -> destruction de tous les éléments du tableau data en partant de la fin
        ~A() (x = 4) ... ~A() (x = 0)
    */

    // Différence entre push_back et emplace_back :
    // push_back    construit un objet temporaire, le *copie* et le détruit
    // emplace_back construit un objet temporaire, le *move*  et le détruit (dans mon implémentation)
    std::cout << std::endl << "push_back(A()) : " << std::endl;
    t.push_back(A());
    std::cout << std::endl << "emplace_back() : " << std::endl;
    t.emplace_back();
    t.print();
    // Note :
    //  Il est aussi possible de gérer la mémoire avec un "allocateur" (par exemple std::malloc)
    //  afin d'éviter les initialisations d'objets à chaque realloc.
    //  On pourrait alors avoir emplace_back qui construit directement l'élément dans son emplacement,
    //  sans objet temporaire (comme dans le conteneur vector).

    std::cout << std::endl;

    A::reset();
    std::cout << std::endl;

    // Libération automatique de la mémoire
}

void demo_move_clone() {

    std::cout << std::endl;

    array<A> t(4);
    std::cout << "t = ";
    t.print();

    array<A> t2 = std::move(t);
    // std::cout << "t = { size = " << t.size() << ", capacity = " << t.capacity() << " }" << std::endl;
    std::cout << std::endl << "t = ";
    t.print();
    std::cout << "t2 = ";
    t2.print();

    std::cout << std::endl;

    array<A> t3 = t2.clone();
    std::cout << std::endl << "t2 = ";
    t2.print();
    std::cout << "t3 = ";
    t3.print();

    A::reset();
    std::cout << std::endl;
}

void demo_vector() {

    std::cout << std::endl;

    // Comparaison avec std::vector
    std::cout << "vector<A>" << std::endl;

    std::cout << std::endl << "std::vector<A> v(4);" << std::endl;
    std::vector<A> v(4);

    std::cout << std::endl << "v.push_back(A());" << std::endl;
    v.push_back(A());

    std::cout << std::endl << "v.push_back(A());" << std::endl;
    v.push_back(A());

    std::cout << std::endl << "v.emplace_back();" << std::endl;
    v.emplace_back();

    A::reset();
    std::cout << std::endl;
}

int main() {

    int size = 10;

    array<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }

    arr.print();
    std::cout << std::endl;

    demo_memory_management();
    demo_move_clone();
    demo_vector();

    return EXIT_SUCCESS;
}