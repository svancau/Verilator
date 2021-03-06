#include "V3Global.h"
#include "V4VhdlFrontend.h"
#include "V4VhdlTranslate.h"
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace std;
using namespace TinyProcessLib;

V4VhdlFrontend::V4VhdlFrontend(V3ParseSym &symtable) : symt(symtable) {

}

void V4VhdlFrontend::allocateTemp() {
  const int length = 32;
  auto randchar = []() -> char
  {
    const char charset[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[ rand() % max_index ];
  };
  struct stat buffer;
  do {
    stringstream ss;
    ss << "/tmp/nvc.";
    for (int i = 0; i < length; ++i) {
      ss << randchar();
    }
    tempFilename = ss.str();
  } while(stat(tempFilename.c_str(), &buffer) == 0);
}

string V4VhdlFrontend::getTempName() {
  return tempFilename;
}

void V4VhdlFrontend::parseFiles() {
  const V3StringList& vhdFiles = v3Global.opt.vhdFiles();
  // Skip if there are no VHDL files to analyze
  if (!vhdFiles.size())
    return;

  allocateTemp();
  ostringstream oss;
  oss << "nvc -a --relax=prefer-explicit ";
  for (V3StringList::const_iterator it = vhdFiles.begin(); it != vhdFiles.end(); ++it) {
    string filename = *it;
    oss << filename << " ";
  }
  oss << "--dump-json " << getTempName();
  string command = oss.str();
  const char *cmd = command.c_str();

  Process nvc_process(cmd, "", [](const char *bytes, size_t n) {
    cout << string(bytes, n);
  }, [](const char *bytes, size_t n) {
    cout << string(bytes, n);
  });

  // Check for missing sim or parse error
  if (nvc_process.get_exit_status() == 127) {
    v3fatal("nvc VHDL simulator is not properly installed or not in your $PATH");
  }
  else if (nvc_process.get_exit_status() != 0) {
    v3fatal("nvc failed to parse one of your input files");
  }

  V4VhdlTranslate xlate(symt);
  xlate.translate(getTempName());

  //if (unlink(getTempName().c_str()) == -1)
  //  v3error("Failed to remove file " << getTempName() << "error " << errno);

}