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
  /// @brief A base class upon which an application can be built.
  class Application {
   public:
    /// @brief The container type used to hold arguments.
    typedef std::vector<std::string> arg_vector;

    /// @brief A class used to hold platform-specific data that is sometimes
    /// pertinent that would typically be obtained in a traditional main()
    /// function.
    #ifdef NX_OS_WINDOWS
    struct PlatformData {
      // These members are named matching winapi documentation.

      /// @brief The hInstance of the application, as passed to WinMain()
      HINSTANCE hInstance;
      /// @brief The value of nCmdShow as passed to WinMain()
      int nCmdShow;
    };
    #else
    struct PlatformData {
    };
    #endif

    /// @brief Destructor.
    virtual ~Application();

    /// @brief Constructs with no stored arguments.
    Application();

    /// @brief Constructs using the provided stored arguments.
    ///
    /// @param argc The number of arguments passed.
    /// @param argv The array of argument values.
    Application(const int argc, const char* const * const argv);

    /// @brief The driver function of the application.  Must be overridden.
    ///
    /// @return The exit code of the application.
    virtual int main()=0;

    /// @brief Populates the stored arguments with the ones provided.
    ///
    /// @param argc The number of arguments passed.
    /// @param argv The array of argument values.
    /// @return true if the arguments were set successfully.
    bool set_arguments(const int argc, const char* const * const argv);

    /// @brief Accessor for the stored arguments.
    /// @return A const reference to the stored arguments.
    const arg_vector& arguments() const;

    /// @brief Accessor for the stored arguments.
    /// @return A reference to the stored arguments.
    arg_vector& arguments();

    /// @brief Accessor for any platform-specific data.
    /// @return A const pointer to the PlatformData.
    const PlatformData* platform_data() const;

    /// @brief Accessor for any platform-specific data.
    /// @return A pointer to the PlatformData.
    PlatformData* platform_data();

    /// @brief Sets the platform-specific data, transferring ownership to
    /// this object.
    ///
    /// @param data The new platform-specific data.  Must be allocated with
    ///  new.
    void set_platform_data(PlatformData*data);

   private:
    /// @brief The platform-specific data.
    std::unique_ptr<PlatformData> platform_data_;
    /// @brief The stored arguments.
    arg_vector arguments_;
  };

  /// @brief The main application.  It is defined as an external to ensure
  /// a linker error if the user does not instantiate an application.
  extern Application& application;

}  // namespace nx

#endif  // INCLUDE_NX_APPLICATION_H_
