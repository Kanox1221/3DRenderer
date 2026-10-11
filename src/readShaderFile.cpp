#include "readShaderFile.hpp"
#include <fstream>
#include <sstream>

std::string readShaderFile(const std::string &filename)
{
    /* Reference : CPSC427 SimpleGL */
    std::cout << "[INFO] Renderer : Loading shader filename " << filename << std::endl;

    /* KNX : Input File Stream (File -> Data)*/
    std::ifstream ifs(filename);

    if (!ifs.good())
    {
        std::cerr << "[ERROR] Invalid filename loading shader from file " << filename << std::endl;
        return "";
    }

    /* KNX : Output String Stream (Data -> String)*/
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}