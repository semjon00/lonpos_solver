#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>

using namespace std;
#define FOR(a,b,c) for(int (a)=(b); (a)<(c); (a)++)

int OUTER_DIM = 9;
int INNER_DIM = 4;
int PIECES_N = 12;

int RIDDLE = 7;

vector<int> read_file_ints(const string& s) {
    ifstream fff(s);
    vector<int> ans;
    int n = -1;
    while (fff >> n) {
        ans.push_back(n);
    }
    fff.close();
    return ans;
}

inline vector<vector<bool>> mirror(vector<vector<bool>> orig) {
    vector<vector<bool>> deriv = orig;
    FOR(i,0,orig.size()) {
        FOR(u,0,orig.size()) {
            deriv[i][u] = orig[u][i];
        }
    }
    return deriv;
}

inline vector<vector<bool>> rotate(vector<vector<bool>> orig) {
    vector<vector<bool>> deriv = orig;
    FOR(i,0,orig.size()) {
        FOR(u,0,orig.size()) {
            deriv[i][u] = orig[u][3-i];
        }
    }
    return deriv;
}

u_int64_t brute_return = 0ull;
inline void bruting(const vector<int> &my_pieces_num,
             int cur_piece_index,
             const vector<vector<u_int64_t>> &pieces,
             u_int64_t occupied,
             const u_int64_t &target,
             unordered_map<u_int64_t,u_int64_t> &configurations,
             u_int64_t trace) {
    if (cur_piece_index == my_pieces_num.size()) {
        configurations[occupied] = trace;
        if (configurations.find(occupied ^ target) != configurations.end()) {
            brute_return = occupied ^ target;
            //cout << endl << "Return: " << brute_return << endl;
        }
    } else {
        FOR(i,0,pieces[my_pieces_num[cur_piece_index]].size()) {
            if (cur_piece_index == 0) {
                cout << "\r" << i << " out of " << pieces[my_pieces_num[cur_piece_index]].size() << std::flush;
            }
            if ((pieces[my_pieces_num[cur_piece_index]][i] & occupied) == 0) {
                bruting(my_pieces_num,
                        cur_piece_index + 1,
                        pieces,
                        pieces[my_pieces_num[cur_piece_index]][i] | occupied,
                        target,
                        configurations,
                        (trace << 3) + i);
            }
        }
    }
}

void vis(vector<vector<int>> &holes, u_int64_t piece) {
    cout << '\n';
    vector<vector<char>> vis(OUTER_DIM, vector<char> (OUTER_DIM, ' '));
    FOR(i,0,OUTER_DIM) {
        FOR(u,0,OUTER_DIM) {
            if (holes[i][u] == 255) vis[i][u] = '.';
            if (piece & (1l << holes[i][u])) vis[i][u] = 'x';
        }
    }
    FOR(i,0,OUTER_DIM) {
        FOR(u,0,OUTER_DIM) {
            cout << vis[i][u];
        }
        cout << '\n';
    }
    cout << endl << endl;
}

int main() {
    ifstream donefile("../riddles/" + to_string(RIDDLE) + "-done");
    vector<vector<int>> holes(OUTER_DIM, vector<int>(OUTER_DIM, 255));
    int hole_counter = 0;
    FOR(i,0,OUTER_DIM) {
        string line; donefile >> line;
        FOR(u,0,OUTER_DIM) {
            if (line[u] == '.') {
                holes[i][u] = hole_counter;
                hole_counter++;
            }
        }
    }
    donefile.close();
    vector<int> removed = read_file_ints("../riddles/" + to_string(RIDDLE) + "-removed");
    vis(holes, (u_int64_t)0);

//    auto viz = (u_int64_t)17109556451ull;
//    if (viz) {
//        vis(holes, viz);
//        exit(0);
//    }

    vector<vector<u_int64_t>> pieces(PIECES_N);
    FOR(piece_i,0,PIECES_N) {
        ifstream piecefile("../pieces/" + to_string(piece_i));
        vector<vector<bool>> piece(INNER_DIM, vector<bool> (INNER_DIM, false));
        FOR(i,0,INNER_DIM) {
            string line; piecefile >> line;
            FOR(u,0,INNER_DIM) {
                piece[i][u] = (line[u] == 'x');
            }
        }
        piecefile.close();

        FOR(rot_i,0,4) {
            FOR(mirror_i, 0, 2) {
                FOR(delta_i, 1 - INNER_DIM, OUTER_DIM + INNER_DIM - 1) {
                    FOR(delta_u, 1 - INNER_DIM, OUTER_DIM + INNER_DIM - 1) {
                        u_int64_t cur = 0;
                        bool disallow = false;
                        FOR(i,0,INNER_DIM) {
                            FOR(u,0,INNER_DIM) {
                                if (piece[i][u]) {
                                    if (delta_i + i < 0 || delta_i + i >= OUTER_DIM) disallow = true;
                                    if (delta_u + u < 0 || delta_u + u >= OUTER_DIM) disallow = true;
                                    if (disallow) break;
                                    if (holes[delta_i + i][delta_u + u] == 255) disallow = true;
                                    if (disallow) break;
                                    cur |= (1l << holes[delta_i + i][delta_u + u]);
                                }
                            }
                            if (disallow) break;
                        }
                        if (!disallow) {
                            pieces[piece_i].push_back(cur);
                        }
                    }
                }
                piece = mirror(piece);
            }
            piece = rotate(piece);
        }

        sort( pieces[piece_i].begin(), pieces[piece_i].end());
        pieces[piece_i].erase(
                unique(pieces[piece_i].begin(), pieces[piece_i].end() ),
                pieces[piece_i].end());
        //FOR(i,0,pieces[piece_i].size()) {
        //    vis(holes, pieces[piece_i][i]);
        //}
    }

    vector<int> leftPieces;
    vector<int> rightPieces;
    FOR(i,0,PIECES_N) {
        if (!std::count(removed.begin(), removed.end(), i)) {
            swap(leftPieces, rightPieces);
            leftPieces.push_back(i);
        }
    }

    u_int64_t complete_field = (1l << hole_counter) - 1l;
    unordered_map<u_int64_t,u_int64_t> configurations;
    configurations[0] = 0;
    cout << endl << "Processing the left group" << endl;
    bruting(leftPieces, 0, pieces, 0, complete_field, configurations, 0);
    cout << endl << "Processing the right group" << endl;
    bruting(rightPieces, 0, pieces, 0, complete_field, configurations, 0);

    cout << endl << endl;
    for (int p : leftPieces) {
        cout << p << ' ';
    }
    vis(holes, brute_return);
}
