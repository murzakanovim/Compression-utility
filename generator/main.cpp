#include <iostream>
#include <ctime>
#include <fstream>
#include <string>

const size_t COUNT_OF_CHARS = 1048576;
//const size_t COUNT_OF_CHARS = 1000;
const size_t COUNT_OF_STRINGS = 1000;

const std::string ID_FILE_NAME = "../../database/id.txt";
const std::string TYPE_FILE_NAME = "../../database/type.txt";
const std::string SUBJECT_FILE_NAME = "../../database/subjects.txt";
const std::string TIMESTAMP_FILE_NAME = "../../database/timestamp.txt";
const std::string ZIP_EVENT_FILE_NAME = "../../database/zip_event.txt";
const std::string TS_VECTOR_FILE_NAME = "../../database/ts_vector.txt";
const std::string T_EVENT_FILE_NAME = "../../database/t_event.sql";

// Откуда брать символы из таблицы ASCII.
enum Region {
    LOWERS,
    CAPITALS,
    NUMBERS
};

void generate_events() {
    std::ofstream fout(ZIP_EVENT_FILE_NAME);

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
    std::ofstream fout(T_EVENT_FILE_NAME);
    std::ifstream idFile(ID_FILE_NAME);
    std::ifstream typeFile(TYPE_FILE_NAME);
    std::ifstream subjectsFile(SUBJECT_FILE_NAME);
    std::ifstream timestampFile(TIMESTAMP_FILE_NAME);
    std::ifstream zip_eventFile(ZIP_EVENT_FILE_NAME);
    std::ifstream ts_vectorFile(TS_VECTOR_FILE_NAME);

    for (size_t i = 0; i < COUNT_OF_STRINGS; i++) {
        std::string id;
        std::string type;
        std::string subjects;
        std::string timestamp;
        std::string zip_event;
        std::string ts_vector;

        std::getline(idFile, id);
        std::getline(typeFile, type);
        std::getline(subjectsFile, subjects);
        std::getline(timestampFile, timestamp);
        std::getline(zip_eventFile, zip_event);
        std::getline(ts_vectorFile, ts_vector);

        fout
                << "insert into t_event (id, type, subjects, timestamp, zip_event, ts_vector) values ("
                << id << ", '"
                << type << "', '"
                << subjects << "', '"
                << timestamp << "', '"
                << zip_event << "', '"
                << ts_vector << "');\n";
    }

    fout.close();
    idFile.close();
    typeFile.close();
    subjectsFile.close();
    timestampFile.close();
    zip_eventFile.close();
    ts_vectorFile.close();
}

int main() {
    srand(time(NULL));
    generate_events();
    generate_queries();
}