#include "sourcefile.h"

#include "util.h"

SourceFile::SourceFile()
{

}

SourceFile::SourceFile(const char* fileName)
    : _fileName(fileName)
{

}

SourceFile::SourceFile(const std::string& fileName)
    : _fileName(fileName)
{

}

SourceFile::~SourceFile()
{
}

bool SourceFile::load()
{
    if (_fileName.empty())
        return false;

    if (!_source.empty())
        return true;

    _source = Util::loadFile(_fileName);
    return !_source.empty();
}

std::string SourceFile::getSource()
{
    if (_source.empty())
    {
        load();
    }
    return _source;
}

std::string SourceFile::fileName() const
{
    return _fileName;
}

void SourceFile::setFileName(const std::string& fileName)
{
    if (_fileName != fileName)
    {
        _source.clear();
        _fileName = fileName;
    }
}


