#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>

using namespace std;

class board {

    public:

        struct step {
            int di, dj;
        };

        struct cell_coord {
            int i, j;

            void operator+=(step st) {
                i += st.di;
                j += st.dj;
            }
        };

        enum cell_type {
            X, O, Empty
        };
        
        static cell_type cell_of(char c) {
            switch (c) {
                case 'X': return X;
                case 'O': return O;
                default:  return Empty;
            }
        }

        static char char_of_cell_type(cell_type cell) {
            switch (cell) {
                case X:  return 'X';
                case O:  return 'O';
                default: return ' ';
            }
        }
        
        enum path_type {
            Horizontal, Vertical, Diagonal_Up, Diagonal_Down
        };
        static pair<char, step> char_step_of_path_type(path_type type) {
            switch(type) {
                case Horizontal:    return { '-',  {  0, 1 } };
                case Vertical:      return { '|',  {  1, 0 } };
                case Diagonal_Up:   return { '/',  { -1, 1 } };
                case Diagonal_Down: return { '\\', {  1, 1 } };
            };
        }

        struct path {
            cell_coord first;
            path_type type;
        };

        enum end_type {
            X_Wins, O_Wins, Draw, NotOverYet
        };

        static void print_message(end_type outcome) {
            switch(outcome) {
                case X_Wins:
                    cout << "The winner is X." << endl;
                    break;
                case O_Wins:
                    cout << "The winner is O." << endl;
                    break;
                case Draw:
                    cout << "The game ended in a draw!" << endl;
                    break;
                case NotOverYet:
                    cout << "The game isn't over yet!" << endl;
                    break;
            }
        }

        struct result {
            end_type outcome;
            optional<path> winning_path;
        };

        vector<cell_type> grid;
        int n; // size of the board
        int g; // length of the winning path
        bool contains_empty = false;

        board(int n, int g)
            : grid(n * n), n(n), g(g) {}

        void create_row(string row, int index_row) {
            const int index_start_row = index_row * n;
            for (int j = 0; j < n; j++) {
                grid[index_start_row + j] = cell_of(row[j]);
                if (row[j] == ' ')
                    contains_empty = true;
            }
        }

        void print_grid(const optional<path>& winning_path) const {

            vector<char> s(n * n);
            for (int i = 0; i < grid.size(); i++) {
                s[i] = char_of_cell_type(grid[i]);
            }

            if (winning_path.has_value()) {
                const auto [ w, st ] = char_step_of_path_type(winning_path->type);
                cell_coord cell = winning_path->first;
                for (int i = 0; i < g; i++) {
                    s[cell.i * n + cell.j] = w;
                    cell += st;
                }
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    cout << s[i * n + j];
                }
                cout << endl;
            }
        }

        optional<result> search(int i, int j, path_type p_type) const {
            int sequence_length = 0;
            
            cell_coord first = { -1, -1 };
            const auto [ _, st ] = char_step_of_path_type(p_type);
            
            cell_coord pos = { i - st.di, j - st.dj };
            cell_type current_type = Empty;

            for (int k = 0; k < n; k++) {

                pos += st;
                if (pos.i < 0 || pos.i >= n || pos.j < 0 || pos.j >= n)
                    break;
                
                cell_type cell = grid[pos.i * n + pos.j];
                
                if (cell != Empty && cell == current_type) {
                    sequence_length++;
                    if (sequence_length == g)
                        return result {
                            (cell == X) ? X_Wins : O_Wins,
                            optional<path>({ first, p_type })
                        };
                }
                else {
                    sequence_length = 1;
                    first = { pos.i, pos.j };
                    current_type = cell;
                }
            }
            return nullopt;
        }

        result search_for_winning_path() const {
            for (int i = 0; i < n; i++) {
                const optional<result> res = search(i, 0, Horizontal);
                if (res.has_value())
                    return res.value();
            }
            for (int j = 0; j < n; j++) {
                const optional<result> res = search(0, j, Vertical);
                if (res.has_value())
                    return res.value();
            }
            
            // Diagonal
            for (int i_start = 0; i_start < n; i_start += n - 1) {
                for (int j = 0; j < n; j++) {
                    const optional<result> res_up = search(i_start, j, Diagonal_Up);
                    if (res_up.has_value())
                        return res_up.value();

                    const optional<result> res_down = search(i_start, j, Diagonal_Down);
                    if (res_down.has_value())
                        return res_down.value();
                }
            }
            for (int i = 0; i < n; i++) {
                const optional<result> res_up = search(i, 0, Diagonal_Up);
                if (res_up.has_value())
                    return res_up.value();

                const optional<result> res_down = search(i, 0, Diagonal_Down);
                if (res_down.has_value())
                    return res_down.value();
            }
            return {
                contains_empty ? NotOverYet : Draw,
                nullopt
            };
        }
};

int main()
{
    int n;
    int g;
    cin >> n >> g; cin.ignore();

    board bd(n, g);

    for (int i = 0; i < n; i++) {
        string row;
        getline(cin, row);
        //cerr << "debug: |" << row << "|" << endl;

        bd.create_row(row, i);
    }

    board::result res = bd.search_for_winning_path();
    bd.print_grid(res.winning_path);
    board::print_message(res.outcome);
}