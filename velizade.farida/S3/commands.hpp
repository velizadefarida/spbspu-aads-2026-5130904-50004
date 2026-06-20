#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include "hashTable.hpp"
#include "graph.hpp"
#include "vector.hpp"

namespace velizade
{
  Vector<std::string> splitString(const std::string& str);
  bool tryParseUInt(const std::string& str, unsigned int& out);
  void throwInvalid();

  template <class T>
  void sortVector(Vector<T>& vec)
  {
    size_t n = vec.getSize();
    for (size_t i = 0; i < n; ++i)
    {
      for (size_t j = i + 1; j < n; ++j)
      {
        if (vec[j] < vec[i])
        {
          std::swap(vec[i], vec[j]);
        }
      }
    }
  }

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
}

#endif
