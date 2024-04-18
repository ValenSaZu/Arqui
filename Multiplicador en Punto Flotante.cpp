#include <iostream>
#include <cstdint>
#include <bitset>
#include <cmath>

using namespace std;

void imprimirPatronBits(float numero) {
    bitset<sizeof(float) * 8> bits(*(reinterpret_cast<uint32_t*>(&numero)));
    cout << "Patrón de bits: " << bits << endl;
}

float multiplicarFlotantes(float a, float b) {

    uint32_t bits_a = *(reinterpret_cast<uint32_t*>(&a));
    uint32_t bits_b = *(reinterpret_cast<uint32_t*>(&b));

    uint32_t signo_a = (bits_a >> 31) & 0x1;
    uint32_t exponente_a = (bits_a >> 23) & 0xFF;
    uint32_t significando_a = bits_a & 0x7FFFFF;

    uint32_t signo_b = (bits_b >> 31) & 0x1;
    uint32_t exponente_b = (bits_b >> 23) & 0xFF;
    uint32_t significando_b = bits_b & 0x7FFFFF;

    if (significando_a == 0 || significando_b == 0) {
        cout << "Resultado: 0" << endl;
        return 0.0f;
    }

    int32_t suma_exponentes = exponente_a - 127 + exponente_b - 127;
    int32_t exponente_resultado = suma_exponentes + 127;

    uint64_t multiplicacion_significandos = static_cast<uint64_t>(significando_a) * static_cast<uint64_t>(significando_b);

    if (multiplicacion_significandos & (1ULL << 47)) {
        multiplicacion_significandos >>= 1;
        exponente_resultado++;
    }

    if (exponente_resultado < 0) {
        cout << "Resultado: Subflujo" << endl;
        return 0.0f;
    }
    else if (exponente_resultado > 255) {
        cout << "Resultado: Sobreflujo" << endl;
        return INFINITY;
    }

    uint32_t resultado = (signo_a ^ signo_b) << 31;
    resultado |= (exponente_resultado & 0xFF) << 23;
    resultado |= (multiplicacion_significandos >> 24) & 0x7FFFFF;

    return *(reinterpret_cast<float*>(&resultado));
}

int main() {
    float a, b;
    cout << "Ingrese el primer valor flotante: ";
    cin >> a;
    cout << "Ingrese el segundo valor flotante: ";
    cin >> b;

    float resultado = multiplicarFlotantes(a, b);
    cout << "Resultado: " << resultado << endl;

    cout << "Patrón de bits de a:" << endl;
    imprimirPatronBits(a);
    cout << "Patrón de bits de b:" << endl;
    imprimirPatronBits(b);
    cout << "Patrón de bits del resultado:" << endl;
    imprimirPatronBits(resultado);

    return 0;
}
