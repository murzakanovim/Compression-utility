#include <iostream>
#include <ctime>
#include <fstream>

const size_t COUNT_OF_CHARS = 1048576;
const size_t COUNT_OF_STRINGS = 1000;

// Откуда брать символы из таблицы ASCII.
enum Region {
    LOWERS,
    CAPITALS,
    SYMBOLS,
    NUMBERS
};

void generate_events(std::string fileName) {
    std::ofstream fout(fileName);

    std::string str = "";
    for (size_t i = 0; i < COUNT_OF_STRINGS; i++) {
        str = "";
        for (size_t j = 0; j < COUNT_OF_CHARS; j++) {
            Region region = Region(rand() % 3);
            switch (region) {
                case LOWERS:
                    str += (char) (rand() % 26 + 97);
                    break;
                case CAPITALS:
                    str += (char) (rand() % 26 + 65);
                    break;
                case SYMBOLS:
                    str += (char) (rand() % 15 + 33);
                    break;
                case NUMBERS:
                    str += (char) (rand() % 10 + 48);
                    break;
            }
        }
        fout << str << "\n";
    }
    fout.close();
}

void generate_queries() {

}

int main() {
    srand(time(NULL));
    generate_events("../../database/zip_event.txt");
}