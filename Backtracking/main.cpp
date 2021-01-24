#include <bit>
#include <vector>
#include <iostream>
#include <algorithm>

namespace base {
   template<typename T, int N>
   struct matrix : std::vector<matrix<T, N - 1>> {
      template<typename... P>
      matrix(int n, const P&... param)
      : std::vector<matrix<T, N - 1>>(n, matrix<T, N - 1>(param...)) {
      }
   };

   template<typename T>
   struct matrix<T, 1> : std::vector<T> {
      matrix(int n, const T& v = T( ))
      : std::vector<T>(n, v) {
      }
   };
}

int f(unsigned r, unsigned s) {
   if (r > s) {
      return f(s, r);
   } else if (r == 1) {
      return s;
   } else if (int s_ = std::bit_ceil(s); s_ / 2 < r) {
      return s_;
   } if (int r_ = std::bit_ceil(r); r_ < s) {
      return r_ + f(r, s - r_);
   }
   return -1;
}

/**/
bool es_intercalada(base::matrix<int, 2>& m, int r, int s) {
   for (int i = 0; i < r - 1; ++i) {
      for (int j = 0; j < s - 1; ++j) {
         for (int u = i + 1; u < r; ++u) {
            for (int v = j + 1; v < s; ++v) {
               auto dos = bool(m[i][j] == m[u][v] && m[u][j] == m[i][v]);
               auto cuatro = bool(m[i][j] != m[i][v] && m[i][v] != m[u][v] && m[u][v] != m[u][j] && m[u][j] != m[i][j] && m[i][j] != m[u][v] && m[u][j] != m[i][v]);
               if (dos == false && cuatro == false) {
                  return false;
               }
            }
         }
      }
   }
   return true;
}

/**

bool es_intercalada_(base::matrix<int, 2>& m, int r, int s) {
   for (int i = r - 1; i >= 0; --i) {
      for (int j = s - 1; j >= 0; --j) {
         auto dos = bool(m[i][j] == m[r][s] && m[r][j] == m[i][s]);
         auto cuatro = bool(m[i][j] != m[i][s] && m[i][s] != m[r][s] && m[r][s] != m[r][j] && m[r][j] != m[i][j] && m[i][j] != m[r][s] && m[r][j] != m[i][s]);
         if (dos == false && cuatro == false) {
            return false;
         }
      }
   }
   return true;
}
/**/

void resuelve(int i, int j, int& r, int& s, base::matrix<int, 2>& m, std::vector<long long>& usados_fila, std::vector<long long>& usados_columna, unsigned hs, unsigned diferentes, unsigned cuantos, bool& encontrada) {
   if (cuantos == r * s && hs == std::popcount(diferentes)) {
      std::cout << "Matriz encontrada!\n";
      for (int i_ = 0; i_ < r; ++i_) {
         for (int j_ = 0; j_ < s; ++j_) {
            std::cout << m[i_][j_] << " ";
         }
         std::cout << "\n";
      }
      encontrada = true;
      return;
   }

   if (encontrada == false) {
      for (int c = 1; c <= hs; ++c) {
         int color = (1 << c);
         if ((color & usados_fila[i]) == 0 && (color & usados_columna[j]) == 0) {
            m[i][j] = c;
            if (!es_intercalada(m, i, j)) {
               m[i][j] = 0; continue;
            }
            diferentes |= color;
            usados_fila[i] |= color;
            usados_columna[j] |= color;
            resuelve(i + (j == s - 1), (j + 1) % s, r, s, m, usados_fila, usados_columna, hs, diferentes, cuantos + 1, encontrada);
            m[i][j] = 0;
            diferentes &= ~color;
            usados_fila[i] &= ~color;
            usados_columna[j] &= ~color;
         }
      }
   }
}

int main( ) {
   int r, s;
   std::cin >> r >> s;

   int hopf_stiefel = f(r, s);
   std::cout << "Minimo de colores a usar: " << hopf_stiefel << "\n";

   base::matrix<int, 2> m(r, s);
   std::vector<long long> usados_fila(r), usados_columna(s);
/**
   for (int i = 0; i < r; ++i) {
      for (int j = 0; j < s; ++j) {
         std::cin >> m[i][j];
      }
   }
   std::cout << (es_intercalada_(m, r - 1, s - 1) ? "Es intercalada" : "No es intercalada") << "\n";
/**/
   bool encontrada = false;
   std::cerr << "Para " << hopf_stiefel << ":\n";
   resuelve(0, 0, r, s, m, usados_fila, usados_columna, hopf_stiefel, 0, 0, encontrada);

   encontrada = false;
   std::cerr << "Para " << hopf_stiefel - 1 << ":\n";
   resuelve(0, 0, r, s, m, usados_fila, usados_columna, hopf_stiefel - 1, 0, 0, encontrada);
   /**/
}
