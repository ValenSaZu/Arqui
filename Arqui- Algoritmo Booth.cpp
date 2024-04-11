#include <iostream>
#include <vector>

using namespace std;

vector<int> decimalABinarioConSigno(int num, int n) {
    vector<int> binario(n, 0);
    int i = n - 1;
    bool negativo = false;

    if (num < 0) {
        negativo = true;
        num = -num;
    }

    while (num > 0) {
        binario[i] = num % 2;
        num /= 2;
        i--;
    }

    if (negativo) {
        for (int j = 0; j < n; j++) {
            binario[j] = !binario[j];
        }
        int carry = 1;
        for (int j = n - 1; j >= 0; j--) {
            int sum = binario[j] + carry;
            binario[j] = sum % 2;
            carry = sum / 2;
        }
    }

    return binario;
}

int binarioADecimalConSigno(vector<int>& binario) {
    int decimal = 0;
    int signo = 1;
    if (binario[0] == 1) {
        signo = -1;

        for (int i = 0; i < binario.size(); i++) {
            binario[i] = !binario[i];
        }

        int carry = 1;
        for (int i = binario.size() - 1; i >= 0; i--) {
            int sum = binario[i] + carry;
            binario[i] = sum % 2;
            carry = sum / 2;
        }
    }

    for (int i = 1; i < binario.size(); i++) {
        decimal += binario[i] * (1 << (binario.size() - 1 - i));
    }
    return decimal * signo;
}

vector<int> sumaBinaria(vector<int>& num1, vector<int>& num2) {
    vector<int> suma(num1.size(), 0);
    int acarreo = 0;
    for (int i = num1.size() - 1; i >= 0; i--) {
        int s = num1[i] + num2[i] + acarreo;
        suma[i] = s % 2;
        acarreo = s / 2;
    }
    return suma;
}

vector<int> restaBinaria(vector<int>& num1, vector<int>& num2) {
    vector<int> diferencia(num1.size(), 0);
    vector<int> complemento(num2.size(), 0);

    for (int i = 0; i < num2.size(); i++) {
        complemento[i] = !num2[i];
    }

    int carry = 1;
    for (int i = num2.size() - 1; i >= 0; i--) {
        int sum = complemento[i] + carry;
        complemento[i] = sum % 2;
        carry = sum / 2;
    }

    diferencia = sumaBinaria(num1, complemento);

    return diferencia;
}

void corrimientoAritmeticoDerecha(vector<int>& A, vector<int>& Q, int& Q_1, int n) {
    Q_1 = Q[n-1];
    Q[0] = A[n - 1];
    for (int i = n - 1; i > 1; i--) {
        Q[i] = Q[i - 1];
        A[i] = A[i - 1];
    }
}

vector<int> multiplicacionBooth(int multiplicando, int multiplicador, int n) {
    vector<int> A(n, 0);
    vector<int> Q = decimalABinarioConSigno(multiplicando, n);
    vector<int> M = decimalABinarioConSigno(multiplicador, n);
    int Q_1 = 0;
    int count = n;
    while (count > 0) {
        if (Q[n-1] == 1 && Q_1 == 0) {
            A = sumaBinaria(A, M);
        }
        else if (Q[n-1] == 0 && Q_1 == 1) {
            A = restaBinaria(A, M);
        }
        corrimientoAritmeticoDerecha(A, Q, Q_1, n);
        count--;
    }
    vector<int> resultado(A.size(), 0);
    resultado = sumaBinaria(A, Q);
    return resultado;
}

int main() {
    int multiplicando, multiplicador, n;
    cout << "Ingrese el multiplicando: ";
    cin >> multiplicando;
    cout << "Ingrese el multiplicador: ";
    cin >> multiplicador;
    cout << "Ingrese la cantidad de bits (n): ";
    cin >> n;

    vector<int> resultado = multiplicacionBooth(multiplicando, multiplicador, n);

    cout << "El resultado de la multiplicación es: ";
    for (int i = 0; i < resultado.size(); i++) {
        cout << resultado[i];
    }
    cout << endl;
    cout << binarioADecimalConSigno(resultado);

    return 0;
}
