// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_RESPONSE_FILTER_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_RESPONSE_FILTER_CTOCPP_H_
#pragma once

#ifndef BUILDING_CEF_SHARED
#pragma message("Warning: "__FILE__" may be accessed DLL-side only")
#else  // BUILDING_CEF_SHARED

#include "include/cef_response_filter.h"
#include "include/capi/cef_response_filter_capi.h"
#include "libcef_dll/ctocpp/ctocpp.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed DLL-side only.
class CefResponseFilterCToCpp
    : public CefCToCpp<CefResponseFilterCToCpp, CefResponseFilter,
        cef_response_filter_t> {
 public:
  CefResponseFilterCToCpp();

  // CefResponseFilter methods.
  bool InitFilter() override;
  FilterStatus Filter(void* data_in, size_t data_in_size, size_t& data_in_read,
      void* data_out, size_t data_out_size,
      size_t& data_out_written) override;
};

#endif  // BUILDING_CEF_SHARED
#endif  // CEF_LIBCEF_DLL_CTOCPP_RESPONSE_FILTER_CTOCPP_H_
