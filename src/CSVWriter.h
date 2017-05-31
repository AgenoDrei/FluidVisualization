#pragma once

#include <string>
#include <fstream>

#define SEPERATOR ","

class CSVWriter {
private:
    int currentFrame = 0;

    std::ofstream file;
public:
    CSVWriter(std::string path);
    ~CSVWriter();

    void writeFrame(std::string algorithm, long frametime, int fps, int reflection, int shadows);
};



