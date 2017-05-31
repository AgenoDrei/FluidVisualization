#include "CSVWriter.h"

CSVWriter::CSVWriter(std::string path) {
    file.open(path);
    file<<"Frame number"<<SEPERATOR<<"Algorithm"<<SEPERATOR<<"Frametime"<<SEPERATOR<<"FPS"<<SEPERATOR<<"Reflection"<<SEPERATOR<<"Shadows"<<std::endl;
}

CSVWriter::~CSVWriter() {
    if(file.is_open()) {
        file.close();
    }
}

void CSVWriter::writeFrame(std::string algorithm, long frametime, int fps, int reflection, int shadows) {
    file<<currentFrame<<SEPERATOR<<algorithm<<SEPERATOR<<frametime<<SEPERATOR<<fps<<SEPERATOR<<reflection<<SEPERATOR<<shadows<<std::endl;
    currentFrame++;
}
