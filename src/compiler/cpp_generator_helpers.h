/*
 *
 * Copyright 2014, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef NET_GRPC_COMPILER_CPP_GENERATOR_HELPERS_H__
#define NET_GRPC_COMPILER_CPP_GENERATOR_HELPERS_H__

#include <map>
#include <string>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

using namespace std;

namespace grpc_cpp_generator {

inline bool StripSuffix(string* filename, const string& suffix) {
  if (filename->length() >= suffix.length()) {
    size_t suffix_pos = filename->length() - suffix.length();
    if (filename->compare(suffix_pos, string::npos, suffix) == 0) {
      filename->resize(filename->size() - suffix.size());
      return true;
    }
  }

  return false;
}

inline string StripProto(string filename) {
  if (!StripSuffix(&filename, ".protodevel")) {
    StripSuffix(&filename, ".proto");
  }
  return filename;
}

inline string StringReplace(string str, const string& from, const string& to) {
  size_t pos = 0;

  for (;;) {
    pos = str.find(from, pos);
    if (pos == string::npos) {
      break;
    }
    str.replace(pos, from.length(), to);
    pos += to.length();
  }

  return str;
}

inline string DotsToColons(const string& name) {
  return StringReplace(name, ".", "::");
}

inline string DotsToUnderscores(const string& name) {
  return StringReplace(name, ".", "_");
}

inline string ClassName(const google::protobuf::Descriptor* descriptor,
                        bool qualified) {
  // Find "outer", the descriptor of the top-level message in which
  // "descriptor" is embedded.
  const google::protobuf::Descriptor* outer = descriptor;
  while (outer->containing_type() != NULL) outer = outer->containing_type();

  const string& outer_name = outer->full_name();
  string inner_name = descriptor->full_name().substr(outer_name.size());

  if (qualified) {
    return "::" + DotsToColons(outer_name) + DotsToUnderscores(inner_name);
  } else {
    return outer->name() + DotsToUnderscores(inner_name);
  }
}

}  // namespace grpc_cpp_generator

#endif  // NET_GRPC_COMPILER_CPP_GENERATOR_HELPERS_H__
