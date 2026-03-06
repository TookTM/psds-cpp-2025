#include <stdexcept>
#include <cstddef>
#include <iostream>

void PrintBits(long long value, size_t bytes) {
    if (bytes < 1 || bytes > 8) {
        return;
    }
    
    std::cout << "0b";
    
    size_t total_bits = bytes * 8;
    size_t bits_printed = 0;
    
    // Проходим по всем битам от старшего к младшему
    for (int byte = bytes - 1; byte >= 0; --byte) {
        for (int bit = 7; bit >= 0; --bit) {
            // Получаем текущий бит (сдвигаем и маскируем)
            long long current_bit = (value >> (byte * 8 + bit)) & 1;
            std::cout << current_bit;
            bits_printed++;
            
            if (bits_printed % 4 == 0 && bits_printed < total_bits) {
                std::cout << "'";
            }
        }
    }
    
    std::cout << std::endl;
}
