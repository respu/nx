//
// Copyright (C) 2013 Jacob McIntosh <nacitar at ubercpp dot com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

/// @file
/// Tools to allow for an object-oriented application approach.

#ifndef INCLUDE_NX_APPLICATION_H_
#define INCLUDE_NX_APPLICATION_H_


#include <memory>
#include <vector>
#include <string>

#include "nx/core.h"

namespace nx {
  /// A base class upon which an application can be built.
  class Application {
   public:
    /// The container type used to hold arguments.
    typedef std::vector<std::string> arg_vector;

    /// A class used to hold platform-specific data that is sometimes pertinent
    /// that would typically be obtained in a tradtional main() function.
    #ifdef NX_OS_WINDOWS
    struct PlatformData {
      // These members are named matching winapi documentation.
      HINSTANCE hInstance;
      int nCmdShow;
    };
    #else
    struct PlatformData {
    };
    #endif

    virtual ~Application();

    Application();

    /// Constructs the application using the provided arguments.
    Application(const int argc, const char* const * const argv);

    /// The driver function of the application; to be overriden by a subclass.
    virtual int main()=0;

    /// Populates the arguments using the argc/argv pair passed.
    bool set_arguments(const int argc, const char* const * const argv);

    /// Returns a const reference to the arguments to the Application.
    const arg_vector& arguments() const;
    /// Returns a reference to the arguments to the Application.
    arg_vector& arguments();

    /// Returns a const pointer to platform specific data for this Application.
    const PlatformData* platform_data() const;
    /// Returns a pointer to platform specific data for this Application.
    PlatformData* platform_data();
    /// Sets the platform specific data for this Application.
    void set_platform_data(PlatformData*data);

   private:
    /// The platform specific data.
    std::unique_ptr<PlatformData> platform_data_;
    /// The arguments.
    arg_vector arguments_;
  };

  /// The main application;  defined as an external to ensure a linker error if
  /// the user does not instantiate an application.
  extern Application& application;

}  // namespace nx

#endif  // INCLUDE_NX_APPLICATION_H_