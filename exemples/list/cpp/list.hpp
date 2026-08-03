#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <functional>

template<typename T> // Template: la classe list peut contenir n'importe quel type T
class list {

    private:        // L'utilisateur n'a pas accès à l'implémentation de la liste
        struct cell {
            T value;
            cell* next;

            cell(T val, cell* next = nullptr) // Valeur par défaut pour next si l'on ne donne pas de deuxième argument
                : value(val), next(next) {}
        };

        cell* head = nullptr; // Valeurs par défaut
        int size = 0;


        /* Méthodes privées */
        // cell& get_cell(int index) const;
        cell& get_cell(int index) const {
            if (index < 0 || index > size)
                throw std::range_error("Wrong index");

            cell* curr = head;
            if (curr == nullptr)
                throw std::range_error("Empty list");
            
            for (int i = 0; curr != nullptr; curr = curr->next, i++) {
                if (i == index)
                    return *curr;
            }
            throw std::logic_error("List size does not match actual number of cells");
        }

        cell& get_last() const {
            return get_cell(size - 1);
        }

    public: // Interface accessible à l'utilisateur
    
        /* Constructeur par défaut */
        list() {} // head et size prennent les valeurs par défaut

        /* Destructeur */
        ~list() noexcept {
            cell* curr = head;
            while (curr != nullptr) {
                cell* next = curr->next;
                delete curr;
                curr = next;
            }
            head = nullptr;
            size = 0;
        }

        /* Affectation par move : transfère la propriété de la ressource */
        list& operator=(list&& l) {
            head = l.head;
            size = l.size;
            l.head = nullptr;
            l.size = 0;
            return *this;
        }
        /* Constructeur par move, idem */
        list(list&& l) {
            *this = std::move(l);
        }

        /* Suppression du constructeur par copie et de l'affectation par copie */
        list(const list&)            = delete;
        list& operator=(const list&) = delete;

        /* Méthodes publiques */
        void push_front(T val) {
            head = new cell(val, head);
            size++;
        }

        void push_back(T val) {

            if (size == 0) {
                push_front(val);
                return;
            }
            cell& lastcell = get_last();
            lastcell.next = new cell(val); // next = nullptr (argument par défaut)
            size++;
        }

        T get(int index) const {
            const cell& cell = get_cell(index);
            return cell.value;
        }

        void set(int index, T val) {
            cell& cell = get_cell(index);
            cell.value = val;
        }

        void insert(int index, T val) {
            if (index == 0) {
                push_front(val);
                return;
            }

            cell& prev = get_cell(index - 1);
            prev.next = new cell(val, prev.next);
            size++;
        }

        void delete_at(int index) {
            if (index == 0) {
                cell* first = head;
                if (first == nullptr)
                    throw std::range_error("Empty list");
                head = first->next;
                size--;
                delete first;
                return;
            }

            cell& prev = get_cell(index - 1);
            cell* cell = prev.next;
            if (cell == nullptr)
                throw std::range_error("Wrong index");
            prev.next = cell->next;
            size--;
            delete cell;
        }

        void print() const {
            
            std::cout << "[ ";
            cell* curr = head;
            if (curr == nullptr) {
                std::cout << "]\n";
                return;
            }
            
            while (curr->next != nullptr) {
                std::cout << curr->value << ", ";
                curr = curr->next;
            }
            std::cout << curr->value << " ]\n";
        }

        list reverse() const {
            list revlist;
            cell* curr = head;
            while (curr != nullptr) {
                revlist.push_front(curr->value);
                curr = curr->next;
            }
            return revlist;
        }

        /* map accepte n'importe quelle fonction T -> U et renvoie une list<U> */
        template<typename U>
        list<U> map(const std::function<U(T)>& f) const {
            
            list<U> newlist;
            cell* curr = head;
            while (curr != nullptr) {
                const auto [ value, next ] = *curr; // Structured-binding : pour nommer les attributs
                newlist.push_front(f(value));
                curr = next;
            }
            return newlist.reverse();
        }
};

#endif