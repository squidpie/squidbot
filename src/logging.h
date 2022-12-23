#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace logging = boost::log;
namespace sev { using boost::log::trivial::info; }

logging::sources::severity_logger< logging::trivial::severity_level > squidlog;

void init_logging(const char* name) {
  add_file_log(
      logging::keywords::file_name = name,
      logging::keywords::target_file_name = name,
      logging::keywords::format = (
                            logging::expressions::stream
                                   << "["
                                   << logging::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
                                   << "] <" << logging::trivial::severity << "> " << logging::expressions::smessage
                          )
      );
  logging::core::get()->set_filter(
      logging::trivial::severity >= logging::trivial::info
      );
  logging::add_common_attributes();
}

#endif