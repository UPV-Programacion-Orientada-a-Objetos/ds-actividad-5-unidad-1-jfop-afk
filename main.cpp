#include <iostream>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int f, int c) : _filas(f), _columnas(c) {}

    virtual ~MatrizBase() {}
    // Metodos virtuales puros
    virtual void cargarValores() = 0;
    virtual void imprimir() const = 0;
    // Metodo de suma polimorfica
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    // Metodo para acceder a un valor para la suma
    virtual T obtenerValor(int f, int c) const = 0;

    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return sumar(otra);
    }
    // necesario para verificar compatibilidad
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

    void copiarProfundo(const MatrizDinamica<T>& otra) {
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;

        this->_datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            this->_datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; ++j) {
                this->_datos[i][j] = otra._datos[i][j];
            }
        }
    }

    void liberarMemoria() {
        if (this->_datos != NULL) {
            for (int i = 0; i < this->_filas; ++i) {
                delete[] this->_datos[i];
            }
            delete[] this->_datos;
            this->_datos = NULL;
        }
    }

public:
    MatrizDinamica(int f, int c) : MatrizBase<T>(f, c), _datos(NULL) {
        this->_datos = new T*[f];
        for (int i = 0; i < f; ++i) {
            this->_datos[i] = new T[c];
        }
    }

    ~MatrizDinamica() override {
        std::cout << "Liberando memoria (Matriz Dinamica)..." << std::endl;
        liberarMemoria();
    }

    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(0, 0) {
        copiarProfundo(otra);
    }

    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        if (this != &otra) {
            liberarMemoria();
            copiarProfundo(otra);
        }
        return *this;
    }

    void setValor(int f, int c, T valor) {
        if (f >= 0 && f < this->_filas && c >= 0 && c < this->_columnas) {
            this->_datos[f][c] = valor;
        }
    }

    void cargarValores() override {
        T valor = 0;
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                if (i == 0 && j == 0) this->_datos[i][j] = (T)1.5;
                else if (i == 0 && j == 1) this->_datos[i][j] = (T)2.0;
                else if (i == 1 && j == 0) this->_datos[i][j] = (T)0.0;
                else if (i == 1 && j == 1) this->_datos[i][j] = (T)1.0;
                else if (i == 2 && j == 0) this->_datos[i][j] = (T)4.5;
                else if (i == 2 && j == 1) this->_datos[i][j] = (T)3.0;
                else this->_datos[i][j] = (T)valor++;
            }
        }
    }

    T obtenerValor(int f, int c) const override {
        if (f >= 0 && f < this->_filas && c >= 0 && c < this->_columnas) {
            return this->_datos[f][c];
        }
        return T();
    }

    void imprimir() const override {
        for (int i = 0; i < this->_filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << this->_datos[i][j] << "  ";
            }
            std::cout << "|" << std::endl;
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            std::cout << "ERROR: Dimensiones incompatibles para la suma." << std::endl;
            return NULL;
        }
        // Crear una nueva MatrizDinamica para el resultado
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);

        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                T val1 = this->_datos[i][j];
                T val2 = otra.obtenerValor(i, j);
                resultado->setValor(i, j, val1 + val2);
            }
        }
        return resultado;
    }
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                _datos[i][j] = T();
            }
        }
    }

    ~MatrizEstatica() override {
    }

    void cargarValores() override {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i == 0 && j == 0) _datos[i][j] = (T)0.5;
                else if (i == 0 && j == 1) _datos[i][j] = (T)1.0;
                else if (i == 1 && j == 0) _datos[i][j] = (T)2.0;
                else if (i == 1 && j == 1) _datos[i][j] = (T)3.0;
                else if (i == 2 && j == 0) _datos[i][j] = (T)1.0;
                else if (i == 2 && j == 1) _datos[i][j] = (T)1.0;
                else _datos[i][j] = T();
            }
        }
    }

    T obtenerValor(int f, int c) const override {
        if (f >= 0 && f < M && c >= 0 && c < N) {
            return _datos[f][c];
        }
        return T();
    }

    void imprimir() const override {
        for (int i = 0; i < M; ++i) {
            std::cout << "| ";
            for (int j = 0; j < N; ++j) {
                std::cout << _datos[i][j] << "  ";
            }
            std::cout << "|" << std::endl;
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            std::cout << "ERROR: Dimensiones incompatibles para la suma." << std::endl;
            return NULL;
        }

        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(M, N);

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                T val1 = _datos[i][j];
                T val2 = otra.obtenerValor(i, j); // Polimorfismo en la lectura
                resultado->setValor(i, j, val1 + val2);
            }
        }
        return resultado;
    }

    void setValor(int f, int c, T valor) {
        if (f >= 0 && f < M && c >= 0 && c < N) {
            _datos[f][c] = valor;
        }
    }
};

int main() {

    std::cout << "--- Sistema gerico de Algebra Lineal ---" << std::endl;
    std::cout << "\n>> Demostracion de Genericidad (Tipo FLOAT) <<" << std::endl;

    std::cout << "\n// 1. Creacion de Matriz Dinamica (a traves del puntero base)" << std::endl;
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    std::cout << "Creando Matriz Dinamica A (3x2)..." << std::endl;
    A->cargarValores();
    std::cout << "A =" << std::endl;
    A->imprimir();

    std::cout << "\n// 2. Creacion de Matriz Estatica (a traves del puntero base)" << std::endl;
    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    std::cout << "Creando Matriz Estatica B (3x2)..." << std::endl;
    B->cargarValores();
    std::cout << "B =" << std::endl;
    B->imprimir();

    std::cout << "\n// 3. Operacion Polimorfica (Suma)" << std::endl;
    std::cout << "SUMANDO: Matriz C = A + B ..." << std::endl;
    MatrizBase<float>* C = *A + *B;

    if (C != NULL) {
        std::cout << "\nMatriz Resultado C (3x2, Tipo FLOAT):" << std::endl;
        C->imprimir();
    }

    // ====================================================================
    // DEMOSTRACION DE GENERICIDAD (TIPO INT)
    // ====================================================================

    std::cout << "\n==========================================================" << std::endl;
    std::cout << "\n>> Demostracion de Genericidad (Tipo INT) <<" << std::endl;

    std::cout << "\n// 1. Creacion de Matriz Dinamica INT (a traves del puntero base)" << std::endl;
    MatrizBase<int>* A_int = new MatrizDinamica<int>(3, 2);
    std::cout << "Creando Matriz Dinamica A_int (3x2)..." << std::endl;
    A_int->cargarValores();
    std::cout << "A_int =" << std::endl;
    A_int->imprimir();

    std::cout << "\n// 2. Creacion de Matriz Estatica INT (a traves del puntero base)" << std::endl;
    MatrizBase<int>* B_int = new MatrizEstatica<int, 3, 2>();
    std::cout << "Creando Matriz Estatica B_int (3x2)..." << std::endl;
    B_int->cargarValores();
    std::cout << "B_int =" << std::endl;
    B_int->imprimir();

    std::cout << "\n// 3. Operacion Polimorfica (Suma) INT" << std::endl;
    std::cout << "SUMANDO: Matriz C_int = A_int + B_int ..." << std::endl;
    MatrizBase<int>* C_int = *A_int + *B_int;

    if (C_int != NULL) {
        std::cout << "\nMatriz Resultado C_int (3x2, Tipo INT):" << std::endl;
        C_int->imprimir();
    }

    std::cout << "\n>> Demostracion de Limpieza de Memoria <<" << std::endl;

    std::cout << "Llamando al destructor de C..." << std::endl;
    delete C;

    std::cout << "Llamando al destructor de A..." << std::endl;
    delete A;

    delete B;

    std::cout << "Llamando al destructor de C_int..." << std::endl;
    delete C_int;

    std::cout << "Llamando al destructor de A_int..." << std::endl;
    delete A_int;

    delete B_int;

    std::cout << "Sistema cerrado." << std::endl;

    return 0;
}
