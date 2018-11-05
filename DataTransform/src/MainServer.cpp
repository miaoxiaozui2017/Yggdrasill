#include <iostream>
#include <fstream>
#include <string>

#include <tclap/CmdLine.h>
#include <json/json.h>

using namespace std;

using namespace TCLAP;
using namespace Json;

int main(int argc, char ** argv)
{
  try
  {
    CmdLine cmd("What is this MainServer message", ' ', "0.1");

    ValueArg<std::string> nameArg("c", "name", "Path of config file", true, "homer", "FileName");
    cmd.add(nameArg);

    cmd.parse(argc, argv);

    std::string name = nameArg.getValue();
    std::cout << name << std::endl;
    // 读取json文件
    Value root;
    std::ifstream fin(name);
    if (!fin.is_open())
    {
      std::cerr << "Open failed." << std::endl;
      return 1;
    }
    fin.close();
  }
  catch (ArgException &e)
  {
    std::cerr << "error : " << e.error() << " for arg " << e.argId() << std::endl;
  }
  return 0;
}