#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include "hash_table.hpp"
#include "graph.hpp"
#include "vector.hpp"

namespace velizade
{

  class Application
  {
  public:
    Application();
    void run(const std::string& filename);

  private:
    void loadFromFile(const std::string& filename);
    void processLine(const std::string& line);

    static void cmdGraphs(Application* app, const Vector<std::string>& args);
    static void cmdVertexes(Application* app, const Vector<std::string>& args);
    static void cmdOutbound(Application* app, const Vector<std::string>& args);
    static void cmdInbound(Application* app, const Vector<std::string>& args);
    static void cmdBind(Application* app, const Vector<std::string>& args);
    static void cmdCut(Application* app, const Vector<std::string>& args);
    static void cmdCreate(Application* app, const Vector<std::string>& args);
    static void cmdMerge(Application* app, const Vector<std::string>& args);
    static void cmdExtract(Application* app, const Vector<std::string>& args);

    using CommandFunc = void (*)(Application*, const Vector<std::string>&);
    HashTable<std::string, CommandFunc> commands_;
    HashTable<std::string, Graph> graphs_;
  };

  
  template<class T>
  void sortVector(Vector<T>& vec);
  Vector<std::string> splitString(const std::string& str);
  bool tryParseUInt(const std::string& str, unsigned long long& out);
  void throwInvalid();

}

#endif
