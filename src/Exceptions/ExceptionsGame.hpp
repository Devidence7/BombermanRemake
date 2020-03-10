#pragma once
#include <exception>
#include <string>

class ExceptionLoadImage : public std::exception{
    std::string message = "Exception Load Image";
    int exceptioId = 0;
    public:
        ExceptionLoadImage(){}
        ExceptionLoadImage(std::string m, int id=0) : message(m), exceptioId(id){}
};


class NoImplementedException : public std::exception{
    std::string message = "Exception Load Image";
    int exceptioId = 0;
    public:
        NoImplementedException(){}
        NoImplementedException(std::string m, int id=0) : message(m), exceptioId(id){}
};