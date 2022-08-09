#ifndef LOGGER
#define LOGGER

#include "plog/Init.h"
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Log.h> 

namespace engine {
class Logger {
public:
  inline static void init(plog::Severity severyity) {
    plog::init(severyity, &m_ConsoleAppender); // Step2: initialize the logger
  }
private:
  inline static plog::ConsoleAppender<plog::TxtFormatter> m_ConsoleAppender{};

};
} // namespace engine

#endif // !LOGGER