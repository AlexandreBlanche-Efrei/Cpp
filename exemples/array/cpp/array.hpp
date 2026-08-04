#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <algorithm> // Pour std::max
#include <memory>    // Pour std::unique_ptr

template<typename T>
requires (std::is_default_constructible_v<T> && std::is_move_constructible_v<T>) // Contraintes sur le type T ("traits")
class array {

    private:

        static constexpr std::size_t init_capacity = 2;

        std::size_t size_;          // Convention : les indices et tailles, sont des std::size_t
        std::size_t capacity_;      // Ajout d'un '_' pour éviter l'ambiguïté avec les méthodes size() et capacity()
        std::unique_ptr<T[]> data;  // S'alloue avec new[], se libère (automatiquement) avec delete[]

    public:

        array(std::size_t size = 0) // Valeur par défaut 0 si aucun argument
            :   size_(size),
                capacity_(std::max(size_, init_capacity)),
                data(std::make_unique<T[]>(capacity_)) {}

        ~array() noexcept {} // std::unique_ptr s'occupe de libérer la mémoire allouée

        // Affectation par move
        array& operator=(array&& arr) noexcept {
            data          = std::move(arr.data);
            size_         = arr.size_;
            capacity_     = arr.capacity_;
            arr.size_     = 0;
            arr.capacity_ = 0;
            return *this;
        }

        // Constructeur par move
        array(array&& arr) noexcept {
            *this = std::move(arr);
        }

        array(const array&)            = delete; // Constructeur et affectation par copie supprimés
        array& operator=(const array&) = delete; // (on pourrait les définir par copie profonde, chaque élément copié dans le nouveau array)

        // Crée un nouvel array identique au précédent (copie profonde)
        array clone() const {
            array arr(size());
            std::copy(&data[0], &data[size_], &arr.data[0]);
            return arr;
        }

        std::size_t size() const {
            return size_;
        }

        std::size_t capacity() const {
            return capacity_;
        }

        // Sert de get et set
        T& operator[](std::size_t index) {
            return data[index];
        }

        // Surcharge const : uniquement get
        const T& operator[](std::size_t index) const {
            return data[index];
        }

        void push_back(T val) {
            if (size_ >= capacity_)
                realloc();

            data[size_] = val;
            size_++;
        }

        template<typename... Args>
        void emplace_back(Args&... args) {
            if (size_ >= capacity_)
                realloc();

            data[size_] = T(args...);
            size_++;
        }

        void insert(std::size_t index, T val) {
    
            if (index > size_)
                throw std::range_error("Wrong index for insertion");

            if (size_ >= capacity_)
                realloc();

            if (index == size_) {
                push_back(val);
            }
            else {
                shift_right(index);
                data[index] = val;
                size_++;
            }
        }

        void delete_at(std::size_t index) {
            shift_left(index + 1);
            size_--;
        }

        void print() const {

            std::cout << "[ ";
            if (size_ != 0) {
                for (std::size_t i = 0; i < size_ - 1; i++) {
                    std::cout << data[i] << ", ";
                }
                std::cout << data[size_ - 1] << " ";
            }
            std::cout << "]\n";
        }

    private:

        void realloc() {
            std::size_t new_capacity = 2 * capacity_;
            std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);
            std::ranges::move(&data[0], &data[size_], &new_data[0]);
            data      = std::move(new_data);
            capacity_ = new_capacity;
        }

        // Shifts the elements of indices index_start .. size-1 one position to the right
        // It is assumed that capacity >= size + 1
        void shift_right(std::size_t index_start) {

            // On ne peut pas utiliser std::copy ou std::move ici, car les plages à copier se recouvrent
            for (std::size_t i = size_; i > index_start; i--) {
                data[i] = data[i - 1];
            }
        }

        // Shifts the elements of indices index_start .. size-1 one position to the left
        void shift_left(std::size_t index_start) {
            
            if (index_start == 0)
                throw std::range_error("Wrong index 0");
            if (size_ == 0)
                throw std::range_error("Empty array");

            for (std::size_t i = index_start - 1; i < size_ - 1; i++) {
                data[i] = data[i + 1];
            }
        }
};

#endif