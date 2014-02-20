#ifndef SOURCEFILE_H
#define SOURCEFILE_H

#include <string>

class SourceFile
{
public:
    SourceFile();
    explicit SourceFile(const char* fileName);
    explicit SourceFile(const std::string& fileName);
    ~SourceFile();

    bool load();

    std::string getSource();
    std::string fileName() const;
    void setFileName(const std::string& name);

private:
    std::string _fileName;
    std::string _source;
};

#endif // SOURCEFILE_H
